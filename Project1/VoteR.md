The #include statements, which include standard C++ library headers (iostream, fstream, sstream, string, cctype, algorithm)and user-defined headers (Exceptions.h, NvraRecord.h, NvraComparator.h, TemplatedArray.h, Search.h, Sorter.h). These headers likely define classes, functions, and exceptions used in the program.

When using namespace std; statement brings the std namespace into the current scope, allowing direct access to standard library functions and objects without specifying the std:: prefix.

```c++
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
    
    // start data manipulation loop
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
```
The main() function serves as the entry point of the program.

Several variables are declared to store data, such as $${the\ name\ of\ the\ current\ file\ being\ processed\ \color{lightgreen}file\\_name\,}$$ $${the\ total\ number\ of\ files\ processed\ \color{lightgreen}total\\_files,}$$
$${the\ total\ number\ of\ records\ read\ \color{lightgreen}total\\_read,}$$ $${the\ total\ number\ of\ valid\ records\ read\ \color{lightgreen}total\\_valid,}$$ 
$${and\ the\ total\ number\ of\ records\ stored\ \color{lightgreen}total\\_stored.}$$ 
$${A\ TemplatedArray\ that\ will\ store\ NvraRecord\ objects\ is\ \color{lightgreen}fullArray.}$$

The try-catch block wraps the data_input_loop function call. It attempts to read and process data using the data_input_loop function. If an exception is thrown, it is caught, and an error message is displayed along with the function name. The program returns -1 to indicate an error occurred.

A condition is checked to determine if file_name is empty and total_read is 0. If both conditions are true, the program exits with a return value of 0. It means that the program exit when the total number of records is 0 after all input files are read.

Another try-catch block wraps the data_manipulation_loop function call. It attempts to perform data manipulation operations using the data_manipulation_loop function.

The fullArray is deallocated by calling its destructor explicitly. But it is usually not recommended to call destructors explicitly. In normal cases, objects are destroyed automatically when they go out of scope or are deleted.\
Double deallocation/resources leaking: Both the explicit destructor call and the automatic memory management will try to deallocate the same resource, leading to undefined behavior (related to heap and stack deallocation). \
Incomplete destruction order: In complex programs with multiple objects and dependencies, explicitly defining destructors can make it challenging to manage the destruction order correctly. The automatic destruction provided by the language ensures a proper and predictable order of object destruction. \
Better change to delete fullArray.

```c++
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
```
It prompts the user to enter the data file name. The entered file name is stored in the file_name variable. If the entered file name is empty, the loop is terminated.

If the file name is not empty, the function attempts to open the file. If the file opening fails, an error message is displayed, and the loop continues to prompt the user again. \
If the file opening succeeds, an object named uniqueID is created to store unique record IDs. The single_file_read_in function is called to read records from an individual file and process the records contained by the file.
Not necessary to explicitly call destructor.

```c++
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
```
Several variables are declared: option (stores the user's chosen option), col (stores the user's chosen column for sorting or searching), and sortedCol (stores the column number for sorting, initialized to -1).

It prompts the user to enter an option. The entered option is stored in the option variable. If the length of the entered option is not 1 or if it is a numeric character, the loop continues to prompt the user again.

The transformed option is checked using if statements to determine the user's choice: \
If the option is "q", the program breaks the loop then quit. \
If the option is "o", an inner loop is started to handle the output operation: The user is prompted to enter the output file name. The record_dump function is then called to save the records. \
If the option is "s", the user is prompted to enter the sort field (column). A Comparator object is created. The `Sorter<NvraRecord>::sort` function is called to sort the fullArray using the created comparator. \
If the option is "f", the user is prompted to enter the search field. The object_find function is called to perform the search operation.

```c++
void single_file_read_in(unsigned long& total_read, unsigned long& total_valid,
    unsigned long& total_stored, TemplatedArray<unsigned long>& uniqueID,
    TemplatedArray<NvraRecord>& fullArray, std::ifstream& file_in)
{
    // Function to read records from a single file

    std::string each_record;
    unsigned long irow = 0, single_read = 0;

    // Starting to read the file line by line
    while (file_in.good() && !file_in.eof())
    {
        int validFlag = 0;
        NvraRecord currentRecord;
        getline(file_in, each_record);
        std::istringstream string_in(each_record);

        // Skip the first line and blank lines at the end
        if (!each_record.empty() && irow != 0)
        {
            std::string var;
            unsigned long icol = 0;

            // Separate parameters by commas
            while (getline(string_in, var, ','))
            {
                // Construct the current record
                record_fill(currentRecord, var, icol, validFlag);
                ++icol;
            }

            // Save valid/latest updated records to the array
            try
            {
                update_record(total_read, total_valid, total_stored, single_read,
                    uniqueID, currentRecord, fullArray, validFlag);
            }
            catch (Exception& Err)
            {
                std::cout << Err.getMsg() << std::endl;
                throw Exception("single_file_read_in");
            }
        }

        ++irow;
    }
}
```
The function enters a loop that reads the file line by line.\
If the line is not empty and it's not the first line of the file, a NvraRecord object is created to store the data for the current record.\
The line is split into individual parameters, then the record_fill function is called to populate the currentRecord object with the parsed values from the line.\
Finally, the update_record function is called to store the record in the fullArray.\
The loop continues until all lines of the file have been read.

