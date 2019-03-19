#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
public:
	static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
};

// Implementation goes here

#endif