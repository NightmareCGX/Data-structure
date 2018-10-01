/*   
 *   Program  for reading, filtering and writing data in reverse order
 *   Author:  Sijie Pan
 *   Date:    Sep. 3, 2018
 *   Version: 0.3
 *   Purpose:
 *   1. read and write data
 *   2. Use arrays to store the data as it is read in.
 *   3. Allocate and deallocate memory dynamiclly using new and delete.
 *   4. Functionality to be encapsulated with a class.
 *   5. Complete/use the given design elements, including two structures and one function plus main.
*/



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include "NvraArray.h"
#include "NvraRecord.h"
using namespace std;



// Declaring functions before using.
int check_duplicate(int ID, vector<int>& unique_ID_list);
void check_invalid(NvraRecord& currentrecord, int& return_ID);



int main(){
    
    // Open the statistics file. If it's fail to open the file, then exit.
    std::ifstream file_in("NVRA_Statistics_July_2018_SEB_DFH.csv", ios::in);
    if (file_in.fail()) { std::cout << "File is not found in current directory!" << std::endl; return -1;}
    
    std::string each_record;
    int irow = 0;
    
    // allocate new space for 10 NVRA records, and space for a single record.
    NvraArray currentArray;
    NvraRecord* currentRecord = new NvraRecord;
    std::vector<int> currentIDs;
    
    // Starting to read file line by line.
    while (file_in.good() && !file_in.eof()){
        
        getline(file_in, each_record);
        std::istringstream string_in(each_record);
        
        // Skip the first line and blank line at the end
        if (!each_record.empty() && irow != 0){
            
            std::string var;
            int icol_int = 0, icol_str = 0, icol = 0;
            
            // Seperate parameters by commas
            int icol_num = 0;
            while (getline(string_in, var, ',')){
                
                // Construct current record
                if (icol != 3 && icol != 11 && icol != 12){ 
                    currentRecord->addNum(std::stoi(var));
                }
                // Storage 3 string columns.
                else{
                    currentRecord->addString(var);
                }
                
                ++icol;
                
            }
            
            // Check if there's a duplicated ID for the first column
            // or invalide data in each row
            // and return a error flag (-99999/-99998) if circumstances exist.
            int temp_ID = check_duplicate(currentRecord->getNum(0), currentIDs);
            if (temp_ID >= 0) check_invalid(*currentRecord, temp_ID);
                
            // Save records to the array.
            currentArray.add(currentRecord);
            currentIDs.push_back(temp_ID);
            
        }
        
        ++irow;
        
    }
    
    delete currentRecord;
    currentRecord = nullptr;
    
    // Output records or error messages in reverse order.
    NvraRecord record4print;
	--irow;
    
    for (int i = irow; i > 0 ; --i){
        
        record4print = currentArray.get(i);
        if (!record4print.getString(0).empty()){
            if (currentIDs[i - 1] == -99999){
                std::cout << "Warning: Duplicate record ID " << record4print.getNum(0) 
                    << " at line " << i << "." << std::endl;
            }
            else if (currentIDs[i - 1] == -99998){
                std::cout << "Warning: Invalid data at line " << i << "." << std::endl;
            }
            else{
                std::cout << record4print << std::endl;
            }
        }
        
        
    }
    
    // deallocate data space.
    currentArray.~NvraArray();
    
}



// Function for checking if a record is duplicated.
int check_duplicate(int ID, vector<int>& unique_ID_list){
    
    // Initialize return ID with input ID
    int return_ID = ID;
    // check whether the record is duplicated.
    if (std::find(unique_ID_list.begin(), unique_ID_list.end(), ID) != unique_ID_list.end()){
        
        // If the record is duplicted, replace the ID with an error flag for duplication.
        return_ID = -99999;
        
    }
    
    return return_ID;
    
}



// Function for checking if a record is invalid.
void check_invalid(NvraRecord& currentrecord, int& return_ID){
    
    for (int icol = 1; icol < MAX_NUMS; ++icol){
        
        // If it is an invalid record, replace return ID with an error flag for invalid data.
        if (currentrecord.getNum(icol) < 0){
            
            return_ID = -99998;
            
        }
        
    }
    
}