#include "Table.h"
#include <iostream>
#include <string>

const int defRows = 3;
const int defCols = 3;

Table::Table() : numRows(defRows), numCols(defCols), autoFit(true), visibleCellSymbols(7) {
    for (size_t i = 0; i < numRows; i++) {
        MyVector<unique_ptr<BaseCell>> row;
        for (size_t j = 0; j < numCols; j++) {
            unique_ptr<BaseCell> emptyCell = nullptr;
            row.push_back(move(emptyCell));
        }
        cells.push_back(move(row));
    }
}

Table::Table(size_t rows, size_t cols) : numRows(rows), numCols(cols), autoFit(true), visibleCellSymbols(7) {
    for (size_t i = 0; i < numRows; i++) {
        MyVector<unique_ptr<BaseCell>> row;
        for (size_t j = 0; j < numCols; j++) {
            unique_ptr<BaseCell> emptyCell = nullptr;
            row.push_back(move(emptyCell));
        }
        cells.push_back(move(row));
    }
}

Table::Table(size_t rows, size_t cols, bool autoFit, int visibleCellSymbols)
    : numRows(rows), numCols(cols), autoFit(autoFit), visibleCellSymbols(visibleCellSymbols) {
    for (size_t i = 0; i < numRows; i++) {
        MyVector<unique_ptr<BaseCell>> row;
        for (size_t j = 0; j < numCols; j++) {
            unique_ptr<BaseCell> emptyCell = nullptr;
            row.push_back(move(emptyCell));
        }
        cells.push_back(move(row));
    }
}

void Table::initializeCell(size_t row, size_t col) {
    if (isValidPosition(row, col) && cells[row][col] == nullptr) {
        cells[row][col] = CellFactory::createCell(MyString(""));
    }
}

bool Table::isValidPosition(size_t row, size_t col) const {
    return row < numRows && col < numCols;
}

void Table::setCell(size_t row, size_t col, const MyString& input) {
    if (!isValidPosition(row, col)) {
        cout << "Error: Invalid position (" << row << ", " << col << ")" << endl;
        return;
    }

    // Check for circular reference BEFORE creating the cell
    if (input.length() > 1 && input.data()[0] == '=') {
        size_t refLength = input.length() - 1;
        char* refBuffer = new char[refLength + 1];
        for (size_t i = 0; i < refLength; i++) {
            refBuffer[i] = input.data()[i + 1];
        }
        refBuffer[refLength] = '\0';
        MyString reference(refBuffer);
        delete[] refBuffer;

        size_t targetRow, targetCol;
        if (CellFactory::parseCellReference(reference, targetRow, targetCol)) {
            if (targetRow == row && targetCol == col) {
                cells[row][col] = make_unique<ValueCell<MyString>>(MyString("#CIRCULAR!"));
                return;
            }

            BaseCell* targetCell = getCell(targetRow, targetCol);
            if (targetCell && targetCell->getType() == MyString("ReferenceCell")) {
                ReferenceCell* refCell = static_cast<ReferenceCell*>(targetCell);
                if (refCell->getTargetRow() == row && refCell->getTargetCol() == col) {
                    cells[row][col] = make_unique<ValueCell<MyString>>(MyString("#CIRCULAR!"));
                    return;
                }
            }
        }
    }
    cells[row][col] = CellFactory::createCell(input, this);
}

BaseCell* Table::getCell(size_t row, size_t col) const {
    if (!isValidPosition(row, col)) {
        return nullptr;
    }

    return cells[row][col].get();
}

size_t Table::getRowCount() const {
    return numRows;
}

size_t Table::getColumnCount() const {
    return numCols;
}

void Table::setAutoFit(bool autoFit) {
    this->autoFit = autoFit;
}

void Table::setVisibleCellSymbols(int symbols) {
    this->visibleCellSymbols = symbols;
}

bool Table::getAutoFit() const {
    return autoFit;
}

int Table::getVisibleCellSymbols() const {
    return visibleCellSymbols;
}


void Table::addRow() {
    MyVector<unique_ptr<BaseCell>> newRow;
    for (size_t col = 0; col < numCols; col++) {
        unique_ptr<BaseCell> emptyCell = nullptr;
        newRow.push_back(move(emptyCell));
    }
    cells.push_back(move(newRow));
    numRows++;
}

