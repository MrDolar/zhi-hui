/**
 * @file sha.h
 * @brief SHA 哈希算法
 */

#ifndef SHA_H
#define SHA_H

#include "common.h"

/* SHA1 */
typedef struct {
    uint32_t state[5];
    uint64_t count;
    uint8_t buffer[64];
} sha1_ctx_t;

void sha1_init(sha1_ctx_t *ctx);
void sha1_update(sha1_ctx_t *ctx, const uint8_t *data, size_t len);
void sha1_final(sha1_ctx_t *ctx, uint8_t hash[20]);
void sha1(const uint8_t *data, size_t len, uint8_t hash[20]);

/* SHA256 */
typedef struct {
    uint32_t state[8];
    uint64_t count;
    uint8_t buffer[64];
} sha256_ctx_t;

void sha256_init(sha256_ctx_t *ctx);
void sha256_update(sha256_ctx_t *ctx, const uint8_t *data, size_t len);
void sha256_final(sha256_ctx_t *ctx, uint8_t hash[32]);
void sha256(const uint8_t *data, size_t len, uint8_t hash[32]);

/* HMAC */
void hmac_sha256(const uint8_t *key, size_t key_len,
                 const uint8_t *data, size_t data_len,
                 uint8_t hash[32]);

/* 哈希转字符串 */
void sha256_to_hex(const uint8_t *hash, char *hex, size_t len);

/* MD5 */
typedef struct {
    uint32_t state[4];
    uint64_t count;
    uint8_t buffer[64];
} md5_ctx_t;

void md5_init(md5_ctx_t *ctx);
void md5_update(md5_ctx_t *ctx, const uint8_t *data, size_t len);
void md5_final(md5_ctx_t *ctx, uint8_t hash[16]);
void md5(const uint8_t *data, size_t len, uint8_t hash[16]);

#endif /* SHA_H */
