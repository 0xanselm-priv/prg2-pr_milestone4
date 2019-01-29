//
// Created by Parnet on 15.12.2018.
//

#include "TestObject.h"
#include <cmath>

size_t TestControl::err = 0;
const std::string TestControl::__passed = "PASSED";
const std::string TestControl::__failed = "FAILED";

TestObject::TestObject(const std::string & name) {
    this->__name = name;
}

size_t TestObject::__delete() {
    if(__err == 0){
        std::cout << __name <<": "<< TestControl::__passed << std::endl;
    } else {
        std::cerr << __name <<": "<< TestControl::__failed <<" with "<<__err<<" Errors"<<std::endl;
    }
    TestControl::err += __err;
    return __err;
}

bool TestObject::__compare(double a, double b) {
    const double eps = 1e-15;
    return (fabs(a - b) <= eps);
}

