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
    bool autoFit;
    int visibleCellSymbols;

    void initializeCell(size_t row, size_t col);
    bool isValidPosition(size_t row, size_t col) const;
    MyVector<size_t> calculateColumnWidths() const;
    MyString formatCellContent(const MyString& content, size_t width) const;

public:
    Table();
    Table(size_t rows, size_t cols);
    Table(size_t rows, size_t cols, bool autoFit, int visibleCellSymbols);
    ~Table() = default;

    void setCell(size_t row, size_t col, const MyString& input);
    BaseCell* getCell(size_t row, size_t col) const;

    size_t getRowCount() const;
    size_t getColumnCount() const;

    void addRow();
    void addColumn();
    void insertRow(size_t index);
    void insertColumn(size_t index);
    void removeRow(size_t index);
    void removeColumn(size_t index);
    void resize(size_t newRows, size_t newCols);

    void setAutoFit(bool autoFit);
    void setVisibleCellSymbols(int symbols);
    bool getAutoFit() const;
    int getVisibleCellSymbols() const;

    void display() const;
    void printCell(size_t row, size_t col) const;

    void clear();
    bool isEmpty(size_t row, size_t col) const;
};
