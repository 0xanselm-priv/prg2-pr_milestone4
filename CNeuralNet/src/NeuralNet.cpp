//
// Created by Parnet on 10.12.2018.
//

#include "NeuralNet.h"
#include <iostream>
#include <random>
#include <cassert>
#include <fstream>
#include "LinearAlgebra/Factory.h"

#define debug(n) std::cout << #n << ": " << n << std::endl << std::flush
#define debugv(n) std::cout << #n << ": " << (n.size()) << "\t"<< n<<  std::endl << std::flush
#define debugm(n) std::cout << #n << ": " << n.num_rows() <<", "<<n.num_cols() << std::endl << n << std::endl << std::flush
#define debugm_short(n) std::cout << #n << ": " << n.num_rows() <<", "<<n.num_cols() << std::endl << std::flush

#define swap(a, b, tmp) tmp = a; a = b; b = tmp;


NeuralNet::NeuralNet(const std::vector<size_t> &topology) {
    assert(!topology.empty() && "Topology undefined for NeuralNet");

    layer = std::vector<Vector>();
    layer_activated = std::vector<Vector>();

    weights = std::vector<Matrix>();

    layer.emplace_back(Vector()); // input layer has no intermediate result

    Vector v = Vector(topology[0] + 1);
    v[topology[0]] = 1.0;
    layer_activated.emplace_back(v); // for the output of the input vector
    weights.emplace_back(Matrix(0, 0)); // no weights needed to calculate the output of the input vector
    changeWeights.emplace_back(Matrix(0, 0));

    auto en = Factory::random();
    std::normal_distribution<double> distribution(0, 1.0);

    for (size_t i = 1; i < topology.size(); ++i) {
        Vector v = Vector(topology[i] + 1);
        v[topology[i]] = 1.0;
        layer.emplace_back(Vector(topology[i]));
        layer_activated.emplace_back(v);
        Matrix M = Matrix(topology[i], topology[i - 1] + 1); // weights from layer i-1 to i

        for (size_t k = 1; k <= topology[i]; ++k) {
            for (size_t m = 1; m <= topology[i - 1] + 1; ++m) {
                M(k, m) = distribution(en); // initial weights for matrix
            }
        }
        weights.emplace_back(M);
        //if (toggle_traegheit) {
        changeWeights.emplace_back(Matrix(topology[i], topology[i - 1] + 1));
        //}
    }
}

void NeuralNet::setActivationFunction(double (*activation)(double), double (*activation_derived)(double)) {
    this->activation = activation;
    this->activation_derived = activation_derived;

}

void NeuralNet::analyze(const Vector &input) {
    if (this->activation == nullptr) {
        std::cerr << "activation function not set";
        return;
    }
    partial_copy(this->layer_activated[0], input);
    size_t N = this->layer.size(); // -1 ;
    for (size_t i = 1; i < N; ++i) {

        this->layer[i] = this->weights[i] * this->layer_activated[i - 1];
        transform_inplc(this->layer_activated[i], this->layer[i], this->activation);
    }
}

std::string NeuralNet::toString() {
    std::stringstream sout;
    for (size_t i = 0; i < this->layer.size(); ++i) {
        const auto &v = this->layer[i];
        const auto &a = this->layer_activated[i];
        const auto &m = this->weights[i];
        sout << "W" << m << std::endl;
        sout << "L" << std::endl << v << std::endl;
        sout << "A" << std::endl << a << std::endl;
    }
    return sout.str();
}

void NeuralNet::setWeights(size_t index, Matrix &W) {
    this->weights[index] = W;
}

Vector NeuralNet::getOutput() {
    const Vector &output = this->layer_activated.back();
    Vector v = Vector(output.size() - 1);
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] = output[i];
    }
    return v;
}

double avg(const Matrix &m) {
    double sum = 0.0;
    for (size_t i = 0; i < m.num_rows(); ++i) {
        for (size_t j = 0; j < m.num_cols(); ++j) {
            sum += m[i][j];
        }
    }
    return (sum / (m.num_rows() * m.num_cols()));
}

