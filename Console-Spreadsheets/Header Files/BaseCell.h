#pragma once

class MyString;

class BaseCell {
public:
	virtual ~BaseCell();

	virtual MyString toString() const = 0;
	virtual double evaluate() const = 0;
	virtual MyString getType() const = 0;
};