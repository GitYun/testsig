#ifndef HTOHL_TEST_H
#define HTOHL_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

static unsigned long ntohl_test(unsigned long u) {
    return ntohl(u);
}

#ifdef __cplusplus
}
#endif

#endif
