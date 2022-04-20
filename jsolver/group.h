#pragma once

#include <list>
#include <algorithm>

#include "cell.h"
#include "hints.h"

namespace jsolver
{

class CellGroup
{
    Line         _group;
    const Hint & _hint;
public:
    CellGroup(const Hint & hint) : _hint(hint)
    {}

    const Hint & hint() const
    {
        return _hint;
    }

    uint32_t id() const
    {
        return _hint.id();
    }

    auto begin()
    {
        return _group.begin();
    }

    auto end()
    {
        return _group.end();
    }

    Cell & operator[](size_t i)
    {
        if (i >= _group.size())
            throw std::runtime_error("CellGroup: operator [] out of bounds with index: " + std::to_string(i) + " in size: " + std::to_string(_group.size()));
        return _group[i];
    }

    // Line::const_iterator find(uint32_t x, uint32_t y) const
    // {
    //     return std::find_if(_group.begin(), _group.end(), [&](auto & cell)
    //     {
    //         return (cell.get().x() == x && cell.get().y() == y);
    //     });
    // }

    void add(Cell & cell)
    {
        //if (find(cell.x(), cell.y()) == _group.end())
        {
            _group.push_back(cell);
        }
    }

    // void rpop()
    // {
    //     auto it = std::prev(_group.end());
    //     if (it != _group.end())
    //     {
    //         _group.erase(it);
    //     }
    // }

    // void lpop()
    // {
    //     ++_group.begin();
    //     // auto it = _group.begin();
    //     // if (it != _group.end())
    //     // {
    //     //     _group.erase(it);
    //     // }
    // }

    // void remove(Cell & cell)
    // {
    //     auto it = find(cell.x(), cell.y());
    //     if (it != _group.end())
    //     {
    //         _group.erase(it);
    //     }
    // }

    size_t size() const
    {
        return _group.size();
    }

    void clear()
    {
        _group.clear();
    }

    void print() const
    {
        for (auto & cell : _group)
        {
            cell.get().print();
        }
        std::cout << "\n";
    }
};

using Groups = std::list<CellGroup>;

}