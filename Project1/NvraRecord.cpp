#include "NvraRecord.h"



// puts num in array, increments counter
void NvraRecord::addNum(unsigned int num) {
    if (this->numCtr > MAX_NUMS - 1) {
        throw ExceptionIndexOutOfRange("NvraRecord::addNum", this->numCtr, MAX_NUMS - 1);
    }
    
	this->nums[this->numCtr] = num;
	this->numCtr += 1;

	if (this->numCtr == MAX_NUMS) { this->numCtr = 0; }     
}


// puts string in array, increments counter
void NvraRecord::addString(std::string string) {
    if (this->strCtr > MAX_STRINGS - 1) {
        throw ExceptionIndexOutOfRange("NvraRecord::addString", this->strCtr, MAX_STRINGS - 1);
    }
        
    this->strings[this->strCtr] = string;
    this->strCtr += 1;
    
    if (this->strCtr == MAX_STRINGS) { this->strCtr = 0; }
}


// returns (copy of) num at index in array
unsigned int NvraRecord::getNum(unsigned int index) const {
    if (index > MAX_NUMS - 1) {
        throw ExceptionIndexOutOfRange("NvraRecord::getNum", index, MAX_NUMS - 1);
    }   
    
    return this->nums[index];
}


// returns (copy of) string at index in array
std::string NvraRecord::getString(unsigned int index) const {
    if (index > MAX_STRINGS - 1) {
        throw ExceptionIndexOutOfRange("NvraRecord::getString", index, MAX_STRINGS - 1);
    }
    
    return this->strings[index];
}


// sets nums[index] to num, doesn't change counter
void NvraRecord::setNum(unsigned int num, unsigned int index) {
    if (index > MAX_NUMS - 1) {
        throw ExceptionIndexOutOfRange("NvraRecord::setNum", index, MAX_NUMS - 1);
    }
    
    this->nums[index] = num;
}


// sets strings[index] to string, doesn't change counter
void NvraRecord::setString(std::string string, unsigned int index) {
    if (index > MAX_STRINGS - 1) {
        throw ExceptionIndexOutOfRange("NvraRecord::setString", index, MAX_STRINGS - 1);
    }
    
    this->strings[index] = string;
}


// overloaded output operator that sends one formatted record to ostream
std::ostream& operator<<(std::ostream& os, const NvraRecord& record) {
    unsigned int icol_num = 0, icol_str = 0;
    
    for (unsigned int icol = 0; icol <= 23; ++icol) {
        if (icol == 23) { os << record.nums[icol_num]; }
        else if (icol != 3 && icol != 11 && icol != 12) { 
            os << record.nums[icol_num] << ";" ;
            ++icol_num;  
        }
        else {
            os << record.strings[icol_str] << ";" ;
            ++icol_str;
        }
    }
    
    return os;
}