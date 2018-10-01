#include "NvraArray.h"


// constructs array with default capacity
NvraArray::NvraArray(){
	this->data = new NvraRecord[this->capacity];
}


// constructs array with specified capacity
NvraArray::NvraArray(int capacity){
    
	this->data = new NvraRecord[capacity];
    
}


// frees array space as object is deleted
NvraArray::~NvraArray(){
	if (this->data != nullptr){
		delete [] this->data;
		this->data = nullptr;
	}
}


// adds record, increments size, doubles capacity as necessary
void NvraArray::add(NvraRecord* record){
    
    int ind_num = 0, ind_str = 0;
    
    for (int ind = 0; ind < 24; ++ind){
        
        if (ind != 3 && ind != 11 && ind != 12){
            this->data[this->size].addNum(record->getNum(ind_num));
            ++ind_num;
        }
        else{
            this->data[this->size].addString(record->getString(ind_str));
            ++ind_str;
        }
        
    }
    
	this->size += 1;
	if (this->size == this->capacity){
		
		NvraArray* tempArray = new NvraArray(this->capacity*2);
		
	    for (int i = 0; i< this->capacity; ++i){
            
			tempArray->data[i] = this->data[i];
			
	    }
		
		this->~NvraArray();
		this->capacity *= 2;
        this->data = tempArray->data;
		
	}
}


// returns (copy of) record at index
NvraRecord NvraArray::get(int index){
	index -= 1;
	NvraRecord record_index = this->data[index];
	return record_index;
}
