/**
 * @file Fractal.cpp
 * @author  Guy Kornblit
 *
 * @brief Definition file for the Fractal class and it's children. main functionality is to
 * construct the actual board for display.
 */

// ------------------------------ includes ------------------------------

#include "Fractal.h"
#include <iostream>
#include "cmath"

using namespace std;

// -------------------------- Fractal Class functions -------------------------

/**
 * Fractal constructor.
 * generates the base matrix (vector of strings) with spaces, the matrix is in the size of
 * board_dim * board_dim.
 * @param dim wanted dimension for Fractal.
 * @param dimension of the base case for a specific fractal type.
 */
Fractal::Fractal(const int dim, const int dimFactor)
        : _boardDim(pow(dimFactor, dim)), _dimFactor(dimFactor), _fractalDim(dim)
{
    string row(_boardDim, ' ');
    _outputBoard = vector<string>(_boardDim, row);
}

/**
  * prints the fractal to standard output.
*/
void Fractal::draw()
{
    for (const string &row : _outputBoard)
    {
        cout << row << endl;
    }
}

/**
 * this function will be called while constructing a type Fractal object, and will build
 * recursively the Fractal visual representation.
 * @param dim dimension of the wanted Fractal.
 * @param size - size of the output "matrix", mostly Dim Factor by type power by dim.
 * @param i - row idx.
 * @param j - col idx.
 */
void Fractal::build(const int dim, const int fractalSize, const int row, const int col)
{
    if (dim == 1)
    {
        createBaseCase(row, col);
    }
    else
    {
        int subSize = fractalSize / _dimFactor;

        for (int i = 0; i < fractalSize; i += subSize)
        {
            for (int j = 0; j < fractalSize; j += subSize)
            {
                if (gapCondition(i, j, subSize))
                {
                    continue;
                }
                else
                {
                    Fractal::build(dim - 1, subSize, row + i, col + j);
                }
            }
        }
    }
}


// -------------------------- SierpinskiCarpet Class functions -------------------------

/**
 * class ctor, uses functionality from parent class.
 * @param dim - wanted dimension of the Full fractal.
 */
SierpinskiCarpet::SierpinskiCarpet(const int dim)
        : Fractal(dim, CARPET_DIM_FACTOR)
{
    build(_fractalDim, _outputBoard.size(), 0, 0);
}

/**
    * creates the specific base case fractal:
    * ###
    * # #
    * ###
    * @param i - row idx
    * @param j - col idx
    */
void SierpinskiCarpet::createBaseCase(const int row, const int col)
{
    _outputBoard[FIRST_ROW(row)].replace(col, CARPET_DIM_FACTOR, CARPET_FILL);
    _outputBoard[SECOND_ROW(row)].replace(col, CARPET_DIM_FACTOR, CARPET_GAP);
    _outputBoard[THIRD_ROW(row)].replace(col, CARPET_DIM_FACTOR, CARPET_FILL);
}


// -------------------------- SierpinskiTriangle Class functions -------------------------

/**
 * class ctor, uses functionality from parent class.
 * @param dim - wanted dimension of the Full fractal.
 */
SierpinskiTriangle::SierpinskiTriangle(const int dim)
        : Fractal(dim, TRIANGLE_DIM_FACTOR)
{
    build(_fractalDim, _outputBoard.size(), 0, 0);
}

/**
 * creates the specific base case fractal:
 * ##
 * #
 * @param i - row idx
 * @param j - col idx
*/
void SierpinskiTriangle::createBaseCase(const int row, const int col)
{
    _outputBoard[FIRST_ROW(row)].replace(col, TRIANGLE_DIM_FACTOR, TRIANGLE_FILL);
    _outputBoard[SECOND_ROW(row)].replace(col, TRIANGLE_DIM_FACTOR, TRIANGLE_GAP);
}


// -------------------------- VicsekFractal Class functions -------------------------

/**
 * class ctor, uses functionality from parent class.
 * @param dim - wanted dimension of the Full fractal.
*/
VicsekFractal::VicsekFractal(const int dim)
        : Fractal(dim, VICSEK_DIM_FACTOR)
{
    build(dim, _outputBoard.size(), 0, 0);
}

/**
 * creates the specific base case fractal:
 * # #
 *  #
 * # #
 * @param i - row idx
 * @param j - col idx
    */
void VicsekFractal::createBaseCase(const int row, const int col)
{
    _outputBoard[FIRST_ROW(row)].replace(col, VICSEK_DIM_FACTOR, VICSEK_FILL);
    _outputBoard[SECOND_ROW(row)].replace(col, VICSEK_DIM_FACTOR, VICSEK_GAP);
    _outputBoard[THIRD_ROW(row)].replace(col, VICSEK_DIM_FACTOR, VICSEK_FILL);
}