#ifndef SAFEL_IMPL_HEADER
#define SAFEL_IMPL_HEADER

#include <cassert>

#include "memenv.hpp"
#include "alloc.hpp"
#include "option.hpp"

namespace safel
{
    option<void*> memenv_alloc(memenv* env, size_t bytes)
    {
        if(!dynarray_reserve(env->blocks))
            return safel::none;

        option<void*> block = safel::malloc_bytes(bytes);
        
        if(block.is_some())
            dynarray_add_unsafe(env->blocks, block.unwrap());

        return block;
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

        if(!found) return safel::none;
        
        option<void*> new_block = safel::realloc_bytes(block, bytes);

        if(new_block.is_some())
            env->blocks[index] = new_block.unwrap();
        
        return new_block;
    }

    void memenv_free(memenv* env)
    {
        for(size_t i = 0; i < env->blocks.size; i++)
            free(env->blocks[i]);

        dynarray_free(env->blocks);
    }
}

#endif