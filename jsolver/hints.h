#pragma once

#include <vector>

namespace jsolver
{

struct Hint
{
    uint32_t _id;
    uint32_t _value;
    mutable bool _closed = false;
public:
    Hint(uint32_t id, uint32_t value) : _id(id), _value(value)
    {}

    uint32_t id() const
    {
        return _id;
    }

    uint32_t operator*() const
    {
        return _value;
    }

    bool active() const
    {
        return !_closed;
    }

    void close() const
    {
        _closed = true;
    }
};

using Hints = std::vector<Hint>;

struct HintsMap
{
    std::vector<Hints> left;
    std::vector<Hints> top;
};

}
