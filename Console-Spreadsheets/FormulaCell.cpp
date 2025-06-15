#include "FormulaCell.h"
#include "Table.h"

FormulaCell::FormulaCell(FormulaType type, const MyVector<FormulaParameter>& params)
    : formulaType(type), parameters(params), tablePtr(nullptr), hasError(false) {
}

FormulaCell::FormulaCell(const FormulaCell& other)
    : formulaType(other.formulaType), parameters(other.parameters), tablePtr(other.tablePtr),
    hasError(other.hasError), errorMessage(other.errorMessage) {
}

FormulaCell& FormulaCell::operator=(const FormulaCell& other) {
    if (this != &other) {
        formulaType = other.formulaType;
        parameters = other.parameters;
        tablePtr = other.tablePtr;
        hasError = other.hasError;
        errorMessage = other.errorMessage;
    }
    return *this;
}

void FormulaCell::setTablePtr(Table* table) {
    tablePtr = table;
}

Table* FormulaCell::getTablePtr() const {
    return tablePtr;
}

MyString FormulaCell::toString() const {
    switch (formulaType) {
    case FormulaType::SUM: {
        double result = calculateSum();
        if (hasError) {
            return errorMessage;
        }

        // Convert double to string
        int intResult = static_cast<int>(result);
        if (result == static_cast<double>(intResult)) {
            // It's a whole number, display as integer
            if (intResult == 0) return MyString("0");

            bool isNegative = intResult < 0;
            int absValue = isNegative ? -intResult : intResult;

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
        else {
            // It's a decimal, simple representation
            char buffer[64];
            int wholePart = static_cast<int>(result);
            double fractionalPart = result - wholePart;

            // For simplicity, show 2 decimal places
            int decimalPart = static_cast<int>(fractionalPart * 100 + 0.5);

            // Convert whole part
            char wholeStr[32];
            int wholeIndex = 0;
            int absWhole = wholePart < 0 ? -wholePart : wholePart;

            if (absWhole == 0) {
                wholeStr[wholeIndex++] = '0';
            }
            else {
                while (absWhole > 0) {
                    wholeStr[wholeIndex++] = '0' + (absWhole % 10);
                    absWhole /= 10;
                }
            }

            if (wholePart < 0) wholeStr[wholeIndex++] = '-';
            wholeStr[wholeIndex] = '\0';

            // Reverse whole part
            for (int i = 0; i < wholeIndex / 2; ++i) {
                char tmp = wholeStr[i];
                wholeStr[i] = wholeStr[wholeIndex - i - 1];
                wholeStr[wholeIndex - i - 1] = tmp;
            }

            // Combine with decimal
            int bufferIndex = 0;
            for (int i = 0; i < wholeIndex; i++) {
                buffer[bufferIndex++] = wholeStr[i];
            }
            buffer[bufferIndex++] = '.';
            buffer[bufferIndex++] = '0' + (decimalPart / 10);
            buffer[bufferIndex++] = '0' + (decimalPart % 10);
            buffer[bufferIndex] = '\0';

            return MyString(buffer);
        }
    }
    case FormulaType::AVERAGE: {
        double result = calculateAverage();
        if (hasError) {
            return errorMessage;
        }

        // Same number formatting as SUM
        int intResult = static_cast<int>(result);
        if (result == static_cast<double>(intResult)) {
            if (intResult == 0) return MyString("0");

            bool isNegative = intResult < 0;
            int absValue = isNegative ? -intResult : intResult;

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
        else {
            // Decimal formatting
            char buffer[64];
            int wholePart = static_cast<int>(result);
            double fractionalPart = result - wholePart;
            int decimalPart = static_cast<int>(fractionalPart * 100 + 0.5);

            char wholeStr[32];
            int wholeIndex = 0;
            int absWhole = wholePart < 0 ? -wholePart : wholePart;

            if (absWhole == 0) {
                wholeStr[wholeIndex++] = '0';
            }
            else {
                while (absWhole > 0) {
                    wholeStr[wholeIndex++] = '0' + (absWhole % 10);
                    absWhole /= 10;
                }
            }

            if (wholePart < 0) wholeStr[wholeIndex++] = '-';
            wholeStr[wholeIndex] = '\0';

            for (int i = 0; i < wholeIndex / 2; ++i) {
                char tmp = wholeStr[i];
                wholeStr[i] = wholeStr[wholeIndex - i - 1];
                wholeStr[wholeIndex - i - 1] = tmp;
            }

            int bufferIndex = 0;
            for (int i = 0; i < wholeIndex; i++) {
                buffer[bufferIndex++] = wholeStr[i];
            }
            buffer[bufferIndex++] = '.';
            buffer[bufferIndex++] = '0' + (decimalPart / 10);
            buffer[bufferIndex++] = '0' + (decimalPart % 10);
            buffer[bufferIndex] = '\0';

            return MyString(buffer);
        }
    }
    case FormulaType::MAX: {
        double result = calculateMax();
        if (hasError) {
            return errorMessage;
        }

        // Same integer formatting as SUM
        int intResult = static_cast<int>(result);
        if (intResult == 0) return MyString("0");

        bool isNegative = intResult < 0;
        int absValue = isNegative ? -intResult : intResult;

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
    case FormulaType::LEN: {
        int result = calculateLen();
        if (hasError) {
            return errorMessage;
        }

        if (result == 0) return MyString("0");

        char buffer[32];
        int index = 0;

        while (result > 0) {
            buffer[index++] = '0' + (result % 10);
            result /= 10;
        }

        buffer[index] = '\0';

        for (int i = 0; i < index / 2; ++i) {
            char tmp = buffer[i];
            buffer[i] = buffer[index - i - 1];
            buffer[index - i - 1] = tmp;
        }

        return MyString(buffer);
    }
    case FormulaType::CONCAT: {
        MyString result = calculateConcat();
        if (hasError) {
            return errorMessage;
        }
        return result;
    }
    case FormulaType::SUBSTR: {
        MyString result = calculateSubstr();
        if (hasError) {
            return errorMessage;
        }
        return result;
    }
    case FormulaType::COUNT: {
        int result = calculateCount();
        if (hasError) {
            return errorMessage;
        }

        if (result == 0) return MyString("0");

        char buffer[32];
        int index = 0;

        while (result > 0) {
            buffer[index++] = '0' + (result % 10);
            result /= 10;
        }

        buffer[index] = '\0';

        for (int i = 0; i < index / 2; ++i) {
            char tmp = buffer[i];
            buffer[i] = buffer[index - i - 1];
            buffer[index - i - 1] = tmp;
        }

        return MyString(buffer);
    }
    default:
        return MyString("#ERROR!");
    }
}

double FormulaCell::evaluate() const {
    switch (formulaType) {
    case FormulaType::SUM:
        return calculateSum();
    case FormulaType::AVERAGE:
        return calculateAverage();
    case FormulaType::MAX:
        return calculateMax();
    case FormulaType::LEN:
        return static_cast<double>(calculateLen());
    case FormulaType::CONCAT:
        return 0.0; // CONCAT returns string, evaluate returns 0
    case FormulaType::SUBSTR:
        return 0.0; // SUBSTR returns string, evaluate returns 0
    case FormulaType::COUNT:
        return static_cast<double>(calculateCount());
    default:
        return 0.0;
    }
}

MyString FormulaCell::getType() const {
    return MyString("FormulaCell");
}

BaseCell* FormulaCell::clone() const {
    return new FormulaCell(*this);
}

FormulaType FormulaCell::getFormulaType() const {
    return formulaType;
}

const MyVector<FormulaParameter>& FormulaCell::getParameters() const {
    return parameters;
}

MyVector<double> FormulaCell::getParameterValues(const FormulaParameter& param) const {
    MyVector<double> values;

    switch (param.type) {
    case FormulaParameter::SINGLE_CELL: {
        if (tablePtr != nullptr) {
            BaseCell* cell = tablePtr->getCell(param.row, param.col);
            if (cell != nullptr) {
                MyString cellType = cell->getType();
                if (cellType == MyString("int") || cellType == MyString("bool") ||
                    cellType == MyString("ReferenceCell") || cellType == MyString("FormulaCell")) {
                    values.push_back(cell->evaluate());
                }
                // Ignore string cells and empty cells
            }
        }
        break;
    }
    case FormulaParameter::CELL_RANGE: {
        if (tablePtr != nullptr) {
            for (size_t row = param.startRow; row <= param.endRow; row++) {
                for (size_t col = param.startCol; col <= param.endCol; col++) {
                    if (row < tablePtr->getRowCount() && col < tablePtr->getColumnCount()) {
                        BaseCell* cell = tablePtr->getCell(row, col);
                        if (cell != nullptr) {
                            MyString cellType = cell->getType();
                            if (cellType == MyString("int") || cellType == MyString("bool") ||
                                cellType == MyString("ReferenceCell") || cellType == MyString("FormulaCell")) {
                                values.push_back(cell->evaluate());
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    case FormulaParameter::INTEGER_VALUE:
        values.push_back(static_cast<double>(param.intValue));
        break;
    case FormulaParameter::BOOLEAN_VALUE:
        values.push_back(param.boolValue ? 1.0 : 0.0);
        break;
    case FormulaParameter::STRING_VALUE:
        // Ignore string values in numeric calculations
        break;
    }

    return values;
}

MyVector<MyString> FormulaCell::getParameterStringValues(const FormulaParameter& param) const {
    MyVector<MyString> values;

    switch (param.type) {
    case FormulaParameter::SINGLE_CELL: {
        if (tablePtr != nullptr) {
            BaseCell* cell = tablePtr->getCell(param.row, param.col);
            if (cell != nullptr) {
                values.push_back(cell->toString());
            }
        }
        break;
    }
    case FormulaParameter::CELL_RANGE: {
        if (tablePtr != nullptr) {
            for (size_t row = param.startRow; row <= param.endRow; row++) {
                for (size_t col = param.startCol; col <= param.endCol; col++) {
                    if (row < tablePtr->getRowCount() && col < tablePtr->getColumnCount()) {
                        BaseCell* cell = tablePtr->getCell(row, col);
                        if (cell != nullptr) {
                            MyString cellValue = cell->toString();
                            if (cellValue.length() > 0) { // Skip empty cells
                                values.push_back(cellValue);
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    case FormulaParameter::INTEGER_VALUE: {
        // Convert int to string
        int val = param.intValue;
        if (val == 0) {
            values.push_back(MyString("0"));
        }
        else {
            bool isNegative = val < 0;
            int absVal = isNegative ? -val : val;

            char buffer[32];
            int index = 0;

            while (absVal > 0) {
                buffer[index++] = '0' + (absVal % 10);
                absVal /= 10;
            }

            if (isNegative) buffer[index++] = '-';
            buffer[index] = '\0';

            for (int i = 0; i < index / 2; ++i) {
                char tmp = buffer[i];
                buffer[i] = buffer[index - i - 1];
                buffer[index - i - 1] = tmp;
            }

            values.push_back(MyString(buffer));
        }
        break;
    }
    case FormulaParameter::BOOLEAN_VALUE:
        values.push_back(param.boolValue ? MyString("true") : MyString("false"));
        break;
    case FormulaParameter::STRING_VALUE:
        values.push_back(param.stringValue);
        break;
    }

    return values;
}

bool FormulaCell::hasErrorInParameters() const {
    for (size_t i = 0; i < parameters.getSize(); i++) {
        const FormulaParameter& param = parameters[i];

        if (param.type == FormulaParameter::SINGLE_CELL) {
            if (tablePtr != nullptr) {
                BaseCell* cell = tablePtr->getCell(param.row, param.col);
                if (cell != nullptr) {
                    MyString cellValue = cell->toString();
                    if (cellValue == MyString("#VALUE!")) {
                        return true;
                    }
                }
            }
        }
        else if (param.type == FormulaParameter::CELL_RANGE) {
            if (tablePtr != nullptr) {
                for (size_t row = param.startRow; row <= param.endRow; row++) {
                    for (size_t col = param.startCol; col <= param.endCol; col++) {
                        if (row < tablePtr->getRowCount() && col < tablePtr->getColumnCount()) {
                            BaseCell* cell = tablePtr->getCell(row, col);
                            if (cell != nullptr) {
                                MyString cellValue = cell->toString();
                                if (cellValue == MyString("#VALUE!")) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

double FormulaCell::calculateSum() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    double sum = 0.0;
    bool hasValidValue = false;

    for (size_t i = 0; i < parameters.getSize(); i++) {
        MyVector<double> paramValues = getParameterValues(parameters[i]);
        for (size_t j = 0; j < paramValues.getSize(); j++) {
            sum += paramValues[j];
            hasValidValue = true;
        }
    }

    if (!hasValidValue) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    return sum;
}

double FormulaCell::calculateAverage() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    double sum = 0.0;
    size_t count = 0;

    // Must have at least 1 parameter
    if (parameters.getSize() == 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    for (size_t i = 0; i < parameters.getSize(); i++) {
        MyVector<double> paramValues = getParameterValues(parameters[i]);
        for (size_t j = 0; j < paramValues.getSize(); j++) {
            sum += paramValues[j];
            count++;
        }
    }

    if (count == 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    return sum / static_cast<double>(count);
}

double FormulaCell::calculateMax() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    // Must have exactly 1 parameter
    if (parameters.getSize() != 1) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    // Parameter must be a range
    if (parameters[0].type != FormulaParameter::CELL_RANGE) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    MyVector<double> values = getParameterValues(parameters[0]);

    if (values.getSize() == 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0.0;
    }

    double maxValue = values[0];
    for (size_t i = 1; i < values.getSize(); i++) {
        if (values[i] > maxValue) {
            maxValue = values[i];
        }
    }

    return maxValue;
}

int FormulaCell::calculateLen() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0;
    }

    // Must have exactly 1 parameter
    if (parameters.getSize() != 1) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0;
    }

    // Parameter must NOT be a range
    if (parameters[0].type == FormulaParameter::CELL_RANGE) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0;
    }

    MyVector<MyString> values = getParameterStringValues(parameters[0]);

    if (values.getSize() == 0) {
        return 0; // Empty cell has length 0
    }

    return static_cast<int>(values[0].length());
}

MyString FormulaCell::calculateConcat() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Must have exactly 2 parameters
    if (parameters.getSize() != 2) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // First parameter must be a range
    if (parameters[0].type != FormulaParameter::CELL_RANGE) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Get the range values
    MyVector<MyString> rangeValues = getParameterStringValues(parameters[0]);

    // Get the delimiter
    MyVector<MyString> delimiterValues = getParameterStringValues(parameters[1]);
    if (delimiterValues.getSize() == 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    MyString delimiter = delimiterValues[0];

    // If all cells in range are empty
    if (rangeValues.getSize() == 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Concatenate with delimiter
    MyString result = rangeValues[0];
    for (size_t i = 1; i < rangeValues.getSize(); i++) {
        result = result + delimiter + rangeValues[i];
    }

    return result;
}

MyString FormulaCell::calculateSubstr() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Must have exactly 3 parameters
    if (parameters.getSize() != 3) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // First parameter cannot be a range
    if (parameters[0].type == FormulaParameter::CELL_RANGE) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Second and third parameters must be non-negative integers
    if (parameters[1].type != FormulaParameter::INTEGER_VALUE ||
        parameters[2].type != FormulaParameter::INTEGER_VALUE) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    int startIndex = parameters[1].intValue;
    int length = parameters[2].intValue;

    // Check for negative values
    if (startIndex < 0 || length <= 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Get the string parameter
    MyVector<MyString> stringValues = getParameterStringValues(parameters[0]);
    if (stringValues.getSize() == 0) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    MyString sourceString = stringValues[0];
    size_t sourceLen = sourceString.length();

    // Check if startIndex is beyond string length
    if (static_cast<size_t>(startIndex) >= sourceLen) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Check if startIndex + length is beyond string length
    if (static_cast<size_t>(startIndex + length) > sourceLen) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return MyString("");
    }

    // Extract substring
    char* buffer = new char[length + 1];
    const char* sourceData = sourceString.data();

    for (int i = 0; i < length; i++) {
        buffer[i] = sourceData[startIndex + i];
    }
    buffer[length] = '\0';

    MyString result(buffer);
    delete[] buffer;

    return result;
}

int FormulaCell::calculateCount() const {
    hasError = false;

    // Check for #VALUE! in any parameter first
    if (hasErrorInParameters()) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0;
    }

    // Must have exactly 1 parameter
    if (parameters.getSize() != 1) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0;
    }

    // Parameter must be a range
    if (parameters[0].type != FormulaParameter::CELL_RANGE) {
        hasError = true;
        errorMessage = MyString("#VALUE!");
        return 0;
    }

    int count = 0;

    if (tablePtr != nullptr) {
        for (size_t row = parameters[0].startRow; row <= parameters[0].endRow; row++) {
            for (size_t col = parameters[0].startCol; col <= parameters[0].endCol; col++) {
                if (row < tablePtr->getRowCount() && col < tablePtr->getColumnCount()) {
                    BaseCell* cell = tablePtr->getCell(row, col);
                    if (cell != nullptr) {
                        MyString cellValue = cell->toString();
                        if (cellValue.length() > 0) { // Non-empty cell
                            count++;
                        }
                    }
                }
            }
        }
    }

    return count;
}