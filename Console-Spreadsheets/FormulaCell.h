#ifndef FORMULACELL_H
#define FORMULACELL_H

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

// Represents a single parameter in a formula
struct FormulaParameter {
    enum Type {
        SINGLE_CELL,    // A1
        CELL_RANGE,     // A1:B5
        INTEGER_VALUE,  // 42
        BOOLEAN_VALUE,  // true/false
        STRING_VALUE    // "text"
    };

    Type type;

    // For single cell
    size_t row, col;

    // For cell range
    size_t startRow, startCol, endRow, endCol;

    // For values
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

#endif