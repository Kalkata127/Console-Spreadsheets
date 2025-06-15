#pragma once
#include "Table.h"
#include "TableConfig.h"
#include "MyString.h"
#include <iostream>

class ConsoleUI {
private:
    Table* currentTable;
    bool running;

    MyVector<MyString> parseCommand(const MyString& input);
    bool parseCellReference(const MyString& cellRef, size_t& row, size_t& col);

    void handleCellInsert(const MyVector<MyString>& tokens);
    void handleCellDelete(const MyVector<MyString>& tokens);
    void handleCellReference(const MyVector<MyString>& tokens);
    void handleCellFormula(const MyVector<MyString>& tokens);
    void handleAddRow();
    void handleAddColumn();
    void handleInsertRow(const MyVector<MyString>& tokens);
    void handleInsertColumn(const MyVector<MyString>& tokens);
    void handleRemoveRow(const MyVector<MyString>& tokens);
    void handleRemoveColumn(const MyVector<MyString>& tokens);
    void handleDisplay();
    void handleResize(const MyVector<MyString>& tokens);
    void handleExit();
    void handleOpen(const MyVector<MyString>& tokens);
    void handleNew(const MyVector<MyString>& tokens);
    void handleSave(const MyVector<MyString>& tokens);

    // Utility methods
    void printError(const MyString& message);
    void printSuccess(const MyString& message);
    bool isValidCommand(const MyVector<MyString>& tokens);

public:
    ConsoleUI();
    ConsoleUI(Table* table);
    ~ConsoleUI();

    void setTable(Table* table);
    void run();
    void processCommand(const MyString& command);
    void showCommands();
};
