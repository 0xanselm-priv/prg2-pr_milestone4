//
// Created by Parnet on 15.12.2018.
//
#include "main.h"
#include "TestObject.h"


#define EVALUATE(clsname) {clsname var = clsname(#clsname); var.__all(); var.__delete();}

namespace test {
    int main(int argc, char *argv[]) {

        if (TestControl::err > 0) {
            exit(171);
        } else {
            exit(100);
        }
    };

    int main() {
        return main(0, nullptr);
    }
}
