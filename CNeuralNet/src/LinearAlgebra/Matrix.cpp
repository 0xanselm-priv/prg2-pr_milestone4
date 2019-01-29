#include <utility>

//
// Created by Parnet on 15.12.2018.
//

#include <cassert>
#include "Matrix.h"
/**
 * constructors and destructors
 */
Matrix::Matrix() {
    this->columns = 0;
    this->rows = 0;
    this->data = std::vector<Vector>();
}

Matrix::Matrix(const char s[]) {
    Matrix m = Matrix::from_string(std::string(s));
    this->rows = m.rows;
    this->columns = m.columns;
    this->data = std::move(m.data);
}

Matrix::Matrix(std::string s) {
    Matrix m = Matrix::from_string(std::move(s));
    this->rows = m.rows;
    this->columns = m.columns;
    this->data = std::move(m.data);
}
#include <iostream>
Matrix::Matrix(std::initializer_list<std::initializer_list<double>> l){
    this->rows = l.size();
    this->columns = 0;
    data = std::vector<Vector>();
    for(const auto &x :l){
        if(this->columns == 0){
            this->columns = x.size();
        } else {
            assert(this->columns == x.size() && "Different column size in creation of Matrix from initializer list");
        }
        this->data.emplace_back(Vector(x));
    }
}

Matrix::Matrix(std::size_t r, std::size_t c, double val) {
    this->rows = r;
    this->columns = c;
    this->data = std::vector<Vector>(r);
    for (size_t i = 0; i < r; ++i) {
        this->data[i] = Vector(c, val);
    }
}


/**
 * getter and setter
 */



/**
 * access operations
 */
const double &Matrix::operator()(std::size_t r, std::size_t c) const {
    return this->data.at(r - 1).operator()(c);
}


double &Matrix::operator()(std::size_t r, std::size_t c) {
    return this->data.at(r - 1).operator()(c);
}


const Vector &Matrix::operator[](size_t r) const {
    return this->data[r];
}

Vector &Matrix::operator[](std::size_t r) {
    return this->data[r];
}


/**
 * binary operations vector
 */
Vector Matrix::operator*(const Vector &v) const {
    Vector C = Vector(this->rows);
    assert(v.size() == this->columns && "Dimension missmatch in operator Matrix * Vector ");
    for (size_t k = 0; k < this->rows; ++k) {
        double sum = 0.0;
        for (size_t j = 0; j < this->columns; ++j) {
            sum += v[j] * this->data[k][j];
        }
        C[k] = sum;
    }
    return C;
}

Matrix &Matrix::operator+=(const Matrix &m){
    assert( this->rows == m.rows && "Dimensionmissmatch in operator Matrix += Matrix (rows)");
    assert( this->columns == m.columns && "Dimensionmissmatch in operator Matrix += Matrix (columns)");

    for(size_t i =0; i <this->rows; ++i){
        for(size_t j = 0; j < this->columns; ++j){
            this->data[i][j] += m.data[i][j];
        }
    }
    return *this;
}

Matrix &Matrix::operator*=(const double scalar) {
    for(size_t j = 0; j < this->rows; j++){
        this->data[j] *= scalar;
    }
    return *this;
}

Matrix Matrix::operator*(const double scalar) const {
    Matrix res = *this;
    res *= scalar;
    return res;
}

Matrix Matrix::operator+(const Matrix &m) const {
    Matrix res = *this;
    res += m;
    return res;
}

/**
 * binary operations scalar
 */

/**
 * binary operations matrix
 */


/**
 * service methods
 */
Matrix Matrix::from_string(std::string input) {
    size_t n = input.length();
    Matrix m = Matrix();
    std::vector<Vector> vc = std::vector<Vector>();
    if (input[0] == '[' && input[n - 1] == ']') {
        size_t i = 1;
        size_t vpos;

        vpos = input.find(';', i);
        while (vpos != std::string::npos) {
            Vector v = Vector::from_string(input.substr(i, vpos - i));
            vc.push_back(v);
            i = vpos + 1;
            vpos = input.find(';', i);
        }
        Vector ve = Vector::from_string(input.substr(i, n - 1 - i));
        vc.push_back(ve);

    }
    m.rows = vc.size();
    if (m.rows > 0) {
        m.columns = vc[0].size();
    }
    m.data = vc;
    return m;
}


void Matrix::resize(std::size_t r, std::size_t c) {
    resize(r, c, 0.0);
}


void Matrix::resize(std::size_t r, std::size_t c, double val) {
    this->data.resize(r);
    for (size_t i = 0; i < r; ++i) {
        this->data.at(i).resize(c, val);
    }
    this->columns = c;
    this->rows = r;

}


void Matrix::clear() {
    for (size_t i = 0; i < this->columns; ++i) {
        this->data[i].clear();
    }
    this->data.clear();

    this->rows = 0;
    this->columns = 0;

}


