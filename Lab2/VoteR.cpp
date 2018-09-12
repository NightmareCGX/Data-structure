/*   
 *   Program  for reading, filtering and writing data in reverse order
 *   Author:  Sijie Pan
 *   Date:    Sep. 3, 2018
 *   Version: 0.2
 *   Purpose:
 *   1. read and write data
 *   2. Use arrays to store the data as it is read in.
 *   3. Allocate and deallocate memory dynamiclly using new and delete.
 *   4. Complete/use the given design elements, including two structures and one function plus main.
*/



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory.h>
#include <unistd.h>
using namespace std;



// Defination of sturctures for Records and Arrays.
struct nvraRecord
    {
        int nums[21];
        std::string strings[3];
    };

struct nvraArray
    {
        int capacity;
        nvraRecord* data = NULL;
    };



// Declaring functions before using.
void check_duplicate(std::string ID, vector<int>& unique_ID_list, nvraRecord& currentrecord);
void check_invalid(std::string param, vector<int>& unique_ID_list, nvraRecord& currentrecord);
void write_out(nvraRecord& currentrecord, vector<int>& unique_ID_list, int& line);
nvraArray* construct_NvraArray();
void destroy_NvraArray(nvraArray* currentNvraArray);
nvraArray* doubleNvraArray(nvraArray* currentNvraArray);




int main()
{
    
    // Open the statistics file. If it's fail to open the file, then exit.
    std::ifstream file_in("NVRA_Statistics_July_2018_SEB_DFH.csv", ios::in);
    if (file_in.fail()) { std::cout << "File is not found in current directory!" << std::endl; return -1;}
    
    std::string each_record;
    int irow = 0;
    
    // allocate new space for 10 NVRA records.
    nvraArray* currentNvraArray = construct_NvraArray();
    std::vector<int> currentIDs;
    
    // Starting to read file
    while (file_in.good() && !file_in.eof())
    {
        
        getline(file_in, each_record);
        std::istringstream string_in(each_record);
        
        // Skip the first line and blank line at the end
        if (each_record != "\0" && irow != 0)
        {
            
            std::string var;
            vector<int> IDs;
            int temp_ID = 0, icol_int = 0, icol_str = 0, icol = 0;
            
            // Seperate parameters by commas
            while (getline(string_in, var, ','))
            {
                
                // Check if there's a duplicated ID for the first column
                // and give a error flag (-99999) to it.
                // Otherwise save records to the array.
                if (icol == 0)
                {
                    
                    temp_ID = std::stoi(var);
                    check_duplicate(var, currentIDs, currentNvraArray->data[irow - 1]);
                    ++icol_int;
                    
                }
                
                // Check if there's any invalid records (any integer parameters < 0) 
                // if it's not duplicated and give a error flag (-99998) to it.
                // Otherwise save records to the array.
                else if (icol != 3 && icol != 11 && icol != 12)
                {
                    
                    if (currentNvraArray->data[irow - 1].nums[0] >= 0)
                    {
                        
                        check_invalid(var, currentIDs, currentNvraArray->data[irow - 1]);
                        
                    }
                    
                    currentNvraArray->data[irow - 1].nums[icol_int] = std::stoi(var);
                    ++icol_int;
                    
                }
                
                // Storage 3 string columns.
                else
                {
                    
                    currentNvraArray->data[irow - 1].strings[icol_str] = var;
                    ++icol_str;
                    
                }
                
                ++icol;
                
            }
            
            currentIDs.push_back(currentNvraArray->data[irow - 1].nums[0]);
            currentNvraArray->data[irow - 1].nums[0] = temp_ID;
            
        }
        
        // reallocate space for array if it's not enough.
        if (irow == currentNvraArray->capacity)
        {
            
            nvraArray* doubledArray = doubleNvraArray(currentNvraArray);
            destroy_NvraArray(currentNvraArray);
            delete currentNvraArray;
            currentNvraArray = nullptr;
            currentNvraArray = doubledArray;
            doubledArray = nullptr;
            
        }
        
        ++irow;
        
    }
    
    // Output records or error messages in reverse order.
    --irow;
    for (int i = irow; i > 0 ; --i)
    {
        
        if (currentNvraArray->data[i - 1].strings[0] != "\0")
        {
            
            write_out(currentNvraArray->data[i - 1], currentIDs, i);
            
        }
        
    }
    
    // deallocate data space.
    destroy_NvraArray(currentNvraArray);
    delete currentNvraArray;
    currentNvraArray = nullptr;
    
}