```c++
void update_record(unsigned long& total_read, unsigned long& total_valid, unsigned long& total_stored,
    unsigned long& single_read, TemplatedArray<unsigned long>& uniqueID,
    const NvraRecord& record, TemplatedArray<NvraRecord>& fullArray, const int& flag)
{
    // Function to update a record in the array

    if (flag == 1)
    {
        std::cout << "Warning: Invalid data at line " << single_read + 1 << std::endl;
    }
    else if (check_duplication_within_file(fullArray, record, single_read, uniqueID))
    {
        std::cout << "Warning: Duplicate record ID " << record.getNum(0) << " at line "
            << single_read + 1 << "." << std::endl;
        total_valid += 1;
    }
    else
    {
        try
        {
            unsigned long temp = record.getNum(0);
            uniqueID.add(&temp);
            save_to_full_Array(total_stored, record, fullArray);
            total_valid += 1;
        }
        catch (Exception& Err)
        {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("update_record");
        }
    }

    single_read += 1;
    total_read += 1;
}
```
The function begins with a series of checks and updates based on the flag value.\
If the flag is 1, it indicates that the record is invalid. In this case, a warning message is displayed to the user.\
If the record is not flagged as invalid, the function checks for duplication. If a duplication is found, a warning message is displayed to the user, but it is also counted for a valid record but is not stored.\
If the record is valid and not a duplicate, it is added to the fullArray.\

```c++
void save_to_full_Array(unsigned long& total_stored, const NvraRecord& record4save,
    TemplatedArray<NvraRecord>& fullArray)
{
    // Function to save a record to the fullArray

    if (total_stored == 0)
    {
        try
        {
            fullArray.add(&record4save);
            total_stored += 1;
        }
        catch (ExceptionMemoryNotAvailable& Err)
        {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("save_to_full_Array");
        }
    }
    else
    {
        NvraComparator comparator(0);
        long long dup_ind = binarySearch(record4save, fullArray, comparator);

        if (dup_ind < 0)
        {
            unsigned long insert_ind = (dup_ind + 1) * -1;

            if (insert_ind == fullArray.getSize())
            {
                try
                {
                    fullArray.add(&record4save);
                }
                catch (ExceptionMemoryNotAvailable& Err)
                {
                    std::cout << Err.getMsg() << std::endl;
                    throw Exception("save_to_full_Array");
                }
            }
            else
            {
                try
                {
                    fullArray.addAt(&record4save, insert_ind);
                }
                catch (ExceptionIndexOutOfRange& Err)
                {
                    std::cout << Err.getMsg() << std::endl;
                    throw Exception("save_to_full_Array");
                }
                catch (ExceptionMemoryNotAvailable& Err)
                {
                    std::cout << Err.getMsg() << std::endl;
                    throw Exception("save_to_full_Array");
                }
            }

            total_stored += 1;
        }
        else
        {
            unsigned long replace_ind = dup_ind;

            try
            {
                fullArray.replaceAt(&record4save, replace_ind);
            }
            catch (ExceptionIndexOutOfRange& Err)
            {
                std::cout << Err.getMsg() << std::endl;
                throw Exception("save_to_full_Array");
            }
        }
    }
}
```
The function begins by checking if the total_stored count is 0. If it is, it means that the array is empty, and the record can be added directly using the fullArray.add function.
If there are existing records in the fullArray. The function performs a binary search to find the appropriate position to insert the record while maintaining the sorted order.
If the search result shows that the record is not a duplicate, then the record would be inserted at the calculated index.
Otherwise, the new record should replace the existing record.

```c++
void record_fill(NvraRecord& record, const string& var, const int icol, int& validFlag)
{
    // Function to fill a record with data

    if (icol != 3 && icol != 11 && icol != 12)
    {
        if (std::stoi(var) < 0)
        {
            validFlag = 1;
            return;
        }
        record.addNum(std::stoi(var));
    }
    else
    {
        record.addString(var);
    }
}
```
The function checks the column index to determine the type of data to add to the record.
If the icol is not 3, 11, or 12, it means the data is numeric. Before adding the numeric data, the function checks if the value is negative. If it is, the record would be flagged as an invalid record.
If the icol is 3, 11, or 12, it means the data is a string. The function directly adds the var to the record.

