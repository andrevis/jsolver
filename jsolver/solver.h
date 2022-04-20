#pragma once

#include <list>

#include "cross.h"
#include "field.h"
#include "ring_buffer.h"
#include "methods.h"

namespace jsolver
{

class Solver
{
    const Cross & _cross;
    const HintsMap & _hints;

    RingBuffer<Field> _history;
    Field _field;

    using MethodPtr = std::shared_ptr<IMethod>;
    std::list<MethodPtr> _methods;


private:
    template <class TMethod, class...TRest>
    constexpr void _addMethod()
    {
        _methods.emplace_back(new TMethod());
        if constexpr (sizeof...(TRest) > 0)
        {
            _addMethod<TRest...>();
        }
    }

public:
    Solver(Cross & cross) : _cross(cross), _hints(_cross.hints()), _history(10)
    {
        auto [width, height] = _cross.size();
        _field.init(width, height);
        _addMethod<FullFill, BoundsIntersection, Unreachable, NotFit, WallRepulsion>();
    }

    bool solve()
    {
        while (!_field.solved())
        {
            for (auto & method : _methods)
            {
                if (method->need())
                {
                    for (size_t y = 0; y < _field.height(); ++y)
                    {
                        method->apply(_field.raw(y), _hints.left[y]);
                    }

                    for (size_t x = 0; x < _field.width(); ++x)
                    {
                        method->apply(_field.col(x), _hints.top[x]);
                    }

                    _field.print();
                }
            }

            // Field * prev = _history.prev();
            // if (prev && *prev == _field)
            // {
            //     std::cout << "There in no more moves\n";
            //     return false;
            // }

            //auto ref = _history.add(_field.clone());
            //ref.print();
            //Field * last = _history.last();
            //if (last) last->print();

            //_field.print();
            //break;//
        }

        return true;
    }
};

}