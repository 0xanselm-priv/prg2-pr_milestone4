#include <iostream>
#include "test/main.h"
#include "src/NeuralNet.h"
#include "src/SigmoidFunctions.h"

#include <math.h>
#include <string>
#include <fstream>
#include <cstdlib>
//#include <c++/thread>
//#include <windows.h>  windows specific
using namespace std;





int main(int argc, char *argv[]) {
#ifdef TEST
    test::main(argc, argv);
#endif // TEST
    std::cout.precision(17);
    std::vector<size_t> topology = {784,64,10};
    NeuralNet nn = NeuralNet(topology);
    nn.eta = 0.3;
    nn.toggle_traegheit = false;
    nn.toggle_adaptive_learning = false;

    nn.setActivationFunction(&logistic,&logistic_derived);

    nn.train_with_file("/Users/nielsheissel/Downloads/CNeuralNet/mnist/train-images-idx3-ubyte", "/Users/nielsheissel/Downloads/CNeuralNet/mnist/train-labels-idx1-ubyte", 50000);
    cout << "training done" << endl;
    nn.test_with_file("/Users/nielsheissel/Downloads/CNeuralNet/mnist/t10k-images-idx3-ubyte", "/Users/nielsheissel/Downloads/CNeuralNet/mnist/t10k-labels-idx1-ubyte", 5000);

    return 0;
}