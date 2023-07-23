#include "excel.h"
#include "terminal.h"
#include "vector.h"
#include <string>
#include <cstring>

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

const bool isDigit(const char ch) {
    return ch >= '0' && ch <= '9';
}

const bool isLetter(const char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z');
    
}

const bool isValidNumber(const char* charNumber){
    for (size_t i = 0; i < strlen(charNumber); i++){
        if (charNumber[i] < '0' || charNumber[i] > '9'){
            return false;
        }
    }
    return true;
}

std::string removeExtraSpacesFromString(const std::string& string) {
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

std::string removeSpacesFromString(const std::string& string) {
    std::string newString;
    for (size_t i = 0; i < string.size(); ++i) {
        if (string[i] != ' ') {
            newString += string[i];
        }
    }
    return newString;
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool charArrEndsCorrectly(const char* input, const char* endInput) {
    size_t sizeInput = strlen(input);
    size_t sizeEndInput = strlen(endInput);

    for (size_t i = 0; i < sizeEndInput; ++i) {
        if (input[sizeInput - 1 - i] != endInput[sizeEndInput - 1 - i]) {
            return false;
        }
    }
    return true;
}

size_t wordsCounter(const char* input) {
    size_t size = strlen(input);
    size_t spaceCounter = 0;

    for (size_t i = 0; i < size; i++) {
        if (input[i] == ' ') {
            spaceCounter++;
        }
    }
    return spaceCounter + 1;
}

void freeInputArray(char** inputArray, const size_t numberOfWords) {
    for (size_t i = 0; i < numberOfWords; ++i) {
        delete[] inputArray[i];
    }
    delete[] inputArray;
}

const bool validInput(const char* input) { //while(validInput) {}
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

bool isEmptyFile(std::ifstream& iFile) {
    return iFile.peek() == std::ifstream::traits_type::eof();
}

/******************************************************************************************************/
char** Terminal::processInputIntoArray(const char* input) { //delete dynamic memory
    //processes for example const char* "open new file" into [[open'\0'][new'\0'][file'\0']]
    if (input == nullptr) {
        std::cerr << "input is nullptr" << std::endl;
        return nullptr;
    }
    const size_t numberOfWords = wordsCounter(input);

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

const bool Terminal::isLineValid(const std::string& line) const {
    bool quoteOpened = false;
    bool dotFound = false;
    bool specialQuoteOpened = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char current = line[i];
        cout << "current: " << current << endl;
        char next = i + 1 < line.size() ? line[i + 1] : '\0';
        // std::cout << "Current char: " << current << endl;
        switch (current)
        {
        case ',':
            if (quoteOpened) {
                break;
            }
            if (specialQuoteOpened) {
                break;
            }
            if (!(next == '=' || next == '+' || next == '-' || next == '\\' ||next == '\"' || isDigit(next) || next == ',' || next == '\0' || next == '\n')) {
                return false;
            }
            dotFound = false;
            break;
        case '=':
            if (next != '=') {
                return false;
            }
            break;
        case '!':
            if (next != '=') {
                return false;
            }
            break;
        case '<':
            if (next != '=' && !isDigit(next)) {
                return false;
            }
            break;
        case '>':
            if (next != '=' && !isDigit(next)) {
                    return false;
            }
            break;
        case '\"' :
            if (specialQuoteOpened) {
                break;
            }
            if (!specialQuoteOpened && line[i - 1] == '\\') {
                break;
            }

            if (quoteOpened) {
                quoteOpened = false;
                if (next != ',' && i + 1 < line.size()) {
                    return false;
                }
            }
            else {
                quoteOpened = true;
            }
            break;
        case '\\' :
            if (i + 1 < line.size() && next == '\"') {
                // this is your special \" case
                if (quoteOpened) {
                    break;
                }
                
                if (specialQuoteOpened) {
                    specialQuoteOpened = false;
                    
                    if (i + 2 < line.size()) {
                        return false;
                    }
                } 
                else {
                    specialQuoteOpened = true;
                }
            }
            if (!quoteOpened && next != '\"') {
                return false;
            }
            break;

        case '.' :
            if (quoteOpened) {
                break;
            }
            if (specialQuoteOpened) {
                break;
            }
            else if (!quoteOpened && !isDigit(next) && !specialQuoteOpened) {
                return false;
            }
            if (dotFound) {
                return false;
            }
            dotFound = true;
            break;
        default:
            if (!isDigit(current) && !specialQuoteOpened && !quoteOpened &&current != ',' && current != '\"' &&
                current != '=' && current != '+' && current != '-' &&
                current != '<' && current != '>' && current != '.' &&
                current != '\0' && current != '\n') {
                    return false;
                }
            break;
        }
    }
    // The quote should be closed and no dot should be found
    // at the end of a valid line
    return !quoteOpened && !dotFound && !specialQuoteOpened;
}

const bool Terminal::isFileValid(std::ifstream& iFile) const {
    if (!iFile) {
        cerr << "ERROR OPENING FILE ( Terminal::isFileValid )" << endl;
        return false;
    }

    char symbol;
    std::string input;
    std::string newInput;

    while (std::getline(iFile, input)) {
        newInput = removeSpacesFromString(input);
        if (!isLineValid(newInput)) {
            return false;
        }
    }
    return true;
}

const char* Terminal::createTextFile() {
    cout << "Enter a name for a new text file: ";

    char newInput[256];
    cin.getline(newInput, 256);

    size_t numberOfWords = wordsCounter(newInput);

    if (!validInput(newInput) || (!charArrEndsCorrectly(newInput, ".txt")) || numberOfWords != 1) {
        cout << "Invalid input. (file should be .txt)" << endl;
        return nullptr;
    }

    std::ofstream newFile(newInput);
    cout << "New text file \"" << newInput << "\" created successfully." << endl;
    newFile.close();

    char* result = new char[strlen(newInput) + 1];
    strcpy(result, newInput);

    return result;        
    // TO DO: handle newly created file
}

const char* Terminal::openTextFile(const char* input) {
    std::ifstream iFile(input);
    if (!iFile) {
        cerr << "The file you provided does not exist."<< endl;
        iFile.close();

        const char* newInput = createTextFile();
        //TODO: deal with the dynamic memory

        return newInput;
    }
    
    if (isEmptyFile(iFile)) {
        cerr << "The file you provided was empty." << endl;
        return nullptr;
    }

    cout << "File opened successfully." << endl;
    cout << "Excel loaded successfully." << endl;

    processTextFileIntoExcel(iFile);
    iFile.close();
}


void Terminal::processTextFileIntoExcel(std::ifstream& iFile) { //TO DO readTextFile
    if (!iFile) {
        cerr << "ERROR OPENING FILE ( readTextFile )" << endl;
        cerr << "Error state: " << iFile.rdstate() << endl;
        return;
    }
    // checks whether the file is valid
    if (!isFileValid(iFile)) {
        cerr << "The spreadsheet you provided was invalid. Please make sure it is valid." << endl;
        return; 
    }

    size_t commaCounter = 0;
    Vector commaArray;
    char symbol;
    bool quoteOpened = false;

    iFile.clear(); // Clear any flags
    iFile.seekg(0, std::ios::beg); 

    while (iFile.get(symbol)) {
        if (symbol == '\"') {
            quoteOpened == true ? quoteOpened = false : quoteOpened = true;
        }
        if (symbol == ',' && !quoteOpened) {
            commaCounter++;
        }
        else if (symbol == '\n') {
            commaArray.pushBack(commaCounter);
            commaCounter = 0;
        }
    }

    if (iFile.eof()) {
        commaArray.pushBack(commaCounter);
    }
    else {
        cerr << "ERROR END OF FILE NOT REACHED ( Terminal::readTextFile )" << endl;
        return;
    }

    //delete this part when finished
    // cout << "Number of Rows: " << commaArray.getSize() << endl;
    // for (size_t i = 0; i < commaArray.getSize(); ++i) {
    //     cout << "Number of commas in row " << i << ' ' << commaArray[i] << endl;
    // }

    const int newRows = commaArray.getSize();
    const int newColumns = commaArray.getMaxElement() + 1;
    // cout << "Number of Columns: " << newColumns << endl;
    
    Excel newExcel(newRows, newColumns);

    operateExcel(newExcel, iFile, commaArray);
}

void Terminal::operateExcel(Excel &excel, std::ifstream &iFile, const Vector _commaArray) {
    changeEditMode(true);

    iFile.clear(); // Clear any flags
    iFile.seekg(0, std::ios::beg);

    if (!iFile) {
        cerr << "ERROR OPENING FILE ( Terminal::operateExcel )" << endl;
        return;
    }

    std::string line;
    std::string newLine;
    std::string quoteLine;
    bool quoteOpened = false;

    std::cout << "Excel being worked on." << std::endl;
    for (size_t i = 0; i < excel.getNumberOfRows(); ++i) {
        std::getline(iFile, line);
        newLine = removeSpacesFromString(line);
        quoteLine = newLine;

        size_t startPos = 0;
        size_t j = 0;

        while (j < excel.getNumberOfColumns() - 1) {
            size_t quotePos = quoteLine.find('"');
            
            if (quotePos != std::string::npos) {
                quoteOpened = !quoteOpened;
                quoteLine = quoteLine.substr(quotePos + 1);
            }

            size_t commaPos = newLine.find(',', startPos);
            std::string cellValue;

            if (commaPos != std::string::npos || !quoteOpened) {
                cellValue = newLine.substr(startPos, commaPos - startPos);
                startPos = commaPos + 1;
            }
            else {
                cellValue = newLine.substr(startPos);
                startPos = newLine.size();
            }
            // If the cell value is empty, set it to "0"
            if (cellValue.empty()) {
                cellValue = "0";
                excel.setElementInMatrix(i, j, "0", CellType::Default);
                j++;
                continue;
            }
            excel.setElementInMatrix(i, j, cellValue.c_str(), CellType::String);
            ++j;
        }
        // If there are still columns remaining, fill them with "0"
        while (j < excel.getNumberOfColumns()) {
            excel.setElementInMatrix(i, j, "0", CellType::Default);
            ++j;
        }
    }
    cout << "-----SPREADSHEET-----" << endl;
    excel.printSpreadsheet();
    
    cout << "-------EXCEL----------" << endl;
    excel.printExcel();

    cout << "----TYPES-----" << endl;

    for (size_t i = 0; i < excel.getNumberOfRows(); ++i) {
        excel.printCellTypeRow(i);
    }

    currentExcel = excel;
}

void Terminal::processCommand(const char* string, bool& flag) {
    const size_t numberOfWords = wordsCounter(string);
    char** InputArray = processInputIntoArray(string);

    if (strcmp(InputArray[0], "open") == 0 && numberOfWords == 2 && validInput(string) && editMode == false) {
        openTextFile(InputArray[1]);
        
        freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "exit") == 0 && numberOfWords == 1 && validInput(string)) {
        freeInputArray(InputArray, numberOfWords);
        flag = true;
    }

    else if (strcmp(InputArray[0], "close") == 0 && numberOfWords == 1 && validInput(string) && editMode == true) {
        //closeExcel();
        changeEditMode(false);        
        cout << "Excel closed successfully." << endl;
    }

    else if (strcmp(InputArray[0], "print") == 0 && numberOfWords == 1 && validInput(string)) {
        if (editMode) {
            currentExcel.printExcel();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }
    }

    else {
        editMode == true ? cerr << "You are in edit mode. (use \"close\" to stop edit mode)" << endl : cerr << "Incorrect input. " << endl;
    }
}