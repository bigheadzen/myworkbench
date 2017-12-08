#include "common.h"
#include "constant.h"

#include "ping_server.h"

static PingServer g_ping_server;

static void on_connection(uv_stream_t *server, int status) {
    auto tcp_server = (uv_tcp_t *)server;
    ASSERT(tcp_server->data);

    ((PingServer *)tcp_server->data)->connect_cb(server, status);
}

void on_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
    *buf = uv_buf_init((char*) malloc(size), size);
    ASSERT(buf->base);
}

static void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    auto conn = (uv_tcp_t *)client;
    ASSERT(conn->data);

    ((PingServer *)conn->data)->read_cb(client, nread,  buf);
}

static void on_write(uv_write_t *req, int status) {
    ASSERT(req->data);

    ((PingServer *)req->data)->write_cb(req, status);
}

static void on_close(uv_handle_t *handle) {
    free(handle);
}

int PingServer::run() {
    uv_tcp_t* tcp_server;
    struct sockaddr_in addr;
    int r;
    auto loop = uv_default_loop();

    r = uv_ip4_addr("0.0.0.0", SERVER_PORT, &addr);
    ASSERT(r == 0);

    tcp_server = (uv_tcp_t*) malloc(sizeof(*tcp_server));
    ASSERT(tcp_server);
    tcp_server->data = (void *)this;

    r = uv_tcp_init(loop, tcp_server);
    ASSERT(r == 0);

    r = uv_tcp_bind(tcp_server, (const struct sockaddr*)&addr, 0);
    ASSERT(r == 0);

    r = uv_listen((uv_stream_t*)tcp_server, SOMAXCONN, on_connection);
    ASSERT(r == 0);

    tcp_server_ = tcp_server;
    loop_ = loop;

    fprintf(stdout, "Listening on port: %d\n", SERVER_PORT);

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}

void PingServer::connect_cb(uv_stream_t *server, int status) {
    if (status) {
        fprintf(stderr, "Error in connect_cb, status: %d\n", status);
        return;
    }

    auto conn = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop_, conn);
    conn->data = (void *)this;

    int r = uv_accept(server, (uv_stream_t *)conn);
    if (r) {
        fprintf(stderr, "Failed to accept new client connection, error: %d\n", r);
        uv_close((uv_handle_t *)conn, on_close);
        free(conn);
        return;
    }

    uv_read_start((uv_stream_t *)conn, on_alloc, on_read);
}

void PingServer::read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread == UV_EOF) {
      uv_close((uv_handle_t*) client, on_close);
      return;
    } 

    if (nread == 0) {
        free(buf->base);
        return;
    }

    fprintf(stdout, "%ld bytes read\n", nread);

    WriteReq *wr = (WriteReq*) malloc(sizeof(WriteReq));
    wr->buf =  uv_buf_init(buf->base, nread);

    wr->req.data = (void *) this;

    uv_write(&wr->req, client, &wr->buf, 1/*nbufs*/, on_write);
}

void PingServer::write_cb(uv_write_t *req, int status) {
    WriteReq* wr;
    wr = (WriteReq*) req;

    if (status) {
        fprintf(stderr, "Failed to write. status: %d\n", status);
    } else {
        int written = wr->buf.len;
        ASSERT(wr->req.type == UV_WRITE);
        fprintf(stdout, "%d bytes written\n", written);
    }


    /* Free the read/write buffer and the request */
    free(wr->buf.base);
    free(wr);
}

int main(int argc, char *argv[]) {
    fprintf(stdout, "Start PingServer, port: %d\n", SERVER_PORT);
    g_ping_server.run();
    return 0;
}
