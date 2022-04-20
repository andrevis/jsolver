#pragma once


#include "i_method.h"
#include "group.h"

namespace jsolver
{

class NotFit : public IMethod
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

        auto cell = cell_begin;

        while (cell != cell_end && !cell->get().is_unknown())
        {
            ++cell;
        }

        bool is_empty = (cell == cell_begin ? cell : --cell)->get().is_empty();
        ++cell;

        CellGroup group(*hint);
        while (cell != cell_end && cell->get().is_unknown())
        {
            group.add(*cell);
            ++cell;
        }

        if (cell != cell_end && cell->get().is_empty())
        {
            if (is_empty && group.size() > 0 && group.size() < **hint)
            {
                groups.emplace_back(std::move(group));
            }
        }

        return groups;
    }

public:
    void apply(Line && line, const Hints & hints) override
    {
        std::cout << "APPLY: NotFit\n";

        Groups left = _get_groups(line.begin(), line.end(), hints.begin(), hints.end());
        for (auto & group : left)
        {
            for (auto & cell : group)
            {
                cell.get().set(CellState::EMPTY);
            }
        }

        Groups right = _get_groups(line.rbegin(), line.rend(), hints.rbegin(), hints.rend());
        for (auto & group : right)
        {
            for (auto & cell : group)
            {
                cell.get().set(CellState::EMPTY);
            }
        }
    }
};

}