#pragma once

#include "i_method.h"
#include "group.h"

namespace jsolver
{

class WallRepulsion : public IMethod
{

    template <class TCellIterator, class THintIterator>
    Groups _get_groups(const TCellIterator & cell_begin, const TCellIterator & cell_end, THintIterator hints_begin, THintIterator hints_end) const
    {
        Groups groups;

        auto hint = std::find_if(hints_begin, hints_end, [](const Hint & h)
        {
            return h.active();
        });
        if (hint == hints_end) return groups;

        size_t filled = 0;
        CellGroup group(*hint);
        
        auto cell = cell_begin;
        while (cell != cell_end && !cell->get().is_unknown())
        {
            ++cell;
        }

        if (cell != cell_begin && cell->get().is_unknown())
        {
            --cell;
            if (cell->get().is_filled())
            {
                while (cell != cell_begin && cell->get().is_filled())
                {
                    ++filled;
                    --cell;
                }
                while (cell != cell_end && !cell->get().is_unknown())
                {
                    ++cell;
                }
                if (filled > 0)
                {
                    while (cell != cell_end && cell->get().is_unknown() && filled + group.size() < **hint)
                    {
                        group.add(*cell);
                        ++cell;
                    }
                    if (filled + group.size() == **hint)
                    {
                        if (cell != cell_end)
                        {
                            cell->get().set(CellState::EMPTY);
                        }
                        hint->close();
                    }
                    if (group.size() > 0)
                    {
                        groups.emplace_back(std::move(group));
                    }
                }
            }
        }
        return groups;
    }

public:
    void apply(Line && line, const Hints & hints) override
    {
        std::cout << "APPLY: WallRepulsion\n";
        Groups left = _get_groups(line.begin(), line.end(), hints.begin(), hints.end());
        for (auto & group : left)
        {
            for (auto & cell : group)
            {
                cell.get().set(CellState::FILLED);
            }
        }

        Groups right = _get_groups(line.rbegin(), line.rend(), hints.rbegin(), hints.rend());
        for (auto & group : right)
        {
            for (auto & cell : group)
            {
                cell.get().set(CellState::FILLED);
            }
        }
    }

};

}