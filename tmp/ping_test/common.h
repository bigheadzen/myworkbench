#ifndef COMMON_H__
#define COMMON_H_

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>

#include <uv.h>

using namespace std;

#define ASSERT(expr) \
    do { \
        assert((expr)); \
        if (!(expr)) { \
            fprintf(stderr, "ASSERT failed. %s\n", #expr); \
            exit(-1); \
        } \
    } while(0)

#endif

struct WriteReq {
    uv_write_t req;
    uv_buf_t buf;
};

