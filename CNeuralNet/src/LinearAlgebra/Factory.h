//
// Created by Parnet on 16.12.2018.
//

#ifndef LINEARALGEBRA_FACTORY_H
#define LINEARALGEBRA_FACTORY_H

#include <random>

class Factory {
    static std::default_random_engine engine;
public:
    static std::default_random_engine & random();
};


#endif //LINEARALGEBRA_FACTORY_H
