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

        // Check if it's a formula
        bool hasOpenParen = false;
        bool hasCloseParen = false;
        for (size_t i = 0; i < reference.length(); i++) {
            if (reference.data()[i] == '(') {
                hasOpenParen = true;
            }
            if (reference.data()[i] == ')') {
                hasCloseParen = true;
            }
        }

        if (hasOpenParen && hasCloseParen) {
            // It's a formula
            MyString formulaName, parametersString;
            if (parseFormula(reference, formulaName, parametersString)) {
                FormulaType type = getFormulaType(formulaName);
                MyVector<FormulaParameter> params = parseFormulaParameters(parametersString, table);

                auto formulaCell = make_unique<FormulaCell>(type, params);
                if (table != nullptr) {
                    formulaCell->setTablePtr(table);
                }
                return formulaCell;
            }
            else {
                return make_unique<ValueCell<MyString>>(MyString("#ERROR!"));
            }
        }
        else {
            // It's a simple cell reference
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

bool CellFactory::parseFormula(const MyString& input, MyString& formulaName, MyString& parametersString) {
    const char* str = input.data();
    size_t openParenPos = 0;
    size_t closeParenPos = 0;

    // Find opening parenthesis
    for (size_t i = 0; i < input.length(); i++) {
        if (str[i] == '(') {
            openParenPos = i;
            break;
        }
    }

    // Find closing parenthesis 
    for (size_t i = input.length(); i > 0; i--) {
        if (str[i - 1] == ')') {
            closeParenPos = i - 1;
            break;
        }
    }

    if (openParenPos >= closeParenPos || openParenPos == 0) {
        return false;
    }

    // Extract formula name
    char* nameBuffer = new char[openParenPos + 1];
    for (size_t i = 0; i < openParenPos; i++) {
        nameBuffer[i] = str[i];
    }
    nameBuffer[openParenPos] = '\0';
    formulaName = MyString(nameBuffer);
    delete[] nameBuffer;

    // Extract parameters
    if (closeParenPos > openParenPos + 1) {
        size_t paramLength = closeParenPos - openParenPos - 1;
        char* paramBuffer = new char[paramLength + 1];
        for (size_t i = 0; i < paramLength; i++) {
            paramBuffer[i] = str[openParenPos + 1 + i];
        }
        paramBuffer[paramLength] = '\0';
        parametersString = MyString(paramBuffer);
        delete[] paramBuffer;
    }
    else {
        parametersString = MyString("");
    }

    return true;
}

FormulaType CellFactory::getFormulaType(const MyString& formulaName) {
    if (formulaName == MyString("SUM")) {
        return FormulaType::SUM;
    }
    else if (formulaName == MyString("AVERAGE")) {
        return FormulaType::AVERAGE;
    }
    else if (formulaName == MyString("MAX")) {
        return FormulaType::MAX;
    }
    else if (formulaName == MyString("LEN")) {
        return FormulaType::LEN;
    }
    else if (formulaName == MyString("CONCAT")) {
        return FormulaType::CONCAT;
    }
    else if (formulaName == MyString("SUBSTR")) {
        return FormulaType::SUBSTR;
    }
    else if (formulaName == MyString("COUNT")) {
        return FormulaType::COUNT;
    }

    return FormulaType::SUM; // Default
}


MyVector<FormulaParameter> CellFactory::parseFormulaParameters(const MyString& parametersString, Table* table) {
    MyVector<FormulaParameter> parameters;

    if (parametersString.length() == 0) {
        return parameters;
    }

    // Split by commas 
    const char* str = parametersString.data();
    size_t start = 0;

    for (size_t i = 0; i <= parametersString.length(); i++) {
        if (i == parametersString.length() || str[i] == ',') {
            if (i > start) {
                // Extract parameter
                size_t paramLen = i - start;
                char* paramBuffer = new char[paramLen + 1];
                for (size_t j = 0; j < paramLen; j++) {
                    paramBuffer[j] = str[start + j];
                }
                paramBuffer[paramLen] = '\0';

                // Trim whitespace
                while (paramLen > 0 && (paramBuffer[0] == ' ' || paramBuffer[0] == '\t')) {
                    for (size_t k = 0; k < paramLen; k++) {
                        paramBuffer[k] = paramBuffer[k + 1];
                    }
                    paramLen--;
                }
                while (paramLen > 0 && (paramBuffer[paramLen - 1] == ' ' || paramBuffer[paramLen - 1] == '\t')) {
                    paramBuffer[paramLen - 1] = '\0';
                    paramLen--;
                }

                MyString param(paramBuffer);
                delete[] paramBuffer;

                FormulaParameter fp = parseParameter(param, table);
                parameters.push_back(fp);
            }
            start = i + 1;
        }
    }

    return parameters;
}

FormulaParameter CellFactory::parseParameter(const MyString& param, Table* table) {
    FormulaParameter fp;

    if (param.length() == 0) {
        fp.type = FormulaParameter::INTEGER_VALUE;
        fp.intValue = 0;
        return fp;
    }

    const char* str = param.data();

    // Check for range (contains ':')
    bool hasColon = false;
    for (size_t i = 0; i < param.length(); i++) {
        if (str[i] == ':') {
            hasColon = true;
            break;
        }
    }

    if (hasColon) {
        // It's a range
        size_t startRow, startCol, endRow, endCol;
        if (parseRange(param, startRow, startCol, endRow, endCol)) {
            fp.type = FormulaParameter::CELL_RANGE;
            fp.startRow = startRow;
            fp.startCol = startCol;
            fp.endRow = endRow;
            fp.endCol = endCol;
        }
        else {
            // Invalid range, treat as string
            fp.type = FormulaParameter::STRING_VALUE;
            fp.stringValue = param;
        }
        return fp;
    }

    // Check for single cell reference
    size_t row, col;
    if (parseCellReference(param, row, col)) {
        fp.type = FormulaParameter::SINGLE_CELL;
        fp.row = row;
        fp.col = col;
        return fp;
    }

    // Check for boolean
    if (param == MyString("true")) {
        fp.type = FormulaParameter::BOOLEAN_VALUE;
        fp.boolValue = true;
        return fp;
    }
    if (param == MyString("false")) {
        fp.type = FormulaParameter::BOOLEAN_VALUE;
        fp.boolValue = false;
        return fp;
    }

    // Check for quoted string
    if (param.length() >= 2 && str[0] == '"' && str[param.length() - 1] == '"') {
        fp.type = FormulaParameter::STRING_VALUE;

        size_t contentLength = param.length() - 2;
        char* buffer = new char[contentLength + 1];
        for (size_t i = 0; i < contentLength; i++) {
            buffer[i] = str[i + 1];
        }
        buffer[contentLength] = '\0';
        fp.stringValue = MyString(buffer);
        delete[] buffer;

        return fp;
    }

    // Check for integer
    bool isNegative = false;
    size_t start = 0;

    if (param.length() > 0 && str[0] == '-') {
        isNegative = true;
        start = 1;
    }

    bool isNumber = true;
    if (start >= param.length()) {
        isNumber = false;
    }
    else {
        for (size_t i = start; i < param.length(); i++) {
            char c = str[i];
            if (!(c >= '0' && c <= '9')) {
                isNumber = false;
                break;
            }
        }
    }

    if (isNumber) {
        int number = 0;
        for (size_t i = start; i < param.length(); i++) {
            number = number * 10 + (str[i] - '0');
        }
        if (isNegative) {
            number *= -1;
        }
        fp.type = FormulaParameter::INTEGER_VALUE;
        fp.intValue = number;
        return fp;
    }

    // Default to string
    fp.type = FormulaParameter::STRING_VALUE;
    fp.stringValue = param;
    return fp;
}

bool CellFactory::parseRange(const MyString& range, size_t& startRow, size_t& startCol, size_t& endRow, size_t& endCol) {
    const char* str = range.data();
    size_t colonPos = 0;

    // Find colon position
    for (size_t i = 0; i < range.length(); i++) {
        if (str[i] == ':') {
            colonPos = i;
            break;
        }
    }

    if (colonPos == 0 || colonPos >= range.length() - 1) {
        return false;
    }

    // Extract start cell
    char* startBuffer = new char[colonPos + 1];
    for (size_t i = 0; i < colonPos; i++) {
        startBuffer[i] = str[i];
    }
    startBuffer[colonPos] = '\0';
    MyString startCell(startBuffer);
    delete[] startBuffer;

    // Extract end cell
    size_t endLength = range.length() - colonPos - 1;
    char* endBuffer = new char[endLength + 1];
    for (size_t i = 0; i < endLength; i++) {
        endBuffer[i] = str[colonPos + 1 + i];
    }
    endBuffer[endLength] = '\0';
    MyString endCell(endBuffer);
    delete[] endBuffer;

    // Parse both cells
    if (!parseCellReference(startCell, startRow, startCol) ||
        !parseCellReference(endCell, endRow, endCol)) {
        return false;
    }

    // Ensure start <= end
    if (startRow > endRow || startCol > endCol) {
        size_t tempRow = startRow, tempCol = startCol;
        startRow = endRow; startCol = endCol;
        endRow = tempRow; endCol = tempCol;
    }

    return true;
}