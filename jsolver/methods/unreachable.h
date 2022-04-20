#pragma once

#include "i_method.h"
#include "group.h"

namespace jsolver
{

class Unreachable : public IMethod
{
    //size_t _groups_count = 0;
private:

    // size_t _get_groups_count(const Line & line) const
    // {
    //     size_t groups = 0;

    //     for (auto cell = line.begin(); cell != line.end(); ++cell)
    //     {
    //         size_t filled = 0;
    //         while (cell != line.end() && cell->get().is_filled())
    //         {
    //             ++filled;
    //             ++cell;
    //         }

    //         if (filled) ++groups;
    //     }
    //     return groups;
    // }

    template <class THintIterator>
    size_t _get_hints_count(THintIterator hints_begin, THintIterator hints_end) const
    {
        size_t count = 0;
        for (auto hint = hints_begin; hint != hints_end; ++hint)
        {
            ++count;
        }
        return count;
    }


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

        CellGroup group(*hint);
        size_t filled = 0, empty = 0;

        const bool not_filled = (cell != cell_end && !cell->get().is_filled());

        while (cell != cell_end && cell->get().is_empty())
        {
            ++empty;
            ++cell;
        }

        while (cell != cell_end && cell->get().is_unknown())
        {
            group.add(*cell);
            ++cell;
        }


        while (cell != cell_end && cell->get().is_filled())
        {
            ++filled;
            ++cell;
        }

        uint32_t total = filled + group.size();
        if (not_filled && filled > 0 && group.size() > 0 &&
                (group.size() < **hint || _get_hints_count(hints_begin, hints_end) == 1) &&
                total > **hint)
        {
            CellGroup empty_group(*hint);
            for (size_t i = 0; i < total - **hint; ++i)
            {
                empty_group.add(group[i]);
            }
            groups.emplace_back(std::move(empty_group));
        }
        return groups;
    }

public:
    void apply(Line && line, const Hints & hints) override
    {
        std::cout << "APPLY: Unreachable\n";
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