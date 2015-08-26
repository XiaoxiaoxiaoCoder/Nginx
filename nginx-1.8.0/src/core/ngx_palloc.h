
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)             //页最大大小

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)                    //默认页大小

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

/*
 * ngx_pool_cleanup_s　结构体定义
 */
struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;          //cleanup 函数指针
    void                 *data;             //cleanup 的数据指针
    ngx_pool_cleanup_t   *next;             //下一个clanup
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

/*
 * ngx_pool_large_s 结构体定义
 */
struct ngx_pool_large_s {
    ngx_pool_large_t     *next;             //下一个 ngx_pool_large_s 指针
    void                 *alloc;            //内存指针
};

/*
 * ngx_pool_data_t 结构体定义
 */
typedef struct {
    u_char               *last;             //空闲区起始地址
    u_char               *end;              //data区结束地址
    ngx_pool_t           *next;             //ngx_pool_s　指针
    ngx_uint_t            failed;           //失败次数
} ngx_pool_data_t;

/*
 * ngx_pool_s 结构体定义
 */
struct ngx_pool_s {
    ngx_pool_data_t       d;                //数据区
    size_t                max;              //当前空间大小
    ngx_pool_t           *current;          //当前 pool 指针
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;            //large 块内存起始指针
    ngx_pool_cleanup_t   *cleanup;          //clean handler 链表头指针
    ngx_log_t            *log;              //日志钩子函数
};

/*
 * ngx_pool_cleanup_file_t 结构体定义
 */
typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);

ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
