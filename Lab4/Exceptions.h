#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include<iostream>
#include<string>

using namespace std;

class Exception {
    
private:
    string ErrMsg;
    
public:
    Exception(const string& msg_in){
        ErrMsg = msg_in;
    }
    
    ~Exception(){}
    
};


class ExceptionIndexOutOfRange : Exception {
    
private:
    string ErrMsg;
    
public:
    ExceptionIndexOutOfRange(const string& msg_in, unsigned long ind, unsigned long bound) : Exception(msg_in){
        ErrMsg = "Out of range index occurs in function " + msg_in + ".\nIndex is " + to_string(ind) + ".\nBoundary is " + to_string(bound) + ".";
    }
    
    string getMsg() const {
        return ErrMsg.c_str();
    }
    
};

#endif
