#include "CellFactory.h"

std::unique_ptr<BaseCell> CellFactory::createCell(const MyString& input) {

    if (input.length() == 0) {
        return std::make_unique<ValueCell<MyString>>(MyString(""));
    }

    const char* str = input.data();

    // Formula or reference (future implementation)
    if (str[0] == '=') {
        return std::make_unique<ValueCell<MyString>>(MyString("ERROR: Formula parsing not implemented"));
    }

    if (input == "true") return std::make_unique< ValueCell<bool>>(true);
    if (input == "false") return std::make_unique<ValueCell<bool>>(false);

    if (input.length() >= 2 && str[0] == '"' && str[input.length() - 1] == '"') {
        if (input.length() == 2) {
            return std::make_unique<ValueCell<MyString>>(MyString(""));
        }

        size_t contentLength = input.length() - 2;
        char* buffer = new char[contentLength + 1];

        for (size_t i = 0; i < contentLength; i++) {
            buffer[i] = str[i + 1];
        }
        buffer[contentLength] = '\0';

        MyString stripped(buffer);
        delete[] buffer;

        return std::make_unique<ValueCell<MyString>>(stripped);
    }

    bool isNegative = false;
    size_t start = 0;

    if (input.length() > 0 && str[0] == '-') {
        isNegative = true;
        start = 1;
    }

    bool isNumber = true;
    if (start >= input.length()) {
        isNumber = false;
    }
    else {
        for (size_t i = start; i < input.length(); i++) {
            char c = str[i];
            if (!(c >= '0' && c <= '9')) {
                isNumber = false;
                break;
            }
        }
    }

    if (isNumber) {
        int number = 0;
        for (size_t i = start; i < input.length(); i++) {
            number = number * 10 + (str[i] - '0');
        }
        if (isNegative) {
            number *= -1;
        }
        return std::make_unique<ValueCell<int>>(number);
    }

    return std::make_unique<ValueCell<MyString>>(input);
}