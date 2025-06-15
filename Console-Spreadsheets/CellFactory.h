#pragma once

#include <memory>
#include "MyString.h"
#include "BaseCell.h"
#include "ValueCell.hpp"
#include "FormulaCell.h"
#include "ReferenceCell.h"

class Table;

static class CellFactory {
private:
    static bool parseFormula(const MyString& input, MyString& formulaName, MyString& parametersString);
    static FormulaType getFormulaType(const MyString& formulaName);
    static MyVector<FormulaParameter> parseFormulaParameters(const MyString& parametersString, Table* table);
    static FormulaParameter parseParameter(const MyString& param, Table* table);
    static bool parseRange(const MyString& range, size_t& startRow, size_t& startCol, size_t& endRow, size_t& endCol);

public:
    static unique_ptr<BaseCell> createCell(const MyString& input);

    static unique_ptr<BaseCell> createCell(const MyString& input, Table* table);

    static bool parseCellReference(const MyString& reference, size_t& row, size_t& col);
};

