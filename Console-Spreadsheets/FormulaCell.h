#pragma once

#include "BaseCell.h"
#include "MyString.h"
#include "MyVector.hpp"

class Table; // Forward declaration

enum class FormulaType {
    SUM,
    AVERAGE,
    MAX,
    LEN,
    CONCAT,
    SUBSTR,
    COUNT
};

struct FormulaParameter {
    enum Type {
        SINGLE_CELL,    
        CELL_RANGE,     
        INTEGER_VALUE,  
        BOOLEAN_VALUE, 
        STRING_VALUE    
    };

    Type type;

    size_t row, col;

    size_t startRow, startCol, endRow, endCol;

    int intValue;
    bool boolValue;
    MyString stringValue;

    FormulaParameter() : type(INTEGER_VALUE), row(0), col(0),
        startRow(0), startCol(0), endRow(0), endCol(0),
        intValue(0), boolValue(false) {}
};

class FormulaCell : public BaseCell {
private:
    FormulaType formulaType;
    MyVector<FormulaParameter> parameters;
    Table* tablePtr;
    mutable bool hasError;
    mutable MyString errorMessage;

    // Helper methods
    double evaluateParameter(const FormulaParameter& param) const;
    MyVector<double> getParameterValues(const FormulaParameter& param) const;
    MyVector<MyString> getParameterStringValues(const FormulaParameter& param) const;
    bool hasErrorInParameters() const;
    double calculateSum() const;
    double calculateAverage() const;
    double calculateMax() const;
    int calculateLen() const;
    MyString calculateConcat() const;
    MyString calculateSubstr() const;
    int calculateCount() const;

public:
    FormulaCell(FormulaType type, const MyVector<FormulaParameter>& params);
    FormulaCell(const FormulaCell& other);
    FormulaCell& operator=(const FormulaCell& other);
    ~FormulaCell() = default;

    void setTablePtr(Table* table);
    Table* getTablePtr() const;

    MyString toString() const override;
    double evaluate() const override;
    MyString getType() const override;
    BaseCell* clone() const override;

    // Formula-specific 
    FormulaType getFormulaType() const;
    const MyVector<FormulaParameter>& getParameters() const;
};