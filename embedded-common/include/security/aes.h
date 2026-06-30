/**
 * @file aes.h
 * @brief AES 加密
 */

#ifndef AES_H
#define AES_H

#include "common.h"

/* AES 密钥长度 */
typedef enum {
    AES_128 = 16,
    AES_192 = 24,
    AES_256 = 32,
} aes_key_len_t;

/* AES 模式 */
typedef enum {
    AES_ECB = 0,
    AES_CBC,
    AES_CTR,
} aes_mode_t;

/* AES 上下文 */
typedef struct {
    uint32_t rk[60];    /* 轮密钥 */
    uint8_t iv[16];     /* 初始向量 */
    aes_key_len_t key_len;
    aes_mode_t mode;
} aes_ctx_t;

/* 初始化 */
int aes_init(aes_ctx_t *ctx, const uint8_t *key, aes_key_len_t len, aes_mode_t mode, const uint8_t *iv);

/* 加密 */
int aes_encrypt(aes_ctx_t *ctx, const uint8_t *in, uint8_t *out, size_t len);

/* 解密 */
int aes_decrypt(aes_ctx_t *ctx, const uint8_t *in, uint8_t *out, size_t len);

/* 简化接口 */
int aes_encrypt_ecb(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len);
int aes_decrypt_ecb(const uint8_t *key, const uint8_t *in, uint8_t *out, size_t len);

/* PKCS7 填充 */
size_t pkcs7_pad(uint8_t *buf, size_t len, size_t block_size);
size_t pkcs7_unpad(uint8_t *buf, size_t len);

#endif /* AES_H */
