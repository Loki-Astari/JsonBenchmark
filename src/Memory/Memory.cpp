#pragma vera-pushoff
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#pragma vera-pop

#if     defined(__APPLE__)
#define THOR_MEMORY_INSERT 2
#elif   defined(__linux__)
#define THOR_MEMORY_INSERT 1
#else
#error  "You need to work out how to install these memory libraries before the standard C ones"
#endif

#include "MemoryConfig.h"
#include "Memory.h"
#include <stdlib.h>


bool MemoryInfo::active = false;
bool MemoryInfo::alreadyIn = false;
void MemoryInfo::alloc(void* ptr, std::size_t size, bool malloc)
{
    if (active && !alreadyIn && ptr && size)
    {
        alreadyIn = true;
        MemoryInfo& instance = getInstance();
        instance.countData[size].first++;
        instance.ptrInfo[ptr] = size;

        (malloc ? instance.mallocCount : instance.reallocCount)++;
        instance.currentSize += size;
        instance.peakSize = std::max(instance.peakSize, instance.currentSize);

        alreadyIn = false;
    }
}
void MemoryInfo::free(void* ptr)
{
    if (active && !alreadyIn && ptr)
    {
        alreadyIn = true;
        MemoryInfo& instance = getInstance();
        auto find = instance.ptrInfo.find(ptr);
        if (find != instance.ptrInfo.end())
        {
            std::size_t size = find->second;
            instance.countData[size].second++;

            instance.freeCount++;
            instance.currentSize -= size;
        }

        alreadyIn = false;
    }
}
MemoryInfo& MemoryInfo::getInstance()
{
    static MemoryInfo instance;
    return instance;
}

#if THOR_MEMORY_INSERT == 1
#include <dlfcn.h>
extern "C"
void free(void *ptr) noexcept
{
    void *(*libc_free)(void*) = (void *(*)(void*))dlsym(RTLD_NEXT, "free");
    libc_free(ptr);

    MemoryInfo::free(ptr);
}

extern "C"
void* malloc(size_t size) noexcept
{
    void *(*libc_malloc)(size_t) = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
    void* result = libc_malloc(size);

    MemoryInfo::alloc(result, size, true);

    return result;
}

extern "C"
void* realloc(void *ptr, size_t size) noexcept
{
    void *(*libc_realloc)(void*, size_t) = (void *(*)(void*, size_t))dlsym(RTLD_NEXT, "realloc");
    void* result = libc_realloc(ptr, size);

    MemoryInfo::free(ptr);
    MemoryInfo::alloc(result, size, false);

    return result;
}
#endif
#if THOR_MEMORY_INSERT == 2
extern "C"
void* mycalloc(size_t count, size_t size) noexcept
{
    void* result = calloc(count, size);

    MemoryInfo::alloc(result, count * size, true);

    return result;
}

extern "C"
void myfree(void *ptr) noexcept
{
    free(ptr);

    MemoryInfo::free(ptr);
}

extern "C"
void* mymalloc(size_t size) noexcept
{
    void* result = malloc(size);

    MemoryInfo::alloc(result, size, true);

    return result;
}

extern "C"
void* myrealloc(void *ptr, size_t size) noexcept
{
    void* result = realloc(ptr, size);

    MemoryInfo::free(ptr);
    MemoryInfo::alloc(result, size, false);

    return result;
}

extern "C"
void* myreallocf(void *ptr, size_t size) noexcept
{
    void* result = reallocf(ptr, size);

    MemoryInfo::free(ptr);
    MemoryInfo::alloc(result, size, false);

    return result;
}

extern "C"
void* myvalloc(size_t size) noexcept
{
    void* result = valloc(size);

    MemoryInfo::alloc(result, size, true);

    return result;
}
#pragma vera-pushoff
#define DYLD_INTERPOSE(_replacement,_replacee) \
   __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
            __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacement, (const void*)(unsigned long)&_replacee };
#pragma vera-pop

DYLD_INTERPOSE(mycalloc, calloc)
DYLD_INTERPOSE(myfree, free)
DYLD_INTERPOSE(mymalloc, malloc)
DYLD_INTERPOSE(myrealloc, realloc)
DYLD_INTERPOSE(myreallocf, reallocf)
DYLD_INTERPOSE(myvalloc, valloc)

#endif
