//
// Created by Parnet on 15.12.2018.
//

#ifndef LINEARALGEBRA_TESTOBJECT_H
#define LINEARALGEBRA_TESTOBJECT_H
#include <string>
#include <iostream>

class TestControl {
public:
    static const std::string __passed;
    static const std::string __failed;
    static size_t err;
};



class TestObject {
protected:
    std::string __name;
    size_t __err = 0;

public:
    explicit TestObject(const std::string& name = "");
    size_t __delete();

    //void __errormessage(std::string operation, size_t line);
    //void __verify(bool a);
    bool __compare(double a, double b);
    //void __compare(double a, double b, double tolerance);

    virtual void __all() = 0;

};
#define CONSTRUCTOR(a) explicit a(const std::string & name) : TestObject(name) {};
#define VERIFY(a) (if (a == 0) {std::cerr << __name <<" (" << TestControl::__failed << ": (" << #a <<") @ line " << __LINE__ << std::endl; __err++;})
#define COMPARE(a,b) (if (!__compare(a,b)) {std::cerr << __name <<" (" << TestControl::__failed << ": (" << #a <<") @ line " << __LINE__ << std::endl; __err++;})

#endif //LINEARALGEBRA_TESTOBJECT_H
