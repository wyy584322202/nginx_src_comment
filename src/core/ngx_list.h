
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;	

struct ngx_list_part_s {		//����Ľڵ�
    void             *elts;			//�ڵ���������׵�ַ
    ngx_uint_t        nelts;		//�ڵ����Ѿ�ʹ���˵�(��ֵ�˵�Ԫ�ص�)����
    ngx_list_part_t  *next;			//�¸��ڵ�ĵ�ַ
};


typedef struct {					//����
    ngx_list_part_t  *last;			//�����β�ڵ�ĵ�ַ
    ngx_list_part_t   part;			//������׽ڵ�
    size_t            size;			//�ڵ�������Ԫ�صĴ�С
    ngx_uint_t        nalloc;		//ÿ���ڵ�����������Ԫ�ظ���
    ngx_pool_t       *pool;			//�ڴ��
} ngx_list_t;

//����һ������
//����nΪÿ���ڵ�����������Ԫ�ظ����� sizeΪ�ڵ�������Ԫ�صĴ�С����������ĵ�ַ
ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

//��ʼ������
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

//��������ķ���
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

//���ؿ������Ԫ�ص������еĵ�ַ
//������������е�β�ڵ��е����黹û��������Ͳ�������µĽڵ㣬������е�β�ڵ��Ѿ����˾����һ���µĽڵ�
//Ȼ����ӵ��µ�β�ڵ��������
//ʹ�÷���:
//ngx_str_t * str = ngx_list_push(testlist);
//if(str == NULL)
//	return NGX_ERROR;
//str->len = sizeof("hello");
//str->data = "hello";
void *ngx_list_push(ngx_list_t *list);



#endif /* _NGX_LIST_H_INCLUDED_ */
