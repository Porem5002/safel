
#ifndef SAFEL_IALLOCATOR_HEADER
#define SAFEL_IALLOCATOR_HEADER

#include <cstdlib>
#include <cstdint>
#include <cassert>

#include "alloc.hpp"
#include "option.hpp"

namespace safel
{
    using alloc_func = option<void*> (*)(void* allocator, size_t size);
    using realloc_func = option<void*> (*)(void* allocator, void* block, size_t size);    
    using free_single_func = void (*)(void* allocator, void* block);
    using free_all_func = void (*)(void* allocator);

    enum iallocator_mask : uint8_t
    {
        ALLOCATOR_FLAG_FREE_SINGLE = 0x01,
        ALLOCATOR_FLAG_FREE_ALL = 0x02,
        ALLOCATOR_FLAG_FULL_FREE = ALLOCATOR_FLAG_FREE_SINGLE | ALLOCATOR_FLAG_FREE_ALL,
    };

    struct iallocator_info
    {
        alloc_func alloc;
        realloc_func realloc;
        free_single_func free_single;
        free_all_func free_all;
        iallocator_mask mask;
    };

    struct iallocator
    {
        void* allocator;
        const iallocator_info* info;

        constexpr iallocator_mask mask() const
        {
            return info->mask;
        }

        constexpr option<void*> alloc(size_t size) const
        {
            return info->alloc(allocator, size);
        }

        constexpr option<void*> realloc(void* block, size_t size) const
        {
            return info->realloc(allocator, block, size);
        }

        constexpr void free_single(void* block) const
        {
            info->free_single(allocator, block);
        }

        constexpr void free_all() const
        {
            info->free_all(allocator);
        }
    };

    constexpr iallocator_info allocator_info_init(alloc_func alloc, realloc_func realloc, free_single_func free_single)
    {
        iallocator_info info = {
            alloc,
            realloc,
            free_single,
            [](void*) { assert(false); },
            ALLOCATOR_FLAG_FREE_SINGLE,
        };

        return info;
    }

    constexpr iallocator_info allocator_info_init(alloc_func alloc, realloc_func realloc, free_all_func free_all)
    {
        iallocator_info info = {
            alloc,
            realloc,
            [](void*, void*) { assert(false); },
            free_all,
            ALLOCATOR_FLAG_FREE_ALL
        };

        return info;
    }

    constexpr iallocator_info allocator_info_init(
        alloc_func alloc, realloc_func realloc, free_single_func free_single, free_all_func free_all)
    {
        iallocator_info info = {
            alloc,
            realloc,
            free_single,
            free_all,
            ALLOCATOR_FLAG_FULL_FREE
        };

        return info;
    }

    constexpr iallocator_info libc_allocator_info = allocator_info_init(
        [](void*, size_t bytes) { return safel::malloc_bytes(bytes); },
        [](void*, void* block, size_t bytes) { return safel::realloc_bytes(block, bytes); },
        [](void*, void* block) { return safel::free(block); });

    constexpr iallocator libc_allocator = { nullptr, &libc_allocator_info };
}

#endif