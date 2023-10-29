
#ifndef _UTILITY_HEADER_INCLUDED_
#define _UTILITY_HEADER_INCLUDED_

#include <string>
#include <fstream>
#include "cell.h"
#include "formulaType.h"

namespace utility {
    void getErrorMessage(std::string& errorMessage, const char errorChar);
    const std::string getResultFromFormula(const long double& firstValue, const long double& secondValue, const FormulaType& formulaType);
    const CellType getStringCellType(const char* string);
    const FormulaType getFormulaType(const char firstChar, const char secondChar);
    const char* getStringAfterQuote(const char* string);

    char** processInputIntoArray(const char* input);
    void freeInputArray(char** inputArray, const size_t numberOfWords);

    std::string extractNumbersFromString(const std::string& string);
    std::string extractFormulaFromString(const std::string& string);
    std::string removeExtraSpacesFromString(const std::string& string);
    std::string removeSpacesFromString(const std::string& string);

    size_t wordsCounter(const char* input);
    size_t wordsCounter2(const char* input);

    const char* makeStringValueForCell(const char* string);
    const char* fixFileString(const char* string);
}

#endif