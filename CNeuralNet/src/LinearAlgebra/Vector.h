//
// Created by Parnet on 15.12.2018.
//

#ifndef CLINEARALGEBRA_VECTOR_H
#define CLINEARALGEBRA_VECTOR_H


#include <vector>
#include <sstream>

class Matrix;

class Vector {

public:
    /**
     * constructors and destructors
     */
    Vector();

    Vector(const Vector &v);

    Vector(std::initializer_list<double> l);

    Vector(std::string s);

    Vector(const char s[]);

    explicit Vector(size_t n, double val = 0.0);

    ~Vector();

    /**
     * access operations
     */
    virtual double &operator[](size_t i);

    virtual const double &operator[](size_t i) const;

    const double &operator()(size_t i) const;

    double &operator()(size_t i);


    /**
     * binary operations vector
     */
    Vector &operator+=(const Vector &v);

    Vector &operator-=(const Vector &v);

    Vector operator+(const Vector &v) const;

    Vector operator-(const Vector &v) const;

    double operator*(const Vector &v) const;

    bool operator==(const Vector &d) const;

    bool operator!=(const Vector &d) const;

    /**
     * binary operations Scalar
     */
    Vector &operator+=(double d);

    Vector operator+(double d) const;

    Vector &operator-=(double d);

    Vector operator-(double d) const;

    Vector &operator*=(double d);

    Vector operator*(double d) const;

    Vector &operator/=(double d);

    Vector operator/(double d) const;

    // todo Scalar + Vector
    // todo Scalar - Vector
    // todo Scalar * Vector
    // (todo Scalar / Vector)
    /**
     * service methods
     */
    std::size_t size() const;


    void resize(std::size_t sz, double val = 0.0);

    void clear();

    std::string to_string() const;

    static Vector from_string(std::string basic_string);

    friend double max_norm(const Vector &v);

    friend double euklidian_norm(const Vector &v);

    friend double p_norm(const Vector &v, double p);

    int max_index();


    /**
     * generators
     */
    Vector static random(size_t dim);

    Vector static random(size_t dim, double min, double max);



protected:
private:
    void init(size_t n, std::vector<double> &data);

    std::vector<double> data;
    std::size_t n;
};


double min(const Vector &v);

double max(const Vector &v);

double norm(const Vector &v);

Vector abs(const Vector &v);

double compensatedSum(const Vector &v);

Matrix outerProduct(const Vector &a, const Vector &b);

Vector componentProduct(const Vector &a, const Vector &b);

Vector componentDiv(const Vector &a, const Vector &b);

std::ostream &operator<<(std::ostream &stream, const Vector &v);

#include "Matrix.h"
#endif //CLINEARALGEBRA_VECTOR_H
