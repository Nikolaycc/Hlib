#ifndef HLIB_H
#define HLIB_H

#include <unistd.h>
#include <string.h>
// std
#include <stdio.h>
#include <stdlib.h>
// uv
#include <uv.h>
// constants
const int CLOSE = 0;
const int DONE = 1;
const int ERROR = -1;
const int SOCKERR = -2;
const int HTTPERR = -3;
const int BACKLOG = SOMAXCONN;

// uv
typedef struct http Http;
typedef struct w_req write_req_t;

uv_loop_t *loop;
uv_buf_t b;

struct http {
    uv_tcp_t ser;
    struct sockaddr_in addr;
};

struct w_req {
    uv_write_t req;
    uv_buf_t buf;
};

Http* h_default_server() {
    Http* e = malloc(sizeof(Http));
    loop = uv_default_loop();

    return e;
}

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    printf("%s", b.base);
    free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
        b = req->buf;
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    }

    free(buf->base);
}

void on_new_conn(uv_stream_t* ser, int status) {
    printf("Connected\n");

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    
    if (uv_accept(ser, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        printf("Disconnected\n");
        uv_close((uv_handle_t*) client, NULL);
    }
}

int h_listen(Http* h, int port, void (*cb)()) {
    cb();
    uv_tcp_init(loop, &h->ser);

    uv_ip4_addr("0.0.0.0", port, &h->addr);

    uv_tcp_bind(&h->ser, (const struct sockaddr*)&h->addr, 0);
    int l = uv_listen((uv_stream_t*) &h->ser, BACKLOG, on_new_conn);
    if (l) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(l));
        return SOCKERR;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}
// uv end
#endif
