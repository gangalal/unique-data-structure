#pragma once
#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H
#include "Exceptions.h"

/**
 * stores Nvrarecord data and resizes array as required
 * @author gangalal
 * @version 10/29/2018
 */

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;		// capacity used in no arg constructor

template <typename T>
class TemplatedArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;	// maximum capacity, in items
	unsigned long size = 0;								// actual number of items currently in array
	T* data = NULL;										// pointer to array of any type
	void doubleCapacity();								// method to double array capacity
	void halveCapacity();								// method to halve array capacity 
public:
	TemplatedArray();								    // constructs array with default capacity
	TemplatedArray(unsigned long capacity);			    // constructs array with specified capacity
	virtual ~TemplatedArray();						    // frees array space as object is deleted
	void add(const T* item);						    // adds item, increments size, doubles capacity as necessary
	void addAt(const T* item, unsigned long index);	    // adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(const T* item, unsigned long index); // replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);				    // removes item at index, shifts following back
	T get(unsigned long index) const;				    // returns (copy of) item at index
	unsigned long getCapacity() const;				    // returns the current capacity of the array
	unsigned long getSize() const;					    // returns the current number of items in the array
	T operator[](unsigned long index) const;		    // returns (reference to) item at index
};

/**
* an empty constructor intialize array of size 10
*/
template<typename T>
TemplatedArray <T>::TemplatedArray()
{
	data = new T[capacity];
	if (data == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

/**
* constructor with user define capacity
* initialze the templated array of user define size
* @param capacity passes user define capacity
*/
template<typename T>
TemplatedArray <T>::TemplatedArray(unsigned long capacity)
{
	this->capacity = capacity;
	data = new T[this->capacity];
	if (data == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
}

/**
* doubles the size of array when full
*/
template <typename T>
void TemplatedArray <T>::doubleCapacity()
{
	capacity = capacity * 2;
	T* temp = new T[capacity];
	if (temp == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	for (unsigned long i = 0; i < size; i++)
	{
		temp[i] = this->data[i];
	}
	delete[] data;
	data = temp;
}

/**
*halves the capacity of an array during removing element if size is less than half of capacity after removing
*
*/
template<typename T>
void TemplatedArray <T>::halveCapacity()
{
	capacity = capacity / 2;
	T* temp = new T[capacity];
	if (temp == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	for (unsigned long i = 0; i < size; i++)
	{
		temp[i] = data[i];
	}
	delete[] data;
	data = temp;
}

/**
* free ups the memory after data has been used
*/
template<typename T>
TemplatedArray <T>::~TemplatedArray()
{
	//if (data != NULL)
	delete[] data;
}

/**
* adds item at the end of array, increments size, doubles capacity as necessary
* @param item passes templated item to add
*/
template<typename T>
void TemplatedArray<T>::add(const T* item)
{
	if (size >= capacity)
	{
		try
		{
			doubleCapacity();
		}
		catch (ExceptionMemoryNotAvailable* e) {
			throw e;
		}
	}
	data[size] = *item;
	size++;
}

/**
* adds item at user define index, increments size, doubles capacity as necessary
* @param item passes templated item to add
* @param index passes index to at item at
*/
template <typename T>
void TemplatedArray<T>::addAt(const T* item, unsigned long index)
{
	if (index > size) {
		throw new ExceptionIndexOutOfRange;
	}
	if (size == capacity) {
		try {
			doubleCapacity();
		}
		catch (ExceptionMemoryNotAvailable* e) {
			throw e;
		}
	}
	if (size > 0) {
		for (unsigned long i = size; i > index; --i) {
			data[i] = data[i - 1];
		}
	}
	data[index] = *item;
	++size;
}

/**
* replaces item at index, otherwise unchanged
* @param item passes templated item to replace
* @param index passes index to at replace at
*/
template<typename T>
void TemplatedArray <T>::replaceAt(const T* item, unsigned long index)
{
	if (index >= size)
	{
		throw new ExceptionIndexOutOfRange();
	}
	else
	{
		data[index] = *item;
	}
}

/**
* removes item at index, otherwise unchanged
* @param item passes templated item to remove
* @param index passes index to at remove at
*/
template<typename T>
void TemplatedArray <T>::removeAt(unsigned long index)
{
	if (index >= size)
	{
		throw new ExceptionIndexOutOfRange();
	}
	//TheType* temp = new TheType[capacity];
	for (unsigned long i = index; i < size - 1; i++)
	{
		data[i] = data[i + 1];
	}
	size--;
	if (size <= capacity / 2)
	{
		try
		{
			halveCapacity();
		}
		catch (ExceptionMemoryNotAvailable* e) {
			throw e;
		}
	}

}

/**
* returns (copy of) record at index
* @param index passes index to at return at
*/
template<typename T>
T TemplatedArray <T>::get(unsigned long index) const
{
	if (index >= size)
	{
		throw new ExceptionIndexOutOfRange();
	}
	else
	{
		//cout << "check1" << endl;
		return data[index];
	}
}

/**
* returns the capacity of current array
*/
template<typename T>
unsigned long TemplatedArray<T>::getCapacity() const
{
	return capacity;
}

/**
* returns the size of current array
*/
template<typename T>
unsigned long TemplatedArray<T>::getSize() const
{
	return size;
}

/**
* returns (reference to) item at index
* @param index passes index whose item to be returned
*/
template<typename T>
T TemplatedArray<T>::operator[](unsigned long index) const
{
	if (index >= size)
	{
		throw new ExceptionIndexOutOfRange();
	}
	else
	{
		return data[index];
	}
}
#endif
