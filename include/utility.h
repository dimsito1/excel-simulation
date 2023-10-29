#include <string>
#include <fstream>

#ifndef _UTILITY_HEADER_INCLUDED_
#define _UTILITY_HEADER_INCLUDED_

namespace utility {
    char** processInputIntoArray(const char* input);
    bool boolArrIsAllFalseOrAllTrue(const bool* array, const size_t size);
    bool isDigitInteger(const char* number);
    const char* getStringAfterQuote(const char* string);
    bool isValidNumber(const char* number);
    bool isDouble(const char* charNumber);
    std::string extractNumbersFromString(const std::string& string);
    std::string extractFormulaFromString(const std::string& string);
    std::string removeExtraSpacesFromString(const std::string& string);
    std::string removeSpacesFromString(const std::string& string);
    bool fileExists(const std::string& filename);
    bool charArrEndsCorrectly(const char* input, const char* endInput);
    size_t wordsCounter(const char* input);
    size_t wordsCounter2(const char* input);
    void freeInputArray(char** inputArray, const size_t numberOfWords);
    bool validInput(const char* input);
    const char* makeStringValueForCell(const char* string);
    const char* fixFileString(const char* string);
    
    inline bool isEmptyFile(std::ifstream& iFile) {
        return iFile.peek() == std::ifstream::traits_type::eof();
    }
    inline bool isDigit(const char ch) {
        return ch >= '0' && ch <= '9';
    }
    inline bool isLetter(const char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z');
    }
}

#endif