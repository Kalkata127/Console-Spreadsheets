#include "FormulaCell.h"
#include "Table.h"

FormulaCell::FormulaCell(FormulaType type, const MyVector<FormulaParameter>& params)
    : formulaType(type), parameters(params), tablePtr(nullptr) {
}

FormulaCell::FormulaCell(const FormulaCell& other)
    : formulaType(other.formulaType), parameters(other.parameters), tablePtr(other.tablePtr) {
}

FormulaCell& FormulaCell::operator=(const FormulaCell& other) {
    if (this != &other) {
        formulaType = other.formulaType;
        parameters = other.parameters;
        tablePtr = other.tablePtr;
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
        if (std::isnan(result)) {
            return MyString("#VALUE!");
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
    default:
        return MyString("#ERROR!");
    }
}

double FormulaCell::evaluate() const {
    switch (formulaType) {
    case FormulaType::SUM:
        return calculateSum();
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
        // Ignore string values in SUM
        break;
    }

    return values;
}

double FormulaCell::calculateSum() const {
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
        return -1.0;
    }

    return sum;
}