#pragma once

namespace jsolver
{

enum class CellState : uint8_t
{
    UNKNOWN,
    EMPTY,
    FILLED
};

class Cell
{
    uint32_t _x, _y;
    CellState _state = CellState::UNKNOWN;
public:
    Cell() {}
    Cell(uint32_t x, uint32_t y, CellState state = CellState::UNKNOWN) : _x(x), _y(y), _state(state)
    {}

    Cell(const Cell & other)
    {
        _x = other._x;
        _y = other._y;
        _state = other._state;
    }

    std::pair<uint32_t, uint32_t> xy() const
    {
        return {_x, _y};
    }

    uint32_t x() const
    {
        return _x;
    }

    uint32_t y() const
    {
        return _y;
    }

    bool is_empty() const
    {
        return _state == CellState::EMPTY;
    }

    bool is_filled() const
    {
        return _state == CellState::FILLED;
    }

    bool is_unknown() const
    {
        return _state == CellState::UNKNOWN;
    }

    CellState state() const
    {
        return _state;
    }

    void set(CellState state)
    {
        _state = state;
    }

    void print() const
    {
        switch(_state)
        {
            case CellState::EMPTY:
                std::cout << ".";
                break;
            case CellState::FILLED:
                std::cout << "X";
                break;
            case CellState::UNKNOWN:
                std::cout << " ";
                break;
            default:
                std::cout << "?";
                break;
        }
    }
};

using Line = std::vector<std::reference_wrapper<Cell>>;

}