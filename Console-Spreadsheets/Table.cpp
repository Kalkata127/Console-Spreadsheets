#include "Table.h"
#include <iostream>

Table::Table() : numRows(3), numCols(3) {
    for (size_t i = 0; i < numRows; i++) {
        MyVector<std::unique_ptr<BaseCell>> row;
        for (size_t j = 0; j < numCols; j++) {
            std::unique_ptr<BaseCell> emptyCell = nullptr;
            row.push_back(std::move(emptyCell)); 
        }
        cells.push_back(std::move(row));
    }
}

Table::Table(size_t rows, size_t cols) : numRows(rows), numCols(cols) {
    for (size_t i = 0; i < numRows; i++) {
        MyVector<std::unique_ptr<BaseCell>> row;
        for (size_t j = 0; j < numCols; j++) {
            std::unique_ptr<BaseCell> emptyCell = nullptr;
            row.push_back(std::move(emptyCell)); 
        }
        cells.push_back(std::move(row));
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
        cout << "Error: Invalid position (" << row << ", " << col << ")" << std::endl;
        return;
    }

    cells[row][col] = CellFactory::createCell(input);
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

void Table::display() const {
    cout << "    |";
    for (size_t col = 0; col < numCols; col++) {
        cout << "   " << (col + 1) << "   |";
    }
    cout << std::endl;

    cout << "----|";
    for (size_t col = 0; col < numCols; col++) {
        cout << "-------|";
    }
    cout << std::endl;

    for (size_t row = 0; row < numRows; row++) {
        char rowLetter = 'A' + static_cast<char>(row);
        cout << " " << rowLetter << "  |";

        for (size_t col = 0; col < numCols; col++) {
            cout << " ";

            if (cells[row][col] != nullptr) {
                MyString cellContent = cells[row][col]->toString();
                cout << cellContent;

                size_t contentLength = cellContent.length();
                for (size_t i = contentLength; i < 5; i++) {
                    cout << " ";
                }
            }
            else {
                cout << "     "; 
            }

            cout << " |";
        }
        cout << std::endl;

        cout << "----|";
        for (size_t col = 0; col < numCols; col++) {
            cout << "-------|";
        }
        cout << std::endl;
    }
}