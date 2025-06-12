#include<stdexcept>


MyString::MyString()
{
	string = NULL;
}



MyString::MyString(const char* string)
{
	this->string = new char[strlen(string) + 1];
	strcpy_s(this->string, strlen(string) + 1, string);
}


MyString::MyString(const MyString& string) {
	if (string.string) {
		this->string = new char[string.length() + 1];
		strcpy_s(this->string, string.length() + 1, string.string);
	}
	else {
		this->string = NULL;
	}
}

size_t MyString::length() const {
	if (string)
		return strlen(string);
	else
		return 0;
}

MyString& MyString::operator=(const char* str)
{
	if (string) {
		delete string;
		string = NULL;
	}
	this->string = new char[strlen(str) + 1];
	strcpy_s(this->string, strlen(str) + 1, str);
	return *this;
}

MyString& MyString::operator=(const MyString& string)
{
	if (this->string) {
		delete this->string;
		this->string = NULL;
	}
	if (string.string) {
		this->string = new char[strlen(string.string) + 1];
		strcpy_s(this->string, strlen(string.string) + 1, string.string);
	}
	return *this;
}

bool MyString::operator<(const MyString& string) const
{
	if (strcmp(this->string, string.string) < 0) {
		return true;
	}
	return false;
}

bool MyString::operator==(const MyString& string) const
{
	if (strcmp(this->string, string.string) == 0) {
		return true;
	}
	return false;
}

bool MyString::operator!=(const MyString& string) const
{
	return !(*this == string);
}

MyString MyString::operator+(const MyString& string)
{
	if (this->string == NULL) {
		*this = string;
		return *this;
	}
	else {
		char* temp;
		temp = new char[strlen(this->string) + strlen(string.string) + 1];
		strcpy_s(temp, strlen(this->string) + 1, this->string);
		strcat(temp, string.string);
		MyString myString(temp);
		delete temp;
		return myString;
	}

}

ostream& operator<<(ostream& os, const MyString& string) {
	if (string.string) {
		os << string.string;
	}
	return os;
}

istream& operator >> (istream& is, MyString& string)
{
	char buffer[1024];
	is >> buffer;


	if (string.string) {
		delete string.string;
		string.string = NULL;
	}

	string.string = new char[strlen(buffer) + 1];
	strcpy_s(string.string, strlen(buffer) + 1, buffer);

	return is;
}


const char* MyString::data() const {
	return string;
}

char MyString::charAt(int index) const
{
	if (index > length() - 1 || index < 0) {
		throw std::out_of_range("out of range");
	}

	return string[index];
}

MyString::~MyString()
{
	if (string) {
		delete string;
		string = NULL;
	}
}