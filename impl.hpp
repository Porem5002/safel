#ifndef SAFEL_IMPL_HEADER
#define SAFEL_IMPL_HEADER

#include <cassert>

#include "memenv.hpp"
#include "option.hpp"

namespace safel
{
    option<void*> memenv_alloc(memenv* env, size_t bytes)
    {
        if(!dynarray_reserve(env->blocks))
            return option<void*>::none();

        void* block = malloc(bytes);
        
        if(block != nullptr)
        {
            dynarray_add_unsafe(env->blocks, block);
            return option<void*>::some(block);
        }

        return option<void*>::none();
    }

    inline option<void*> memenv_realloc(memenv* env, void* block, size_t bytes)
    {
        if(block == nullptr)
            return memenv_alloc(env, bytes);

        bool found = false;
        size_t index;

        for(size_t i = 0; i < env->blocks.size; i++)
        {
            if(env->blocks[i] == block)
            {
                found = true;
                index = i;
                break;
            }
        }

        if(!found) return option<void*>::none();
        
        void* new_block = realloc(block, bytes);

        if(new_block == nullptr)
            return option<void*>::none();

        env->blocks[index] = new_block;
        return option<void*>::some(new_block);
    }

    void memenv_free(memenv* env)
    {
        for(size_t i = 0; i < env->blocks.size; i++)
            free(env->blocks[i]);

        dynarray_free(env->blocks);
    }
}

#endif