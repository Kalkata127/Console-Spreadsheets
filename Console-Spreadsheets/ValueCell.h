#pragma once
#include "BaseCell.h"
#include "MyString.h"

template<typename T>
class ValueCell : public BaseCell {
private:
    T value;

public:
    ValueCell(const T& val) : value(val) {}

    MyString toString() const override {
        return MyString("Unsupported");
    }

    double evaluate() const override {
        return 0.0;
    }

    MyString getType() const override {
        return MyString("unknown");
    }

    BaseCell* clone() const override {
        return new ValueCell<T>(*this);
    }
};

// ---- SPECIALIZATION FOR int ----
template<>
inline MyString ValueCell<int>::toString() const {
    if (value == 0) return MyString("0");

    bool isNegative = value < 0;
    int absValue = isNegative ? -value : value;

    char buffer[32];
    int index = 0;

    while (absValue > 0) {
        buffer[index++] = '0' + (absValue % 10);
        absValue /= 10;
    }

    if (isNegative) buffer[index++] = '-';
    buffer[index] = '\0';

    for (int i = 0; i < index / 2; ++i) {
        char tmp = buffer[i];
        buffer[i] = buffer[index - i - 1];
        buffer[index - i - 1] = tmp;
    }

    return MyString(buffer);
}

template<>
inline double ValueCell<int>::evaluate() const {
    return static_cast<double>(value);
}

template<>
inline MyString ValueCell<int>::getType() const {
    return MyString("int");
}

template<>
inline BaseCell* ValueCell<int>::clone() const {
    return new ValueCell<int>(*this);
}

// ---- SPECIALIZATION FOR bool ----
template<>
inline MyString ValueCell<bool>::toString() const {
    return value ? MyString("true") : MyString("false");
}

template<>
inline double ValueCell<bool>::evaluate() const {
    return value ? 1.0 : 0.0;
}

template<>
inline MyString ValueCell<bool>::getType() const {
    return MyString("bool");
}

template<>
inline BaseCell* ValueCell<bool>::clone() const {
    return new ValueCell<bool>(*this);
}

// ---- SPECIALIZATION FOR MyString ----
template<>
inline MyString ValueCell<MyString>::toString() const {
    return value;
}

template<>
inline double ValueCell<MyString>::evaluate() const {
    return 0.0;
}

template<>
inline MyString ValueCell<MyString>::getType() const {
    return MyString("MyString");
}

template<>
inline BaseCell* ValueCell<MyString>::clone() const {
    return new ValueCell<MyString>(*this);
}
