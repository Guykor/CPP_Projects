/**
 * @file Fractal.h
 * @author  Guy Kornblit
 *
 * @brief decleration file for classes of type Fractal.
 *
 * @section DESCRIPTION
 * This file declares the Fractal abstract class, and it's three children (different types of
 * Fractals)
 */


#ifndef FRACTAL_H
#define FRACTAL_H
// ------------------------------ includes ------------------------------
#include "vector"
#include "string"

// -------------------------- const definitions -------------------------
// helper MACROS
#define FIRST_ROW(ROW) (ROW)
#define SECOND_ROW(ROW) (ROW + 1)
#define THIRD_ROW(ROW) (ROW + 2)

// SierpinskiCarpet Consts
#define CARPET_DIM_FACTOR 3
#define CARPET_FILL "###"
#define CARPET_GAP "# #"

// SierpinskiTriangle Consts
#define TRIANGLE_DIM_FACTOR 2
#define TRIANGLE_FILL "##"
#define TRIANGLE_GAP "# "

// Vicsek Consts.
#define VICSEK_DIM_FACTOR 3
#define VICSEK_FILL "# #"
#define VICSEK_GAP " # "

using namespace std;

enum SupportedTypes
{
    SIERPINSKI_CARPET = 1,
    SIERPINSKI_TRIANGLE = 2,
    VICSEK_FRACTAL = 3
};


// -------------------------- Fractal Class -------------------------

/**
 * @class Fractal is an abstract class defining different type of fractals.
 * the Fractal class holds a container for the visual representation of the instance of one of
 * Fractal children, that is built by a the build function that uses different arguments per type.
 */
class Fractal
{
private:
    /**
     * holds the size of the Fractal in the given dimension. Namely, this number will be the type
     * dimension factor by the power of the given dim.
     */
    int _boardDim;

    /**
     * the fractal type dimension factor - i.e the dimension of the base case fractal.
     */
    int _dimFactor;

protected:
    /**
     * provided dimension for the Fractal.
     */
    int _fractalDim;
    /**
     * Visual representation of the Fractal.
     */
    vector<string> _outputBoard;

    /**
     * this function will be called while constructing a type Fractal object, and will build
     * recursively the Fractal visual representation.
     * @param dim dimension of the wanted Fractal.
     * @param size - size of the output "matrix", mostly Dim Factor by type power by dim.
     * @param i - row idx.
     * @param j - col idx.
     */
    virtual void build(int dim, int size, int i, int j);

    /**
     * pure virtual function that creates the specific base case fractal for the type.
     * @param i - row idx
     * @param j - col idx
     */
    virtual void createBaseCase(int i, int j) = 0;

    /**
     * Due to the fact that every fractal type has a different location for the gap in it's
     * pattern, this function will be called within the build function and decide in which cases
     * the base case won't be reached.
     * by default - skip  every time reached subMatrix in idx [current_dim-1,current_dim-1].
     * @param i row idx
     * @param j col idx
     * @return true if condition is met (skip), false otherwise
     */
    virtual bool gapCondition(const int i, const int j, const int subSize)
    {
        return (i == subSize && j == subSize);
    };

    /**
     * Fractal constructor.
     * @param dim wanted dimension.
     * @param dimFactor every type will have a dimension for it's base case.
     */
    Fractal(int dim, int dimFactor);

public:

    /**
     * class dtor.
     */
    virtual ~Fractal() = default;

    /**
     * prints the fractal to standard output.
     */
    void draw();
};


// -------------------------- SierpinskiCarpet Fractal Class -------------------------
/**
 * @class Fractal of type SierpinskiCarpet, holds specific information relevant to creating
 * visual representation of the Fractal.
 */
class SierpinskiCarpet : public Fractal
{
private:
    /**
     * creates the specific base case fractal:
     * ###
     * # #
     * ###
     * @param i - row idx
     * @param j - col idx
     */
    void createBaseCase(int i, int j) override;

public:
    /**
     * class ctor, uses functionality from parent class.
     * @param dim - wanted dimension of the Full fractal.
     */
    SierpinskiCarpet(int dim);

};

// -------------------------- SierpinskiTriangle Fractal Class -------------------------


/**
 * @class Fractal of type SierpinskiTriangle, holds specific information relevant to creating
 * visual representation of the Fractal.
 */
class SierpinskiTriangle : public Fractal
{
private:
    /**
     * creates the specific base case fractal:
     * ##
     * #
     * @param i - row idx
     * @param j - col idx
     */
    void createBaseCase(int i, int j) override;

public:
    /**
     * class ctor, uses functionality from parent class.
     * @param dim - wanted dimension of the Full fractal.
     */
    SierpinskiTriangle(int dim);

};

// -------------------------- VicsekFractal Class -------------------------
/**
 * @class this is the VicsekFractal class.
 */
class VicsekFractal : public Fractal
{
private:
    /**
    * creates the specific base case fractal:
    * # #
    *  #
    * # #
    * @param i - row idx
    * @param j - col idx
    */
    void createBaseCase(int i, int j) override;

    /**
     * determine base on the matrix size and indices if it suppose to be a blank submatrix.
     * in this case it should be every time that we reach an index that is a multipication of the
     * dimension factor for the fractal, except cases when (i=j).
     * @param i - row idx
     * @param j - col idx
     * @param subSize size of the matrix handled.
     * @return true if skip is needed, false otherwise.
     */
    bool gapCondition(const int i, const int j, const int subSize) override
    {
        return (i != j && (i == subSize || j == subSize));
    }

public:
    /**
    * class ctor, uses functionality from parent class.
    * @param dim - wanted dimension of the Full fractal.
    */
    VicsekFractal(int dim);

};

#endif //FRACTAL_H
