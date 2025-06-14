#pragma once

#include <memory>
#include "MyString.h"
#include "BaseCell.h"
#include "ValueCell.hpp"
#include "ReferenceCell.h"

class Table;

static class CellFactory {
public:
    static unique_ptr<BaseCell> createCell(const MyString& input);

    static unique_ptr<BaseCell> createCell(const MyString& input, Table* table);

    static bool parseCellReference(const MyString& reference, size_t& row, size_t& col);
};

