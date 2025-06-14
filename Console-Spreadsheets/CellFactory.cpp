#include "CellFactory.h"
#include "Table.h"

std::unique_ptr<BaseCell> CellFactory::createCell(const MyString& input) {
    return createCell(input, nullptr);
}

std::unique_ptr<BaseCell> CellFactory::createCell(const MyString& input, Table* table) {
    if (input.length() == 0) {
        return nullptr;
    }

    const char* str = input.data();

    if (str[0] == '=' && input.length() > 1) {
        size_t refLength = input.length() - 1;
        char* refBuffer = new char[refLength + 1];

        for (size_t i = 0; i < refLength; i++) {
            refBuffer[i] = str[i + 1];
        }
        refBuffer[refLength] = '\0';

        MyString reference(refBuffer);
        delete[] refBuffer;

        size_t row, col;
        if (parseCellReference(reference, row, col)) {
            auto refCell = make_unique<ReferenceCell>(row, col);
            if (table != nullptr) {
                refCell->setTablePtr(table);
            }
            return refCell;
        }
        else {
            return make_unique<ValueCell<MyString>>(MyString("#REF!"));
        }
    }

    if (input == MyString("true")) {
        return make_unique<ValueCell<bool>>(true);
    }
    if (input == MyString("false")) {
        return make_unique<ValueCell<bool>>(false);
    }

    if (input.length() >= 2 && str[0] == '"' && str[input.length() - 1] == '"') {
        if (input.length() == 2) {
            return make_unique<ValueCell<MyString>>(MyString(""));
        }

        size_t contentLength = input.length() - 2;
        char* buffer = new char[contentLength + 1];

        for (size_t i = 0; i < contentLength; i++) {
            buffer[i] = str[i + 1];
        }
        buffer[contentLength] = '\0';

        MyString stripped(buffer);
        delete[] buffer;

        return make_unique<ValueCell<MyString>>(stripped);
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
        return make_unique<ValueCell<int>>(number);
    }

    return make_unique<ValueCell<MyString>>(input);
}

bool CellFactory::parseCellReference(const MyString& reference, size_t& row, size_t& col) {
    if (reference.length() < 2) {
        return false; 
    }

    const char* str = reference.data();
    size_t i = 0;

    if (str[i] < 'A' || str[i] > 'Z') {
        return false;
    }

    col = str[i] - 'A';
    i++;

    if (i >= reference.length() || str[i] < '1' || str[i] > '9') {
        return false; 
    }

    size_t rowNumber = 0;
    while (i < reference.length() && str[i] >= '0' && str[i] <= '9') {
        rowNumber = rowNumber * 10 + (str[i] - '0');
        i++;
    }

    if (i != reference.length()) {
        return false; 
    }

    if (rowNumber == 0) {
        return false; 
    }
    row = rowNumber - 1;

    return true;
}