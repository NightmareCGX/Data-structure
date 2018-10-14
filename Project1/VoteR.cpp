/*   
 *   Program  for reading, filtering and writing data in reverse order
 *   Author:  Sijie Pan
 *   Date:    Oct. 12, 2018
 *   Version: 1.0
 *   Purpose:
 *   1. Use C++ file I/O to read and write files while using C++ standard I/O (cin/cout) for user interaction,
 *      using appropriate exception handling.
 *   2. Encapsulate primitive arrays inside a temlated class that provides controlled access to the array data,
 *      retains information on array capacity and use, and can be used to store data of any class or primitive type.
 *   3. Integrate appropriate exception handling into the templated array class.
 *   4. Efficiently sort and search the data based on the field specified by the user (using comparator).
 *   5. Use proper design, coding style, documentation, and formatting.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include "Exceptions.h"
#include "NvraRecord.h"
#include "NvraComparator.h"
#include "TemplatedArray.h"
#include "Search.h"
#include "Sorter.h"
using namespace std;



// Declaring functions before using.
void data_input_loop(std::string& file_name, int& total_files, unsigned long& total_read, unsigned long& total_valid,
        unsigned long& total_stored, TemplatedArray<NvraRecord>& fullArray);
void data_manipulation_loop(std::string& file_name, unsigned long& total_read, unsigned long& total_valid,
        unsigned long& total_stored, TemplatedArray<NvraRecord>& fullArray);
void single_file_read_in(unsigned long& total_read, unsigned long& total_valid, unsigned long& total_stored,
        TemplatedArray<unsigned long>& uniqueID, TemplatedArray<NvraRecord>& fullArray, std::ifstream& file_in);
void update_record(unsigned long& total_read, unsigned long& total_valid, unsigned long& total_stored, unsigned long& single_read, 
        TemplatedArray<unsigned long>& uniqueID, const NvraRecord& record, TemplatedArray<NvraRecord>& fullArray, const int& flag);
void save_to_full_Array(unsigned long& total_stored, const NvraRecord& record4save, TemplatedArray<NvraRecord>& fullArray);
void record_fill(NvraRecord& record, const string& var, const int icol, int& validFlag);
int record_dump(const std::string& file_name, const unsigned long& total_read, const unsigned long& total_valid,
        const unsigned long& total_stored, const TemplatedArray<NvraRecord>& fullArray);
        void object_find(const std::string& col, const int& sortedCol, const TemplatedArray<NvraRecord>& fullArray);
bool check_duplication_within_file(const TemplatedArray<NvraRecord>& array, const NvraRecord& currentrecord, 
                const unsigned long& single_read, const TemplatedArray<unsigned long>& uniqueID);
bool is_number(const std::string& inStr);



int main() {
    // allocate necessary space for storing data.
    std::string file_name;
	int total_files = 0;
    unsigned long total_read = 0;
    unsigned long total_valid = 0;
    unsigned long total_stored = 0;
    // allocate new space for all NVRA records.
    TemplatedArray<NvraRecord> fullArray;
    
    // start data input loop
    try { data_input_loop(file_name, total_files, total_read, total_valid, total_stored, fullArray); }
    catch (Exception& Err) {
        std::cout << Err.getMsg() << std::endl;
        std::cout << "Error function: data_input_loop." << std::endl;
        return -1;
    }
    if (file_name.empty() && total_read == 0) return 0;
	
    // start data manupulation loop
    try { data_manipulation_loop(file_name, total_read, total_valid, total_stored, fullArray); }
    catch (Exception& Err) {
        std::cout << Err.getMsg() << std::endl;
        std::cout << "Error function: data_manipulation_loop." << std::endl;
        return -1;
    }
    
    // deallocate data space.
    fullArray.~TemplatedArray();
    return 0;
}



// function of data input loop.
void data_input_loop(std::string& file_name, int& total_files, unsigned long& total_read, unsigned long& total_valid,
                    unsigned long& total_stored, TemplatedArray<NvraRecord>& fullArray) {
    while (true) {
        std::cout << "Enter data file name: ";
        std::getline(std::cin, file_name);
        // If input name is empty then exit loop.
        if (file_name.empty()) break;
        // Open the statistics file. If it's inaccessible, then require users to re-enter.
        std::ifstream file_in(file_name, ios::in);
        if (file_in.fail()) {
    		std::cout << "File is not available." << std::endl;
    		continue;
        }
    
        TemplatedArray<unsigned long> uniqueID;
        single_file_read_in(total_read, total_valid, total_stored, uniqueID, fullArray, file_in);
        file_in.close();
        total_files += 1;
        uniqueID.~TemplatedArray();
    }
}


// function of data manipulation loop.
void data_manipulation_loop(std::string& file_name, unsigned long& total_read, unsigned long& total_valid,
                        unsigned long& total_stored, TemplatedArray<NvraRecord>& fullArray) {
    std::string option;
    std::string col;
    int sortedCol = -1;
    
    while (true) {
        option = "u";
        std::cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: ";
        std::getline(std::cin, option);
        if (option.length() != 1 || is_number(option)) { continue; }
        std::transform(option.begin(), option.end(), option.begin(), ::tolower);
        //cin.ignore();
        
        if (option == "q") {
            std::cout << "Thanks for using VoteR." << std::endl;
            break;
        }
        else if (option == "o") {
            while (true) {
                std::cout << "Enter output file name: ";
                std::getline(std::cin, file_name);
                int status = record_dump(file_name, total_read, total_valid, total_stored, fullArray);
                if (status == 0) { break; }
            }
        }
        else if (option == "s") {
            std::cout << "Enter sort field (0-23): ";
            std::getline(std::cin, col);
            if (!is_number(col)) { continue; }
			if (stoi(col) <= 23 && stoi(col) >= 0) {
                sortedCol = stoi(col);
				NvraComparator comparator(sortedCol);
                Sorter<NvraRecord>::sort(fullArray, comparator);
			}
			else { continue; }
        }
        else if (option == "f") {
            std::cout << "Enter search field (0-23): ";
            std::getline(std::cin, col);
            object_find(col, sortedCol, fullArray);
        }
    }
}


// Read in records from a single file.
void single_file_read_in(unsigned long& total_read, unsigned long& total_valid, unsigned long& total_stored,
                    TemplatedArray<unsigned long>& uniqueID, TemplatedArray<NvraRecord>& fullArray, std::ifstream& file_in) {
    std::string each_record;
    unsigned long irow =0, single_read = 0;
    
    // Starting to read file line by line.
    while (file_in.good() && !file_in.eof()) {
        int validFlag = 0;
        NvraRecord currentRecord;
        getline(file_in, each_record);
        std::istringstream string_in(each_record);
    
        // Skip the first line and blank line at the end
        if (!each_record.empty() && irow != 0) {
            std::string var;
            unsigned long icol = 0;
        
            // Seperate parameters by commas
            while (getline(string_in, var, ',')) {
                // Construct current record
                record_fill(currentRecord, var, icol, validFlag);
                ++icol;
            }
            
            // Save valid/latest updated records to the array.
            try{
                update_record(total_read, total_valid, total_stored, single_read, 
                            uniqueID, currentRecord, fullArray, validFlag);
            }
            catch (Exception& Err) {
                std::cout << Err.getMsg() << std::endl;
                throw Exception("single_file_read_in");
            }
        }
    
        ++irow;
    }
}


// Updates record to Array
void update_record(unsigned long& total_read, unsigned long& total_valid, unsigned long& total_stored, unsigned long& single_read,
                TemplatedArray<unsigned long>& uniqueID, const NvraRecord& record, TemplatedArray<NvraRecord>& fullArray,
                const int& flag) {
    if (flag == 1) { std::cout << "Warning: Invalid data at line " << single_read + 1 << std::endl; }
    else if (check_duplication_within_file(fullArray, record, single_read, uniqueID)) {
        std::cout << "Warning: Duplicate record ID " << record.getNum(0) << " at line " 
                << single_read + 1 << "." << std::endl;
        total_valid += 1;
    }
    else {
        try {
            unsigned long temp = record.getNum(0);
            uniqueID.add(&temp);
            save_to_full_Array(total_stored, record, fullArray);
            total_valid += 1;
        }
        catch (Exception& Err) {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("update_record");
        }
    }
    
    single_read += 1;
    total_read += 1;
}


// Save record from one file to the full Array.
void save_to_full_Array(unsigned long& total_stored, const NvraRecord& record4save, TemplatedArray<NvraRecord>& fullArray) {
    if (total_stored == 0) {
        try {
            fullArray.add(&record4save);
            total_stored += 1;
        }
        catch (ExceptionMemoryNotAvailable& Err) {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("save_to_full_Array");
        }
    }
    else {
        NvraComparator comparator(0);
        long long dup_ind = binarySearch(record4save, fullArray, comparator);
        
        if (dup_ind < 0) {
            unsigned long insert_ind = (dup_ind + 1) * -1;
            
            if (insert_ind == fullArray.getSize()) {
                try {
                    fullArray.add(&record4save);
                }
                catch (ExceptionMemoryNotAvailable& Err) {
                    std::cout << Err.getMsg() << std::endl;
                    throw Exception("save_to_full_Array");
                }
            }
            else {
                try {
                    fullArray.addAt(&record4save, insert_ind);
                }
                catch (ExceptionIndexOutOfRange& Err) {
                    std::cout << Err.getMsg() << std::endl;
                    throw Exception("save_to_full_Array");
                }
                catch (ExceptionMemoryNotAvailable& Err) {
                    std::cout << Err.getMsg() << std::endl;
                    throw Exception("save_to_full_Array");
                }
            }
            
            total_stored += 1;
        }
        else {
            unsigned long replace_ind = dup_ind;
            
            try {
                fullArray.replaceAt(&record4save, replace_ind);
            }
            catch (ExceptionIndexOutOfRange& Err) {
                std::cout << Err.getMsg() << std::endl;
                throw Exception("save_to_full_Array");
            }
        }
    }
}


void record_fill(NvraRecord& record, const string& var, const int icol, int& validFlag) {
    if (icol != 3 && icol != 11 && icol != 12) {
        if (std::stoi(var) < 0) { validFlag = 1; return;}
        record.addNum(std::stoi(var));
    }
    else {
        record.addString(var);
    }
}


int record_dump(const std::string& file_name, const unsigned long& total_read, const unsigned long& total_valid,
            const unsigned long& total_stored, const TemplatedArray<NvraRecord>& fullArray) {
    if (file_name.empty()) {
        for (unsigned long i = 0; i < fullArray.getSize(); ++i) {
            std::cout << fullArray.get(i) << std::endl;
        }
        std::cout << "Data lines read: " << total_read << "; Valid NVRA records read: "
            << total_valid << "; NVRA records in memory: " << total_stored << std::endl;
    }
    else {
        std::ofstream file_out(file_name, ios::out);
        if (file_out.fail()) {
    		std::cout << "File is not available." << std::endl;
            return -1;
        }
        else {
            for (unsigned long i = 0; i < fullArray.getSize(); ++i) {
                file_out << fullArray.get(i) << std::endl;
            }
            file_out << "Data lines read: " << total_read << "; Valid NVRA records read: "
                << total_valid << "; NVRA records in memory: " << total_stored << std::endl;
            file_out.close();
        }
    }
    
    return 0;
}


void object_find(const std::string& col, const int& sortedCol, const TemplatedArray<NvraRecord>& fullArray) {
    if (!is_number(col)) { return; }
    if (stoi(col) > 23 || stoi(col) < 0) { return; }
    
    unsigned long count = 0;
    std::string inText;
    NvraRecord searchItem;
    
    if (stoi(col) == 3 || stoi(col) == 11 || stoi(col) == 12) {
        std::cout << "Enter exact text on which to search: ";
        std::getline(std::cin, inText);
        if (inText.empty()) { return; }
        
        if (stoi(col) == 3) searchItem.setString(inText, 0);
        if (stoi(col) == 11) searchItem.setString(inText, 1);
        if (stoi(col) == 12) searchItem.setString(inText, 2);
        
        count = search(searchItem, fullArray, stoi(col), sortedCol);
    }
    else {
        std::cout << "Enter non-negative field value: ";
        std::getline(std::cin, inText);
        if (inText.empty() || !is_number(inText)) { return; }
        if (stoi(inText) < 0) { return; }

        if (stoi(col) < 3) {
            searchItem.setNum(stoi(inText), stoi(col));
        }
        else if (stoi(col) < 11) {
            searchItem.setNum(stoi(inText), stoi(col) - 1);
        }
        else {
            searchItem.setNum(stoi(inText), stoi(col) - 3);
        }
        
        count = search(searchItem, fullArray, stoi(col), sortedCol);
    }
    
    std::cout << "NVRA records found: " << count << std::endl;
}


// Function for checking if a record is duplicated.
bool check_duplication_within_file(const TemplatedArray<NvraRecord>& array, const NvraRecord& currentrecord, 
                const unsigned long& single_read, const TemplatedArray<unsigned long>& uniqueID) {
    unsigned long num_records =uniqueID.getSize();
    if (single_read == 0 || num_records == 0) { return false; }
    
    for (unsigned long i = 0; i < num_records; ++i) {
        try{
            // check whether the record is duplicated. If the record is duplicted, return true.
            NvraComparator comparator(0);
            if (comparator.compare(currentrecord, array.get(i)) == 0) { return true; }
        }
        catch (Exception& Err) { 
            std::cout << Err.getMsg() << std::endl;
            throw Exception("check_duplication_within_file");
        }
    }
    
    // Otherwise, return false
    return false;
}


// check if input is a number
bool is_number(const std::string& inStr) {
	std::string::const_iterator iter = inStr.begin();
	while (iter != inStr.end() && std::isdigit(*iter)) { ++iter; }
	return !inStr.empty() && iter == inStr.end();
	//return !inStr.empty() and std::find_if(inStr.begin(), inStr.end(), 
	//			[](char c) {return !std::isdigit(c);} ) == inStr.end();
}
