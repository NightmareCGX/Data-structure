// These definitions may be used as a basis for your templated array class. However, you are
// not required to use them exactly as they are. You may change details as you see fit.
// Nonetheless, you must retain the signatures of all public member functions (methods) and
// their described functionality, so that they may be used in unit tests.

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
	TemplatedArray();								    // constructs array with default capacity
	TemplatedArray(unsigned long capacity);			    // constructs array with specified capacity
    TemplatedArray(const TemplatedArray<T>& copied);        // constructs array having exact same size as copied one
	virtual ~TemplatedArray();						    // frees array space as object is deleted
	void add(const T* item);						    // adds item, increments size, doubles capacity as necessary
	void addAt(const T* item, unsigned long index);	    // adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(const T* item, unsigned long index); // replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);				    // removes item at index, shifts following back
	T get(unsigned long index) const;				    // returns (copy of) item at index
	unsigned long getCapacity() const;				    // returns the current capacity of the array
	unsigned long getSize() const;					    // returns the current number of items in the array
	T operator[](unsigned long index) const;		    // returns (copy of) item at index
};



// implementation goes here
// constructs array with default capacity
template <typename T> TemplatedArray<T>::TemplatedArray() {
	this->data = new T[this->capacity];
	if (this->data == nullptr) throw ExceptionMemoryNotAvailable("TemplatedArray (Default constructor)");
}


// constructs array with specified capacity
template <typename T> TemplatedArray<T>::TemplatedArray(unsigned long capacity) {
	this->data = new T[capacity];
    if (this->data == nullptr) throw ExceptionMemoryNotAvailable("TemplatedArray (constructor with args)");
}


// constructs array having exact same size as copied one
template <typename T> TemplatedArray<T>::TemplatedArray(const TemplatedArray<T>& copied) {
    this->capacity = copied.getCapacity();
    this->size = copied.getSize();
    this->data = new T[this->capacity];
    if (this->data == nullptr) throw ExceptionMemoryNotAvailable("TemplatedArray (Copy constructor)");
    
    for (unsigned long i = 0; i < copied.getSize(); i++) { this->data[i] = copied.data[i]; }
}


// frees array space as object is deleted
template <typename T> TemplatedArray<T>::~TemplatedArray() {
	if (this->data != nullptr) { delete [] this->data; this->data = nullptr; }
}


// method to double array capacity
template <typename T> void TemplatedArray<T>::doubleCapacity() {
    TemplatedArray* tempArray = new TemplatedArray(this->capacity*2);
    if (tempArray == nullptr) throw ExceptionMemoryNotAvailable("doubleCapacity");
    
    for (unsigned long i = 0; i< this->capacity; ++i) { tempArray->data[i] = this->data[i]; }

    this->~TemplatedArray();
    this->capacity *= 2;
    this->data = tempArray->data;
    tempArray->data = nullptr;
    delete tempArray;
}


// method to halve array capacity
template <typename T> void TemplatedArray<T>::halveCapacity() {
    this->capacity /= 2;
    TemplatedArray* tempArray = new TemplatedArray(this->capacity);
    if (tempArray == nullptr) throw ExceptionMemoryNotAvailable("halveCapacity");
    
    for (unsigned long i = 0; i< this->capacity; ++i) { tempArray->data[i] = this->data[i]; }

    this->~TemplatedArray();
    this->data = tempArray->data;
    tempArray->data = nullptr;
    delete tempArray;
}


// adds record, increments size, doubles capacity as necessary
template <typename T> void TemplatedArray<T>::add(const T* item) {
    if (this->size >= this->capacity) {
        try { this->doubleCapacity(); }
        catch (ExceptionMemoryNotAvailable& ReturnErr) {
            std::cout << ReturnErr.getMsg() << std::endl;
            throw ExceptionMemoryNotAvailable("TemplatedArray<T>::add");
        }
    }
        
    this->data[this->size] = *item;
	this->size += 1;
}


// add item to location index, shifting following items
template <typename T> void TemplatedArray<T>::addAt(const T* item, unsigned long index) {
    if (index > this->size) {
        throw ExceptionIndexOutOfRange("TemplatedArray::addAt, not a valid index for function addAT", index, this->size);
    }
    else if (index == this->size) {
        throw ExceptionIndexOutOfRange("TemplatedArray::addAt, please use function add instead", index, this->size);
    }

    if (this->size >= this->capacity) {
        try { this->doubleCapacity(); }
        catch (ExceptionMemoryNotAvailable& ReturnErr) {
            std::cout << ReturnErr.getMsg() << std::endl;
            throw ExceptionMemoryNotAvailable("TemplatedArray<T>::add");
        }
    }
    
    for (unsigned long irow = this->size - 1; irow < this->size && irow >= index; --irow) {
        this->data[irow + 1] = this->data[irow];
    }
    this->data[index] = *item;
	this->size += 1;
}


// replaces item at index, otherwise unchanged
template <typename T> void TemplatedArray<T>::replaceAt(const T* item, unsigned long index) {
    if (index >= this->size) {
        throw ExceptionIndexOutOfRange("TemplatedArray::replaceAt, not a valid index for replacement", index, this->size - 1);
    }
    this->data[index] = *item;
}


// removes item located at index, shifts following back
template <typename T> void TemplatedArray<T>::removeAt(unsigned long index) {
    if (index >= this->size) {
        throw ExceptionIndexOutOfRange("TemplatedArray::removeAt, not a valid index for removing item", index, this->size -1);
    }
    this->size -= 1;
    
    for (unsigned long irow = index; irow < this->size; ++irow) {
        this->data[irow] = this->data[irow+1];
    }
    
    if (this->size <= this->capacity/2) {
        try { this->halveCapacity(); }
        catch (ExceptionMemoryNotAvailable& ReturnErr) {
            std::cout << ReturnErr.getMsg() << std::endl;
            throw ExceptionMemoryNotAvailable("TemplatedArray<T>::removeAt")
        }
    }
}


// returns (copy of) record at index
template <typename T> T TemplatedArray<T>::get(unsigned long index) const {
    if (index >= this->capacity) {
        throw ExceptionIndexOutOfRange("TemplatedArray::get", index, this->capacity - 1);
    }
    
	return this->data[index];
}


// returns the current capacity of the array
template <typename T> unsigned long TemplatedArray<T>::getCapacity() const {
	return this->capacity;
}


// returns the current capacity of the array
template <typename T> unsigned long TemplatedArray<T>::getSize() const {
	return this->size;
}


// Override operator [] for returning record at index
template <typename T> T TemplatedArray<T>::operator[](unsigned long index) const {
    if (index >= this->capacity) {
        throw ExceptionIndexOutOfRange("TemplatedArray::get", index, this->capacity - 1);
    }
    
    return this->data[index];
}

#endif
