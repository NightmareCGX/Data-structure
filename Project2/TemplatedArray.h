#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H

#include "Exceptions.h"

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
	TemplatedArray();									// constructs array with default capacity
	TemplatedArray(unsigned long capacity);				// constructs array with specified capacity
	virtual ~TemplatedArray();							// frees array space as object is deleted
	void add(const T* item);							// adds item, increments size, doubles capacity as necessary
	void addAt(const T* item, unsigned long index);		// adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(const T* item, unsigned long index);	// replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);					// removes item at index, shifts following back
	T get(unsigned long index) const;					// returns (copy of) item at index
	unsigned long getCapacity() const;					// returns the current capacity of the array
	unsigned long getSize() const;						// returns the current number of items in the array
	T operator[](unsigned long index) const;			// returns (copy of) item at index
};

// Implementation goes here

#endif