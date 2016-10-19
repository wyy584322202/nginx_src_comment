
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

/*����������q��ʼ��*/
#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q

/*��������Ƿ�Ϊ�գ��������true��ʾ�����ǿյ�*/
#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)

/*��Ԫ��x���뵽��������h��ͷ��*/
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x

/*[Ԫ�ط���]*/
#define ngx_queue_insert_after   ngx_queue_insert_head

/*β������*/
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x

/*���ص�һ��Ԫ�صĽṹ��ָ��*/
#define ngx_queue_head(h)                                                     \
    (h)->next

/*������������һ��Ԫ��*/
#define ngx_queue_last(h)                                                     \
    (h)->prev

/*�������������Ľṹ��ָ��*/
#define ngx_queue_sentinel(h)                                                 \
    (h)

/*[Ԫ�ط���]������һ��Ԫ��*/
#define ngx_queue_next(q)                                                     \
    (q)->next

/*[Ԫ�ط���]����ǰһ��Ԫ��*/
#define ngx_queue_prev(q)                                                     \
    (q)->prev


#if (NGX_DEBUG)
/*�Ƴ�Ԫ��x*/
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else
/*�Ƴ�Ԫ��x*/
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif

/*qΪ�ָ�㣬��h�����ֳ�h��n��������q���Ϊn������׽ڵ�*/
#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;

/*�ϲ�h��n����*/
#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;

/*[Ԫ�ط���],q��Ԫ�ؽṹ��,type�ǰ�����Ԫ�صĽṹ������ͣ�link��qԪ�ؽṹ�������*/
/*��ȡ����Ԫ�ؽṹ��Ľṹ���׵�ַ(���������ʵ��linux�ں�����һ����һ���ṹ��*/
/*ֻҪ������Ԫ�ؽṹ��Ե�������ӵ�����������)*/
#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))

/*������������Ԫ��*/
ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);

/*����cmp����Ҫ�Լ�ʵ��*/
void ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
