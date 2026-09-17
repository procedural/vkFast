#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

void free(void *ptr) {
    if (!ptr) return;

    // Call the real glibc free function
    void (*real_free)(void*) = dlsym(RTLD_NEXT, "free");
    //real_free(ptr);
}
