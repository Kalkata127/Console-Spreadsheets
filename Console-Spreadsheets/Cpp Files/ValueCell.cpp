#include "ValueCell.h"
#include <cstring>

// Специализация за int
template<>
ValueCell<int>::ValueCell(const int& val) : value(val) {}

template<>
const int& ValueCell<int>::getValue() const { return value; }

template<>
MyString ValueCell<int>::toString() const {
    char buffer[32];
    int temp = value;
    bool negative = temp < 0;
    if (negative) temp = -temp;

    int i = 0;
    if (temp == 0) {
        buffer[i++] = '0';
    }
    else {
        while (temp > 0) {
            buffer[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }

    if (negative) buffer[i++] = '-';

    for (int j = 0; j < i / 2; ++j) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }

    buffer[i] = '\0';
    return MyString(buffer);
}

template<>
double ValueCell<int>::evaluate() const {
    return static_cast<double>(value);
}

template<>
MyString ValueCell<int>::getType() const {
    return MyString("int");
}

template<>
BaseCell* ValueCell<int>::clone() const {
    return new ValueCell<int>(*this);
}


// Специализация за bool
template<>
ValueCell<bool>::ValueCell(const bool& val) : value(val) {}

template<>
const bool& ValueCell<bool>::getValue() const { return value; }

template<>
MyString ValueCell<bool>::toString() const {
    return value ? MyString("true") : MyString("false");
}

template<>
double ValueCell<bool>::evaluate() const {
    return value ? 1.0 : 0.0;
}

template<>
MyString ValueCell<bool>::getType() const {
    return MyString("bool");
}

template<>
BaseCell* ValueCell<bool>::clone() const {
    return new ValueCell<bool>(*this);
}


// Специализация за MyString
template<>
ValueCell<MyString>::ValueCell(const MyString& val) : value(val) {}

template<>
const MyString& ValueCell<MyString>::getValue() const { return value; }

template<>
MyString ValueCell<MyString>::toString() const {
    return MyString("\"") + value + MyString("\"");
}

template<>
double ValueCell<MyString>::evaluate() const {
    return 0.0;
}

template<>
MyString ValueCell<MyString>::getType() const {
    return MyString("string");
}

template<>
BaseCell* ValueCell<MyString>::clone() const {
    return new ValueCell<MyString>(*this);
}
