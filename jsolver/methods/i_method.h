#pragma once

#include "cell.h"

namespace jsolver
{

class IMethod
{
public:
    virtual bool need() const
    {
        return true;
    }

    virtual void apply(Line && line, const Hints & hint) = 0;
    virtual ~IMethod() = default;
};
//https://habr.com/ru/post/418069/
}