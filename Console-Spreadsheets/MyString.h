#pragma once
#include <stdexcept>
#include <iostream>
using namespace std;

class MyString {
private:
    char* str;
    size_t len;

    void copyString(const char* source);
    void free();

public:
    MyString();
    MyString(const char* string);
    MyString(const MyString& string);

    virtual ~MyString();

    size_t length() const;
    const char* data() const;
    char charAt(int index) const;

    MyString& operator=(const char* str);
    MyString& operator=(const MyString& string);

    bool operator<(const MyString& string) const;
    bool operator==(const MyString& string) const;
    bool operator!=(const MyString& string) const;

    MyString operator+(const MyString& string) const;

    friend ostream& operator<<(ostream& os, const MyString& string);
    friend istream& operator>>(istream& is, MyString& string);
};
