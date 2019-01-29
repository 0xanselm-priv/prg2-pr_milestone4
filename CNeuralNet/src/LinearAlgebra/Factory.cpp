//
// Created by Parnet on 16.12.2018.
//

#include "Factory.h"
#include <chrono>

std::default_random_engine Factory::engine = std::default_random_engine(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
//std::default_random_engine Factory::engine = std::default_random_engine(0L);

std::default_random_engine &Factory::random() {
        return Factory::engine;
}