```c++
int record_dump(const std::string& file_name, const unsigned long& total_read, const unsigned long& total_valid,
    const unsigned long& total_stored, const TemplatedArray<NvraRecord>& fullArray)
{
    // Function to dump records to a file or console

    if (file_name.empty())
    {
        for (unsigned long i = 0; i < fullArray.getSize(); ++i)
        {
            std::cout << fullArray.get(i) << std::endl;
        }
        std::cout << "Data lines read: " << total_read << "; Valid NVRA records read: "
            << total_valid << "; NVRA records in memory: " << total_stored << std::endl;
    }
    else
    {
        std::ofstream file_out(file_name, ios::out);
        if (file_out.fail())
        {
            std::cout << "File is not available." << std::endl;
            return -1;
        }
        else
        {
            for (unsigned long i = 0; i < fullArray.getSize(); ++i)
            {
                file_out << fullArray.get(i) << std::endl;
            }
            file_out << "Data lines read: " << total_read << "; Valid NVRA records read: "
                << total_valid << "; NVRA records in memory: " << total_stored << std::endl;
            file_out.close();
        }
    }

    return 0;
}
```
The function first checks if the name of the output file is empty. If it is, it means that the records should be displayed on the console. The function iterates over the fullArray and prints each record.\
If the file_name is not empty, it means that the records should be written to a file. the function tries to iterate over the fullArray and to write each record to the file.

```c++
void object_find(const std::string& col, const int& sortedCol, const TemplatedArray<NvraRecord>& fullArray)
{
    // Function to find objects based on a specified field

    if (!is_number(col))
    {
        return;
    }
    if (stoi(col) > 23 || stoi(col) < 0)
    {
        return;
    }

    unsigned long count = 0;
    std::string inText;
    NvraRecord searchItem;

    if (stoi(col) == 3 || stoi(col) == 11 || stoi(col) == 12)
    {
        std::cout << "Enter exact text on which to search: ";
        std::getline(std::cin, inText);
        if (inText.empty())
        {
            return;
        }

        if (stoi(col) == 3)
            searchItem.setString(inText, 0);
        if (stoi(col) == 11)
            searchItem.setString(inText, 1);
        if (stoi(col) == 12)
            searchItem.setString(inText, 2);

        count = search(searchItem, fullArray, stoi(col), sortedCol);
    }
    else
    {
        std::cout << "Enter non-negative field value: ";
        std::getline(std::cin, inText);
        if (inText.empty() || !is_number(inText))
        {
            return;
        }
        if (stoi(inText) < 0)
        {
            return;
        }

        if (stoi(col) < 3)
        {
            searchItem.setNum(stoi(inText), stoi(col));
        }
        else if (stoi(col) < 11)
        {
            searchItem.setNum(stoi(inText), stoi(col) - 1);
        }
        else
        {
            searchItem.setNum(stoi(inText), stoi(col) - 3);
        }

        count = search(searchItem, fullArray, stoi(col), sortedCol);
    }

    std::cout << "NVRA records found: " << count << std::endl;
}
```
If the col corresponds to a string field (3, 11, or 12), the function prompts the user to enter the exact text to search for.\
If the entered text is empty, the function returns without performing any search. \
If the col corresponds to a numeric field, the function prompts the user to enter a non-negative field value to search for.\
If the entered value is empty, not a number or negative, the function returns without performing any search. \
The function creates a searchItem of type NvraRecord and sets the appropriate string or numeric field based on the col value. \
And finally prints the search result.

```c++
bool check_duplication_within_file(const TemplatedArray<NvraRecord>& array, const NvraRecord& currentrecord,
                const unsigned long& single_read, const TemplatedArray<unsigned long>& uniqueID)
{
    // Function to check if a record is duplicated within a file

    unsigned long num_records = uniqueID.getSize();
    if (single_read == 0 || num_records == 0)
    {
        return false;
    }

    for (unsigned long i = 0; i < num_records; ++i)
    {
        try
        {
            // Check whether the record is duplicated. If the record is duplicated, return true.
            NvraComparator comparator(0);
            if (comparator.compare(currentrecord, array.get(i)) == 0)
            {
                return true;
            }
        }
        catch (Exception& Err)
        {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("check_duplication_within_file");
        }
    }

    // Otherwise, return false
    return false;
}
```
The function first checks if it's the first record being read or if there are no records stored in the uniqueID array. \
If either condition is true, it means there are no previous records to compare against, so the function returns false indicating that there is no duplication. \
If there are previous records, the function iterates over the uniqueID  to compare each record with the current record.\
The function uses the NvraComparator to compare the current record with each record record in the array.\
If the comparator indicates that the records are equal, it means the current record is a duplicate, and the function returns true.

```c++
bool is_number(const std::string& inStr)
{
    // Function to check if a string represents a number

    std::string::const_iterator iter = inStr.begin();
    while (iter != inStr.end() && std::isdigit(*iter))
    {
        ++iter;
    }
    return !inStr.empty() && iter == inStr.end();
}
```
The function iterates over each character in the string. If The function reaching the end of the string or finding a non-digit character, the iteration ends.\
After the iteration, the function returns true if the input string is not empty and the iterator has reached the end of the string. This indicates that all characters in the string are digits.
