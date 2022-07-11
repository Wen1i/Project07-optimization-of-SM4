#include <stdio.h>
#include <stdlib.h>

#include "sm4_simd_sbox.h"
int main() {
    // 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    unsigned char key[16 * 8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab,
                                 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98,
                                 0x76, 0x54, 0x32, 0x10};
    // 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    // 00 00 ... 00
    unsigned char in[16 * 8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                                0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    SM4_Key sm4_key;
    int success = SM4_KeyInit(key, &sm4_key);
    if (success) {
        SM4_Encrypt_x8(in, in, sm4_key);
        printf("C:\n");
        for (int j = 0; j < 8; j++) {
            printf("\t");
            for (int i = 0; i < 16; i++) {
                printf("%02x ", in[i + 16 * j]);
            }
            printf("\n");
        }

        printf("P:\n");
        SM4_Decrypt_x8(in, in, sm4_key);
        for (int j = 0; j < 8; j++) {
            printf("\t");
            for (int i = 0; i < 16; i++) {
                printf("%02x ", in[i + 16 * j]);
            }
            printf("\n");
        }
        
        SM4_KeyDelete(sm4_key);
    }
    system("pause");
    return 0;
}
