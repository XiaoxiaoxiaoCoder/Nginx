
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>

/*
 * 创建一个 ngx_list, 按照指定的元素大小size和个数n
 */
ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, sizeof(ngx_list_t));
    if (list == NULL) {
        return NULL;
    }

    if (ngx_list_init(list, pool, n, size) != NGX_OK) {
        return NULL;
    }

    return list;
}

/*
 * push 一个元素至list中， 实际只返回数据存储区的指针
 */
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;                                                 //尾节点 ngx_list_part_t

    if (last->nelts == l->nalloc) {                                 //元素已满

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));        //再分配一个 ngx_list_part_t
        if (last == NULL) {
            return NULL;
        }

        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);      //分配数据区
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;                                       //插入尾部
        l->last = last;                                             //重置尾节点
    }

    elt = (char *) last->elts + l->size * last->nelts;              //实际数据区
    last->nelts++;                                                  //计数+1

    return elt;
}
