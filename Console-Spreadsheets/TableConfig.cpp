#include "TableConfig.h"
#include <cstdlib>

TableConfig::TableConfig()
    : initialTableRows(0), initialTableCols(0), maxTableRows(0), maxTableCols(0),
    autoFit(false), visibleCellSymbols(0), initialAlignment(Alignment::LEFT),
    clearConsoleAfterCommand(false),
    hasInitialTableRows(false), hasInitialTableCols(false), hasMaxTableRows(false),
    hasMaxTableCols(false), hasAutoFit(false), hasVisibleCellSymbols(false),
    hasInitialAlignment(false), hasClearConsoleAfterCommand(false) {
}

TableConfig::TableConfig(const TableConfig& other)
    : initialTableRows(other.initialTableRows), initialTableCols(other.initialTableCols),
    maxTableRows(other.maxTableRows), maxTableCols(other.maxTableCols),
    autoFit(other.autoFit), visibleCellSymbols(other.visibleCellSymbols),
    initialAlignment(other.initialAlignment), clearConsoleAfterCommand(other.clearConsoleAfterCommand),
    hasInitialTableRows(other.hasInitialTableRows), hasInitialTableCols(other.hasInitialTableCols),
    hasMaxTableRows(other.hasMaxTableRows), hasMaxTableCols(other.hasMaxTableCols),
    hasAutoFit(other.hasAutoFit), hasVisibleCellSymbols(other.hasVisibleCellSymbols),
    hasInitialAlignment(other.hasInitialAlignment), hasClearConsoleAfterCommand(other.hasClearConsoleAfterCommand) {
}

TableConfig& TableConfig::operator=(const TableConfig& other) {
    if (this != &other) {
        initialTableRows = other.initialTableRows;
        initialTableCols = other.initialTableCols;
        maxTableRows = other.maxTableRows;
        maxTableCols = other.maxTableCols;
        autoFit = other.autoFit;
        visibleCellSymbols = other.visibleCellSymbols;
        initialAlignment = other.initialAlignment;
        clearConsoleAfterCommand = other.clearConsoleAfterCommand;
        hasInitialTableRows = other.hasInitialTableRows;
        hasInitialTableCols = other.hasInitialTableCols;
        hasMaxTableRows = other.hasMaxTableRows;
        hasMaxTableCols = other.hasMaxTableCols;
        hasAutoFit = other.hasAutoFit;
        hasVisibleCellSymbols = other.hasVisibleCellSymbols;
        hasInitialAlignment = other.hasInitialAlignment;
        hasClearConsoleAfterCommand = other.hasClearConsoleAfterCommand;
    }
    return *this;
}

bool TableConfig::loadFromFile(const MyString& filename) {
    ::ifstream file(filename.data());

    if (!file.is_open()) {
        cout << "ABORTING! Could not open config file: " << filename.data() << ::endl;
        return false;
    }

    char line[1000];
    while (file.getline(line, 1000)) {
        MyString lineStr(line);

        // Skip empty lines
        if (lineStr.length() == 0) {
            continue;
        }

        // Parse the property
        if (!parseProperty(lineStr)) {
            file.close();
            return false;
        }
    }

    file.close();

    // Check if all required properties are set
    if (!allPropertiesSet()) {
        cout << "ABORTING! Not all required properties are set in config file." << ::endl;
        return false;
    }

    return true;
}

bool TableConfig::parseProperty(const MyString& line) {
    // Find the colon separator
    const char* lineData = line.data();
    size_t colonPos = 0;
    bool foundColon = false;

    for (size_t i = 0; i < line.length(); i++) {
        if (lineData[i] == ':') {
            colonPos = i;
            foundColon = true;
            break;
        }
    }

    if (!foundColon) {
        cout << "ABORTING! Invalid line format (missing ':'): " << line.data() << ::endl;
        return false;
    }

    // Extract property name
    char* propertyBuffer = new char[colonPos + 1];
    for (size_t i = 0; i < colonPos; i++) {
        propertyBuffer[i] = lineData[i];
    }
    propertyBuffer[colonPos] = '\0';
    MyString propertyName(propertyBuffer);
    delete[] propertyBuffer;

    // Extract value
    size_t valueLength = line.length() - colonPos - 1;
    char* valueBuffer = new char[valueLength + 1];
    for (size_t i = 0; i < valueLength; i++) {
        valueBuffer[i] = lineData[colonPos + 1 + i];
    }
    valueBuffer[valueLength] = '\0';
    MyString value(valueBuffer);
    delete[] valueBuffer;

    // Validate and set the property
    return validateAndSetProperty(propertyName, value);
}

bool TableConfig::isValidPropertyName(const MyString& propertyName) {
    return (propertyName == MyString("initialTableRows") ||
        propertyName == MyString("initialTableCols") ||
        propertyName == MyString("maxTableRows") ||
        propertyName == MyString("maxTableCols") ||
        propertyName == MyString("autoFit") ||
        propertyName == MyString("visibleCellSymbols") ||
        propertyName == MyString("initialAlignment") ||
        propertyName == MyString("clearConsoleAfterCommand"));
}

