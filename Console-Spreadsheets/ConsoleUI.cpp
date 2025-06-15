#include "ConsoleUI.h"
#include <iostream>

ConsoleUI::ConsoleUI() : currentTable(nullptr), running(false) {}

ConsoleUI::ConsoleUI(Table* table) : currentTable(table), running(false) {}

void ConsoleUI::setTable(Table* table) {
    currentTable = table;
}

void ConsoleUI::run() {
    if (!currentTable) {
        cout << "No table loaded. Please load a table first.\n";
        return;
    }

    running = true;
    cout << "=== Spreadsheet Console Interface ===\n";
    cout << "Type 'show' to display table, 'exit' to quit\n\n";

    currentTable->display();

    char inputBuffer[1024]; // Buffer 
    while (running) {
        cout << "\n> ";
        cin.getline(inputBuffer, 1000);

        MyString inputLine(inputBuffer);
        if (inputLine.length() > 0) {
            processCommand(inputLine);
        }
    }
}

void ConsoleUI::processCommand(const MyString& command) {
    MyVector<MyString> tokens = parseCommand(command);

    if (tokens.getSize() == 0) {
        return;
    }

    MyString firstToken = tokens[0];

    // Handle different command patterns
    if (firstToken == MyString("exit")) {
        handleExit();
    }
    else if (firstToken == MyString("show")) {
        handleDisplay();
    }
    else if (firstToken == MyString("add_row")) {
        handleAddRow();
    }
    else if (firstToken == MyString("add_col")) {
        handleAddColumn();
    }
    else if (tokens.getSize() >= 2 && tokens[1] == MyString("insert")) {
        handleCellInsert(tokens);
    }
    else if (tokens.getSize() >= 2 && tokens[1] == MyString("delete")) {
        handleCellDelete(tokens);
    }
    else if (tokens.getSize() >= 2 && tokens[1].data()[0] == '=') {
        // Check if it's a formula or simple reference
        MyString formula = tokens[1];
        bool hasParentheses = false;
        for (size_t i = 0; i < formula.length(); i++) {
            if (formula.data()[i] == '(' || formula.data()[i] == ')') {
                hasParentheses = true;
                break;
            }
        }

        if (hasParentheses) {
            handleCellFormula(tokens);
        }
        else {
            handleCellReference(tokens);
        }
    }
    else if (firstToken == MyString("insert_row") && tokens.getSize() >= 2) {
        handleInsertRow(tokens);
    }
    else if (firstToken == MyString("insert_col") && tokens.getSize() >= 2) {
        handleInsertColumn(tokens);
    }
    else if (firstToken == MyString("remove_row") && tokens.getSize() >= 2) {
        handleRemoveRow(tokens);
    }
    else if (firstToken == MyString("remove_col") && tokens.getSize() >= 2) {
        handleRemoveColumn(tokens);
    }
    else if (firstToken == MyString("resize") && tokens.getSize() >= 3) {
        handleResize(tokens);
    }
    else {
        printError(MyString("Unknown command. Valid commands:"));
        showCommands();
    }
}

MyVector<MyString> ConsoleUI::parseCommand(const MyString& input) {
    MyVector<MyString> tokens;
    const char* str = input.data();
    size_t len = input.length();
    size_t start = 0;

    for (size_t i = 0; i <= len; i++) {
        if (i == len || str[i] == ' ') {
            if (i > start) {
                // Create substring from start to i
                size_t tokenLen = i - start;
                char* buffer = new char[tokenLen + 1];
                for (size_t j = 0; j < tokenLen; j++) {
                    buffer[j] = str[start + j];
                }
                buffer[tokenLen] = '\0';
                tokens.push_back(MyString(buffer));
                delete[] buffer;
            }
            start = i + 1;
        }
    }

    return tokens;
}

