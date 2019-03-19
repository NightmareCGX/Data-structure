#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

// you may add additional functions here to support the operation of your code

// returns FIRST array location matching the given item (based on the comparator)
// if item is not found, returns (-(insertion point) - 1), where insertion point is where the item would be inserted
template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator) {
	// put implementation for binarySearch here
}

#endif