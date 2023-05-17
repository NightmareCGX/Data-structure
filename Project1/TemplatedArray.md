This is a C++ header file that defines a templated array class called TemplatedArray. It provides functionality to store and manipulate an array of elements of any type.

```c++
#ifndef TEMPLATED_ARRAY_H
#define TEMPLATED_ARRAY_H
```
The #ifndef and #define directives are used for header file inclusion guards. They ensure that the header file is only included once during compilation to prevent duplicate declarations.

```c++
#include "Exceptions.h"
```
This line includes another header file called "Exceptions.h", which is used to define exception classes used in this project.

```c++
const unsigned long DEFAULT_ARRAY_CAPACITY = 10;
```
This line declares a constant variable DEFAULT_ARRAY_CAPACITY with an initial value of 10. It represents the default capacity used when constructing an array without specifying the capacity.

```c++
template <typename T>
class TemplatedArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;
	unsigned long size = 0;
	T* data = NULL;
	void doubleCapacity();
	void halveCapacity();
public:
	TemplatedArray();
	TemplatedArray(unsigned long capacity);
    TemplatedArray(const TemplatedArray<T>& copied);
	virtual ~TemplatedArray();
	void add(const T* item);
	void addAt(const T* item, unsigned long index);
	void replaceAt(const T* item, unsigned long index);
	void removeAt(unsigned long index);
	T get(unsigned long index) const;
	unsigned long getCapacity() const;
	unsigned long getSize() const;
	T operator[](unsigned long index) const;
};
```
This block defines the template class. It has private member variables capacity, size, and data. \
capacity represents the maximum capacity that the array can hold, \
size represents the actual number of elements in the array, \
and data is a pointer to the dynamically allocated array of type T. 

The class also declares private methods `doubleCapacity()` and `halveCapacity()`, which are used to resize the array.

The public section of the class contains the constructors, the destructor, and various methods.

`add(const T* item)`: Adds an item to the array, adjusts the size attribute of the array, and doubles the capacity if necessary.\
`addAt(const T* item, unsigned long index)`: Adds an item at a specific index in the array, shifting the following items. It doubles the capacity if necessary.\
`replaceAt(const T* item, unsigned long index)`: Replaces the item at a specific index with a new item.\
`removeAt(unsigned long index)`: Removes the item at a specific index in the array and shifts the following items back. It halves the capacity if necessary.\
`get(unsigned long index) const`: Returns a copy of the item at a specific index in the array.\
`getCapacity() const`: Returns the current capacity of the array.\
`getSize() const`: Returns the current number of items in the array.\
`operator[](unsigned long index) const`: Overloads the [] operator to allow accessing elements of the array by index.

```c++
template <typename T> TemplatedArray<T>::TemplatedArray() {
	this->data = new T[this->capacity];
	if (this->data == nullptr) throw ExceptionMemoryNotAvailable("TemplatedArray (Default constructor)");
}
```
This is the implementation of the default constructor. It dynamically allocates an array of type T with the default capacity and checks if the memory allocation was successful. If not, it throws an ExceptionMemoryNotAvailable exception.

```c++
template <typename T> TemplatedArray<T>::TemplatedArray(unsigned long capacity) {
	this->data = new T[capacity];
    if (this->data == nullptr) throw ExceptionMemoryNotAvailable("TemplatedArray (constructor with args)");
}
```
This is the implementation of the constructor that takes a capacity argument. It dynamically allocates an array of type T with the specified capacity and checks if the memory allocation was successful. If not, it throws an ExceptionMemoryNotAvailable exception.

```c++
template <typename T> TemplatedArray<T>::TemplatedArray(const TemplatedArray<T>& copied) {
    this->capacity = copied.getCapacity();
    this->size = copied.getSize();
    this->data = new T[this->capacity];
    if (this->data == nullptr) throw ExceptionMemoryNotAvailable("TemplatedArray (Copy constructor)");
    
    for (unsigned long i = 0; i < copied.getSize(); i++) { this->data[i] = copied.data[i]; }
}
```
This is the implementation of the copy constructor. It creates a new array with the same capacity and size as the copied array. It copies the elements from the copied array to the new array using a loop.

```c++
template <typename T> TemplatedArray<T>::~TemplatedArray() {
	if (this->data != nullptr) { delete [] this->data; this->data = nullptr; }
}
```
This is the implementation of the destructor. It checks if the data pointer is not nullptr and deletes the dynamically allocated array.

```c++
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
```
This function is responsible for doubling the capacity of the array. \
It creates a new array with twice the capacity, copies the elements from the original array to the temporary array, updates the capacity and data pointers (point to the new array), and deallocate the original array. \
This function is called when the size of the array reaches its current capacity, and more space is needed to accommodate additional elements.

```c++
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
```
This function is responsible for halving the capacity of the array. \
It creates a new  array with half the capacity, copies the elements from the original array to the temporary array, updates the capacity and data pointers (point to the new array), and deallocates the original array. 
This function is called when the size of the array becomes significantly smaller than its current capacity, allowing the array to save memory.

```c++
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
```
This function adds an item to the end of the array. \
If the array's size reaches its capacity, the function calls `doubleCapacity()` to increase the capacity before adding the item.

```c++
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
```
This function adds an item at a specific index in the array. \
It shifts the existing elements from the specified index and onwards to the right to make space for the new item. \
If the array is already full, it calls doubleCapacity() to increase the capacity before adding the item. 

```c++
template <typename T> void TemplatedArray<T>::replaceAt(const T* item, unsigned long index) {
    if (index >= this->size) {
        throw ExceptionIndexOutOfRange("TemplatedArray::replaceAt, not a valid index for replacement", index, this->size - 1);
    }
    this->data[index] = *item;
}
```
This function replaces the item at a specific index in the array with a new item. \
It directly assigns the new item to the specified index without changing the size or capacity of the array.

```c++
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
```
This function removes the item at a specific index from the array. \
It shifts the subsequent elements to the left to fill the gap created by the removal. \
If the size of the array becomes significantly smaller than half its capacity after the removal, it calls halveCapacity() to reduce the capacity. 

```c+
template <typename T> T TemplatedArray<T>::get(unsigned long index) const {
    if (index >= this->size) {
        throw ExceptionIndexOutOfRange("TemplatedArray::get", index, this->size - 1);
    }
    
	return this->data[index];
}
```
This function returns a copy of the item at a specific index in the array. \
It performs a bounds check to ensure that the index is within the valid range of the array.

```c++
template <typename T> unsigned long TemplatedArray<T>::getCapacity() const {
	return this->capacity;
}
```
This function returns the current capacity of the array.

```c++
template <typename T> unsigned long TemplatedArray<T>::getSize() const {
	return this->size;
}
```
This function returns the current number of items in the array.


```c++
template <typename T> T TemplatedArray<T>::operator[](unsigned long index) const {
    if (index >= this->capacity) {
        throw ExceptionIndexOutOfRange("TemplatedArray::get", index, this->capacity - 1);
    }
    
    return this->data[index];
}
```
This part is the implementation of the operator[] function, which overloads the subscript operator to allow accessing elements of the array by index.
