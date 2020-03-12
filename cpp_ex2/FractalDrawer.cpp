/**
 * @file FractalDrawer.cpp
 * @author  Guy Kornblit
 *
 * @brief program that prints fractal of several types.
 *
 * @section DESCRIPTION
 * The program gets command file and outputs a print of the fractal.
 * Input  : csv with two columns (type, dim).
 * Process: creates Fractals as required.
 * Output : prints to stdout the fractals wanted in reverse order than given.
 */

// ------------------------------ includes ------------------------------
#include "vector"
#include "Fractal.h"
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/tokenizer.hpp"
#include "string"
#include <iostream>

// -------------------------- const definitions -------------------------
#define ARGS_START_IDX 1
#define ARGS_COUNT (ARGS_START_IDX + 1)
#define PATH_IDX 1
#define USAGE_MSG "Usage:   FractalDrawer <file path>"
#define FILE_PATH_SUFFIX_LOWER ".csv"
#define FILE_PATH_SUFFIX_UPPER ".CSV"
#define INVALID_INPUT_MSG "Invalid input"
#define VALID_COLS_NUM 2
#define FRACTAL_TYPE_COL 1
#define FRACTAL_DIM_COL 2
#define MAX_DIM 6
#define MIN_DIM 1

#define COL_WITH_SPACE_ALLOWED 2
using namespace std;
namespace fs = boost::filesystem;

// ------------------------------ functions -----------------------------
/**
 * This function checks if a string ends with a given substring.
 * @param str str to check
 * @param suffix substring.
 * @return bool
 */
bool hasSuffix(const string &str, const string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/**
 * prints error msg and exit program with EXIT_FAILURE
 */
void invalidInput()
{
    cerr << INVALID_INPUT_MSG << endl;
    exit(EXIT_FAILURE);
}

/**
 * checks that file has .csv postfix to path.
 * checks that this is a file (and not a directory).
 * check if the file exists.
 * check if the file is not empty.
 * @param path - path to the csv file.
 */
void validateCommandFilePath(string const &path)
{
    if (!hasSuffix(path, FILE_PATH_SUFFIX_LOWER) &&
        !hasSuffix(path, FILE_PATH_SUFFIX_UPPER))
    {
        invalidInput();
    }

    else
    {
        const fs::path filePath(path);
        if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
        {
            invalidInput();
        }
    }
}

/**
 * checks if a given string contains only digits.
 * @param s string to check
 * @return true if whole string contains digits, false otherwise.
 */
bool isDigit(const string &s)
{
    for (const char &c : s)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

/**
 * checks if the column value is valid, and allows a single space after the last col value
 * @param s argument to check
 * @param col number of columns
 * @return true if valid, false otherwise.
 */
bool colValValid(const string &s, const int col)
{
    if (col == COL_WITH_SPACE_ALLOWED && s.back() == ' ')
    {
        //allows one space after the argument.
        string cleaned = s.substr(0, s.size() - 1);
        return isDigit(cleaned);
    }
    else
    {
        return isDigit(s);
    }
}
/**
 * parses line in the csv file, each line needs be in the following format:
 * 1. two columns.
 * 2. every column contains an int num.
 * @param line - string representing a line of the csv file.
 * @param type - output for the valid type in the line
 * @param dim - output for the dimension of the fractal.
 */
void parseFileLine(const string &line, int &type, int &dim)
{
    boost::char_separator<char> sep(",");
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    tokenizer tokens(line, sep);
    int columnIdx = 1;
    for (const string &colVal : tokens)
    {
        if (columnIdx > VALID_COLS_NUM || !colValValid(colVal, columnIdx))
        {
            invalidInput();
        }
        if (columnIdx == FRACTAL_TYPE_COL)
        {
            type = stoi(colVal);
        }
        else if (columnIdx == FRACTAL_DIM_COL)
        {
            dim = stoi(colVal);
        }
        ++columnIdx;
    }
}

/**
 * factory for fractal objects. given a type and dimension the factory will check if the
 * arguments valid and output with a new obj in the provided vector.
 * will check:
 * 1. fractal type is supported.
 * 2. dimension should be at a predetermined range.
 * @param type - int representation of a fractal type.
 * @param dim - int dim of the fractal.
 * @param outVec - a vector to add the new Fractal object.
 */
void fractalFactory(const int &type, const int &dim, vector<Fractal*> &outVec)
{
    if (dim < MIN_DIM || dim > MAX_DIM)
    {
        invalidInput();
    }
    else
    {
        switch (type)
        {
            case SIERPINSKI_CARPET:
            {
                auto* carpet = new SierpinskiCarpet(dim);
                outVec.push_back(carpet);
                break;
            }
            case SIERPINSKI_TRIANGLE:
            {
                auto triangle = new SierpinskiTriangle(dim);
                outVec.push_back(triangle);
                break;
            }
            case VICSEK_FRACTAL:
            {
                auto vicsekFractal = new VicsekFractal(dim);
                outVec.push_back(vicsekFractal);
                break;
            }
            default:
                invalidInput();
        }
    }
}

/**
 * reads CSV input file and constructs a relevant Fractal Object accordingly.
 * The file needs to follow this format
 * 1. each row is a statement of a fractal tree to be drawn.
 * 2. each row contains two columns.
 * 3. first col (fractal type) should be within the num of supported types.
 * 4. second column  should be in a determined range.
 * @param path - path to the csv file.
 * @param outVec - output vector containing built Fractal objects.
*/
void processCommandFile(const string &path, vector<Fractal*> &outVec)
{
    validateCommandFilePath(path);
    fs::ifstream file(path);

    // check if object is valid
    if (!file)
    {
        invalidInput();
    }
    string line;
    while (getline(file, line))
    {
        int fractalType, fractalDim = 0;
        parseFileLine(line, fractalType, fractalDim);
        fractalFactory(fractalType, fractalDim, outVec);
    }
}

/**
 * Prints program usage to stdout.
 */
void usage()
{
    cerr << USAGE_MSG << endl;
}

/**
 * iterates backwards on the output Fractal Vector and Prints the given fractal in reverse order
 * than received.
 * @param vec output Fractal vector
 */
void outputFractals(vector<Fractal*> &vec)
{
    for (auto it = vec.rbegin(); it != vec.rend(); it++)
    {
        (*(*it)).draw();
        cout << endl;
    }
}

/**
 * used to free the vector of Fractals.
 * @param vector  vector of Fractal pointers.
 */
void freeResources(vector<Fractal*> &vector)
{
    for (auto fractal : vector)
    {
        delete fractal;
        fractal = nullptr;
    }
}

/**
 * Main function for the FractalDrawer,  gets from CLI a path to a csv file containing two
 * columns for the type of the wanted fractal and it's dimension, parses the file and prints the
 * fractal to standard output in reverse order.
 * @param argc - number of arguments, expecting one argument (the path)
 * @param argv
 * @return EXIT_SUCCESS or EXIT_FAILURE if anything wrong with the input.
 */
int main(int argc, char** argv)
{
    if (argc != ARGS_COUNT)
    {
        usage();
        exit(EXIT_FAILURE);
    }
    string filePath = argv[PATH_IDX];

    vector<Fractal*> vector;
    processCommandFile(filePath, vector);
    outputFractals(vector);
    freeResources(vector);
    return EXIT_SUCCESS;
}
