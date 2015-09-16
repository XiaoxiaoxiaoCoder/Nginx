
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHMEM_H_INCLUDED_
#define _NGX_SHMEM_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/*
 * ngx_shm_t　共享内存结构体定义
 */
typedef struct {
    u_char      *addr;      //内存区起始地址
    size_t       size;      //内存大小
    ngx_str_t    name;      //内存名字
    ngx_log_t   *log;       //记录日志的 ngx_log_t 对象
    ngx_uint_t   exists;    //表示共享内存是否已经分配过得表示位，为1时表示已经存在/* unsigned  exists:1;  */
} ngx_shm_t;


ngx_int_t ngx_shm_alloc(ngx_shm_t *shm);
void ngx_shm_free(ngx_shm_t *shm);


#endif /* _NGX_SHMEM_H_INCLUDED_ */
