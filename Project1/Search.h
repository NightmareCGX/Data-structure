// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator) {
	// implementation goes here    
    long long lowbdy = 0;
    long long upbdy = array.getSize() - 1;
    long long ind = (lowbdy + upbdy) / 2;
    int foundValue = -1;
    T tempRecord;
    
    while (lowbdy <= upbdy) {
        tempRecord = array[ind];
        
        int result = comparator.compare(item, tempRecord);
        if (result < 0) { upbdy = ind - 1; }
        else if (result > 0) { lowbdy = ind + 1; }
        else { foundValue = 1; break; }
        
        ind = (lowbdy + upbdy) / 2;
    }
    if (foundValue == -1) { ind = lowbdy * (foundValue) - 1; }
    
    return ind;
}


template <typename T>
unsigned long search(const T& item, const TemplatedArray<T>& array, const int& searchCol, const int& sortedCol) {
    unsigned long count = 0;
    NvraComparator comparator(searchCol);
    if (searchCol == sortedCol) {
        long long firstind = binarySearch(item, array, comparator);
        if (firstind >= 0) {
            std::cout << array[firstind] << std::endl;
            count += 1;
            
            for (unsigned long ind = firstind - 1; ind < firstind; --ind) {
                if (comparator.compare(item, array[ind]) == 0) {
                    std::cout << array[ind] << std::endl;
                    count += 1;
                }
                else { break; }
            }
            
            for (unsigned long ind = firstind + 1; ind < array.getSize(); ++ind) {
                if (comparator.compare(item, array[ind]) == 0) {
                    std::cout << array[ind] << std::endl;
                    count += 1;
                }
                else { break; }
            }
        }
    }
    else {
        for (unsigned long ind = 0; ind < array.getSize(); ++ind) {
            if (comparator.compare(item, array[ind]) == 0) {
                std::cout << array[ind] << std::endl;
                count += 1;
            }
        }
    }
    
    return count;
}

#endif
