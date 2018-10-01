#include "NvraRecord.h"


// puts num in array, increments counter
void NvraRecord::addNum(int num){
	
	this->nums[this->numCtr] = num;
	this->numCtr += 1;
    
	if (this->numCtr == MAX_NUMS){
		this->numCtr = 0;
	}
    
}


// puts string in array, increments counter
void NvraRecord::addString(std::string string){
	
	this->strings[this->strCtr] = string;
	this->strCtr += 1;
    
	if (this->strCtr == MAX_STRINGS){
		this->strCtr = 0;
	}
    
}


// returns (copy of) num at index in array
int NvraRecord::getNum(int index){
	return this->nums[index];
}


// returns (copy of) string at index in array
std::string NvraRecord::getString(int index){
	return this->strings[index];
}


// overloaded output operator that sends one formatted record to ostream
std::ostream& operator<<(std::ostream& os, const NvraRecord& record){
    
    int icol_num = 0, icol_str = 0;
    for (int icol = 0; icol <= 23; ++icol){
        
        if (icol == 23){
            os << record.nums[icol_num];
        }
        else if (icol != 3 && icol != 11 && icol != 12){ 
            os << record.nums[icol_num] << ";" ;
            ++icol_num;  
        }
        else{
            os << record.strings[icol_str] << ";" ;
            ++icol_str;
        }
        
    }
    
    return os;
    
}