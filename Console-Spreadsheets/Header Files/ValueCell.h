#pragma once
#include "BaseCell.h"

template<typename T>
class ValueCell : public BaseCell {
private:
	T value;
public:
	ValueCell(const T& val);

	const T& getValue() const;

	MyString toString() const override;
	MyString getType() const override;
	BaseCell* clone() const override;
};