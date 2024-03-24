#ifndef SAFEL_ALLOC_HEADER
#define SAFEL_ALLOC_HEADER

#include <cstdlib>
#include <iostream>

#include "option.hpp"

namespace safel
{
    inline option<void*> malloc_bytes(size_t bytes)
    {
        void* p = std::malloc(bytes);
        return p != nullptr ? safel::some(p) : safel::none;
    }

    inline option<void*> realloc_bytes(void* block, size_t new_bytes)
    {
        void* p = std::realloc(block, new_bytes);
        return p != nullptr ? safel::some(p) : safel::none;
    }

    inline void* xmalloc_bytes(size_t bytes, const char* msg = "FATAL ERROR: Ran out of memory!")
    {
        option<void*> p = malloc_bytes(bytes);

        if(p.is_none())
        {
            std::cout << msg << std::endl;
            abort();
        }

        return p.unwrap();
    }

    template<typename T>
    option<T*> malloc(size_t new_count = 1)
    {
        T* p = (T*)std::malloc(sizeof(T) * new_count);
        return p != nullptr ? safel::some(p) : safel::none;
    }

    template<typename T>
    option<T*> realloc(T* block, size_t new_count)
    {
        T* p = (T*)std::realloc(block, sizeof(T) * new_count);
        return p != nullptr ? safel::some(p) : safel::none;
    }

    template<typename T>
    T* xmalloc(size_t count = 1, const char* msg = "FATAL ERROR: Ran out of memory!")
    {
        option<T*> p = malloc<T>(count);

        if(p.is_none())
        {
            std::cout << msg << std::endl;
            abort();
        }

        return p.unwrap();
    }

    inline void free(void* block)
    {
        return std::free(block); 
    }
}

#endif