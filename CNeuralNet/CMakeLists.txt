cmake_minimum_required(VERSION 3.12)
project(CNeuralNet)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(CNeuralNet main.cpp src/SigmoidFunctions.cpp src/NeuralNet.cpp src/LinearAlgebra/Vector.cpp src/LinearAlgebra/Matrix.cpp src/LinearAlgebra/Factory.cpp  test/main.cpp test/TestObject.cpp)