// Function for constructing a new array with space to hold 10 records.
nvraArray* construct_NvraArray()
{
    nvraArray* currentNvraArray = new nvraArray;
    currentNvraArray->capacity = 10;
    currentNvraArray->data = new nvraRecord[currentNvraArray->capacity];
    return currentNvraArray;
}


// Function for destroying a new array with space to hold 10 records.
void destroy_NvraArray(nvraArray* currentNvraArray)
{
    delete [] currentNvraArray->data;
    currentNvraArray->data = nullptr;
}


// Function for doubling the size of the array.
// i.e., create a new array with doubled capacity, deallocate the memory 
//       where the old pointer pointing, divert the old pointer to the
//       new array.
nvraArray* doubleNvraArray(nvraArray* currentNvraArray)
{
    nvraArray* doubleArray = new nvraArray;
    doubleArray->capacity = 2 * currentNvraArray->capacity;
    doubleArray->data = new nvraRecord[doubleArray->capacity];
    for (int i = 0; i< currentNvraArray->capacity; ++i)
    {
        doubleArray->data[i] = currentNvraArray->data[i];
    }
    return doubleArray;
}


// Function for checking if a record is duplicated.
void check_duplicate(std::string ID, vector<int>& unique_ID_list, nvraRecord& currentrecord)
{
    // check whether the record is duplicated.
    if (std::find(unique_ID_list.begin(), unique_ID_list.end(), std::stoi(ID)) != unique_ID_list.end())
    {
        // If the record is duplicted, replace the ID with the error flag for duplication.
        currentrecord.nums[0] = -99999;
    }
    else
    {
        // If not, return the original ID to the record array.
        currentrecord.nums[0] = std::stoi(ID);
    }
}


// Function for checking if a record is invalid.
void check_invalid(std::string param, vector<int>& unique_ID_list, nvraRecord& currentrecord)
{
    if (std::stoi(param) < 0)
    {
        // If it is an invalid record, replace the ID with the error flag for invalid data.
        currentrecord.nums[0] = -99998;
    }
}


// Function for printing out records as well as error messages.
void write_out(nvraRecord& currentrecord, vector<int>& unique_ID_list, int& line)
{
    // If the record is not duplicated or invalid.
    if (unique_ID_list[line - 1] >= 0)
    {
        // output record as integers except the 4th 12th and 13th colomn which should be string.
        int icol_num = 0, icol_str = 0;
        for (int icol = 0; icol <= 23; ++icol)
        {
            if (icol == 23)
            {
                std::cout << currentrecord.nums[icol_num] << std::endl;
                ++icol_num;
            }
            else if (icol != 3 && icol != 11 && icol != 12)
            {
                std::cout << currentrecord.nums[icol_num] << ";" ;
                ++icol_num;
            }
            else
            {
                std::cout << currentrecord.strings[icol_str] << ";" ;
                ++icol_str;
            }
        }
    }
    // If the record is duplicated, output the error message for duplication.
    else if (unique_ID_list[line - 1] == -99999)
    {
        std::cout << "Warning: Duplicate record ID " << currentrecord.nums[0] << " at line " << line << "." << std::endl;
    }
    // If the record is invalid, output the error message for invalid record.
    else if (unique_ID_list[line - 1] == -99998)
    {
        std::cout << "Warning: Invalid data at line " << line << "." << std::endl;
    }
    // For those unknown error codes.
    else
    {
        std::cout << "The error code " << unique_ID_list[line - 1] << " is not recognized." << std::endl;
    }
}
