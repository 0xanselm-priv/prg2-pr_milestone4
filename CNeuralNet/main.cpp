#include <iostream>
//#include "test/main.h"

#include "src/NeuralNet.h"
#include "src/SigmoidFunctions.h"

//#include <math.h>
#include <string>
#include <fstream>
#include <cstdlib>
//#include <c++/thread>
//#include <windows.h>  windows specific
using namespace std;





int main(int argc, char *argv[]) {
#ifdef TEST
    //test::main(argc, argv);
#endif // TEST
    std::cout.precision(17);


    NeuralNet nn = NeuralNet({784,64,10});

    nn.eta = 0.3;
    nn.toggle_adaptive_learning = true;
    nn.setActivationFunction(&logistic,&logistic_derived);

    //nn.train_with_file("/Users/nielsheissel/Downloads/CNeuralNet/mnist/train-images-idx3-ubyte", "/Users/nielsheissel/Downloads/CNeuralNet/mnist/train-labels-idx1-ubyte", 500);
    cout << "training done" << endl;
    //nn.test_with_file("/Users/nielsheissel/Downloads/CNeuralNet/mnist/t10k-images-idx3-ubyte", "/Users/nielsheissel/Downloads/CNeuralNet/mnist/t10k-labels-idx1-ubyte", 5000);

    vector <float> m(28, 0.5);
    vector < vector <float> > ms(28, m);
    cout << ms[26][1] << endl;
    cout << nn.propagate(ms)[1];

    return 0;
}