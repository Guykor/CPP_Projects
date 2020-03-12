//
// Created by Guy on 12/23/2019.
//

#include <iostream>
#include <iomanip>
#include "Matrix.h"

Matrix::Matrix(int rows, int cols)
: _dims{rows, cols}, _matrix(new float[rows*cols]), _length(rows*cols){}


Matrix::Matrix()
: Matrix(DEFAULT_SIZE, DEFAULT_SIZE){}

Matrix::Matrix(const Matrix &m)// copy ctor.
: Matrix(m._dims.rows, m._dims.cols)
{
    for (int i = 0; i < _dims.rows * _dims.rows; i++)
    {
        _matrix[i] = m._matrix[i];
    }
}

Matrix::~Matrix()
{
    delete[] _matrix;
    //todo: understand if handles deletion of heap allocated objs.
}
Matrix& Matrix::operator=(const Matrix &m)
{
    if (this == &m)
    {
        return *this;
    }
    Matrix dumbMatrix (m); // copy ctor
    swap(*this, dumbMatrix);
    return *this;
}

// uses std::swap which calls the copy ctor in order to avoid code duplication when using operator=.
//copy and swap idiom.
// todo: change and consult meny.
void swap(Matrix &oldMatrix, Matrix &newMatrix)
{
    std::swap(oldMatrix._dims.rows, newMatrix._dims.rows);
    std::swap(oldMatrix._dims.cols, newMatrix._dims.cols);
    std::swap(oldMatrix._matrix, newMatrix._matrix);
}

int Matrix::getRows() const
{
    return _dims.rows;
}

int Matrix::getCols() const
{
    return _dims.cols;
}

Matrix& Matrix::vectorize()
{
    _dims.rows = _length;
    _dims.cols = 1;
    return *this;
}

float& Matrix::operator()(int i, int j) const
{
    return (*this)(i,j); //todo: check if legal to call a non const enviroment function.
}

float& Matrix::operator()(int i, int j)
{
    int realIdx = (i * _dims.cols) + j;
    return (*this)[realIdx];
}

float& Matrix::operator[](int i) const
{
    return (*this)[i];
}
float& Matrix::operator[](int i)
{
    return _matrix[i];
}

Matrix Matrix::operator*(const Matrix &m) const
{
    if (_dims.cols == m._dims.rows)
    {
        Matrix res(_dims.rows, m._dims.cols);
        for (int i = 0; i < _dims.rows; i++)
        {
            for (int j = 0; j < m._dims.cols; j++)
            {
                for (int k = 0; k <  res._length ; k++)
                {
                    res(i,j) = (*this)(i,k) * m(k,j);
                }
            }
        }
        return res;
    }
    std::cerr << MATRICES_MULT_DIM_ERR << std::endl;
    exit(1);
}

Matrix Matrix::operator+(const Matrix &m) const
{
    if (_dims.rows == m._dims.rows && _dims.cols == m._dims.cols)
    {
        Matrix res(_dims.rows, _dims.cols);
        for (int i = 0; i < _dims.rows * _dims.cols; i++)
        {
            res[i] = _matrix[i] + m[i];
        }
    }
    std::cerr << ADD_DIM_ERR << std::endl;
    exit(1);
}

Matrix &Matrix::operator+=(const Matrix &m)
{
    *this = *this + m;
}

Matrix Matrix::operator*(const float c) const
{
    Matrix res(*this);
    for (int i = 0; i < _length; i++)
    {
       res[i] *= c;
    }
    return res;
}

Matrix operator*(const float c, const Matrix &m)
{
    return m * c;
}

std::ifstream &operator>>(std::ifstream &is, Matrix &m)
{

    for (int i = 0; i < m._length; i++)
    {
        if (is >> m[i])
        {
            continue;
        }
        break;
    }
    if (is.eof())
    {
        return is;
    }
    std::cerr << READ_FILE_ERROR<< std::endl;
}


std::ofstream &operator<<(std::ofstream &os, const Matrix &m)
{
    for (int i = 0; i < m._dims.rows; i++)
    {
        std::stringstream stream;
        for (int j = 0; j < m._dims.cols; j++)
        {
            float elem = m(i,j);
            if (elem <= 0.1)
            {
                stream << "  ";
            }
            else
            {
                stream << "**";
            }
        }
        std::string row = stream.str();
        std::cout << row << std::endl;
    }
}

void Matrix::plainPrint() const
{
    for (int i = 0; i < _dims.rows; i++)
    {
        std::stringstream stream;
        for (int j = 0; j < _dims.cols; j++)
        {
             stream << std::fixed << std::setprecision(3) <<  (*this)(i,j) << " ";
        }
        std::string row = stream.str();
        std::cout << row << std::endl;
    }
}





