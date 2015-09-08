
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_CONFIG_H_INCLUDED_
#define _NGX_HTTP_CONFIG_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    /*
     *所有模块的main config 数组
     */
    void        **main_conf;

    /*
     *所有模块的 server config 数组
     */
    void        **srv_conf;

    /*
     *所哟模块的location config 数组
     */
    void        **loc_conf;
} ngx_http_conf_ctx_t;


typedef struct {
    /*
     *在解析配置文件中 http{} 配置块前调用
     */
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);

    /*
     *在解析配置文件中http{} 配置块后调用
     */
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);

    /*
     *创建http模块的 main config
     */
    void       *(*create_main_conf)(ngx_conf_t *cf);

    /*
     *初始化http模块的 main config
     */
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    /*
     *创建http模块的 server config
     */
    void       *(*create_srv_conf)(ngx_conf_t *cf);

    /*
     *合并http模块中的server config，用于实现server config到main config的指令继承、覆盖
     */
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    /*
     *创建http模块的 location config
     */
    void       *(*create_loc_conf)(ngx_conf_t *cf);

    /*
     *合并http模块的location config，用于实现 location config到 server config的指令继承、覆盖
     */
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;


#define NGX_HTTP_MODULE           0x50545448   /* "HTTP" */

#define NGX_HTTP_MAIN_CONF        0x02000000
#define NGX_HTTP_SRV_CONF         0x04000000
#define NGX_HTTP_LOC_CONF         0x08000000
#define NGX_HTTP_UPS_CONF         0x10000000
#define NGX_HTTP_SIF_CONF         0x20000000
#define NGX_HTTP_LIF_CONF         0x40000000
#define NGX_HTTP_LMT_CONF         0x80000000


#define NGX_HTTP_MAIN_CONF_OFFSET  offsetof(ngx_http_conf_ctx_t, main_conf)
#define NGX_HTTP_SRV_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, srv_conf)
#define NGX_HTTP_LOC_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, loc_conf)


#define ngx_http_get_module_main_conf(r, module)                             \
    (r)->main_conf[module.ctx_index]
#define ngx_http_get_module_srv_conf(r, module)  (r)->srv_conf[module.ctx_index]
#define ngx_http_get_module_loc_conf(r, module)  (r)->loc_conf[module.ctx_index]


#define ngx_http_conf_get_module_main_conf(cf, module)                        \
    ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]
#define ngx_http_conf_get_module_srv_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->srv_conf[module.ctx_index]
#define ngx_http_conf_get_module_loc_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->loc_conf[module.ctx_index]

#define ngx_http_cycle_get_module_main_conf(cycle, module)                    \
    (cycle->conf_ctx[ngx_http_module.index] ?                                 \
        ((ngx_http_conf_ctx_t *) cycle->conf_ctx[ngx_http_module.index])      \
            ->main_conf[module.ctx_index]:                                    \
        NULL)


#endif /* _NGX_HTTP_CONFIG_H_INCLUDED_ */
