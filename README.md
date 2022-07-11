# optimization-of-SM4
在此构造了 32bit 的查找表，将 SM4 的线性变换 L 和 S 盒合并为 4 个 256x32bit 的查找表。使用SIMD指令进行并行查表。<br>
S盒操作为 $ x0,x1,x2,x3→S(x0),S(x1),S(x2),S(x3) $，其中 xi为 8bit 字。<br>
为了提升效率，可将S盒与后续的循环移位变换 L合并，即<br>
$$ L(S(x0),S(x1),S(x2),S(x3))=L(S(x0)<<24)⊕L(S(x1)<<16)⊕L(S(x2)<<8)⊕L(S(x3))$$
可定义4个 8bit→32bit8bit→32bit 查找表Ti。<br>
$ T0(x)=L(S(x)<<24) $<br>
$ T1(x)=L(S(x)<<16) $<br>
$ T2(x)=L(S(x)<<8) $<br>
$ T3(x)=L(S(x)) $<br>
于是，可以将8-bit S盒转换为4个8-bit输入32-bit输出表。非线性变换可通过掩码、移位、异或、查表操作实现。<br>
SM4 是以 32bit 为单位迭代的，故可以利用 256bit 向量寄存器，进行 8 组并行加解密。第 i 个 256bit 向量寄存器存储 8 组输入的第 i 组 32bit 值。之后的针对 32bit 的循环移位、异或等操作，都将转化为 8 组消息的并行操作。

# 流程如下：
输入明文，轮密钥Kr,参数为 32bit 查找表T<br>
1.加载8组消息至$ imm_{i} $, i∈[0,3]<br>
2.通过打包函数打包消息，使得 Xi 对应第 i个小分组<br>
3.for i=0→31<br>
4.  Ki← 轮密钥<br>
5.  Temp←X1⊕X2⊕X3⊕Ki<br>
6.  Temp←T(Temp)⊕X0<br>
7.  X0,X1,X2,X3←X1,X2,X3,Temp<br>
8.X0,X1,X2,X3←X3,X2,X1,X0<br>
9.通过打包函数，将Xi打包回原始的状态$ imm_{i} $<br>
10.存储$ imm_{i} $至输出对应的内存中<br>


# 优化效果： 
![image](https://user-images.githubusercontent.com/104118101/178234820-97390578-2f39-4a73-8257-a13063854b9d.png)

使用 SIMD 指令，可以很好的实现程序的并行性。在算法内部，将非线性函数 S 和线性函数 L 合并为 T 函数，制作成 4 个 256x32bit 的查找表；在查表过程中，利用 SIMD 指令mm256 i32gather epi32 进行 32 位的并行查表，相当于一次查找 4 个 s 盒（平凡算法下，S 盒的输入为 8 比特，输出也为 8 比特）将程序性能理论上提高 4 倍。在 ECB 工作模式下，我们一次加密 8 组，相当于每次加密 128*8  比特，将程序性能理论上提升 8 倍。综上，该优化理论上可以实现 32 倍的加速。由实验数据可以看出，当数据规模较大时，使用 SIMD 算法的优化比达到了 26，较为理想。
