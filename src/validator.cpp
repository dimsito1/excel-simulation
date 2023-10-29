#include "validator.h"
#include "utility.h"
#include "terminal.h"
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;

std::string validate::isLineValid(const std::string& line, const unsigned rowNumber) {
    bool quoteOpened = false;
    bool dotFound = false;
    bool quoteShouldNotBeOpened = false;
    bool formulaStarted = false;
    bool formulaLettersFound[4] = {};

    unsigned currentRow = rowNumber;
    unsigned currentColumn = 0;

    bool isValid = true;
    std::string errorCharacter;
    std::string errorMessage;
    std::string tempString;

    for (size_t i = 0; i < line.size(); ++i) {
        char current = line[i];
        char next = i + 1 < line.size() ? line[i + 1] : '\0';
        char prev = i > 0 ? line[i - 1] : '\0';
        switch (current)
        {
        case ',':
            dotFound = false;
            if (formulaStarted) {
                if (!validate::boolArrIsAllFalseOrAllTrue(formulaLettersFound, 4)) {
                    errorMessage = "formula not finished at: ";

                    isValid = false;
                    goto endOfLoop;

                }
                else {
                    for (size_t i = 0; i < 4; ++i) {
                        formulaLettersFound[i] = false;
                    }
                }
            }
            if (formulaStarted) {
                formulaStarted = false;
            }
            if (!quoteOpened) {
                if (isValid) {
                    currentColumn++;
                }
                break;
            }
            if (quoteOpened) {
                break;
            }
            if (!(next == '=' || next == '+' || next == '-' || next == '\\' || next == '\"'
            || validate::isDigit(next) || next == ',' || next == '\0' || next == '\n' || next == 'R')) {
                utility::getErrorMessage(errorMessage, next);

                isValid = false;
                goto endOfLoop;
            }
            break;
        case 'C':
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted || next ==',') {
                utility::getErrorMessage(errorMessage, current);
                    
                isValid = false;
                goto endOfLoop;
            }
            else if (formulaStarted && !validate::isDigit(next)) {
                utility::getErrorMessage(errorMessage, next);
                    
                isValid = false;
                goto endOfLoop;
            }
            else if (!formulaLettersFound[1]) {
                formulaLettersFound[1] = true;
            }
            else if (!formulaLettersFound[3]) {
                formulaLettersFound[3] = true;
            }
            else if (formulaLettersFound[1] && formulaLettersFound[3]) {
                utility::getErrorMessage(errorMessage, next);
                    
                isValid = false;
                goto endOfLoop;
            }
            break;
        case 'R':
            if (quoteOpened) {
                break;
            }
            else if (!validate::isDigit(next) || next == ',') {
                utility::getErrorMessage(errorMessage, next);

                isValid = false;
                goto endOfLoop;
            }
            else if (!formulaLettersFound[0]) {
                formulaLettersFound[0] = true;
                formulaStarted = true;
            }
            else if (!formulaLettersFound[2]) {
                formulaLettersFound[2] = true;
            }
            else if (formulaLettersFound[0] && formulaLettersFound[2]) {
                utility::getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }

            break;
        case '<':
        case '>':
            if (quoteOpened) {
                break;
            }
            else if (next == 'R' && formulaStarted) {
                break;
            }
            else if (!formulaStarted || !validate::isDigit(prev) || (next != 'R' && next != '=')) {
                utility::getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            break;
        case '=': 
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted) {
                utility::getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            else if ((next == '=' && !validate::isDigit(prev)) || (prev == '=' && next != 'R')) {
                utility::getErrorMessage(errorMessage, current);
                
                isValid = false;
                goto endOfLoop;
            }
        break;

        case '+':
        case '-':
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted && !validate::isDigit(next) && prev != ',' && prev != '\0') {
                utility::getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            else if (formulaStarted && (next != 'R' || !validate::isDigit(prev))) {
                utility::getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            break;

        case '/':
        case '*':
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted || next != 'R') {
                utility::getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            else if (formulaStarted) {
                if (!validate::isDigit(prev) || next != 'R') {
                    utility::getErrorMessage(errorMessage, current);

                    isValid = false;
                    goto endOfLoop;
                }
            }
            break;
        case '\"' :
            if (quoteShouldNotBeOpened) {
                errorMessage = "Quote should not be opened at: ";
                isValid = false;
                goto endOfLoop;
            }
            if (quoteOpened && prev == '\\' && (next == ',' || next == '\n')) {
                quoteOpened = false;
                break;
            }
            if (quoteOpened && prev == '\\') {
                break;
            }

            if (quoteOpened) {
                quoteOpened = false;
                if (next != ',' && next != '\n') {
                    errorMessage = "Wrong use of character \"";
                    if (next == '\"' && next != ',') {
                        std::string tempNext = "\\\"";
                        errorMessage += tempNext;
                    }
                    else {
                        errorMessage += next;
                    }
                    errorMessage += "\" at: ";
                    isValid = false;
                    goto endOfLoop;
                }
            }
            else {
                quoteOpened = true;
            }
            break;

        case '\n' :
            if (quoteOpened) {
                errorMessage = "Quote not closed at: ";
                isValid = false;
                goto endOfLoop;
            }
            if (formulaStarted) {
                formulaStarted = false;
            }
            if (isValid) {
                currentColumn = 0;
            }

            break;

        case '\\' :
            if (i + 1 < line.size() && next == '\"') {
                if (quoteOpened) {
                    break;
                }
            }
            if (!quoteOpened) {
                errorMessage = "Wrong use of an extra dot \"\\\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            break;

        case '.' :
            if (quoteOpened) {
                break;
            }
            if (dotFound) {
                errorMessage = "Wrong use of an extra dot \".\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            
            if (!quoteOpened && !validate::isDigit(next)) {

                utility::getErrorMessage(errorMessage, next);

                isValid = false;
                goto endOfLoop;
            }
            dotFound = true;
            break;
        default:
            if (formulaStarted) {
                if (validate::isDigit(current)) {
                    if (next != 'C' && next != '-' && next != '+' && next != '*'
                    && next != ',' && next != '=' && next != '<' && next != '>'
                    && next != '/' && prev != 'R' && prev != 'C'&& !validate::isDigit(next)) {
                        
                        utility::getErrorMessage(errorMessage, next);

                        isValid = false;
                        goto endOfLoop;
                    }
                }
            }
            if (!validate::isDigit(current) && !quoteOpened &&current != ',' && current != '\"' &&
                current != '=' && current != '+' && current != '-' &&
                current != '<' && current != '>' && current != '.' &&
                current != '\0' && current != '\n' && current != 'R') {

                    utility::getErrorMessage(errorMessage, current);

                    isValid = false;
                    goto endOfLoop;
                }
            break;
        }
    }
    endOfLoop:

    // The quote should be closed and no dot should be found
    // at the end of a valid line
    // formula should be closed
    // bool array is either all true or all false
    
    bool areAllSame = validate::boolArrIsAllFalseOrAllTrue(formulaLettersFound, 4);
    
    if (!quoteOpened && !dotFound && isValid && !formulaStarted && areAllSame) {
        return "1";
    }

    else {
        tempString += errorMessage + 
                    '\n' + "Row: " + std::to_string(currentRow + 1) + ", " +
                    "Column: " + std::to_string(currentColumn + 1);
        return tempString;
    }
}

bool validate::isFileValid(std::ifstream& iFile) {
    if (!iFile) {
        cerr << "ERROR OPENING FILE ( Terminal::isFileValid )" << endl;
        return false;
    }

    char symbol;
    std::string input;
    std::string newInput;

    std::string isLineValidString;
    unsigned rowNumber = 0;

    while (std::getline(iFile, input)) {
        newInput = utility::removeSpacesFromString(input);
        newInput += '\n';
        isLineValidString = isLineValid(newInput, rowNumber);
        rowNumber++;
        if (isLineValidString != "1") {
            cerr << "The spreadsheet you provided was invalid. Please make sure it is valid." << endl;
            cerr << "Error message- " << isLineValidString << endl;
            return false;
        }
    }

    return true;
}

bool validate::isDigitInteger(const char* number) {
    for (size_t i = 0; i < strlen(number); ++i) {
        if (!validate::isDigit(number[i])) {
            return false;
        }
    }
    return true;
}

bool validate::isValidNumber(const char* charNumber) {
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

bool validate::isDouble(const char* charNumber){
    bool isDouble = false;
    for (size_t i = 0; i < strlen(charNumber); i++){
        if (charNumber[i] == '.') {
            isDouble = true;
        }
    }
    return isDouble;
}

bool validate::boolArrIsAllFalseOrAllTrue(const bool* array, const size_t size) {
    for (int i = 1; i < size; ++i) {
        if (array[i] != array[0]) {
            return false;
        }
    }
    return true;
}

bool validate::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool validate::cStringEndsCorrectly(const char* input, const char* endInput) {
    size_t sizeInput = strlen(input);
    size_t sizeEndInput = strlen(endInput);

    for (size_t i = 0; i < sizeEndInput; ++i) {
        if (input[sizeInput - 1 - i] != endInput[sizeEndInput - 1 - i]) {
            return false;
        }
    }
    return true;
}

bool validate::isValidInput(const char* input) {
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

bool validate::isStringValidForCell(std::string& string) {
    bool dotFound = false;
    size_t size = string.size();

    if (string[0] == '\"') {
        if (string[size - 1] != '\"') {
            return false;
        }
    }

    if (string[0] == '+' || string[0] == '-' || validate::isDigit(string[0])) {
        for (size_t i = 1; i < size - 1; ++i) {
            if (string[i + 1] == '.') {
                if (i == 0) {
                    return false;
                }
                if (dotFound) {
                    return false;
                }
                dotFound = true;
            }
            if (!validate::isDigit(string[i]) && string[i] != '.') {
                return false;   
            }
        }
        return true;
    }

    else if (string[0] == '\"' && string[size - 1] == '\"') {
        for (size_t i = 1; i < size - 1; ++i) {
            if (string[i] == ' ') {
                continue;
            }
            if (string[i] == '\\' && string[i + 1] == '\"') {
                i += 2;
                continue;
            }
            if (string[i] == '\"' && string[i - 1]) {
                return false;
            }
            else if (string[i] == '\"' && i != size - 1 && string[i - 1] != '\\') {
                return false;
            }
        }
        return true;
    }
    return false;
}