void Table::addColumn() {
    for (size_t row = 0; row < numRows; row++) {
        unique_ptr<BaseCell> emptyCell = nullptr;
        cells[row].push_back(move(emptyCell));
    }
    numCols++;
}

void Table::insertRow(size_t index) {
    if (index > numRows) {
        cout << "Error: Invalid row index " << index << endl;
        return;
    }

    MyVector<unique_ptr<BaseCell>> newRow;
    for (size_t col = 0; col < numCols; col++) {
        unique_ptr<BaseCell> emptyCell = nullptr;
        newRow.push_back(move(emptyCell));
    }

    cells.insert(move(newRow), index);
    numRows++;
}

void Table::insertColumn(size_t index) {
    if (index > numCols) {
        cout << "Error: Invalid column index " << index << endl;
        return;
    }

    for (size_t row = 0; row < numRows; row++) {
        unique_ptr<BaseCell> emptyCell = nullptr;
        cells[row].insert(move(emptyCell), index);
    }
    numCols++;
}

void Table::removeRow(size_t index) {
    if (index >= numRows) {
        cout << "Error: Invalid row index " << index << endl;
        return;
    }

    if (numRows <= 1) {
        cout << "Error: Cannot remove last row" << endl;
        return;
    }

    for (size_t i = index; i < numRows - 1; i++) {
        cells[i] = move(cells[i + 1]);
    }

    cells.pop_back();
    numRows--;
}

void Table::removeColumn(size_t index) {
    if (index >= numCols) {
        cout << "Error: Invalid column index " << index << endl;
        return;
    }

    if (numCols <= 1) {
        cout << "Error: Cannot remove last column" << endl;
        return;
    }

    for (size_t row = 0; row < numRows; row++) {
        for (size_t col = index; col < numCols - 1; col++) {
            cells[row][col] = move(cells[row][col + 1]);
        }
        cells[row].pop_back();
    }
    numCols--;
}

void Table::resize(size_t newRows, size_t newCols) {
    if (newRows > numRows) {
        while (numRows < newRows) {
            addRow();
        }
    }
    else if (newRows < numRows) {
        while (numRows > newRows && numRows > 1) {
            removeRow(numRows - 1);
        }
    }

    if (newCols > numCols) {
        while (numCols < newCols) {
            addColumn();
        }
    }
    else if (newCols < numCols) {
        while (numCols > newCols && numCols > 1) {
            removeColumn(numCols - 1);
        }
    }
}

MyVector<size_t> Table::calculateColumnWidths() const {
    MyVector<size_t> widths;
    size_t globalMaxWidth = 1;

    for (size_t col = 0; col < numCols; col++) {
        MyString colHeader;
        if (col + 1 < 10) {
            char buffer[2];
            buffer[0] = '0' + static_cast<char>(col + 1);
            buffer[1] = '\0';
            colHeader = MyString(buffer);
        }
        else {
            char buffer[10];
            size_t tempCol = col + 1;
            int digits = 0;
            while (tempCol > 0) {
                buffer[digits++] = '0' + (tempCol % 10);
                tempCol /= 10;
            }

            for (int i = 0; i < digits / 2; i++) {
                char temp = buffer[i];
                buffer[i] = buffer[digits - 1 - i];
                buffer[digits - 1 - i] = temp;
            }
            buffer[digits] = '\0';
            colHeader = MyString(buffer);
        }

        size_t headerWidth = colHeader.length();
        if (headerWidth > globalMaxWidth) {
            globalMaxWidth = headerWidth;
        }

        for (size_t row = 0; row < numRows; row++) {
            MyString cellContent;
            if (cells[row][col] != nullptr) {
                cellContent = cells[row][col]->toString();
            }
            else {
                cellContent = MyString(""); 
            }

            size_t contentWidth = cellContent.length();
            if (contentWidth > globalMaxWidth) {
                globalMaxWidth = contentWidth;
            }
        }
    }

    //Padding
    globalMaxWidth += 2;

    for (size_t col = 0; col < numCols; col++) {
        widths.push_back(globalMaxWidth);
    }

    return widths;
}

