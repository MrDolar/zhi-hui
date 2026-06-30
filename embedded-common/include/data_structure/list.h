/**
 * @file list.h
 * @brief 双向链表
 */

#ifndef LIST_H
#define LIST_H

#include "common.h"

/* 链表节点 */
typedef struct list_node {
    struct list_node *prev;
    struct list_node *next;
} list_node_t;

/* 初始化节点 */
#define LIST_NODE_INIT(name) { &(name), &(name) }

/* 静态链表头 */
#define LIST_HEAD(name) list_node_t name = LIST_NODE_INIT(name)

/* 初始化 */
INLINE void list_init(list_node_t *node) {
    node->prev = node->next = node;
}

/* 插入节点（尾部） */
INLINE void list_add(list_node_t *head, list_node_t *node) {
    node->next = head;
    node->prev = head->prev;
    head->prev->next = node;
    head->prev = node;
}

/* 插入节点（头部） */
INLINE void list_add_head(list_node_t *head, list_node_t *node) {
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

/* 删除节点 */
INLINE void list_del(list_node_t *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node->next = node;
}

/* 判断是否为空 */
INLINE bool list_is_empty(const list_node_t *head) {
    return head->next == head;
}

/* 遍历宏 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, tmp, head) \
    for (pos = (head)->next, tmp = pos->next; pos != (head); \
         pos = tmp, tmp = pos->next)

/* 获取容器 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/* 获取第一个元素 */
#define list_first_entry(head, type, member) \
    list_entry((head)->next, type, member)

/* 获取链表长度 */
INLINE size_t list_len(const list_node_t *head) {
    size_t count = 0;
    list_node_t *pos;
    list_for_each(pos, head) count++;
    return count;
}

#endif /* LIST_H */
