#ifndef SAFEL_ARRAY_HEADER
#define SAFEL_ARRAY_HEADER

#include "iallocator.hpp"

namespace safel 
{
    template<typename T>
    struct array
    {
        iallocator allocator;
        T* data;
        size_t size;

        T& operator [](size_t index)
        {
            assert(index < size);
            return data[index];
        }

        const T& operator [](size_t index) const
        {
            assert(index < size);
            return data[index];
        }
    };

    template<typename T>
    constexpr option<array<T>> array_new(size_t size, iallocator allocator)
    {
        auto p = allocator.alloc(sizeof(T) * size);

        if(p.is_none()) return option<array<T>>::none();

        array<T> arr;
        arr.allocator = allocator;
        arr.data = (T*)p.unwrap();
        arr.size = size;
        return option<array<T>>::some(arr);
    }

    template<typename T>
    constexpr void array_free(array<T>& arr)
    {
        if(arr.allocator.mask() & ALLOCATOR_FLAG_FREE_SINGLE)
            arr.allocator.free_single(arr.data);
        
        arr.data = nullptr;
        arr.size = 0;
    }

    template<typename T>
    struct dynarray
    {
        static constexpr size_t START_SIZE = 8;

        iallocator allocator;
        T* data;
        size_t cap;
        size_t size;

        T& operator [](size_t index)
        {
            assert(index < size);
            return data[index];
        }

        const T& operator [](size_t index) const
        {
            assert(index < size);
            return data[index];
        }
    };

    template<typename T>
    constexpr dynarray<T> dynarray_new(iallocator allocator)
    {
        dynarray<T> arr;
        arr.allocator = allocator;
        arr.data = nullptr;
        arr.cap = 0;
        arr.size = 0;
        return arr;
    }

    template<typename T>
    constexpr bool dynarray_reserve(dynarray<T>& arr)
    {
        if(arr.cap == 0)
        {
            size_t new_cap =  dynarray<T>::START_SIZE;
            auto p = arr.allocator.alloc(sizeof(T) * new_cap);
            
            if(p.is_none()) return false;

            arr.data = (T*)p.unwrap();
            arr.cap = new_cap;
        }

        if(arr.size >= arr.cap)
        {
            size_t new_cap = arr.cap + arr.cap / 2;
            auto p = arr.allocator.realloc(arr.data, sizeof(T) * new_cap);

            if(p.is_none()) return false;

            arr.data = (T*)p.unwrap();
            arr.cap = new_cap;
        }

        return true;
    }

    template<typename T>
    constexpr bool dynarray_add(dynarray<T>& arr, T value)
    {
        if(!dynarray_reserve(arr))
            return false;

        arr.data[arr.size] = value;
        arr.size++;
        return true;
    }

    template<typename T>
    constexpr void dynarray_add_unsafe(dynarray<T>& arr, T value)
    {
        assert(arr.size < arr.cap);
        arr.data[arr.size] = value;
        arr.size++;
    }

    template<typename T>
    constexpr void dynarray_clear(dynarray<T>& arr)
    {
        arr.size = 0;
    }

    template<typename T>
    constexpr void dynarray_free(dynarray<T>& arr)
    {
        if(arr.allocator.mask() & ALLOCATOR_FLAG_FREE_SINGLE)
            arr.allocator.free_single(arr.data);
        
        arr = dynarray_new<T>(arr.allocator);
    }

    template<typename T>
    constexpr option<array<T>> array_from_dynarray(dynarray<T>& arr)
    {
        auto p = arr.allocator.realloc(arr.data, sizeof(T) * arr.size);

        if(p.is_none()) return option<array<T>>::none();

        T* data = (T*)p.unwrap();

        array<T> r = {
            arr.allocator,
            data,
            arr.size,
        };

        arr = dynarray_new<T>(arr.allocator);

        return option<array<T>>::some(r);
    }

    template<typename T>
    constexpr dynarray<T> dynarray_from_array(array<T>& arr)
    {   
        dynarray<T> r = {
            arr.allocator,
            arr.data,
            arr.size,
            arr.size,
        };
        
        arr.data = nullptr;
        arr.size = 0;

        return r;
    }
}

#endif