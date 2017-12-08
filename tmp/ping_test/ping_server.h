#ifndef PING_SERVER_H_
#define PING_SERVER_H_

#include <uv.h>

class PingServer {
public:
    int run();
    int stop();

    void connect_cb(uv_stream_t *server, int status);
    void read_cb(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    void write_cb(uv_write_t *req, int status);

private:
    uv_tcp_t *tcp_server_;
    uv_loop_t *loop_;
};

#endif
