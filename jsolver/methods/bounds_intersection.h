#pragma once

#include <functional>

#include "i_method.h"
#include "group.h"

namespace jsolver
{

class BoundsIntersection : public IMethod
{
public:
    template <class TCellIterator, class THintIterator>
    Groups _get_groups(const TCellIterator & cell_begin, const TCellIterator & cell_end, THintIterator hints_begin, THintIterator hints_end) const
    {
        Groups groups;

        auto hint = hints_begin;
        auto cell = cell_begin;
        while (hint != hints_end && cell != cell_end)
        {
            CellGroup group(*hint);
            while (cell != cell_end && cell->get().is_empty())
            {
                ++cell;
            }

            uint32_t value = **hint;
            while (cell != cell_end && !cell->get().is_empty() && value-- > 0)
            {
                group.add(*cell);
                ++cell;
            }

            if (group.size() == **hint)
            {
                groups.emplace_back(std::move(group));
                ++cell;
                ++hint;
                continue;
            }
        }

        return groups;
    }

    CellGroup _intersection(CellGroup & left, CellGroup & right) const
    {
        CellGroup group(left.hint());

        for (auto & l : left)
        {
            for (auto & r : right)
            {
                if (l.get().xy() == r.get().xy())
                {
                    group.add(l); //copy
                    break;
                }
            }
        }
        return group;
    }

    Groups _intersection(Groups & left, Groups & right) const
    {
        Groups intersecions;
        for (auto & x : left)
        {
            auto it = std::find_if(right.begin(), right.end(), [id = x.id()](const CellGroup & group)
            {
                return id == group.id();
            });
            if (it != right.end())
                intersecions.push_back(_intersection(x, *it));
        }
        return intersecions;
    }

    void apply(Line && line, const Hints & hints) override
    {
        std::cout << "APPLY: Bounds Intersection\n";

        Groups left = _get_groups(line.begin(), line.end(), hints.begin(), hints.end());
        Groups right = _get_groups(line.rbegin(), line.rend(), hints.rbegin(), hints.rend());

        for (auto & group : _intersection(left, right))
        {
            for (auto & cell : group)
            {
                cell.get().set(CellState::FILLED);
            }

            if (group.size() == *group.hint())
            {
                group.hint().close();
            }
        }
    }
};

}