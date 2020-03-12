//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

class Activation
{
private:
    ActivationType type;
    Matrix activateRelu(const Matrix &m) const;
    Matrix activateSoftmax(const Matrix &m) const;

public:
    Activation(ActivationType actType);
    Matrix operator()(const Matrix &m) const;
};

#endif //ACTIVATION_H
