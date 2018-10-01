/* cppprimer.cpp
 *
 * Created on: 2013.11.7
 *
 * Author: Caroline
 */ 

#include <iostream>
#include <cstddef>
#include <utility>

class HighHeel {
	
	friend std::ostream &operator<< (std::ostream& os, const HighHeel& hh);
	friend std::istream &operator>> (std::istream& is, HighHeel& hh);
	friend HighHeel operator+ (const HighHeel& lhs, const HighHeel& rhs);
	
	public:	
		    int heel() const{
				return (wedgeHeel + kittenHeel + stilettoHeel);	
            }
            int boot() const{
                return (kinkyBoot + thighHighBoot);
            }
    private:	
            int wedgeHeel = 2; 
            int kittenHeel = 2; 
            int stilettoHeel = 2; 
            int kinkyBoot = 5; 
            int thighHighBoot = 5; 
};

std::ostream &operator<< (std::ostream& os, const HighHeel& hh){
    os << "Heels: " << hh.heel() << " "
            << "Boots: " << hh.boot();
    return os;
}
std::istream &operator>> (std::istream& is, HighHeel& hh){
    is >> hh.wedgeHeel >> hh.kittenHeel >> hh.stilettoHeel
            >> hh.kinkyBoot >> hh.thighHighBoot;
    if ( (is.rdstate() & is.failbit) != 0) //检查错误位
        std::cerr << "Error to input!" << std::endl;
    if (!is)
        hh = HighHeel();
    return is;
}

HighHeel operator+ (const HighHeel& lhs, const HighHeel& rhs){
        HighHeel sum = lhs;
        sum.wedgeHeel += rhs.wedgeHeel;
        sum.kittenHeel += rhs.kittenHeel;
        sum.stilettoHeel += rhs.stilettoHeel;
        sum.kinkyBoot += rhs.kinkyBoot;
        sum.thighHighBoot += rhs.thighHighBoot;
        return sum;
}

int main (void) {
    HighHeel hh;
    std::cout << hh << std::endl;
    std::cout << "Please input heels quantity (5 numbers) :" << std::endl;
    std::cin >> hh;
    std::cout << hh << std::endl;
    HighHeel hh2;
    std::cout << hh + hh2 << std::endl;
    return 0;
}