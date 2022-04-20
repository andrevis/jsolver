#pragma once

#include "i_method.h"

namespace jsolver
{

class FullFill : public IMethod
{
    bool _applied = false;

private:
    uint32_t _sum(const Hints & hints) const
    {
        uint32_t sum = hints.size() - 1; //spares
        for (auto & hint : hints)
        {
            sum += *hint;
        }
        return sum;
    }

public:
    bool need() const override
    {
        return !_applied;
    }

    void apply(Line && line, const Hints & hints) override
    {
        if (line.size() == _sum(hints))
        {
            //std::cout << "APPLY: FullFill\n";

            _applied = true;

            auto cell = line.begin();
            for (const Hint & hint : hints)
            {
                uint32_t value = *hint;

                while (value > 0)
                {
                    cell->get().set(CellState::FILLED);
                    --value;
                    ++cell;
                }

                if (cell != line.end())
                {
                    cell->get().set(CellState::EMPTY);
                    ++cell;
                }
            }
        }
    }

};

}