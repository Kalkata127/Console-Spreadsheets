#pragma once

#include "BaseCell.h"
#include "MyString.h"

class Table;

class ReferenceCell : public BaseCell {
private:
    size_t targetRow;
    size_t targetCol;
    Table* tablePtr; 

public:
    ReferenceCell(size_t row, size_t col);
    ReferenceCell(const ReferenceCell& other);
    ReferenceCell& operator=(const ReferenceCell& other);
    virtual ~ReferenceCell() = default;

    size_t getTargetRow() const;
    size_t getTargetCol() const;

    void setTarget(size_t row, size_t col);

    void setTablePtr(Table* table);

    MyString toString() const override;
    double evaluate() const override;
    MyString getType() const override;
    BaseCell* clone() const override;

private:
    BaseCell* getReferencedCell() const;
};
