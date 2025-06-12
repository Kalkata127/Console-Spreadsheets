#pragma once

class BaseCell;

template<typename T>
class ValueCell : public BaseCell {
private:
	T value;
public:
ValueCell(const T& val) : value(val){}

};