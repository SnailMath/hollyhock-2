#include <stddef.h>
#include <sdk/os/mem.hpp>

void *operator new(size_t size) {
    return Mem_Malloc(size);
}

void *operator new[](size_t size) {
    return Mem_Malloc(size);
}

void operator delete(void *p) {
    Mem_Free(p);
}

void operator delete(void *p, size_t size [[maybe_unused]]) {
    Mem_Free(p);
}

void operator delete[](void *p) {
    Mem_Free(p);
}

void operator delete[](void *p, size_t size [[maybe_unused]]) {
    Mem_Free(p);
}
