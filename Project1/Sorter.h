// Signature must be retained. Implementation must be added.
#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	// additional member functions (methods) and variables (fields) may be added
public:
    static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
	static void QuickSort(TemplatedArray<T>& array, const Comparator<T>& comparator, long long low, long long high);
    static void MergeSort(TemplatedArray<T>& array, TemplatedArray<T>& arraySorted, const Comparator<T>& comparator, long long low, long long high);
    static void swap(TemplatedArray<T>& array, long long i, long long j);
    static void merge(TemplatedArray<T>& array, TemplatedArray<T>& arraySorted, const Comparator<T>& comparator, long long i, long long j, long long m);
};

// implementation goes here

template <typename T> void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator) {
    unsigned long datasize = array.getSize() - 1;
    try {
        if (datasize >= 10000000) { QuickSort(array, comparator, 0, datasize); }
        else {
            TemplatedArray<NvraRecord> tempArray(array);
	    	Sorter<NvraRecord>::MergeSort(tempArray, array, comparator, 0, datasize);
	    	tempArray.~TemplatedArray();
        }
    }
    catch (ExceptionIndexOutOfRange& Err) {
        std::cout << Err.getMsg() << std::endl;
        throw Exception("Sorter<T>::sort");
    }
}


template <typename T> void Sorter<T>::QuickSort(TemplatedArray<T>& array, 
        const Comparator<T>& comparator, long long low, long long high) {
    if (low >= high) return;
    T pivot = array[low];
    long long i = low + 1, j = high;
    
    while (i < j) {
        while (i < j && comparator.compare(pivot, array[j]) == -1) { j--; }
        while (i < j && comparator.compare(pivot, array[i]) >= 0) { i++; }
        if (i < j) swap(array, i, j);
    }
    
    if (comparator.compare(array[low], array[j]) >= 0) {
        try { swap(array, low, j); }
        catch (ExceptionIndexOutOfRange& Err) {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("Sorter<T> QuickSort");
        }
    }
    else{ j--; }
    
    QuickSort(array, comparator, j + 1, high);
    QuickSort(array, comparator, low, j - 1); 
}


template <typename T> void Sorter<T>::MergeSort(TemplatedArray<T>& array, TemplatedArray<T>& arraySorted,
        const Comparator<T>& comparator, long long low, long long high) {
    if (low < high) {
        long long mid = (low + high) / 2;
        
        MergeSort(array, arraySorted, comparator, low, mid);
        MergeSort(array, arraySorted, comparator, mid + 1, high);
        
        try{
            merge(array, arraySorted, comparator, low, high, mid);
        
            for (unsigned long tempind = low; tempind <= high; ++tempind){
                array.replaceAt(&arraySorted[tempind], tempind);
            }
        }
        catch (ExceptionIndexOutOfRange& Err) {
            std::cout << Err.getMsg() << std::endl;
            throw Exception("Sorter<T>::MergeSort");
        }
    }
}


template <typename T> void Sorter<T>::swap(TemplatedArray<T>& array, long long i, long long j) {
    T temp = array[i];
    array.replaceAt(&array[j], i);
    array.replaceAt(&temp, j);
}


template <typename T> void Sorter<T>::merge(TemplatedArray<T>& array, TemplatedArray<T>& arraySorted,
        const Comparator<T>& comparator, long long s, long long e, long long m) {
    T tempRecord;
    long long i = s, j = m + 1, k = s;
    
    while (i < m + 1 && j < e + 1) {
        if (comparator.compare(array[i], array[j]) <= 0) {
            tempRecord = array[i];
            arraySorted.replaceAt(&tempRecord, k);
            i++;
        }
        else {
            tempRecord = array[j];
            arraySorted.replaceAt(&tempRecord, k);
            j++;
        }
        k++;
    }
    
    while (i < m + 1) {
        tempRecord = array[i];
        arraySorted.replaceAt(&tempRecord, k);
        i++;
        k++;
    }
    while (j < e + 1) {
        tempRecord = array[j];
        arraySorted.replaceAt(&tempRecord, k);
        j++;
        k++;
    } 
}

#endif
