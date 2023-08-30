#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include "include/utility.h"

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

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

bool utility::boolArrIsAllFalseOrAllTrue(const bool* array, const size_t size) {
    for (int i = 1; i < size; ++i) {
        if (array[i] != array[0]) {
            return false;
        }
    }
    return true;
}

bool utility::isDigitInteger(const char* number) {
    for (size_t i = 0; i < strlen(number); ++i) {
        if (!isDigit(number[i])) {
            return false;
        }
    }
    return true;
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

bool utility::isValidNumber(const char* charNumber) {
    size_t len = strlen(charNumber);
    bool hasDot = false;
    for (size_t i = 0; i < len; i++) {
        if (charNumber[i] < '0' || charNumber[i] > '9') {
            if (i == 0 && len > 1 && (charNumber[i] == '-' || charNumber[i] == '+')) {
                // Allow a leading minus or plus sign
                continue;
            }
            else if (charNumber[i] == '.' && !hasDot && i > 0 && i < len - 1) {
                // Allow one dot, but not at the beginning or end
                hasDot = true;
            }
            else {
                return false;
            }
        }
    }
    return true;
}

bool utility::isDouble(const char* charNumber){
    bool isDouble = false;
    for (size_t i = 0; i < strlen(charNumber); i++){
        if (charNumber[i] == '.') {
            isDouble = true;
        }
    }
    return isDouble;
}

std::string utility::extractNumbersFromString(const std::string& string) {
    std::string newString;
    for (size_t i = 0; i < string.size(); i++) {
        if (utility::isDigit(string[i])) {
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
        if (!utility::isDigit(string[i]) && string[i] != 'R' && string[i] != 'C') {
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

bool utility::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool utility::charArrEndsCorrectly(const char* input, const char* endInput) {
    size_t sizeInput = strlen(input);
    size_t sizeEndInput = strlen(endInput);

    for (size_t i = 0; i < sizeEndInput; ++i) {
        if (input[sizeInput - 1 - i] != endInput[sizeEndInput - 1 - i]) {
            return false;
        }
    }
    return true;
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

bool utility::validInput(const char* input) {
    size_t length = strlen(input);

    if (input[0] == ' ' || input[length - 1] == ' '){
        return false;
    }

    for (size_t i = 0; i < length - 1; ++i) {
        if (input[i] == ' ' && input[i + 1] == ' ') {
            return false;
        }
    }
    return true;
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