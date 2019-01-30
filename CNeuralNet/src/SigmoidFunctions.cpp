//
// Created by Parnet on 19.12.2018.
//
#include <iostream>
#include "SigmoidFunctions.h"
#include <cmath>

using namespace std;

double logistic(double x){
    return 1.0 / (1.0+exp(-x));
}

double logistic_derived(double x){
    double f = exp(-x);
    return f / (pow((1+f),2));
}

double sigmoid_algebraic(double x){
    return x / sqrt(1.0+x*x);
}

double sigmoid_algebraic_derived(double x){
    double sqx = x*x;
    return 1.0/sqrt(sqx+1) - sqx/pow((sqx+1),(3.0/2.0));
}
