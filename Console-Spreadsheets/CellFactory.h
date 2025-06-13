#pragma once

#include <memory>
#include "MyString.h"
#include "BaseCell.h"
#include "ValueCell.hpp"

static class CellFactory {
public:
    static std::unique_ptr<BaseCell> createCell(const MyString& input);
};
