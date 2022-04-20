#pragma once

#include "vector"

template <class T>
class RingBuffer
{
    class Index
    {
        size_t _index = 0;
        size_t _size;
    public:
        Index(size_t size) : _size(size)
        {}
        size_t get() const
        {
            return _index;
        }
        void operator=(const Index & other)
        {
            _index = other._index;
            _size = other._size;
        }

        bool operator==(const Index & other)
        {
            return _index == other._index;
        }

        size_t operator*() const
        {
            return get();
        }

        Index & operator++()
        {
            ++_index;
            if (_index == _size)
                _index = 0;

            return *this;
        }

        Index & operator--()
        {
            if (_index == 0)
                _index = _size - 1;
            else
                --_index;

            return *this;
        }

        // void operator-(size_t i)
        // {
        //     if (_index == 0)
        //         _index = _size - 1;
        //     else
        //         --_index;
        // }
    };

    std::vector<T> _buffer;
    Index _next, _start, _end;

public:
    RingBuffer(size_t size): _next(size), _start(size), _end(size)
    {
        _buffer.reserve(size);
    }

    T & add(T && el)
    {
        _buffer[*_next] = std::forward<T>(el);
        _end = _next;
        ++_next;
        if (_next == _start)
            ++_start;

        return _buffer[*_end];
    }

    void add(const T & el)
    {
        _buffer[*_next] = el;
        _end = _next;
        ++_next;
        if (_next == _start)
            ++_start;
    }

    T * prev()
    {
        if (_buffer.size() < 2)
            return nullptr;
        auto i = _end;
        return &_buffer[*(--i)];
    }

    T * last()
    {
        if (_buffer.empty())
            return nullptr;

        return &_buffer[*_end];
    }

    T * start()
    {
        if (_buffer.empty())
            return nullptr;

        return &_buffer[*_start];
    }

};