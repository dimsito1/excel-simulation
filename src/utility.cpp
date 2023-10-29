#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include "utility.h"
#include "cell.h"
#include "formulaType.h"
#include "validator.h"

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

void utility::getErrorMessage(std::string& errorMessage, const char errorChar) {
    errorMessage = "Wrong use of character \"";
    errorMessage += errorChar;
    errorMessage += "\" at: ";
}

const std::string utility::getResultFromFormula(const long double& firstValue, const long double& secondValue, const FormulaType& formulaType) {
    std::string resultString;
    long double resultNumber = 0.0L;
    std::ostringstream oss;

    switch (formulaType)
    {
    case FormulaType::Addition:
        resultNumber = firstValue + secondValue;
        oss << resultNumber;
        resultString = oss.str();
        break;

    case FormulaType::Subtraction:
        resultNumber = firstValue - secondValue;
        oss << resultNumber;
        resultString = oss.str();
        break;

    case FormulaType::BiggerOrEqualThan:
        if (firstValue >= secondValue) {
            resultString += '1';
            break;
        }
        resultString += '0';
        break;

    case FormulaType::BiggerThan:
        if (firstValue > secondValue) {
            resultString += '1';
            break;
        }
        resultString += '0';
        break;

    case FormulaType::Division:
        if (secondValue == 0) {
            resultString += "ERROR";
            break;
        }
        resultNumber = firstValue / secondValue;
        oss << resultNumber;
        resultString = oss.str();
        break;

    case FormulaType::Multiplication:
        resultNumber = firstValue * secondValue;
        oss << resultNumber;
        resultString = oss.str();
        break;

    case FormulaType::SmallerOrEqualThan:
        if (firstValue <= secondValue) {
            resultString += '1';
            break;
        }
        resultString += '0';
        break;

    case FormulaType::SmallerThan:
        if (firstValue < secondValue) {
            resultString += '1';
            break;
        }
        resultString += '0';
        break;
    }
    
    return resultString;
}

const FormulaType utility::getFormulaType(const char firstChar, const char secondChar) {
    if (secondChar == '0') {
        switch (firstChar)
        {
        case '+':
            return FormulaType::Addition;
        case '-':
            return FormulaType::Subtraction;
        case '*':
            return FormulaType::Multiplication;
        case '/':
            return FormulaType::Division;
        case '<':
            return FormulaType::SmallerThan;
        case '>':
            return FormulaType::BiggerThan;
        default:
        // Handle unexpected characters
            cerr << "Error unsupported formula type." << endl;
            return FormulaType::None;
        }
    } 
    else {
        switch (firstChar)
        {
        case '<':
            return FormulaType::SmallerOrEqualThan;
        case '>':
            return FormulaType::SmallerOrEqualThan;
        default:
        // Handle unexpected characters
            cerr << "Error unsupported formula type." << endl;
            return FormulaType::None;
        }
    }
}

const CellType utility::getStringCellType(const char* string)  {
    //assuming the Cell has correct values.
    if (string[0] == 'R') {
        return CellType::Formula;
    }
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == '\"') {
            return CellType::String;
        }
        if (string[i] == '.') {
            return CellType::Double;
        }
        if (string[i] > '9' || string[i] < 9) {
            return CellType::String;
        }
    }
    return CellType::Integer;
}

char** utility::processInputIntoArray(const char* input) {
    //processes for example const char* "open new file" into [[open'\0'][new'\0'][file'\0']]
    if (input == nullptr) {
        std::cerr << "input is nullptr" << std::endl;
        return nullptr;
    }
    const size_t numberOfWords = utility::wordsCounter(input);

    char** inputArray = new char*[numberOfWords];

    size_t wordLenCounter = 0;
    size_t currentPosition = 0;
    size_t currentPositionCopy = 0;

    for (size_t i = 0; i < numberOfWords; ++i) {
        for (size_t j = 0; input[j + currentPosition] != ' ' && input[j + currentPosition] != '\0'; ++j) {
            wordLenCounter++;
            currentPositionCopy++;
        }
        inputArray[i] = new char[wordLenCounter + 1];

        for (size_t j = 0; j < wordLenCounter; ++j) {
            inputArray[i][j] = input[j + currentPosition];
        }
        inputArray[i][wordLenCounter] = '\0';

        currentPositionCopy++;
        currentPosition = currentPositionCopy;

        wordLenCounter = 0;
    }
    return inputArray;
}

