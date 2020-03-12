cmake_minimum_required(VERSION 3.15)
project(CPP_ex1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CPP_ex1
        Activation.cpp
        Activation.h
        Dense.cpp
        Dense.h
        Digit.h
        main.cpp
        Matrix.cpp
        Matrix.h
        MlpNetwork.cpp
        MlpNetwork.h)
