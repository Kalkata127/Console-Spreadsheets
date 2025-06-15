#pragma once
#include "MyString.h"
#include <iostream>
#include <fstream>

enum class Alignment {
    LEFT,
    CENTER,
    RIGHT
};

class TableConfig {
private:
    // Required properties
    int initialTableRows;
    int initialTableCols;
    int maxTableRows;
    int maxTableCols;
    bool autoFit;
    int visibleCellSymbols;
    Alignment initialAlignment;
    bool clearConsoleAfterCommand;

    // Validation flags
    bool hasInitialTableRows;
    bool hasInitialTableCols;
    bool hasMaxTableRows;
    bool hasMaxTableCols;
    bool hasAutoFit;
    bool hasVisibleCellSymbols;
    bool hasInitialAlignment;
    bool hasClearConsoleAfterCommand;

    // Helper methods
    bool parseProperty(const MyString& line);
    bool isValidPropertyName(const MyString& propertyName);
    bool validateAndSetProperty(const MyString& propertyName, const MyString& value);
    bool parsePositiveInteger(const MyString& value, int& result);
    bool parseBoolean(const MyString& value, bool& result);
    bool parseAlignment(const MyString& value, Alignment& result);
    void printErrorAndExit(const MyString& propertyName, const MyString& value, const MyString& errorMessage);
    bool allPropertiesSet() const;

public:
    TableConfig();
    TableConfig(const TableConfig& other);
    TableConfig& operator=(const TableConfig& other); 
    ~TableConfig() = default;

    // Load configuration from file
    bool loadFromFile(const MyString& filename);

    int getInitialTableRows() const;
    int getInitialTableCols() const;
    int getMaxTableRows() const;
    int getMaxTableCols() const;
    bool getAutoFit() const;
    int getVisibleCellSymbols() const;
    Alignment getInitialAlignment() const;
    bool getClearConsoleAfterCommand() const;

    // Utility methods
    void printConfig() const;
    bool isValidConfig() const;
};
