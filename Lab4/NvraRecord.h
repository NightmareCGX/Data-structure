#ifndef NVRA_RECORD_H
#define NVRA_RECORD_H

#include "Exceptions.h"
#include <iostream>

const unsigned long MAX_NUMS = 21;					// the maximum numeric fields in each record
const unsigned long MAX_STRINGS = 3;					// the maximum string fields in each record

class NvraRecord {

	// overloaded output operator that sends one formatted record to ostream
	friend std::ostream& operator<<(std::ostream& os, const NvraRecord& record);

private:
	int nums[MAX_NUMS];						// holds the numeric data, in column order
	unsigned long numCtr = 0;							// counts the number of numbers added to the record
	std::string strings[MAX_STRINGS];		// holds the string data, in column order
	unsigned long strCtr = 0;							// count the number of strings added to the record
public:
	void addNum(int num);					// puts num in array, increments counter
	void addString(std::string string);		// puts string in array, increments counter
	int getNum(unsigned long index);					// returns (copy of) num at index in array
	std::string getString(unsigned long index);		// returns (copy of) string at index in array
};
#endif