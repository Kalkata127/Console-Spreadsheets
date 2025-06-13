#pragma once
#include "MyString.h"  // Include full header instead of forward declaration

class BaseCell {
public:
    virtual ~BaseCell() = default;
    virtual MyString toString() const = 0;
    virtual double evaluate() const = 0;
    virtual MyString getType() const = 0;
    virtual BaseCell* clone() const = 0;
};