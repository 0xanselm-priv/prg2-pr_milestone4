//
// Created by Parnet on 15.12.2018.
//

// todo aussagekräftige assertion texte
#ifndef CLINEARALGEBRA_MATRIX_H
#define CLINEARALGEBRA_MATRIX_H

#include <ostream>
#include <vector>
#include <cmath>
#include "Vector.h"


class Matrix  {

public:
    /**
     * constructors and destructors
     */
    Matrix();

    Matrix(const Matrix &m);

    Matrix(std::initializer_list<std::initializer_list<double>> l);

    Matrix(const char input[]);

    Matrix(std::string input);

    Matrix(std::size_t r, std::size_t c, double val = 0.0);

    /**
     * getter and setter
     */

    /**
     * access operations
     */
    const Vector &operator[](std::size_t r) const;

    Vector &operator[](std::size_t r);

    const double &operator()(std::size_t r, std::size_t c) const;

    double &operator()(std::size_t r, std::size_t c);

    /**
     * binary operations vector
     */
    Vector operator*(const Vector &v) const;

    Matrix &operator+=(const Matrix &m);


    Matrix operator+(const Matrix &m) const;

    Matrix& operator *=(const double scalar);

    Matrix operator *(const double scalar) const;


    // todo Matrix += Scalar
    // todo Matrix + Scalar
    // todo Matrix - Matrix
    Matrix operator-(const Matrix &m) const;
    Matrix& operator-=(const Matrix &m);
    // todo Matrix - Scalar
    // todo Matrix -= Matrix
    // todo Matrix -= Scalar


    // todo Scalar + Matrix
    // todo Scalar - Matrix

    // (todo Scalar / Matrix)


    // todo Matrix .+ Matrix
    // todo Matrix .- Matrix
    // todo Matrix .* Matrix
    // todo Matrix ./ Matrix

    //virtual Vector &operator*(const SparseVector &v) const;

    /**
     * binary operations scalar
     */

    /**
     * binary operations matrix
     */


    /**
     * service methods
     */
    void resize(std::size_t r, std::size_t c);

    void resize(std::size_t r, std::size_t c, double val);

    void clear();

    static Matrix from_string(std::string input);

    bool operator==(const Matrix &m) const;

    bool operator!=(const Matrix &m) const;

    /**
     * generators
     */
    static Matrix random(size_t rows, size_t columns, double minval, double maxval);

    void transpose();

    std::size_t num_cols() const;

    std::size_t num_rows() const;
private:
    std::vector<Vector> data;

    std::size_t columns;
    std::size_t rows;
};


Matrix operator*(const double &lhs, const Matrix &rhs);

Vector operator*(const Vector & lhs, const Matrix & rhs);

Matrix reshape(Vector a, size_t rows, size_t cols);

Matrix reshape(Vector a);

std::ostream &operator<<(std::ostream &stream, const Matrix &m);



#endif //CLINEARALGEBRA_MATRIX_H
