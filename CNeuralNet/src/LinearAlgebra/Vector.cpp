//
// Created by Parnet on 15.12.2018.
//

#include <cmath>
#include <ostream>
#include <cassert>
#include <random>

#include "Vector.h"
#include "Factory.h"

Vector::Vector() {
    this->n = 0;
    this->data = std::vector<double>(0);
}

Vector::Vector(const char s[]) {
    Vector v = Vector::from_string(s);
    this->n = v.n;
    this->data = std::move(v.data);
}

Vector::Vector(std::string s) {
    Vector v = Vector::from_string(std::move(s));
    this->n = v.n;
    this->data = std::move(v.data);
}

Vector::Vector(const Vector &v) {
    this->n = v.n;
    this->data = v.data;
}


Vector::Vector(std::size_t len, double val) {
    this->n = len;
    this->data = std::vector<double>(len, val);

}

Vector::~Vector() = default;

void Vector::resize(std::size_t sz, double val) {
    this->data.resize(sz, val);
    this->n = sz;
}

void Vector::clear() {
    this->n = 0;
    this->data.clear();
}

double &Vector::operator[](std::size_t ind) {
    return this->data[ind];
}

const double &Vector::operator[](std::size_t ind) const {
    return this->data[ind];
}

const double &Vector::operator()(std::size_t ind) const {
    return this->data[ind - 1];
}

double &Vector::operator()(std::size_t ind) {
    return this->data[ind - 1];
}


Vector &Vector::operator+=(const Vector &v) {
//#pragma omp parallel num_threads(8)
    for (size_t i = 0; i < n; ++i) {
        this->data[i] += v[i];
    }
    return *this;
}

Vector &Vector::operator-=(const Vector &v) {
    for (size_t i = 0; i < n; ++i) {
        this->data[i] -= v[i];
    }
    return *this;
}

Vector Vector::operator+(const Vector &v) const {
    Vector u = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        u[i] = this->data[i] + v[i];
    }
    return u;
}

Vector Vector::operator-(const Vector &v) const {
    Vector u = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        u[i] = this->data[i] - v[i];
    }
    return u;
}

double Vector::operator*(const Vector &v) const {
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sum += this->data[i] * v[i];
    }
    return sum;
}

Vector &Vector::operator+=(double d) {
    for (size_t i = 0; i < n; ++i) {
        this->data[i] += d;
    }
    return *this;
}

Vector Vector::operator+(double d) const {
    Vector u = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        u[i] = this->data[i] + d;
    }
    return u;
}

Vector &Vector::operator-=(double d) {
    for (size_t i = 0; i < n; ++i) {
        this->data[i] -= d;
    }
    return *this;
}

Vector Vector::operator-(double d) const {
    Vector u = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        u[i] = this->data[i] - d;
    }
    return u;
}

Vector &Vector::operator*=(double d) {
    for (size_t i = 0; i < n; ++i) {
        this->data[i] *= d;
    }
    return *this;
}

Vector Vector::operator*(double d) const {
    Vector u = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        u[i] = this->data[i] * d;
    }
    return u;
}

Vector &Vector::operator/=(double d) {
    for (size_t i = 0; i < n; ++i) {
        this->data[i] /= d;
    }
    return *this;
}

Vector Vector::operator/(double d) const {
    Vector u = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        u[i] = this->data[i] / d;
    }
    return u;
}

Vector Vector::random(size_t dim) {
    return Vector::random(dim, -1, 1);
}


Vector Vector::random(size_t dim, double min, double max) {
    auto &e1 = Factory::random();
    std::uniform_real_distribution<double> uniform_dist(min, max);
    Vector v = Vector(dim);
    for (size_t i = 0; i < dim; ++i) {
        v[i] = uniform_dist(e1);
    }
    return v;
}


Vector Vector::from_string(std::string input) {
    size_t n = input.length();
    Vector v = Vector();
    std::string content = input.substr(1, n - 2);
    std::vector<double> data = std::vector<double>();
    if (input[0] == '[' && input[n - 1] == ']') {
        std::stringstream ss(content);
        double value;
        while (ss >> value) {
            data.push_back(value);
            while (ss.peek() == ',' || ss.peek() == ' ' || ss.peek() == '\n') {
                ss.ignore();
            }
        }
        v.data = data;
        v.n = data.size();
    }
    return v;
}

