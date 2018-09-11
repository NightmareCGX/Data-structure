/*   
 *   Program  for reading, filtering and writing data
 *   Author:  Sijie Pan
 *   Date:    Sep. 3, 2018
 *   Version: 0.1
 *   Purpose:
 *   1. read and write data
 *   2. do minor processing of input
 *   4. proper coding style, documentation, and formatting
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Declaring sub-function before using.
int count_rows(std::ifstream &input, std::string &each_record);
void check_duplicate_invalid(vector<string> &record, vector<int> &unique_ID_list, int &line);
void write_out(vector<string> &record);

int main()
{
    // Open the statistics file. If it's fail to open the file, then exit.
	std::ifstream file_in("NVRA_Statistics_July_2018_SEB_DFH.csv", ios::in);
	if (file_in.fail()) { std::cout << "File is not found in current directory!" << std::endl; return -1;}
	
	std::string content_line;
    // Calculate number of records contained in the file.
    const int num_rows = count_rows(file_in, content_line);
    
    //count_rows(file_in, content_line, num_rows);
    vector<int> record_ID(num_rows - 1);
    // Obtain the record line by line
    for (int i = 0; i <= num_rows; ++i)
    {
    	getline(file_in, content_line);
    	istringstream string_in(content_line);
        // skip blank lines
        if (content_line != "\0")
        {
        	vector<string> vars;
        	string var;
            // Obtain the elements in a single line which are separated by commas.
        	while (getline(string_in, var, ','))
        	{
        		vars.push_back(var);
        	}
            // The first line (i = 0) is the header, skip to the next line.
            if (i != 0)
            {
                // Check if the record is a duplicated one or invalid one, and write
                // a corresponding error message.
                check_duplicate_invalid(vars, record_ID, i);
                // Create/Append an array to carry ID which should be positive numbers
                // except for duplicated data (ID: -99999) and invalid data (ID: -99998).
                record_ID[i - 1] = std::stoi(vars[0]);
                // Print out valid records.
                write_out(vars);
            }
        }
    }
    // Release memory (capacity) used by vectors
    vector<int>().swap(record_ID);
    return 0;
}

// Calculate total rows of the input
int count_rows(std::ifstream &input, std::string &each_record)
{
    int nrows = 0;
    // Count rows if there's an input and the EOF is not reached.
    while (input.good() && !input.eof())
    {
        getline(input, each_record);
        ++nrows;
    }
    --nrows;
    // Move back to the beginning of the file.
    input.clear();
    input.seekg(0, ios::beg);
    return nrows;
}

void check_duplicate_invalid(vector<string> &record, vector<int> &unique_ID_list, int &line)
{
    // check whether the record is duplicated or invalid.
    if (std::find(unique_ID_list.begin(), unique_ID_list.end(), std::stoi(record[0])) != unique_ID_list.end())
    {
        // If the record is duplicted, write out error message indicating duplicated data.
        std::cout << "Warning: Duplicate record ID " << record[0] << " at line " << line << std::endl;
        record[0] = "-99999";
    }
    else
    {
        // If it is not duplicated, check whether the record is valid (non-negative value).
        for (int i_col = 0; i_col <= 23; ++i_col)
        {
            // Colomn 4, 12, 13 are not a integer number.
            if (i_col != 3 && i_col != 11 && i_col != 12)
            {
                if (std::stoi(record[i_col]) < 0)
                {
                    // If it is an invalid record, write out error message indicating invalid data.
                    std::cout << "Warning: Invalid data at line " << line << std::endl;
                    record[0] = "-99998";
                    break;
                }
            }
        }
    }
}

void write_out(vector<string> &record)
{
    // output records if it is not duplicated or invalid.
    if (std::stoi(record[0]) >= 0)
    {
        // output record as integers except the 4th 12th and 13th colomn.
        for (int i_col = 0; i_col < 23; ++i_col)
        {
            if (i_col != 3 && i_col != 11 && i_col != 12)
            {
                std::cout << std::stoi(record[i_col]) << ";" ;
            }
            else
            {
                std::cout << record[i_col] << ";" ;
            }
        }
        std::cout << std::stoi(record[23]) << std::endl;
    }
}