bool ConsoleUI::parseCellReference(const MyString& cellRef, size_t& row, size_t& col) {
    if (cellRef.length() < 2) {
        return false;
    }

    const char* str = cellRef.data();

    // Parse column (A, B, C..)
    if (str[0] < 'A' || str[0] > 'Z') {
        return false;
    }
    col = str[0] - 'A';

    // Parse row number
    size_t rowNum = 0;
    for (size_t i = 1; i < cellRef.length(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
        rowNum = rowNum * 10 + (str[i] - '0');
    }

    if (rowNum == 0) {
        return false;
    }
    row = rowNum - 1; 

    return true;
}

void ConsoleUI::handleCellInsert(const MyVector<MyString>& tokens) {
    if (tokens.getSize() < 3) {
        printError(MyString("Usage: {cell} insert {value}"));
        return;
    }

    size_t row, col;
    if (!parseCellReference(tokens[0], row, col)) {
        printError(MyString("Invalid cell reference"));
        return;
    }

    // Combine all tokens after "insert" as the value
    MyString value;
    for (size_t i = 2; i < tokens.getSize(); i++) {
        if (i > 2) {
            value = value + MyString(" ");
        }
        value = value + tokens[i];
    }

    // Ensure table is large enough
    if (row >= currentTable->getRowCount() || col >= currentTable->getColumnCount()) {
        size_t newRows = (row >= currentTable->getRowCount()) ? row + 1 : currentTable->getRowCount();
        size_t newCols = (col >= currentTable->getColumnCount()) ? col + 1 : currentTable->getColumnCount();
        currentTable->resize(newRows, newCols);
    }

    currentTable->setCell(row, col, value);
    printSuccess(MyString("Cell updated successfully"));
}

void ConsoleUI::handleCellDelete(const MyVector<MyString>& tokens) {
    if (tokens.getSize() < 2) {
        printError(MyString("Usage: {cell} delete"));
        return;
    }

    size_t row, col;
    if (!parseCellReference(tokens[0], row, col)) {
        printError(MyString("Invalid cell reference"));
        return;
    }

    if (row >= currentTable->getRowCount() || col >= currentTable->getColumnCount()) {
        printError(MyString("Cell position out of bounds"));
        return;
    }

    currentTable->setCell(row, col, MyString(""));
    printSuccess(MyString("Cell deleted successfully"));
}

void ConsoleUI::handleCellReference(const MyVector<MyString>& tokens) {
    if (tokens.getSize() < 2) {
        printError(MyString("Usage: {cell} ={referenceCell}"));
        return;
    }

    size_t row, col;
    if (!parseCellReference(tokens[0], row, col)) {
        printError(MyString("Invalid cell reference"));
        return;
    }

    // Check if target cell is within bounds
    if (row >= currentTable->getRowCount() || col >= currentTable->getColumnCount()) {
        printError(MyString("Target cell out of bounds"));
        return;
    }

    // Extract and validate the reference cell from the =CellRef format
    MyString refString = tokens[1];
    if (refString.length() < 2 || refString.data()[0] != '=') {
        printError(MyString("Invalid reference format. Use ={CellRef}"));
        return;
    }

    // Extract the cell reference after the '=' sign
    const char* refStr = refString.data();
    size_t refLen = refString.length() - 1;
    char* refBuffer = new char[refLen + 1];
    for (size_t i = 0; i < refLen; i++) {
        refBuffer[i] = refStr[i + 1];
    }
    refBuffer[refLen] = '\0';
    MyString referencedCell(refBuffer);
    delete[] refBuffer;

    // Validate that the referenced cell exists within table bounds
    size_t refRow, refCol;
    if (!parseCellReference(referencedCell, refRow, refCol)) {
        printError(MyString("Invalid referenced cell format"));
        return;
    }

    if (refRow >= currentTable->getRowCount() || refCol >= currentTable->getColumnCount()) {
        printError(MyString("Referenced cell out of bounds"));
        return;
    }

    currentTable->setCell(row, col, tokens[1]);
    printSuccess(MyString("Reference cell created successfully"));
}

void ConsoleUI::handleCellFormula(const MyVector<MyString>& tokens) {
    if (tokens.getSize() < 2) {
        printError(MyString("Usage: {cell} ={formulaName}({parameters})"));
        return;
    }

    size_t row, col;
    if (!parseCellReference(tokens[0], row, col)) {
        printError(MyString("Invalid cell reference"));
        return;
    }

    // Auto-resize if needed for formulas
    if (row >= currentTable->getRowCount() || col >= currentTable->getColumnCount()) {
        size_t newRows = (row >= currentTable->getRowCount()) ? row + 1 : currentTable->getRowCount();
        size_t newCols = (col >= currentTable->getColumnCount()) ? col + 1 : currentTable->getColumnCount();
        currentTable->resize(newRows, newCols);
    }

    currentTable->setCell(row, col, tokens[1]);
    printSuccess(MyString("Formula cell created successfully"));
}

void ConsoleUI::handleAddRow() {
    currentTable->addRow();
    printSuccess(MyString("Row added successfully"));
}

void ConsoleUI::handleAddColumn() {
    currentTable->addColumn();
    printSuccess(MyString("Column added successfully"));
}

void ConsoleUI::handleInsertRow(const MyVector<MyString>& tokens) {
    // Convert string to number
    const char* str = tokens[1].data();
    size_t index = 0;
    for (size_t i = 0; i < tokens[1].length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            index = index * 10 + (str[i] - '0');
        }
        else {
            printError(MyString("Invalid row index"));
            return;
        }
    }

    if (index > currentTable->getRowCount()) {
        printError(MyString("Row index out of bounds"));
        return;
    }

    currentTable->insertRow(index);
    printSuccess(MyString("Row inserted successfully"));
}

