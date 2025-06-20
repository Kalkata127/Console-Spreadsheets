#pragma once
#include <stdexcept>
#include <iostream>
#include <utility>

using namespace std;

template <typename T>
class MyVector {
public:
	MyVector();
	MyVector(const MyVector& other);
	MyVector(MyVector&& other) noexcept;
	MyVector& operator=(const MyVector<T>& other);
	MyVector& operator=(MyVector<T>&& other) noexcept;
	~MyVector();

	void push_back(const T& element);
	void push_back(T&& element);
	T pop_back();
	void insert(const T& element, size_t position);
	void insert(T&& element, size_t position);
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	size_t getSize() const;
	size_t getCapacity() const;
	void print() const;
	void clear();
private:
	T* data;
	size_t size;
	size_t capacity;

	void copyFrom(const MyVector<T>& other);
	void free();
	void resize();
};

template<typename T>
void MyVector<T>::copyFrom(const MyVector<T>& other)
{
	size = other.size;
	capacity = other.capacity;

	data = new T[capacity];
	for (size_t i = 0; i < size; i++)
	{
		data[i] = other.data[i];
	}
}

template<typename T>
void MyVector<T>::clear() {
	// Properly destroy all elements first
	for (size_t i = 0; i < size; i++) {
		data[i].~T(); // Explicitly call destructor
	}
	size = 0;
}

template<typename T>
void MyVector<T>::free()
{
	if (data != nullptr) {
		// Clear all elements first
		for (size_t i = 0; i < size; i++) {
			data[i].~T(); // Explicitly destroy each element
		}
		delete[] data; 
		data = nullptr;
	}
	size = 0;
	capacity = 0;
}


template<typename T>
void MyVector<T>::resize()
{
	capacity *= 2;
	T* temp = new T[capacity];

	for (size_t i = 0; i < size; i++)
	{
		temp[i] = move(data[i]);
	}

	delete[] data; 
	data = temp;
}

template<typename T>
MyVector<T>::MyVector() : size(0), capacity(4) {
	data = new T[capacity];
}

template<typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
{
	copyFrom(other);
}

// Move constructor
template<typename T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept
	: data(other.data), size(other.size), capacity(other.capacity)
{
	other.data = nullptr;
	other.size = 0;
	other.capacity = 0;
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

// Move assignment operator
template<typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) noexcept
{
	if (this != &other)
	{
		free();
		data = other.data;
		size = other.size;
		capacity = other.capacity;
		other.data = nullptr;
		other.size = 0;
		other.capacity = 0;
	}
	return *this;
}

template<typename T>
MyVector<T>::~MyVector()
{
	free();
}

template<typename T>
void MyVector<T>::push_back(const T& element)
{
	if (size >= capacity)
	{
		resize();
	}

	data[size++] = element;
}

// Move version of push_back
template<typename T>
void MyVector<T>::push_back(T&& element)
{
	if (size >= capacity)
	{
		resize();
	}

	data[size++] = move(element);
}

template<typename T>
T MyVector<T>::pop_back()
{
	if (size <= 0)
	{
		throw out_of_range("Size is 0, can't pop_back");
	}
	size--;
	T result = move(data[size]);
	data[size].~T(); // Explicitly destroy the moved-from object
	return result;
}

template<typename T>
void MyVector<T>::insert(const T& element, size_t index)
{
	if (index >= size)
	{
		throw std::out_of_range("Position must be less than size");
	}

	if (size + 1 >= capacity)
	{
		resize();
	}

	// Shift elements to the right, starting from the end
	for (size_t i = size; i > index; i--)
	{
		data[i] = move(data[i - 1]);
	}

	// Insert the new element
	data[index] = element;
	size++;
}

// Move version of insert
template<typename T>
void MyVector<T>::insert(T&& element, size_t index)
{
	if (index >= size)
	{
		throw out_of_range("Position must be less than size");
	}

	if (size + 1 >= capacity)
	{
		resize();
	}

	// Shift elements to the right, starting from the end
	for (size_t i = size; i > index; i--)
	{
		data[i] = move(data[i - 1]);
	}

	// Insert the new element using move
	data[index] = move(element);
	size++;
}

template<typename T>
T& MyVector<T>::operator[](size_t index)
{
	if (index >= size)
	{
		throw out_of_range("Index must be positive and less than size");
	}

	return data[index];
}

template<typename T>
const T& MyVector<T>::operator[](size_t index) const
{
	if (index >= size)
	{
		throw out_of_range("Index must be positive and less than size");
	}

	return data[index];
}

template<typename T>
size_t MyVector<T>::getSize() const
{
	return size;
}

template<typename T>
size_t MyVector<T>::getCapacity() const
{
	return capacity;
}

template<typename T>
void MyVector<T>::print() const
{
	if (size <= 0)
	{
		cout << "Vector is empty\n";
	}
	for (size_t i = 0; i < size; i++)
	{
		cout << data[i] << " ";
	}
	cout << endl;
}