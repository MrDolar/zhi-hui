/**
 * @file json.h
 * @brief 简易 JSON 解析器
 */

#ifndef JSON_H
#define JSON_H

#include "common.h"

/* JSON 类型 */
typedef enum {
    JSON_NULL = 0,
    JSON_BOOL,
    JSON_INT,
    JSON_FLOAT,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT,
} json_type_t;

/* JSON 节点 */
typedef struct json_node {
    char *key;
    json_type_t type;
    union {
        bool bool_val;
        int int_val;
        float float_val;
        char *str_val;
        struct json_node *child;
    };
    struct json_node *next;
} json_node_t;

/* 解析 JSON 字符串 */
json_node_t *json_parse(const char *str, size_t len);

/* 释放 JSON */
void json_free(json_node_t *root);

/* 查找节点 */
json_node_t *json_find(json_node_t *root, const char *key);

/* 获取值 */
const char *json_get_string(json_node_t *root, const char *key, const char *def);
int json_get_int(json_node_t *root, const char *key, int def);
bool json_get_bool(json_node_t *root, const char *key, bool def);
float json_get_float(json_node_t *root, const char *key, float def);

/* 数组操作 */
json_node_t *json_array_get(json_node_t *arr, int index);
int json_array_size(json_node_t *arr);

/* 生成 JSON（简易） */
int json_serialize(json_node_t *root, char *buf, size_t size);

/* 创建节点 */
json_node_t *json_create_object(void);
json_node_t *json_create_array(void);
json_node_t *json_create_string(const char *val);
json_node_t *json_create_int(int val);

/* 添加节点 */
int json_object_add(json_node_t *obj, const char *key, json_node_t *val);
int json_array_add(json_node_t *arr, json_node_t *val);

#endif /* JSON_H */
