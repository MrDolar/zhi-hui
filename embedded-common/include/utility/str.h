/**
 * @file str.h
 * @brief 字符串工具函数
 */

#ifndef STR_H
#define STR_H

#include "common.h"
#include <string.h>
#include <stdlib.h>

/* 安全拷贝 */
INLINE size_t str_copy(char *dst, size_t dst_len, const char *src) {
    size_t i;
    for (i = 0; i < dst_len - 1 && src[i]; i++)
        dst[i] = src[i];
    dst[i] = '\0';
    return i;
}

/* 拼接 */
INLINE size_t str_append(char *dst, size_t dst_len, const char *src) {
    size_t len = strlen(dst);
    return str_copy(dst + len, dst_len - len, src);
}

/* 格式化 */
size_t str_format(char *buf, size_t len, const char *fmt, ...);

/* 查找子串 */
INLINE const char *str_find(const char *str, const char *sub) {
    return strstr(str, sub);
}

/* 以...开头 */
INLINE bool str_startswith(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

/* 以...结尾 */
INLINE bool str_endswith(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    return str_len >= suffix_len && strcmp(str + str_len - suffix_len, suffix) == 0;
}

/* 去除空白 */
char *str_trim(char *str);

/* 分割字符串 */
size_t str_split(char *str, const char *delim, char **tokens, size_t max_tokens);

/* 转换 */
int str_to_int(const char *str, int def);
float str_to_float(const char *str, float def);
bool str_to_bool(const char *str);

/* 整数转字符串 */
char *int_to_str(int val, char *buf, size_t len);

/* 浮点数转字符串 */
char *float_to_str(float val, uint8_t decimals, char *buf, size_t len);

/* 十六进制 */
char *hex_to_str(const uint8_t *hex, size_t len, char *str, size_t str_len);
size_t str_to_hex(const char *str, uint8_t *hex, size_t hex_len);

/* 大小写转换 */
void str_tolower(char *str);
void str_toupper(char *str);

/* 比较（忽略大小写） */
INLINE int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int diff = tolower(*s1) - tolower(*s2);
        if (diff) return diff;
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

#endif /* STR_H */