void NeuralNet::backPropagation(Vector &target) {
    Vector exttarget = Vector(target.size() + 1);
    exttarget[target.size()] = 1.0;
    partial_copy(exttarget, target);

    size_t N = this->layer.size() - 1;
    Vector old_fehlersignal = Vector();
    Matrix old_newWeights = Matrix();
    /*
     * calculation for the output layer
     */

    auto drv = transform(this->layer[N], this->activation_derived);

    auto fehlersignal = componentProduct(exttarget - this->layer_activated[N], drv);

    reduce(fehlersignal);
    Matrix newWeights = outerProduct(fehlersignal * 2 * this->eta, this->layer_activated[N - 1]);

    old_fehlersignal = fehlersignal; // todo remove copy
    old_newWeights = newWeights; // todo remove copy

    /*
     * calculation for the hidden layer
     */

    for (size_t i = N - 1; i > 0; --i) {
        auto interresult = old_fehlersignal * this->weights[i + 1];
        auto t_drv = transform(this->layer[i], this->activation_derived);
        fehlersignal = componentProduct(interresult, t_drv);
        reduce(fehlersignal);
        newWeights = outerProduct(fehlersignal * 2 * this->eta, this->layer_activated[i - 1]);
        if (toggle_traegheit) {
            this->changeWeights[i + 1] = (1 - alpha) * old_newWeights + alpha * this->changeWeights[i + 1];
            old_newWeights = this->changeWeights[i + 1];
        }

        this->weights[i + 1] += old_newWeights;

        old_fehlersignal = fehlersignal; // todo remove copy
        old_newWeights = newWeights; // todo remove copy
    }

    if (toggle_traegheit) {
        this->changeWeights[1] = (1 - alpha) * old_newWeights + alpha * this->changeWeights[1];
        old_newWeights = this->changeWeights[1];
    }
    this->weights[1] = this->weights[1] + old_newWeights;
}

void NeuralNet::training(Vector input, Vector target) {
    this->iteration += 1;
    if (this->toggle_adaptive_learning && this->iteration % 500 == 0) {
        this->error /= 500;
        if (this->error - prevError > this->comperator) {
            this->eta *= this->factor_up;
        } else {
            this->eta = max(this->eta*this->factor_down, 0.01);
        }
        this->prevError = this->error;
        this->error = 0.0;
    }
    this->analyze(input);

    Vector output = this->getOutput();
    if (this->toggle_adaptive_learning) {
        this->error += this->getError(target);
    }
    this->backPropagation(target);

}

double NeuralNet::getError(Vector target) {
    Vector output = this->getOutput();
    Vector difference = output - target;
    double err = euklidian_norm(difference);
    return err / sqrt(output.size());
}

void NeuralNet::toCAProjectFile(std::string filename) {
    std::ofstream weight_file(filename.c_str());
    weight_file.precision(17);
    weight_file.clear();

    for (size_t layer_num = 0; layer_num != this->weights.size() - 1; layer_num++) {

        weight_file << "Layer " << layer_num + 1 << "\r\n" << "\r\n";

        for (size_t j = 0; j != this->weights[layer_num + 1].num_cols(); j++) {

            if (j == this->weights[layer_num + 1].num_cols() - 1) {
                weight_file << "\r\n" << "Bias " << j + 1 << "\r\n" << "\r\n";
            } else {
                weight_file << "\r\n" << "Neuron " << j + 1 << "\r\n" << "\r\n";
            }

            for (size_t i = 0; i != this->weights[layer_num + 1].num_rows(); i++) {
                weight_file << this->weights[layer_num + 1](i + 1, j + 1) << ' ';
            }

            weight_file << "\r\n";
        }
        weight_file << "\r\n" << "\r\n" << "\r\n";
    }
    weight_file.close();
}

void NeuralNet::display_summary() {
    std::cout << "========================================" << std::endl
              << "Errorrate          " << this->errorrate << std::endl
              << "Adaptive Learning  " << this->toggle_adaptive_learning << std::endl
              << "Lernrate           " << this->eta << std::endl
              << "Factor Down        " << this->factor_down << std::endl
              << "Factor Up          " << this->factor_up << std::endl
              << "Comperator         " << this->comperator << std::endl
              << "Trägheit           " << this->toggle_traegheit << std::endl
              << "Alpha              " << this->alpha << std::endl;
}

NeuralNet NeuralNet::fromCAProjectFile(std::string filename) {
    NeuralNet nn;
    // todo
    return nn;
}


Vector rtransform(Vector &v, double (*fun)(double)) {
    size_t n = v.size();
    Vector w = Vector(n);
    for (size_t i = 0; i < n; ++i) {
        w[i] = fun(v[i]);
    }
    return w;
}

Vector transform(Vector &v, double (*fun)(double)) {
    //std::cout << "transformation" << std::endl;
    size_t n = v.size();
    Vector w = Vector(n + 1);
    for (size_t i = 0; i < n; ++i) {
        w[i] = fun(v[i]);
    }
    w[n] = 1.0;
    return w;
}

void transform_inplc(Vector &w, const Vector &v, double (*fun)(double)) {
    for (size_t i = 0; i < v.size(); ++i) {
        w[i] = fun(v[i]);
    }
}

void partial_copy(Vector &v, const Vector &w) {
    for (size_t i = 0; i < w.size(); i++) {
        v[i] = w[i];
    }
}


Vector reduce(Vector &v) {
    size_t N = v.size();
    v.resize(N - 1);
    return v;
}

void NeuralNet::train_batch(vector<vector<double> > inputs, vector<double> labels, int batch_size) {
    for (int i = 0; i < inputs.size() / batch_size; i++) {

    }
}


