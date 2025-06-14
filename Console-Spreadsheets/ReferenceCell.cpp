#include "ReferenceCell.h"
#include "Table.h"

ReferenceCell::ReferenceCell(size_t row, size_t col)
    : targetRow(row), targetCol(col), tablePtr(nullptr) {
}

ReferenceCell::ReferenceCell(const ReferenceCell& other)
    : targetRow(other.targetRow), targetCol(other.targetCol), tablePtr(other.tablePtr) {
}

ReferenceCell& ReferenceCell::operator=(const ReferenceCell& other) {
    if (this != &other) {
        targetRow = other.targetRow;
        targetCol = other.targetCol;
        tablePtr = other.tablePtr;
    }
    return *this;
}

size_t ReferenceCell::getTargetRow() const {
    return targetRow;
}

size_t ReferenceCell::getTargetCol() const {
    return targetCol;
}

void ReferenceCell::setTarget(size_t row, size_t col) {
    targetRow = row;
    targetCol = col;
}

void ReferenceCell::setTablePtr(Table* table) {
    tablePtr = table;
}

BaseCell* ReferenceCell::getReferencedCell() const {
    if (tablePtr == nullptr) {
        return nullptr;
    }

    return tablePtr->getCell(targetRow, targetCol);
}

MyString ReferenceCell::toString() const {
    BaseCell* referencedCell = getReferencedCell();

    if (referencedCell == nullptr) {
        return MyString("#REF!"); 
    }

    return referencedCell->toString();
}

double ReferenceCell::evaluate() const {
    BaseCell* referencedCell = getReferencedCell();

    if (referencedCell == nullptr) {
        return 0.0; 
    }

    return referencedCell->evaluate();
}

MyString ReferenceCell::getType() const {
    return MyString("ReferenceCell");
}

BaseCell* ReferenceCell::clone() const {
    return new ReferenceCell(*this);
}