MyString Table::formatCellContent(const MyString& content, size_t width) const {
    size_t contentLength = content.length();

    if (contentLength >= width) {
        if (width <= 3) {
            MyString result;
            for (size_t i = 0; i < width; i++) {
                result = result + MyString(".");
            }
            return result;
        }
        else {
            MyString result;
            for (size_t i = 0; i < width - 3; i++) {
                char buffer[2];
                buffer[0] = content.data()[i];
                buffer[1] = '\0';
                result = result + MyString(buffer);
            }
            result = result + MyString("...");
            return result;
        }
    }

    MyString result = content;
    size_t spacesToAdd = width - contentLength;

    // Add spaces to center the content 
    size_t leftSpaces = spacesToAdd / 2;
    size_t rightSpaces = spacesToAdd - leftSpaces;

    // left padding
    for (size_t i = 0; i < leftSpaces; i++) {
        result = MyString(" ") + result;
    }

    // right padding
    for (size_t i = 0; i < rightSpaces; i++) {
        result = result + MyString(" ");
    }

    return result;
}
void Table::display() const {
    MyVector<size_t> columnWidths;

    if (autoFit) {
        columnWidths = calculateColumnWidths();
    }
    else {
        for (size_t col = 0; col < numCols; col++) {
            columnWidths.push_back(static_cast<size_t>(visibleCellSymbols));
        }
    }

    const size_t rowHeaderWidth = 4;

    MyString rowHeaderSpaces;
    MyString rowHeaderDashes;
    for (size_t i = 0; i < rowHeaderWidth; i++) {
        rowHeaderSpaces = rowHeaderSpaces + MyString(" ");
        rowHeaderDashes = rowHeaderDashes + MyString("-");
    }

    cout << rowHeaderSpaces << "|";
    for (size_t col = 0; col < numCols; col++) {
        MyString colHeader;
        if (col + 1 < 10) {
            char buffer[2];
            buffer[0] = '0' + static_cast<char>(col + 1);
            buffer[1] = '\0';
            colHeader = MyString(buffer);
        }
        else {
            char buffer[10];
            size_t tempCol = col + 1;
            int digits = 0;
            while (tempCol > 0) {
                buffer[digits++] = '0' + (tempCol % 10);
                tempCol /= 10;
            }
            for (int i = 0; i < digits / 2; i++) {
                char temp = buffer[i];
                buffer[i] = buffer[digits - 1 - i];
                buffer[digits - 1 - i] = temp;
            }
            buffer[digits] = '\0';
            colHeader = MyString(buffer);
        }

        MyString formattedHeader = formatCellContent(colHeader, columnWidths[col]);
        cout << formattedHeader << "|";
    }
    cout << endl;

    cout << rowHeaderDashes << "|";
    for (size_t col = 0; col < numCols; col++) {
        MyString colSeparator;
        for (size_t i = 0; i < columnWidths[col]; i++) {
            colSeparator = colSeparator + MyString("-");
        }
        cout << colSeparator << "|";
    }
    cout << endl;

    for (size_t row = 0; row < numRows; row++) {
        char rowLetter = 'A' + static_cast<char>(row);
        cout << " " << rowLetter << "  |";

        for (size_t col = 0; col < numCols; col++) {
            MyString cellContent;
            if (cells[row][col] != nullptr) {
                cellContent = cells[row][col]->toString();
            }
            else {
                cellContent = MyString(" ");
            }

            MyString formattedCell = formatCellContent(cellContent, columnWidths[col]);
            cout << formattedCell << "|";
        }
        cout << endl;

        cout << rowHeaderDashes << "|";
        for (size_t col = 0; col < numCols; col++) {
            MyString colSeparator;
            for (size_t i = 0; i < columnWidths[col]; i++) {
                colSeparator = colSeparator + MyString("-");
            }
            cout << colSeparator << "|";
        }
        cout << endl;
    }
}
Table::~Table() {
    cout << "Table destructor starting..." << endl;

    // Properly clear each row
    for (size_t i = 0; i < cells.getSize(); i++) {
        // Reset all unique_ptrs first
        for (size_t j = 0; j < cells[i].getSize(); j++) {
            cells[i][j].reset(); 
        }
    }

    cout << "Table destructor ending..." << endl;
}