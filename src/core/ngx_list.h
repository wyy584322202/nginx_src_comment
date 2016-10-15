
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;	

struct ngx_list_part_s {		//链表的节点
    void             *elts;			//节点中数组的首地址
    ngx_uint_t        nelts;		//节点中已经使用了的(赋值了的元素的)个数
    ngx_list_part_t  *next;			//下个节点的地址
};


typedef struct {					//链表
    ngx_list_part_t  *last;			//链表的尾节点的地址
    ngx_list_part_t   part;			//链表的首节点
    size_t            size;			//节点中数组元素的大小
    ngx_uint_t        nalloc;		//每个节点中数组的最大元素个数
    ngx_pool_t       *pool;			//内存池
} ngx_list_t;

//创建一个链表
//参数n为每个节点中数组的最大元素个数， size为节点中数组元素的大小，返回链表的地址
ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

//初始化链表
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}

//遍历链表的方法
/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */

//返回可以填充元素的链表中的地址
//如果链表中现有的尾节点中的数组还没有填充满就不用添加新的节点，如果现有的尾节点已经满了就添加一个新的节点
//然后添加到新的尾节点的数组中
//使用范例:
//ngx_str_t * str = ngx_list_push(testlist);
//if(str == NULL)
//	return NGX_ERROR;
//str->len = sizeof("hello");
//str->data = "hello";
void *ngx_list_push(ngx_list_t *list);



#endif /* _NGX_LIST_H_INCLUDED_ */
