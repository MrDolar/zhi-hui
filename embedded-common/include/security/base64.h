/**
 * @file base64.h
 * @brief Base64 编解码
 */

#ifndef BASE64_H
#define BASE64_H

#include "common.h"

/* 编码 */
size_t base64_encode(const uint8_t *in, size_t in_len, char *out, size_t out_len);

/* 解码 */
size_t base64_decode(const char *in, uint8_t *out, size_t out_len);

/* 计算编码后长度 */
INLINE size_t base64_encode_len(size_t in_len) {
    return (in_len + 2) / 3 * 4 + 1;
}

/* 计算解码后长度 */
INLINE size_t base64_decode_len(const char *in) {
    size_t len = strlen(in);
    while (len > 0 && in[len-1] == '=') len--;
    return len * 3 / 4;
}

/* URL 安全 Base64 */
size_t base64url_encode(const uint8_t *in, size_t in_len, char *out, size_t out_len);
size_t base64url_decode(const char *in, uint8_t *out, size_t out_len);

#endif /* BASE64_H */
