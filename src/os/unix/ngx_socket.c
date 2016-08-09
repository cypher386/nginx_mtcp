
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <mtcp_api.h>

extern mctx_t mctx;


/*
 * ioctl(FIONBIO) sets a non-blocking mode with the single syscall
 * while fcntl(F_SETFL, O_NONBLOCK) needs to learn the current state
 * using fcntl(F_GETFL).
 *
 * ioctl() and fcntl() are syscalls at least in FreeBSD 2.x, Linux 2.2
 * and Solaris 7.
 *
 * ioctl() in Linux 2.4 and 2.6 uses BKL, however, fcntl(F_SETFL) uses it too.
 */


#if (NGX_HAVE_FIONBIO)

int
ngx_nonblocking(ngx_socket_t s)
{
    int  nb;

    nb = 1;
#ifdef USE_MTCP
	return mtcp_setsock_nonblock(mctx,s);
#else
    return ioctl(s, FIONBIO, &nb);
#endif
}


int
ngx_blocking(ngx_socket_t s)
{
    int  nb;

    nb = 0;
#ifdef USE_MTCP
	return mtcp_setsock_nonblock(mctx,s);
#else
    return ioctl(s, FIONBIO, &nb);
#endif
}

#endif


#if (NGX_FREEBSD)

int
ngx_tcp_nopush(ngx_socket_t s)
{
    int  tcp_nopush;

    tcp_nopush = 1;
#ifdef USE_MTCP
	return mtcp_setsockopt(mctx,s, IPPROTO_TCP, TCP_NOPUSH,
                      (const void *) &tcp_nopush, sizeof(int));
#else

    return setsockopt(s, IPPROTO_TCP, TCP_NOPUSH,
                      (const void *) &tcp_nopush, sizeof(int));
#endif
}


int
ngx_tcp_push(ngx_socket_t s)
{
    int  tcp_nopush;

    tcp_nopush = 0;
#ifdef USE_MTCP
	return mtcp_setsockopt(mctx,s, IPPROTO_TCP, TCP_NOPUSH,
                      (const void *) &tcp_nopush, sizeof(int));
#else

    return setsockopt(s, IPPROTO_TCP, TCP_NOPUSH,
                      (const void *) &tcp_nopush, sizeof(int));
#endif
}

#elif (NGX_LINUX)


int
ngx_tcp_nopush(ngx_socket_t s)
{
    int  cork;

    cork = 1;
#ifdef USE_MTCP
	return mtcp_setsockopt(mctx,s, IPPROTO_TCP, TCP_CORK,
                      (const void *) &cork, sizeof(int));
#else

    return setsockopt(s, IPPROTO_TCP, TCP_CORK,
                      (const void *) &cork, sizeof(int));
#endif
}


int
ngx_tcp_push(ngx_socket_t s)
{
    int  cork;

    cork = 0;
#ifdef USE_MTCP
	return mtcp_setsockopt(mctx,s, IPPROTO_TCP, TCP_CORK,
                      (const void *) &cork, sizeof(int));
#else

    return setsockopt(s, IPPROTO_TCP, TCP_CORK,
                      (const void *) &cork, sizeof(int));
#endif
}

#else

int
ngx_tcp_nopush(ngx_socket_t s)
{
    return 0;
}


int
ngx_tcp_push(ngx_socket_t s)
{
    return 0;
}

#endif
