// Matrix.h

#ifndef MATRIX_H

#define MATRIX_H

#include <fstream>

#define DEFAULT_SIZE 1
#define MATRICES_MULT_DIM_ERR "Error: Matrices sizes are'nt as they should - add dimenson!!@!#!#!$!"
#define ADD_DIM_ERR "Error: Mismatched dimension for addition operator"
#define READ_FILE_ERROR "Error: Invalid file size or format according to matrix"
/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

class Matrix
{
private:
    int _length;
    MatrixDims _dims;
    float *_matrix;
    friend void swap(Matrix &oldMatrix, Matrix &newMatrix);
public:
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const Matrix &m);
    ~Matrix();

    int getRows() const;
    int getCols() const;
    Matrix& vectorize();
    void plainPrint() const;
    Matrix& operator=(const Matrix &m);
    Matrix operator*(const Matrix &m) const;
    Matrix operator+(const Matrix &m) const;
    Matrix& operator+=(const Matrix &m);
    float& operator()(int i, int j) const;
    float& operator()(int i, int j);
    float& operator[](int i) const;
    float& operator[](int i);
    Matrix operator*(const float c) const;


    friend Matrix operator*(const float c, const Matrix &m);
    friend std::ifstream& operator>>(std::ifstream &is, Matrix &m);
    friend std::ofstream& operator<<(std::ofstream &os, const Matrix &m);


};

#endif //MATRIX_H
