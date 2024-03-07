#ifndef SAFEL_MEMENV_HEADER
#define SAFEL_MEMENV_HEADER

#include <cstdlib>
#include <vector>

#include "iallocator.hpp"
#include "array.hpp"

namespace safel
{
    struct memenv;

    option<void*> memenv_alloc(memenv* env, size_t bytes);

    option<void*> memenv_realloc(memenv* env, void* block, size_t bytes);

    void memenv_free(memenv* env);

    constexpr iallocator_info memenv_allocator_info = allocator_info_init(
                [](void* allocator, size_t bytes) { return memenv_alloc((memenv*)allocator, bytes); },
                [](void* allocator, void* block, size_t bytes) { return memenv_realloc((memenv*)allocator, block, bytes); },
                [](void* allocator) { memenv_free((memenv*)allocator); });

    struct memenv
    {
        dynarray<void*> blocks;

        constexpr operator iallocator()
        {
            iallocator a = {
                a.allocator = this, 
                a.info = &memenv_allocator_info
            };

            return a;
        }
    };

    constexpr memenv memenv_new()
    {
        memenv p;
        p.blocks = dynarray_new<void*>(libc_allocator);
        return p;
    }
}

#endif