bool TableConfig::validateAndSetProperty(const MyString& propertyName, const MyString& value) {
    if (!isValidPropertyName(propertyName)) {
        printErrorAndExit(propertyName, value, MyString("Missing property!"));
        return false;
    }

    if (propertyName == MyString("initialTableRows")) {
        int val;
        if (!parsePositiveInteger(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        initialTableRows = val;
        hasInitialTableRows = true;
    }
    else if (propertyName == MyString("initialTableCols")) {
        int val;
        if (!parsePositiveInteger(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        initialTableCols = val;
        hasInitialTableCols = true;
    }
    else if (propertyName == MyString("maxTableRows")) {
        int val;
        if (!parsePositiveInteger(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        maxTableRows = val;
        hasMaxTableRows = true;
    }
    else if (propertyName == MyString("maxTableCols")) {
        int val;
        if (!parsePositiveInteger(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        maxTableCols = val;
        hasMaxTableCols = true;
    }
    else if (propertyName == MyString("autoFit")) {
        bool val;
        if (!parseBoolean(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        autoFit = val;
        hasAutoFit = true;
    }
    else if (propertyName == MyString("visibleCellSymbols")) {
        int val;
        if (!parsePositiveInteger(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        visibleCellSymbols = val;
        hasVisibleCellSymbols = true;
    }
    else if (propertyName == MyString("initialAlignment")) {
        Alignment val;
        if (!parseAlignment(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        initialAlignment = val;
        hasInitialAlignment = true;
    }
    else if (propertyName == MyString("clearConsoleAfterCommand")) {
        bool val;
        if (!parseBoolean(value, val)) {
            printErrorAndExit(propertyName, value, MyString("Invalid value!"));
            return false;
        }
        clearConsoleAfterCommand = val;
        hasClearConsoleAfterCommand = true;
    }

    return true;
}

bool TableConfig::parsePositiveInteger(const MyString& value, int& result) {
    if (value.length() == 0) {
        return false;
    }

    const char* str = value.data();

    // Check if all characters are digits
    for (size_t i = 0; i < value.length(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }

    // Convert to integer
    int number = 0;
    for (size_t i = 0; i < value.length(); i++) {
        number = number * 10 + (str[i] - '0');
    }

    if (number <= 0) {
        return false;
    }

    result = number;
    return true;
}

bool TableConfig::parseBoolean(const MyString& value, bool& result) {
    if (value == MyString("true")) {
        result = true;
        return true;
    }
    else if (value == MyString("false")) {
        result = false;
        return true;
    }

    return false;
}

bool TableConfig::parseAlignment(const MyString& value, Alignment& result) {
    if (value == MyString("left")) {
        result = Alignment::LEFT;
        return true;
    }
    else if (value == MyString("center")) {
        result = Alignment::CENTER;
        return true;
    }
    else if (value == MyString("right")) {
        result = Alignment::RIGHT;
        return true;
    }

    return false;
}

void TableConfig::printErrorAndExit(const MyString& propertyName, const MyString& value, const MyString& errorMessage) {
    cout << "ABORTING! " << propertyName.data() << ":" << value.data()
        << " - " << errorMessage.data() << ::endl;
    exit(1);
}

bool TableConfig::allPropertiesSet() const {
    return hasInitialTableRows && hasInitialTableCols && hasMaxTableRows &&
        hasMaxTableCols && hasAutoFit && hasVisibleCellSymbols &&
        hasInitialAlignment && hasClearConsoleAfterCommand;
}

// Getters
int TableConfig::getInitialTableRows() const {
    return initialTableRows;
}

int TableConfig::getInitialTableCols() const {
    return initialTableCols;
}

int TableConfig::getMaxTableRows() const {
    return maxTableRows;
}

int TableConfig::getMaxTableCols() const {
    return maxTableCols;
}

bool TableConfig::getAutoFit() const {
    return autoFit;
}

int TableConfig::getVisibleCellSymbols() const {
    return visibleCellSymbols;
}

Alignment TableConfig::getInitialAlignment() const {
    return initialAlignment;
}

bool TableConfig::getClearConsoleAfterCommand() const {
    return clearConsoleAfterCommand;
}

void TableConfig::printConfig() const {
    cout << "Configuration loaded:" << ::endl;
    cout << "  initialTableRows: " << initialTableRows << ::endl;
    cout << "  initialTableCols: " << initialTableCols << ::endl;
    cout << "  maxTableRows: " << maxTableRows << ::endl;
    cout << "  maxTableCols: " << maxTableCols << ::endl;
    cout << "  autoFit: " << (autoFit ? "true" : "false") << ::endl;
    cout << "  visibleCellSymbols: " << visibleCellSymbols << ::endl;
    cout << "  initialAlignment: ";
    switch (initialAlignment) {
    case Alignment::LEFT: cout << "left"; break;
    case Alignment::CENTER: cout << "center"; break;
    case Alignment::RIGHT: cout << "right"; break;
    }
    cout << ::endl;
    cout << "  clearConsoleAfterCommand: " << (clearConsoleAfterCommand ? "true" : "false") << ::endl;
}

bool TableConfig::isValidConfig() const {
    return allPropertiesSet();
}