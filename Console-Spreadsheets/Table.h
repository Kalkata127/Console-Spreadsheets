#pragma once

#include <memory>
#include "MyVector.hpp"
#include "BaseCell.h"
#include "CellFactory.h"
#include "MyString.h"

class Table {
private:
    MyVector<MyVector<std::unique_ptr<BaseCell>>> cells;
    size_t numRows;
    size_t numCols;

    // Helper methods
    void initializeCell(size_t row, size_t col);
    bool isValidPosition(size_t row, size_t col) const;

public:
    // Constructors
    Table();
    Table(size_t rows, size_t cols);

    // Destructor
    ~Table() = default;

    // Cell operations
    void setCell(size_t row, size_t col, const MyString& input);
    BaseCell* getCell(size_t row, size_t col) const;

    // Table operations
    void addRow();
    void addColumn();
    void insertRow(size_t index);
    void insertColumn(size_t index);
    void removeRow(size_t index);
    void removeColumn(size_t index);

    // Size operations
    void resize(size_t newRows, size_t newCols);
    size_t getRowCount() const;
    size_t getColumnCount() const;

    // Display operations
    void display() const;
    void printCell(size_t row, size_t col) const;

    // Utility
    void clear();
    bool isEmpty(size_t row, size_t col) const;
};