const char* utility::getStringAfterQuote(const char* string) {
    bool quoteFound = false;
    size_t j = 0;
    char* newString = new char[strlen(string) + 1];
    for (size_t i = 0; i < strlen(string); i++) {

        if (quoteFound) {
            newString[j++] = string[i];
        }
        if (string[i] == '\"' && !quoteFound) {
            quoteFound = true;
            newString[j++] = '\"';
        }
    }
    newString[j] = '\0';

    return newString;
} 

std::string utility::extractNumbersFromString(const std::string& string) {
    std::string newString;
    for (size_t i = 0; i < string.size(); i++) {
        if (validate::isDigit(string[i])) {
            newString += string[i];
        } else if (!newString.empty() && newString.back() != ' ') {
            newString += ' ';
        }
    }
    return newString;
}

std::string utility::extractFormulaFromString(const std::string& string) {
    std::string newString;
    for (size_t i = 0; i < string.size(); i++) {
        if (!validate::isDigit(string[i]) && string[i] != 'R' && string[i] != 'C') {
            newString += string[i];
        }
    }
    if (newString.size() == 1) {
        newString.push_back('0');
    }
    return newString;
}

std::string utility::removeExtraSpacesFromString(const std::string& string) {
    std::string newString;
    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] == ' ' && string[i + 1] != ' ') {
            newString += string[i];
        }
        else if (string[i] != ' ') {
            newString += string[i];
        }
    }
    return newString;
}

std::string utility::removeSpacesFromString(const std::string& string) {
    std::string newString;
    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] != ' ') {
            newString += string[i];
        }
    }
    return newString;
}

size_t utility::wordsCounter(const char* input) {
    size_t size = strlen(input);
    size_t spaceCounter = 0;

    for (size_t i = 0; i < size; i++) {
        if (input[i] == ' ') {
            spaceCounter++;
        }
    }
    return spaceCounter + 1;
}

size_t utility::wordsCounter2(const char* input) {
    size_t size = strlen(input);
    size_t wordCount = 0;
    bool inWord = false;

    for (size_t i = 0; i < size; i++) {
        if (input[i] != ' ' && !inWord) {
            inWord = true;
            wordCount++;
        } else if (input[i] == ' ' && inWord) {
            inWord = false;
        }
    }
    return wordCount;
}

void utility::freeInputArray(char** inputArray, const size_t numberOfWords) {
    for (size_t i = 0; i < numberOfWords; ++i) {
        delete[] inputArray[i];
    }
    delete[] inputArray;
}

const char* utility::makeStringValueForCell(const char* string) {
    const size_t size = strlen(string);

    char* newString = new char[size + 1];
    size_t newSize = 0;
    for (size_t i = 1; i < size - 1; i++) {
        if (string[i] == '\\' && string[i + 1] == '\"') {
            newString[newSize++] = '\"';
            i++;
        } else {
            newString[newSize++] = string[i];
        }
    }
    newString[newSize] = '\0';
    return newString;
}

const char* utility::fixFileString(const char* string) {
    size_t length = strlen(string);
    
    size_t numQuotes = 0;
    for (size_t i = 0; i < length; i++) {
        if (string[i] == '\"') {
            numQuotes++;
        }
    }

    char* newString = new char[length + numQuotes + 1];
    size_t newLength = 0;

    for (size_t i = 0; i < length; i++) {
        if (string[i] == '\"') {
            newString[newLength++] = '\\';
            newString[newLength++] = '\"'; 
        }
        else {
            newString[newLength++] = string[i];
        }
    }
    newString[newLength] = 0;
    return newString;
}