bool Vector::operator==(const Vector &v) const {
    if (v.size() != this->n) {
        return false;
    }
    const double eps = 1e-15;

    for (size_t i = 0; i < this->n; ++i) {
        if (fabs(this->data[i] - v[i]) > eps) {
            return false;
        }
    }
    return true;
}

bool Vector::operator!=(const Vector &d) const {
    return !(*this == d);
}

std::string Vector::to_string() const {
    std::stringstream stream;
    if (!this->size()) {
        stream << "[]";
        return stream.str();
    }
    std::size_t sz = this->size() - 1;
    stream << "[";
    for (std::size_t i = 0; i < sz; ++i) {
        stream << this->operator[](i) << ", ";
    }
    stream << this->operator[](sz) << "]";

    return stream.str();
}

void Vector::init(size_t n, std::vector<double> &data) {
    this->n = n;
    this->data = data;

}

double max_norm(const Vector &a) {
    size_t n = a.n;
    if (n == 0) { return 0.0; };
    double max = fabs(a.data[0]);
    for (size_t i = 1; i < n; ++i) {
        if (fabs(a.data[i]) > max) {
            max = fabs(a.data[i]);
        }
    }
    return max;
}

double norm(const Vector &a) {
    return max_norm(a);
}

size_t Vector::size() const {
    return this->n;
}

double euklidian_norm(const Vector &a) {
    double sum = 0.0;
    for (size_t i = 0; i < a.n; ++i) {
        sum += a.data[i] * a.data[i];
    }
    return sqrt(sum);
}

Vector::Vector(std::initializer_list<double> l) {
    this->n = l.size();
    this->data = l;
}

double p_norm(const Vector &v, double p) {
    assert(p >= 1.0 && "p has to be between 1.0 and +infinity for p_norm(Vector)");
    double sum = 0.0;
    for (size_t i = 0; i < v.n; i++) {
        sum += pow(fabs(v[i]), p);
    }
    return pow(sum, 1.0 / p);
}

Vector abs(const Vector &v) {
    size_t n = v.size();
    Vector s = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        if (v[i] < 0) {
            s[i] = -1 * v[i];
        } else {
            s[i] = v[i];
        }
    }
    return s;
}

double compensatedSum(const Vector &v) {
    double sum = v[0];
    double C = 0.0;
    double Y;
    double T;
    size_t n = v.size();
    for (size_t i = 1; i < n; i++) {
        Y = v[i] - C;
        T = sum + Y;
        C = (T - sum) - Y;
        sum = T;
    }
    return sum;
}

double innerProduct(const Vector &a, const Vector &b) {
    return a * b;
}

Matrix outerProduct(const Vector &a, const Vector &b) {
    size_t rows = a.size();
    size_t cols = b.size();
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = a[i] * b[j];
        }
    }
    return result;
}

Vector componentProduct(const Vector &a, const Vector &b) {
    assert(a.size() == b.size() && "Dimension missmatch in Vector .* Vector");
    const size_t n = a.size();
    Vector res = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        res[i] = a[i] * b[i];
    }
    return res;
}

double min(const Vector &v) {
    const size_t n = v.size();
    double cmin = v[0];
    for (size_t i = 1; i < n; ++i) {
        if (v[i] < cmin) {
            cmin = v[i];
        }
    }
    return cmin;
}

double max(const Vector &v) {
    const size_t n = v.size();
    double cmax = v[0];
    for (size_t i = 1; i < n; ++i) {
        if (v[i] > cmax) {
            cmax = v[i];
        }
    }
    return cmax;
}

Vector componentDiv(const Vector &a, const Vector &b) {
    assert(a.size() == b.size() && "Dimension missmatch in Vectro ./ Vector");
    const size_t n = a.size();
    Vector res = Vector(a);
    for (size_t i = 0; i < n; ++i) {
        res[i] = a[i] / b[i];
    }
    return res;
}


std::ostream &operator<<(std::ostream &stream, const Vector &v) {
    stream << v.to_string();
    return stream;
}

/*
std::istream &operator>>(std::istream &f, Vector &k) {
    auto v = std::vector<double>();
    char c;
    f >> c;

    if (c == '['){
        double value;
        while (f >> value){
            v.emplace_back(value);
        }
    } else {
        throw "Invalid Token";
    }
    return f;
}*/

int Vector::max_index() {
    int max_i = 0;
    for(int i = 0; i < this->n; i++){
        if (this->data[i] > this->data[max_i]) {
            max_i = i;
        }
    }
    return max_i;
}