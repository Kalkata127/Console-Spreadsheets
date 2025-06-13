#include "MyString.h"

void MyString::copyString(const char* source) {
    if (source == nullptr) {
        str = new char[1];
        str[0] = '\0';
        len = 0;
    }
    else {
        len = strlen(source);
        str = new char[len + 1];
        strcpy_s(str, len + 1, source);
    }
}

void MyString::free() {
    delete[] str;
    str = nullptr;
    len = 0;
}

MyString::MyString() {
	str = new char[1];
	str[0] = '\0';
	len = 0;
}

MyString::MyString(const char* string) {
	copyString(string);
}

MyString::MyString(const MyString& other) {
    copyString(other.str);
}

MyString::~MyString() {
    free();
}


size_t MyString::length() const {
    return len;
}

const char* MyString::data() const {
    return str;
}


MyString& MyString::operator=(const char* other) {
    if (str != other) {
        free();
        copyString(other);
    }
    return *this;
}

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        free();
        copyString(other.str);
    }
    return *this;
}

bool MyString::operator<(const MyString & other) const {
    return strcmp(str, other.str) < 0;
}

bool MyString::operator==(const MyString& other) const {
    return strcmp(str, other.str) == 0;
}

bool MyString::operator!=(const MyString& other) const {
    return strcmp(str, other.str) != 0;
}

MyString MyString::operator+(const MyString& other) const {
    MyString result;
    result.free(); 

    result.len = len + other.len;
    result.str = new char[result.len + 1];

    strcpy_s(result.str, result.len + 1, str);
    strcat_s(result.str, result.len + 1, other.str);

    return result;
}

ostream& operator<<(ostream& os, const MyString& string) {
    os << string.str;
    return os;
}

istream& operator>>(istream& is, MyString& string) {
    char buffer[1024]; 
    is >> buffer;
    string = buffer;
    return is;
}