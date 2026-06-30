/**
 * @file http.h
 * @brief HTTP 客户端
 */

#ifndef HTTP_H
#define HTTP_H

#include "common.h"

/* HTTP 方法 */
typedef enum {
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
} http_method_t;

/* HTTP 响应 */
typedef struct {
    int status_code;
    char *body;
    size_t body_len;
    char *headers;
} http_resp_t;

/* HTTP 请求配置 */
typedef struct {
    const char *url;
    http_method_t method;
    const char *headers;
    const char *body;
    size_t body_len;
    uint32_t timeout_ms;
} http_req_t;

/* HTTP 回调 */
typedef void (*http_cb)(http_resp_t *resp, void *arg);

/* 同步请求 */
int http_request(const http_req_t *req, http_resp_t *resp);

/* 释放响应 */
void http_resp_free(http_resp_t *resp);

/* 简化接口 */
INLINE int http_get(const char *url, http_resp_t *resp) {
    http_req_t req = {
        .url = url,
        .method = HTTP_GET,
        .timeout_ms = 10000,
    };
    return http_request(&req, resp);
}

INLINE int http_post(const char *url, const char *data, http_resp_t *resp) {
    http_req_t req = {
        .url = url,
        .method = HTTP_POST,
        .body = data,
        .body_len = strlen(data),
        .headers = "Content-Type: application/json",
        .timeout_ms = 10000,
    };
    return http_request(&req, resp);
}

/* 下载文件 */
int http_download(const char *url, const char *path, http_cb progress_cb);

/* URL 编码/解码 */
size_t url_encode(char *dst, size_t dst_len, const char *src);
size_t url_decode(char *dst, size_t dst_len, const char *src);

#endif /* HTTP_H */