/**
 * generators
 */
Matrix Matrix::random(size_t rows, size_t columns, double minval, double maxval) {
    Matrix m = Matrix(rows, columns);
    for (size_t i = 0; i < rows; ++i) {
        m.data[i] = Vector::random(columns, minval, maxval);
    }
    return m;
}


std::ostream &operator<<(std::ostream &stream, const Matrix &m) {
    const std::size_t nr = m.num_rows();
    const std::size_t nc = m.num_cols();
    stream << std::endl << "[";
    for (std::size_t i = 1; i <= nr; ++i) {
        if (!nc) stream << "[]" << std::endl;
        else {
            stream << "[";
            for (std::size_t j = 1; j < nc; ++j)
                stream << m(i, j) << ", ";
            if (i != nr) {
                stream << m(i, nc) << "];" << std::endl;
            } else {
                stream << m(i, nc) << "]";
            }
        }
    }
    return stream << "]";
}

Matrix reshape(Vector a, size_t rows, size_t cols) {
    Matrix M = Matrix(rows, cols);
    for (size_t i = 0; i < a.size(); i++) {
        M(i / rows + 1, i % rows + 1) = a[i];
    }
    return M;
}


void Matrix::transpose() {
    /*if(this->rows == this->columns){
        for(size_t i = 0; i < this->rows; ++i){
            for(size_t j = i; j < this->columns; ++j){
                double tmp = this->data[i][j];
                this->data[i][j] = this->data[j][i];
                this->data[j][i] = tmp;
            }
        }
    }*/
    auto newdata = std::vector<Vector>(this->columns);
    for (size_t i = 0; i < this->columns; ++i) {
        newdata[i] = Vector(this->rows);
    }

    for (size_t i = 0; i < this->rows; ++i) {
        for (size_t j = 0; j < this->columns; ++j) {
            newdata[j][i] = this->data[i][j];
        }
    }

    this->data = newdata;
    auto tmp = this->rows;
    this->rows = this->columns;
    this->columns = tmp;

}

/*Matrix::Matrix(const AMatrix &m) {
    this->rows = m.num_rows();
    this->columns = m.num_cols();
    this->data = std::vector<Vector>(this->rows);
    for (size_t i = 0; i < this->rows; ++i) {
        this->data[i] = Vector(this->columns);
    }
    for (size_t i = 1; i <= this->rows; i++) {
        for (size_t j = 1; j <= this->columns; j++) {
            this->operator()(i, j) = m(i, j);
        }
    }
}*/

size_t Matrix::num_cols() const {
    return this->columns;
}

size_t Matrix::num_rows() const{
    return this->rows;
}

Matrix::Matrix(const Matrix &m) {
    this->rows = m.rows;
    this->columns = m.columns;
    this->data = std::vector<Vector>(rows);
    for (size_t i = 0; i < rows; ++i) {
        this->data[i] = Vector(columns);
        for(size_t j = 0; j < columns; ++j){
            this->data[i][j] = m.data[i][j];
        }
    }
}

bool Matrix::operator==(const Matrix &m) const {
    if(this->columns != m.columns || this->rows != m.rows){
        return false;
    }

    for(size_t i = 0; i < this->rows; ++i){
        if(this->data[i] != m.data[i]){
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &m) const {
    return !(*this == m);
}

Matrix Matrix::operator-(const Matrix &m) const {
    Matrix res = *this;
    res -= m;
    return res;
}

Matrix &Matrix::operator-=(const Matrix &m) {
    assert( this->rows == m.rows && "Dimensionmissmatch in operator Matrix -= Matrix (rows)");
    assert( this->columns == m.columns && "Dimensionmissmatch in operator Matrix -= Matrix (columns)");

    for(size_t i =0; i <this->rows; ++i){
        for(size_t j = 0; j < this->columns; ++j){
            this->data[i][j] -= m.data[i][j];
        }
    }
    return *this;
}

Vector operator*(const Vector & lhs, const Matrix & rhs){
    size_t rows = rhs.num_rows();
    assert(lhs.size() == rows && "Dimension missmatch in operator Vector * Matrix");
    const size_t cols = rhs.num_cols();
    Vector result = Vector(cols);
    for(size_t i = 0; i < cols; ++i){
        double sum = 0.0;
        for(size_t j = 0; j <  rows; j++){
            sum += lhs[j]*rhs(j+1,i+1);
        }
        result[i] = sum;
    }
    return result;
};

Matrix reshape(Vector a) {
    size_t n = a.size();
    auto rows = (size_t) sqrt(n);
    auto cols = (size_t) sqrt(n);
    Matrix M = Matrix(rows, cols);
    for (size_t i = 0; i < a.size(); i++) {
        M(i / rows + 1, i % rows + 1) = a[i];
    }
    return M;
}

Matrix operator*(const double &lhs, const Matrix &rhs){
    return rhs * lhs;
}