cmake_minimum_required(VERSION 3.27)
project(rationalNumberOperations)

set(CMAKE_CXX_STANDARD 17)

add_executable(rationalNumberOperations Source.cpp
        Rational.h
        Rational.cpp
        Node.cpp
        Node.h
        BigNumber.h
        BigNumber.cpp)
