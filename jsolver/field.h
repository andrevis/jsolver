#pragma once

#include <opencv2/opencv.hpp>

#include <exception>
#include "cell.h"

namespace jsolver
{

class Field
{
    size_t              _width = 0;
    size_t              _height = 0;
    std::vector<Cell>   _cells;

    using Raws = std::vector<Line>;
    using Cols = std::vector<Line>;

public:
    Field()
    {}

    ~Field()
    {
        _cells.clear();
    }

    Field(const Field & other)
    {
        init(other._width, other._height);

        //init(other._width, other._height);
        //std::copy(other._cells.begin(), other._cells.end(), std::back_inserter(_cells));

        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
               at(x, y).set(other.at(x, y).state());
            }
        }
    }

    // Field(Field && other)
    // {
    //     // _width = other._width;
    //     // _height = other._height;
    //     _width = other._width;
    //     _height = other._height;
    //     std::swap(_cells, other._cells);
    // }

    void operator=(const Field & other)
    {
        init(other._width, other._height);


        //std::copy(other._cells.begin(), other._cells.end(), std::back_inserter(_cells));

        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
               at(x, y).set(other.at(x, y).state());
            }
        }
    }

    // void operator=(Field && other)
    // {
    //     // _width = other._width;
    //     // _height = other._height;
    //     init(other._width, other._height);
    //     std::swap(_cells, other._cells);
    // }

    Field clone() const
    {
        Field clone;
        clone.init(_width, _height);

        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
               clone.at(x, y).set(at(x, y).state());
            }
        }
        return clone;
    }

    bool operator==(const Field & other) const
    {
        if (_width != other._width) return false;
        if (_height != other._height) return false;

        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
               if (at(x, y).state() != other.at(x, y).state())
               {
                   return false;
               }

            }
        }
        return true;
    }

    bool solved() const
    {
        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
                if (at(x, y).is_unknown())
                    return false;
            }
        }
        return true;
    }

    void check_bounds(uint32_t x, uint32_t y) const
    {
        if (!(x < _width && y < _height))
            throw std::runtime_error("Out of bounds: " + std::to_string(x) + ", " + std::to_string(y));
    }

    void init(size_t width, size_t height)
    {
        _width = width;
        _height = height;

        _cells.reserve(width * height);
        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
                at(x, y) = Cell(x, y, CellState::UNKNOWN);
            }
        }
    }

    size_t width() const
    {
        return _width;
    }

    size_t height() const
    {
        return _height;
    }

    inline Cell & at(uint32_t x, uint32_t y)
    {
        check_bounds(x, y);
        return _cells[ y * _width  + x ];
    }

    inline const Cell & at(uint32_t x, uint32_t y) const
    {
        check_bounds(x, y);
        return _cells[ y * _width  + x ];
    }

    void set(uint32_t x, uint32_t y, CellState state)
    {
        at(x, y).set(state);
    }

    void refresh()
    {
        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
                at(x, y).set(CellState::UNKNOWN);
            }
        }
    }

    Line raw(uint32_t y)
    {
        Line raw;
        for (size_t x = 0; x < _width; ++x)
        {
            //raw.push_back(at(x, y));
            raw.emplace_back(std::ref(at(x, y)));
        }
        return raw;
    }

    Raws raws()
    {
        Raws raws;
        for (size_t y = 0; y < _height; ++y)
        {
            raws.emplace_back(raw(y));

        }
        return raws;
    }

    Line col(uint32_t x)
    {
        Line col;
        for (size_t y = 0; y < _height; ++y)
        {
            //col.push_back(at(x, y));
            col.emplace_back(std::ref(at(x, y)));
        }
        return col;
    }

    Cols cols()
    {
        Cols cols;
        for (size_t x = 0; x < _width; ++x)
        {
            cols.emplace_back(col(x));
        }
        return cols;
    }

    void print() const
    {
        std::cout << "----------------------------\n";
        for (size_t y = 0; y < _height; ++y)
        {
            for (size_t x = 0; x < _width; ++x)
            {
                at(x, y).print();
                std::cout << " ";
            }
            std::cout << "\n";
        }
        std::cout << "----------------------------\n";
    }
};

}