void NeuralNet::train_stochastic(vector<vector<double> > inputs, vector<double> labels, int i) {
    for (int j = 0; j < i; j++) {
        cout << "Training Image " << j << endl;
        Vector inp(784, 0);
        for (int y = 0; y < 784; y++) {
            inp[y] = inputs[j][y];
        }
        this->analyze(inp);
        Vector output = this->getOutput();
        Vector target = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        target[labels[j]] = 1;
        this->backPropagation(target);

        //cout << inp << endl;
        //cout << labels[j] << endl;
        //cout << target << endl;
    }

    cout << "DONE <--------" << endl;
}

float NeuralNet::test(vector<vector<double> > inputs, vector<double> labels, int i) {
    int errors = 0;
    for (int j = 0; j < i; j++) {
        Vector inp(784, 0);
        for (int y = 0; y < 784; y++) {
            inp[y] = inputs[j][y];
        }
        this->analyze(inp);
        Vector output = this->getOutput();

        int output_num = output.max_index();

        if (output_num != labels[j]) {
            errors++;
            cout << "Output: " << output_num << endl;
            cout << "Target: " << labels[j] << endl;
        } else {
            cout << endl << "CORRECT" << endl;
        }
    }

    cout << endl << endl << "Total errors: " << errors << endl;

    return errors/i;
}


vector<vector<double> > NeuralNet::vector2matrix(vector<double> vec) {
    vector<vector<double> > array;

    for (int i = 0; i < 28; i++) {
        vector<double> arr{};
        for (int j = 0; j < 28; j++) {
            arr.push_back(vec[i * 28 + j]);
        }
        array.push_back(arr);
    }
    return array;
}


int ReverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return ((int) ch1 << 24) + ((int) ch2 << 16) + ((int) ch3 << 8) + ch4;
}


void NeuralNet::read_Mnist(string filename, vector<vector<double> > &vec) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;
        file.read((char *) &magic_number, sizeof(magic_number));
        magic_number = ReverseInt(magic_number);
        file.read((char *) &number_of_images, sizeof(number_of_images));
        number_of_images = ReverseInt(number_of_images);
        file.read((char *) &n_rows, sizeof(n_rows));
        n_rows = ReverseInt(n_rows);
        file.read((char *) &n_cols, sizeof(n_cols));
        n_cols = ReverseInt(n_cols);
        for (int i = 0; i < number_of_images; ++i) {
            vector<double> tp;
            for (int r = 0; r < n_rows; ++r) {
                for (int c = 0; c < n_cols; ++c) {
                    unsigned char temp = 0;
                    file.read((char *) &temp, sizeof(temp));
                    tp.push_back((double) temp / 255);
                }
            }
            vec.push_back(tp);
        }
    }
}

void NeuralNet::read_Mnist_Label(string filename, vector<double> &vec) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;
        file.read((char *) &magic_number, sizeof(magic_number));
        magic_number = ReverseInt(magic_number);
        file.read((char *) &number_of_images, sizeof(number_of_images));
        number_of_images = ReverseInt(number_of_images);
        for (int i = 0; i < number_of_images; ++i) {
            unsigned char temp = 0;
            file.read((char *) &temp, sizeof(temp));
            vec.push_back((double) temp);
        }
    }
}


int NeuralNet::train_with_file(string filepath_img, string filepath_labels, int i) {
    vector<vector<double> > train_img;
    read_Mnist(filepath_img, train_img);
    cout << train_img.size() << endl;
    cout << train_img[0].size() << endl;





    //read MNIST label into double vector
    vector<double> train_lables;
    read_Mnist_Label(filepath_labels, train_lables);
    cout << train_lables.size() << endl;

    this->train_stochastic(train_img, train_lables, i);


    return 0;
}

int NeuralNet::test_with_file(string filepath_img, string filepath_labels, int i) {

    cout << "STARTING to TEST " << endl;

    vector<vector<double> > test_img;
    read_Mnist(filepath_img, test_img);
    cout << test_img.size() << endl;
    cout << test_img[0].size() << endl;

    cout << "STARTING to TEST 2" << endl;


    //read MNIST label into double vector
    vector<double> test_lables;
    read_Mnist_Label(filepath_labels, test_lables);
    cout << test_lables.size() << endl;

    this->test(test_img, test_lables, i);
}

vector <float> NeuralNet::propagate(vector<vector<float> > inp) {
    vector<double> input = matrix2vector(inp);
    Vector vec_inp(784, 0);
    for (int y = 0; y < 784; y++) {
        vec_inp[y] = input[y];
    }
    this->analyze(vec_inp);

    Vector output = this->getOutput();

    int output_num = output.max_index();

    vector <float> out;
    for(int j = 0; j<output.size(); j++){
        out.push_back(output[j]);
    }
    return out;
}

vector<double> NeuralNet::matrix2vector(vector<vector<float> > mat) {
    vector<double> vec;
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat[0].size(); j++) {
            vec.push_back(mat[i][j]);
        }
    }
    return vec;
}
