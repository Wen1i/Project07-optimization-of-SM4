#ifndef SM4_SIMD_SBOX_H
#define SM4_SIMD_SBOX_H

#include <stdint.h>

/**
 * @brief SM4 轮密钥
 */
typedef uint32_t* SM4_Key;

/**
 * @brief 初始化 SM4 轮密钥
 * @param key 128bit长度密钥
 * @param sm4_key SM4 密钥指针
 * @return 函数执行成功返回1，失败返回0
 */
int SM4_KeyInit(uint8_t* key, SM4_Key* sm4_key);

/**
 * @brief SM4 加密（允许in和out对应内存重叠）
 * @param plaintext 输入128x8bit长度明文
 * @param ciphertext 输出128x8bit长度密文
 * @param sm4_key 用于加密的 SM4 密钥
 */
void SM4_Encrypt_x8(uint8_t* plaintext, uint8_t* ciphertext, SM4_Key sm4_key);

/**
 * @brief SM4 解密（允许in和out对应内存重叠）
 * @param ciphertext 输入128bit长度密文
 * @param plaintextt 输出128bit长度明文
 * @param sm4_key 用于加密的 SM4 密钥
 */
void SM4_Decrypt_x8(uint8_t* ciphertext, uint8_t* plaintext, SM4_Key sm4_key);

/**
 * @brief 删除 SM4 轮密钥
 * @param sm4_key SM4 轮密钥
 */
void SM4_KeyDelete(SM4_Key sm4_key);

#endif
