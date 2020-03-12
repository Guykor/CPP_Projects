/**
 * @file SpamDetector.cpp
 * @author  Guy Kornblit
 *
 * @brief this file contains implementation of a spam detector that recieves message and a filter
 * of bad words and their score, and a spam threshold, such that if the msg exceed the threshold
 * of bad points, it'll be declared as SPAM.
 */
// ------------------------------ includes ------------------------------

#include <cstdlib>
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/tokenizer.hpp"
#include <string>
#include <regex>
#include "HashMap.hpp"
#include <vector>
#include <stdexcept>

// ------------------------------ Constants ------------------------------
#define NUM_ARGS 4
#define INVALID_INPUT_MSG "Invalid input"
#define DECLARE_SPAM "SPAM"
#define DECLARE_NOT_SPAM "NOT_SPAM"

#define FAIL -1
#define NOT_INT_FLAG '.'
#define USAGE_MSG "Usage: SpamDetector <database path> <message path> <threshold>"


#define DB_PATH_ARG argv[1]
#define MSG_PATH_ARG argv[2]
#define THRESHOLD_ARG argv[3]
typedef HashMap<string, int> Filter;
namespace fs = boost::filesystem;
using namespace std;

// ------------------------------ Functions ------------------------------

/**
 * prints an informative msg to cerr about the proper CLI arguments.
 */
void usage()
{
    cerr << USAGE_MSG << endl;
    exit(EXIT_FAILURE);
}

/**
 * prints a msg to cerr, and throws an exception.
 */
void invalidInput()
{
    cerr << INVALID_INPUT_MSG << endl;
    throw std::invalid_argument("");
}

/**
 * checks that the csv file contains the proper number of columns.
 * @param tokens - container for column values, after splitting them with ','
 * @return true if number of column is valid, false otherwise.
 */
inline bool colNumInvalid(const boost::tokenizer<boost::char_separator<char>> &tokens)
{
    return distance(tokens.begin(), tokens.end()) != 2;
}

inline bool hasLetters(const string &input)
{
    return any_of(input.cbegin(), input.cend(), ::isalpha);
}
/**
 * parses the bad phrase from the data base given.
 * @param input - the string as appears in the input (CSV file)
 * @param phrase reference to the parsed final string
 * @return true if parsing was successful (the input wasn't an empty string, and contained letters)
 */
bool parsePhrase(const string& input, string &phrase)
{
    if (!input.empty() || hasLetters(input))
    {
        phrase = input;
        return true;
    }
    return false;
}
/**
 * checks if an input string suppose to hold an integer actualy holds it and not another type of
 * number (float/double etc.)
 * @param scoreStr the input str.
 * @return true if the string contains int representation.
 */
inline bool isInt(const string &scoreStr)
{
    return scoreStr.find(NOT_INT_FLAG) == string::npos;
}

/**
 * parses the score argument in the bad words db. the value should be an int that greater or
 * equal zero.
 * @param scoreStr score in the input string
 * @param score reference to a value that will be places as the parsed value
 * @return true if parsing succeeds, false otherwise.
 */
bool parseScore(const string &scoreStr, int &score)
{
    if (!scoreStr.empty() && isInt(scoreStr) && !hasLetters(scoreStr))
    {
        try
        {
            score = stoi(scoreStr);
            if (score >= 0)
            {
                return true;
            }
        }
        catch (invalid_argument& e)
        {
            return false;
        }
    }
    return false;
}



/**
 * parses line in the csv file, each line needs be in the following format:
 * 1. two columns.
 * 2. every column contains valid paremeters.
 * @param phrase - first column parsed (function output)
 * @param score - second column parsed (function output)
 */
void parseCSVLine(const string &line, string &phrase, int &score)
{

    boost::char_separator<char> sep(",", "", boost::keep_empty_tokens);
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    tokenizer tokens(line, sep);
    auto it = tokens.begin();

    if (colNumInvalid(tokens) || !parsePhrase(*it, phrase) || !parseScore(*(++it), score))
    {
        invalidInput();
    }
}

/**
 * validate that a given path is exists in system and points to a file
 * @param path - path to a given obj.
 */
void validatePath(const fs::path &path)
{
    if (!fs::exists(path) || !fs::is_regular_file(path))
    {
        invalidInput();
    }
}

/**
 * converts a string to lower case letters
 * @param str string to convert.
 */
inline void toLowerCase(string &str)
{
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return tolower(c); });
}

/**
 * reads CSV input file and stores every record.
 * @param path path to the DB file
 * @param map a hash map typedef the stored pairs of phrases (strings) and their score of
 * spamness (int).
 */
void parseDataBaseFile(const fs::path &path, Filter &map)
{

    validatePath(path);
    fs::ifstream file(path);

    // check if object is valid
    if (!file)
    {
        invalidInput();
    }
    string line, phrase;
    int score;
    while (getline(file, line))
    {
        parseCSVLine(line, phrase, score);
        toLowerCase(phrase);
        map.insert(phrase, score);
    }
}

/**
 * this function reads the msg file and flatten it to a string that will be returned by value (by
 * moved out by string class), raises an error msg if the file holds as an invalid input.
 * @param path - path to the message file.
 * @return string holding a flatten and lower case version of the message by value (can be moved
 * out).
 */
string parseTextFile(const fs::path &path)
{
    validatePath(path);
    validatePath(path);
    fs::ifstream file(path);

    // check if object is valid
    if (!file)
    {
        invalidInput();
    }
    string line;
    string messageContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    transform(messageContent.begin(), messageContent.end(), messageContent.begin(), ::tolower);
    return messageContent;
}

/**
 * reads the threshold argument and returns it by value if qualifies as positive integer.
 * @param t input string supposed to contain the threshold.
 * @return the parsed threshold.
 */
int parseThreshold(const string &t)
{
    int res;
    if (parseScore(t, res))
    {
        if (res > 0)
        {
            return res;
        }
    }
    invalidInput();
    return FAIL;
}

/**
 * given a message, this function checks for occurences of phrases from the Spam Filter given to
 * the program and for every occurence add the phrase score to a total score. lastly, if the
 * final score passes the threshold provided, meaning the msg is a spam msg, the function will
 * return true.
 * @param msg message to rank
 * @param filter hashmap containing phrases and their scores.
 * @param threshold - positive integer .
 * @return true if total score for msg greater or equal to the threshold.
 */
bool rankMsg(const string &msg, const Filter &filter, const int &threshold)
{
    int rank = 0;
    for (auto &elem : filter)
    {
        int occurrences = 0;
        size_t pos = 0;
        while ((pos = msg.find(elem.first, pos)) != string::npos)
        {
            ++occurrences;
            pos += elem.first.length();
        }
        rank += elem.second * occurrences;
    }
    return rank >= threshold;
}

/**
 * main function of the SpamDetector.
 */
int main(int argc, char *argv[])
{
    if (argc != NUM_ARGS)
    {
        usage();
        return EXIT_FAILURE;
    }
    try
    {
        Filter DB;
        parseDataBaseFile(DB_PATH_ARG, DB);
        string msg = parseTextFile(MSG_PATH_ARG);
        if (rankMsg(msg, DB, parseThreshold(THRESHOLD_ARG)))
        {
            cout << DECLARE_SPAM << endl;
        }
        else
        {
            cout << DECLARE_NOT_SPAM << endl;
        }
        return EXIT_SUCCESS;
    }
    catch (invalid_argument& e)
    {
        return EXIT_FAILURE;
    }
}
