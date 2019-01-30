//
// Created by Parnet on 10.12.2018.
//

#ifndef NEURALNET_NEURALNET_H
#define NEURALNET_NEURALNET_H

#include <vector>
#include "LinearAlgebra/Vector.h"
#include "LinearAlgebra/Matrix.h"

using namespace std;

class NeuralNet {
private:
    std::vector<Vector> layer;
    std::vector<Vector> layer_activated;
    std::vector<Matrix> changeWeights;




    double (*activation)(double) = nullptr;
    double (*activation_derived)(double) = nullptr;

public:
    double eta = 20;
    double alpha = 0.05; // todo alpha in [0,1]

    bool toggle_traegheit = false;
    bool toggle_adaptive_learning = false;

    double factor_up = 1.05;
    double factor_down = 0.5;
    double comperator = 0.01;

    std::vector<Matrix> weights;
    NeuralNet() = default;

    NeuralNet(const std::vector<size_t> & topology);
    ~NeuralNet() = default;

    void setActivationFunction(double (*activation)(double), double (*activation_derived)(double));

    Vector getOutput();

    void setWeights(size_t index, Matrix &W);

    void analyze(const Vector & input);

    void backPropagation(Vector &target);

    void training(Vector input, Vector target);

    void train_batch(vector< vector < double> > inputs, vector< double > labels, int);

    void train_stochastic(vector< vector < double> > inputs, vector< double > labels, int);

    float test(vector< vector < double> > inputs, vector< double > labels, int);

    void toCAProjectFile(std::string filename);

    NeuralNet fromCAProjectFile(std::string filename);

    std::string toString();

    void display_summary();

    int train_with_file(string filepath_img, string filepath_labels,int i);
    float test_with_file(string filepath_img, string filepath_labels, int i);

    vector <float> propagate(vector< vector <float> >);

    vector<double> matrix2vector(vector< vector<float> > mat);
    vector<vector <double> > vector2matrix(vector<double> vec);
    void read_Mnist_Label(string filename, vector<double> &vec);
    void read_Mnist(string filename, vector<vector<double> > &vec);


    size_t iteration = 0;
    double error = 0.0;
    double prevError = 0.0;
    double getError(Vector vector);

    double errorrate = 1.0;
};

Vector transform(Vector &v,  double (*fun)(double));
Vector rtransform(Vector &v, double (*fun)(double));
Vector transform_exlst(Vector &v,  double (*fun)(double));
void transform_inplc(Vector &w, const Vector &v,  double (*fun)(double));

Vector reduce(Vector &v);

// Vector &addBias(Vector &v);
void partial_copy(Vector &v, const Vector &w);

#endif //NEURALNET_NEURALNET_H
