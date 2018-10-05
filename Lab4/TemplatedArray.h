#ifndef NVRA_ARRAY_H
#define NVRA_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;		    // capacity used in no arg constructor

template <typename T> class TemplatedArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;        // maximum capacity, in records
	unsigned long size = 0;								    // actual number of items currently in array
	T* data = NULL;					                        // pointer to array of records
    void doubleCapacity();                                  // method to double array capacity
    void halveCapacity();                                   // method to halve array capacity
public:
	TemplatedArray();								        // constructs array with default capacity
	TemplatedArray(unsigned long capacity);			        // constructs array with specified capacity
	virtual ~TemplatedArray();						        // frees array space as object is deleted
	void add(const T* record);				                // adds record, increments size, doubles capacity as necessary
    void addAt(const T* record, unsigned long index);       // add item to location index, shifting following items
    void replaceAt(const T* record, unsigned long index);         // repalce item at location index
    void removeAt(unsigned long index);                     // remove item located at index, shifting other items
	T get(unsigned long index) const;	                      // returns (copy of) record at index
    unsigned long getCapacity() const;                      // returns the current capacity of the array
    unsigned long getSize() const;					        // returns the current number of items in the array
};



// implementation goes here
// constructs array with default capacity
template <typename T> TemplatedArray<T>::TemplatedArray(){
	this->data = new T[this->capacity];
}


// constructs array with specified capacity
template <typename T> TemplatedArray<T>::TemplatedArray(unsigned long capacity){
	this->data = new T[capacity];
}


// frees array space as object is deleted
template <typename T> TemplatedArray<T>::~TemplatedArray(){
	if (this->data != nullptr){
		delete [] this->data;
		this->data = nullptr;
	}
}


// method to double array capacity
template <typename T> void TemplatedArray<T>::doubleCapacity(){
        
    TemplatedArray* tempArray = new TemplatedArray(this->capacity*2);
    
    for (unsigned long i = 0; i< this->capacity; ++i){
        
        tempArray->data[i] = this->data[i];
	
    }

    this->~TemplatedArray();
    this->capacity *= 2;
    this->data = tempArray->data;
	
}


// method to halve array capacity
template <typename T> void TemplatedArray<T>::halveCapacity(){

    this->capacity /= 2;
    TemplatedArray* tempArray = new TemplatedArray(this->capacity);
    
    for (unsigned long i = 0; i< this->capacity; ++i){
        
        tempArray->data[i] = this->data[i];
	
    }

    this->~TemplatedArray();
    this->data = tempArray->data;
	
}


// adds record, increments size, doubles capacity as necessary
template <typename T> void TemplatedArray<T>::add(const T* record){

    if (this->size >= this->capacity) this->doubleCapacity();

    this->data[this->size] = *record;
    
	this->size += 1;

}


// add item to location index, shifting following items
template <typename T> void TemplatedArray<T>::addAt(const T* record, unsigned long index){

    if (index > this->size){
        throw ExceptionIndexOutOfRange("TemplatedArray::addAt, not a valid index for function addAT", index, this->size);
    }
    else if (index == this->size){
        throw ExceptionIndexOutOfRange("TemplatedArray::addAt, please use function add instead", index, this->size);
    }

    if (this->size >= this->capacity) this->doubleCapacity();
    
    for (unsigned long irow = this->size - 1; irow < this->size && irow >= index; --irow){
        this->data[irow + 1] = this->data[irow];
    }
    
    this->data[index] = *record;
	this->size += 1;

}

// repalce item at location index
template <typename T> void TemplatedArray<T>::replaceAt(const T* record, unsigned long index){
    
    if (index >= this->size){
        throw ExceptionIndexOutOfRange("TemplatedArray::replaceAt, not a valid index for replacement", index, this->size - 1);
    }
    
    this->data[index] = *record;
    
}


// remove item located at index, shifting other items
template <typename T> void TemplatedArray<T>::removeAt(unsigned long index){

    if (index >= this->size){
        throw ExceptionIndexOutOfRange("TemplatedArray::removeAt, not a valid index for removing item", index, this->size -1);
    }
    
    this->size -= 1;
    
    for (unsigned long irow = index; irow < this->size; ++irow){
        this->data[irow] = this->data[irow+1];
    }
    
    if (this->size <= this->capacity/2) this->halveCapacity();
    
}


// returns (copy of) record at index
template <typename T> T TemplatedArray<T>::get(unsigned long index) const{
    if (index >= this->capacity){
        throw ExceptionIndexOutOfRange("TemplatedArray::get", index, this->capacity - 1);
    }
	T record_index = this->data[index];
	return record_index;
}


template <typename T> unsigned long TemplatedArray<T>::getCapacity() const{
	return this->capacity;
}


template <typename T> unsigned long TemplatedArray<T>::getSize() const{
	return this->size;
}


#endif