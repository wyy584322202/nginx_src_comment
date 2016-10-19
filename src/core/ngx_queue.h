
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_


typedef struct ngx_queue_s  ngx_queue_t;

struct ngx_queue_s {
    ngx_queue_t  *prev;
    ngx_queue_t  *next;
};

/*将链表容器q初始化*/
#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q

/*检测容器是否为空，如果返回true表示链表是空的*/
#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)

/*将元素x插入到链表容器h的头部*/
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x

/*[元素方法]*/
#define ngx_queue_insert_after   ngx_queue_insert_head

/*尾部插入*/
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x

/*返回第一个元素的结构体指针*/
#define ngx_queue_head(h)                                                     \
    (h)->next

/*返回链表的最后一个元素*/
#define ngx_queue_last(h)                                                     \
    (h)->prev

/*返回链表容器的结构体指针*/
#define ngx_queue_sentinel(h)                                                 \
    (h)

/*[元素方法]返回下一个元素*/
#define ngx_queue_next(q)                                                     \
    (q)->next

/*[元素方法]返回前一个元素*/
#define ngx_queue_prev(q)                                                     \
    (q)->prev


#if (NGX_DEBUG)
/*移除元素x*/
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else
/*移除元素x*/
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif

/*q为分割点，将h链表拆分成h和n两个链表，q会成为n链表的首节点*/
#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;

/*合并h和n链表*/
#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;

/*[元素方法],q是元素结构体,type是包含该元素的结构体的类型，link是q元素结构体的名字*/
/*获取包含元素结构体的结构体首地址(这个链表其实和linux内核链表一样，一个结构体*/
/*只要把链表元素结构体吃掉，就添加到该链表中了)*/
#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))

/*返回链表中心元素*/
ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);

/*排序，cmp函数要自己实现*/
void ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
