#pragma once
#include "MyString.h"

class BaseCell {
public:
    virtual ~BaseCell() = default;
    virtual MyString toString() const = 0;
    virtual double evaluate() const = 0;
    virtual MyString getType() const = 0;
    virtual BaseCell* clone() const = 0;
};