void ConsoleUI::handleInsertColumn(const MyVector<MyString>& tokens) {
    // Convert string to number
    const char* str = tokens[1].data();
    size_t index = 0;
    for (size_t i = 0; i < tokens[1].length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            index = index * 10 + (str[i] - '0');
        }
        else {
            printError(MyString("Invalid column index"));
            return;
        }
    }

    if (index > currentTable->getColumnCount()) {
        printError(MyString("Column index out of bounds"));
        return;
    }

    currentTable->insertColumn(index);
    printSuccess(MyString("Column inserted successfully"));
}

void ConsoleUI::handleRemoveRow(const MyVector<MyString>& tokens) {
    // Convert string to number
    const char* str = tokens[1].data();
    size_t index = 0;
    for (size_t i = 0; i < tokens[1].length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            index = index * 10 + (str[i] - '0');
        }
        else {
            printError(MyString("Invalid row index"));
            return;
        }
    }

    if (index >= currentTable->getRowCount()) {
        printError(MyString("Row index out of bounds"));
        return;
    }

    currentTable->removeRow(index);
    printSuccess(MyString("Row removed successfully"));
}

void ConsoleUI::handleRemoveColumn(const MyVector<MyString>& tokens) {
    // Convert string to number  
    const char* str = tokens[1].data();
    size_t index = 0;
    for (size_t i = 0; i < tokens[1].length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            index = index * 10 + (str[i] - '0');
        }
        else {
            printError(MyString("Invalid column index"));
            return;
        }
    }

    if (index >= currentTable->getColumnCount()) {
        printError(MyString("Column index out of bounds"));
        return;
    }

    currentTable->removeColumn(index);
    printSuccess(MyString("Column removed successfully"));
}

void ConsoleUI::handleResize(const MyVector<MyString>& tokens) {
    // Parse rows
    const char* rowStr = tokens[1].data();
    size_t newRows = 0;
    for (size_t i = 0; i < tokens[1].length(); i++) {
        if (rowStr[i] >= '0' && rowStr[i] <= '9') {
            newRows = newRows * 10 + (rowStr[i] - '0');
        }
        else {
            printError(MyString("Invalid row count"));
            return;
        }
    }

    // Parse columns
    const char* colStr = tokens[2].data();
    size_t newCols = 0;
    for (size_t i = 0; i < tokens[2].length(); i++) {
        if (colStr[i] >= '0' && colStr[i] <= '9') {
            newCols = newCols * 10 + (colStr[i] - '0');
        }
        else {
            printError(MyString("Invalid column count"));
            return;
        }
    }

    currentTable->resize(newRows, newCols);
    printSuccess(MyString("Table resized successfully"));
}

void ConsoleUI::handleDisplay() {
    currentTable->display();
}

void ConsoleUI::handleExit() {
    running = false;
    cout << "Goodbye!\n";
}

void ConsoleUI::printError(const MyString& message) {
    cout << "Error: " << message.data() << "\n";
}

void ConsoleUI::printSuccess(const MyString& message) {
    cout << "Success: " << message.data() << "\n";
}

void ConsoleUI::showCommands() {
    cout << "\nAvailable commands:\n";
    cout << "  {cell} insert {value}     - Insert value into cell (e.g., A1 insert 42)\n";
    cout << "  {cell} delete             - Delete cell content (e.g., B2 delete)\n";
    cout << "  {cell} ={referenceCell}   - Create cell reference (e.g., C3 =A1)\n";
    cout << "  {cell} ={formula}         - Create formula (e.g., A5 =SUM(A1:C3,6))\n";
    cout << "  add_row                   - Add row at the end\n";
    cout << "  add_col                   - Add column at the end\n";
    cout << "  insert_row {index}        - Insert row at index\n";
    cout << "  insert_col {index}        - Insert column at index\n";
    cout << "  remove_row {index}        - Remove row at index\n";
    cout << "  remove_col {index}        - Remove column at index\n";
    cout << "  resize {rows} {cols}      - Resize table\n";
    cout << "  show                      - Display current table\n";
    cout << "  exit                      - Exit program\n";
}