/*   
 *   Program  for reading, filtering and writing data in reverse order
 *   Author:  Sijie Pan
 *   Date:    Oct. 5, 2018
 *   Version: 0.4
 *   Purpose:
 *   1. Satisfy all requirements of Lab3.
 *   2. Create a template class that encapsulates dynamically resized arrays.
 *   3. Add additional mutator methods to templated array class.
 *   4. Ensure all indexed array class methods throw exceptions if index is out of range.
 *   5. Use proper design, coding style, documentation, and formatting.
*/

extern int ErrCode = 0;

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Exceptions.h"
#include "TemplatedArray.h"
#include "NvraRecord.h"
using namespace std;



// Declaring functions before using.
bool check_duplication(TemplatedArray<NvraRecord>& currentArray, NvraRecord& currentrecord, unsigned long i_records);
bool check_invalid(NvraRecord& currentrecord);



int main(){
    
    // Open the statistics file. If it's fail to open the file, then exit.
    std::ifstream file_in("NVRA_Statistics_July_2018_SEB_DFH.csv", ios::in);
    if (file_in.fail()) { std::cout << "File is not found in current directory!" << std::endl; return -1;}
    
    std::string each_record;
    unsigned long irow = 0;
    
    // allocate new space for 10 NVRA records, and space for a single record.
    TemplatedArray<NvraRecord> currentArray;
    NvraRecord* currentRecord = new NvraRecord;
    
    // Starting to read file line by line.
    while (file_in.good() && !file_in.eof()){
        
        getline(file_in, each_record);
        std::istringstream string_in(each_record);
        
        // Skip the first line and blank line at the end
        if (!each_record.empty() && irow != 0){
            
            std::string var;
            unsigned long icol = 0;
            
            // Seperate parameters by commas
            while (getline(string_in, var, ',')){
                
                // Construct current record
                if (icol != 3 && icol != 11 && icol != 12){
                    try{
                        currentRecord->addNum(std::stoi(var));
                    }
                    catch(ExceptionIndexOutOfRange& Err){
                        std::cout << Err.getMsg() << std::endl;
                        return -1;
                    }
                }
                // Storage 3 string columns.
                else{
                    try{
                        currentRecord->addString(var);
                    }
                    catch(ExceptionIndexOutOfRange& Err){
                        std::cout << Err.getMsg() << std::endl;
                        return -1;
                    }
                }
                
                ++icol;
                
            }
                
            // Save records to the array.
            currentArray.add(currentRecord);
            
        }
        
        ++irow;
        
    }
    
    delete currentRecord;
    currentRecord = nullptr;
    
    
    // Output records or error messages in reverse order.
    NvraRecord record4print;
    
    for (unsigned long i = currentArray.getSize() - 1; i < currentArray.getCapacity(); --i){
        
        try{
            record4print = currentArray.get(i);
            if (!record4print.getString(0).empty()){
                if (check_invalid(record4print)){
                    std::cout << "Warning: Invalid data at line " << i + 1 << "." << std::endl;
                }
                else if (check_duplication(currentArray, record4print, i)){
                    std::cout << "Warning: Duplicate record ID " << record4print.getNum(0)
                            << " at line " << i + 1 << "." << std::endl;
                }
                else{
                    std::cout << record4print << std::endl;
                }
            
            } 
        }
        catch(ExceptionIndexOutOfRange& Err){
            std::cout << Err.getMsg() << std::endl;
            return -1;
        }
        
        
    }
    
    return 0;
    
}



// Function for checking if a record is duplicated.
bool check_duplication(TemplatedArray<NvraRecord>& currentArray, NvraRecord& currentrecord, unsigned long i_records){
    
    NvraRecord comprecord;
    for (unsigned long i = 0; i < i_records; ++i){
        
        try{
            comprecord = currentArray.get(i);
        
            // check whether the record is duplicated.
            if (i != i_records && !check_invalid(comprecord)
                && currentrecord.getNum(0) == comprecord.getNum(0)){
                // If the record is duplicted, return true.
                return true;
            }
        }
        catch(ExceptionIndexOutOfRange& Err){
            throw Err;
        }
        
    }
    
    // Otherwise, return false
    return false;
    
}



// Function for checking if a record is invalid.
bool check_invalid(NvraRecord& currentrecord){
    
    for (unsigned long icol = 1; icol < MAX_NUMS; ++icol){
        
        try{
            // If any data in record less than 0, return true.
            if (currentrecord.getNum(icol) < 0) return true;
        }
        catch(ExceptionIndexOutOfRange& Err){
            throw Err;
        }
        
    }
    
    // Otherwise, return false
    return false;
    
}
