#include "NvraComparator.h"



// constructor
NvraComparator::NvraComparator(unsigned int column) { this->column = column; }


// Comparator returns -1 is item1 < item2, 0 if item1 == item2, +1 if item1 > item2
int NvraComparator::compare(const NvraRecord& item1, const NvraRecord& item2) const {
    int istatus = 0;
    unsigned int icol = 0;
    
    if (this->column == 3 || this->column == 11 || this->column == 12){
        if (this->column == 3) { icol = 0; }
        else if (this->column == 11) { icol = 1; }
        else { icol = 2; }
        
        if (item1.getString(icol) < item2.getString(icol)) { istatus = -1; }
        else if (item1.getString(icol) > item2.getString(icol)) { istatus = 1; }
    }
    else{
        if (this->column < 3) { icol = this->column; }
        else if (this->column < 11) { icol = this->column - 1; }
        else { icol = this->column - 3; }
        
        if (item1.getNum(icol) < item2.getNum(icol)) { istatus = -1; }
        else if (item1.getNum(icol) > item2.getNum(icol)) { istatus = 1; }
    }
    
    return istatus;
}