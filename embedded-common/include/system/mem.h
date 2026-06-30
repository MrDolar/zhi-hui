/**
 * @file mem.h
 * @brief 内存管理
 */

#ifndef MEM_H
#define MEM_H

#include "common.h"

/* 内存信息 */
typedef struct {
    size_t total;
    size_t used;
    size_t free;
    size_t max_free;    /* 最大连续空闲块 */
} mem_info_t;

/* 初始化 */
int mem_init(void *pool, size_t size);

/* 分配 */
void *mem_alloc(size_t size);

/* 释放 */
void mem_free(void *ptr);

/* 重新分配 */
void *mem_realloc(void *ptr, size_t size);

/* 分配并清零 */
void *mem_calloc(size_t count, size_t size);

/* 获取内存信息 */
void mem_get_info(mem_info_t *info);

/* 打印内存信息 */
void mem_dump(void);

/* 检查内存泄漏 */
bool mem_check_leak(void);

/* 内存池统计 */
size_t mem_get_used(void);
size_t mem_get_free(void);

/* 简化接口 */
#define MALLOC(size)        mem_alloc(size)
#define FREE(ptr)           mem_free(ptr)
#define CALLOC(n, size)     mem_calloc(n, size)
#define REALLOC(ptr, size)  mem_realloc(ptr, size)

#endif /* MEM_H */
