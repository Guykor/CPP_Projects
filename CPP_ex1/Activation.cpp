//
// Created by Guy on 12/23/2019.
//

#include "Activation.h"
#include "math.h"
Activation::Activation(ActivationType actType)
: type(actType){}

Matrix Activation::activateRelu(const Matrix &m) const
{
    Matrix res(m);
    for (int i = 0; i < m.getRows(); i++)
    {
        for (int j = 0; j < m.getCols(); j++)
        {
            if (res(i,j) < 0)
            {
                res(i,j) = 0;
            }
        }
    }
    return res;
}

Matrix Activation::activateSoftmax(const Matrix &m) const
{
    //assuming m is a vector! todo: check it.
    Matrix res(m);
    float sum = 0;
    for (int i = 0; i < m.getRows(), i++)
    {
        float expm = std::exp(m[i]);
        res[i] = expm;
        sum += expm;
    }

    return  (1/sum) * res;
}

Matrix Activation::operator()(const Matrix &m) const
{
    if (type == Relu)
    {
        return activateRelu(m);
    }
    else
    {
        return activateSoftmax(m);
    }
}

