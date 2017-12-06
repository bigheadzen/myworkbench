#include <cstdio>
#include <uv.h>

#include "constant.h"

class PingServer {
public:
    int start() {
        uv_tcp_t* tcp_server;
        struct sockaddr_in addr;
        int r;

        r = uv_ip4_addr("0.0.0.0", SERVER_PORT, &addr);
        assert(r == 0);

        tcp_server = (uv_tcp_t*) malloc(sizeof(*tcp_server));
        assert(tcp_server != NULL);

        r = uv_tcp_init(uv_default_loop(), tcp_server);
        assert(r == 0);

        r = uv_tcp_bind(tcp_server, (const struct sockaddr*)&addr, 0);
        assert(r == 0);

        r = uv_listen((uv_stream_t*)tcp_server, SOMAXCONN, on_connection);
        assert(r == 0);
    }
};

int main(int argc, char *argv[]) {
    printf("Hello world\n");
    return 0;
}
