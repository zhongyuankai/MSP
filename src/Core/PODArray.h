#pragma once

#include <vector>
#include <assert.h>

template<typename T>
class PODArray {
    using iterator = T *;

public:
    PODArray()
    : PODArray(8)
    {
    }

    PODArray(size_t n)
    {
        alloc(element_size * n);
    }

    PODArray(PODArray && rhs)
    {
        swap(rhs);
    }

    ~PODArray()
    {
        if (c_start != nullptr)
        {
            free(c_start);
        }
    }

    PODArray & operator=(PODArray && rhs)
    {
        swap(rhs);
        return *this;
    }

    T & operator[](size_t n)
    {
        assert(n < size());
        return t_start()[n];
    }

    size_t size()
    {
        return (c_end_of_storage - c_start) / element_size;
    }

    bool empty()
    {
        return c_end == c_start;
    }

    const char * row_data() const
    {
        return c_start;
    }


    T & front()
    {
        return *t_start();
    }

    T & back()
    {
        return *t_end();
    }

    iterator begin()
    {
        return t_start();
    }

    iterator end()
    {
        return t_end();
    }

    void push_back(T && value)
    {
        if (c_end_of_storage - c_end == 0)
        {
            reserveForNextSize();
        }

        new (t_end()) T(std::forward<T>(value));
        c_end += element_size;
    }

    template<typename... Args>
    void emplace_back(Args &&... args)
    {
        if (c_end_of_storage - c_end == 0)
        {
            reserveForNextSize();
        }

        new (t_end()) T(std::forward<Args>(args)...);
        c_end += element_size;
    }

    void pop_back()
    {
        if (c_start == c_end)
        {
            return;
        }

        c_end -= element_size;
    }

    void reserve(size_t size)
    {
        realloc(size);
    }

    T * data()
    {
        return t_start();
    }

private:
    void reserveForNextSize()
    {
        if (empty())
            alloc(8);
        else
            realloc( 2 * size());
    }

    size_t allocated_bytes()
    {
        return c_end_of_storage - c_start;
    }

    void alloc(size_t size)
    {

        size_t bytes = size * element_size;
        c_start = c_end = reinterpret_cast<char *>(malloc(bytes));
        c_end_of_storage = c_start + bytes;
    }

    void realloc(size_t new_size)
    {
        if (size() == new_size)
        {
            return;
        }

        size_t old_bytes = size() * element_size;
        size_t new_bytes = new_size * element_size;
        char * allocated = reinterpret_cast<char *>(malloc(new_bytes));
        memcpy(allocated, c_start, old_bytes);
        free(c_start);
        c_start = allocated;
        c_end = c_start + old_bytes;
        c_end_of_storage = c_start + new_bytes;
    }

    void swap(PODArray && rhs)
    {
        std::swap(c_start, rhs.c_start);
        std::swap(c_end, rhs.c_end);
        std::swap(c_end_of_storage, rhs.c_end_of_storage);
    }

    T * t_start()
    {
        return reinterpret_cast<T *>(c_start);
    }

    T * t_end()
    {
        return reinterpret_cast<T *>(c_end);
    }

    char * c_start = nullptr;
    char * c_end = nullptr;
    char * c_end_of_storage = nullptr;

    size_t element_size = sizeof(T);
};


