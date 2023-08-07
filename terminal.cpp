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

const bool isValidNumber(const char* charNumber) {
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

const bool isDouble(const char* charNumber){
    bool isDouble = false;
    for (size_t i = 0; i < strlen(charNumber); i++){
        if (charNumber[i] == '.') {
            isDouble = true;
        }
    }
    return isDouble;
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

const std::string Terminal::isLineValid(const std::string& line) const {
    bool quoteOpened = false;
    bool dotFound = false;
    bool specialQuoteOpened = false;

    int currentRow = 0;
    int currentColumn = 0;

    bool isValid = true;
    std::string errorMessage;
    std::string tempString;

    for (size_t i = 0; i < line.size(); ++i) {
        char current = line[i];
        cout << "current: " << current << endl;
        char next = i + 1 < line.size() ? line[i + 1] : '\0';
        // std::cout << "Current char: " << current << endl;
        switch (current)
        {
        case ',':
            if (!quoteOpened && !specialQuoteOpened) {
                currentColumn++;
                cout << "COLUMN: " << currentColumn << endl;
            }
            if (quoteOpened) {
                break;
            }
            if (specialQuoteOpened) {
                break;
            }
            if (!(next == '=' || next == '+' || next == '-' || next == '\\' ||next == '\"' || isDigit(next) || next == ',' || next == '\0' || next == '\n')) {
                isValid = false;
            }
            dotFound = false;
            break;
        case '=':
            if (next != '=') {
                isValid = false;
                break;
            }
            break;
        case '!':
            if (next != '=') {
                isValid = false;
                break;
            }
            break;
        case '<':
            if (next != '=' && !isDigit(next)) {
                isValid = false;
                break;
            }
            break;
        case '>':
            if (next != '=' && !isDigit(next)) {
                isValid = false;
                break;
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
                    isValid = false;
                    break;
                }
            }
            else {
                quoteOpened = true;
            }
            break;

        case '\n' :
            cout << "SPECIAL CASE" << endl;
            if (isValid) {
                currentColumn = 0;
            }
            currentRow++;
            break;

        case '\\' :
            if (i + 1 < line.size() && next == '\"') {
                // this is your special \" case
                if (quoteOpened) {
                    break;
                }
                
                if (specialQuoteOpened) {
                    specialQuoteOpened = false;
                    
                    if (i + 3 < line.size()) {
                        isValid = false;
                        break;
                    }
                } 
                else {
                    specialQuoteOpened = true;
                }
            }
            if (!quoteOpened && next != '\"') {
                isValid = false;
                break;
            }
            break;

        case '.' :
            if (quoteOpened) {
                break;
            }
            if (specialQuoteOpened) {
                break;
            }
            if (!quoteOpened && !specialQuoteOpened && isDigit(next)) {
                break;
            }

            else if (!quoteOpened && !isDigit(next) && !specialQuoteOpened) {
                isValid = false;
                break;
            }
            if (dotFound) {
                isValid = false;
                break;
            }
            dotFound = true;
            break;
        default:
            if (!isDigit(current) && !specialQuoteOpened && !quoteOpened &&current != ',' && current != '\"' &&
                current != '=' && current != '+' && current != '-' &&
                current != '<' && current != '>' && current != '.' &&
                current != '\0' && current != '\n') {
                    isValid = false;
                    break;
                }
            break;
        }
    }
    cout << "Row: " << currentRow << ' ' << "Col: " << currentColumn << endl; 
    // The quote should be closed and no dot should be found
    // at the end of a valid line
    if (!quoteOpened && !dotFound && !specialQuoteOpened && isValid) {
        return "1";
    }
    else {
        tempString += std::to_string(currentRow) + ' ' + std::to_string(currentColumn) + ' ' + errorMessage;
        return tempString;
    }
}

const bool Terminal::isFileValid(std::ifstream& iFile) const {
    if (!iFile) {
        cerr << "ERROR OPENING FILE ( Terminal::isFileValid )" << endl;
        return false;
    }

    char symbol;
    std::string input;
    std::string newInput;

    std::string isLineValidString;

    while (std::getline(iFile, input)) {
        newInput = removeSpacesFromString(input);
        newInput += '\n';
        isLineValidString = isLineValid(newInput);
        if (isLineValidString != "1") {
            cout << "Error message: " << isLineValidString << endl;
            return false;// HANDLE RETURN ERROR MESSAGE
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

    importDataIntoExcel(iFile);
    iFile.close();
}


void Terminal::importDataIntoExcel(std::ifstream& iFile) { //TO DO readTextFile
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

    iFile.clear(); // Clear any flags
    iFile.seekg(0, std::ios::beg); 

    size_t commaCounter = 0;
    Vector commaArray;
    char symbol;
    bool quoteOpened = false;
    bool specialQuoteOpened = false;

    while (iFile.get(symbol)) {
        if (symbol == '\\') {
            if (iFile.peek() == '\"') {
                specialQuoteOpened = !specialQuoteOpened;
            }
        }
        if (symbol == '\"') {
            quoteOpened = !quoteOpened;
        }
        if (symbol == ',' && !quoteOpened && !specialQuoteOpened) {
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

    bool quoteOpened = false;
    bool specialQuoteOpened = false;
    char symbol;

    int excelRows = excel.getNumberOfRows();
    int excelColumns = excel.getNumberOfColumns();

    int j = 0;
    int i = 0;
    std::string cellValue;

    while (iFile.get(symbol)) {
        if (symbol == ' ' && !quoteOpened && !specialQuoteOpened) {
            continue;
        }

        else if (symbol == '\n') {
            if (cellValue.empty()) {
                excel.setElementInMatrix(i, j, "", CellType::Default);
            }
            else {
                setCellBasedOnType(excel, cellValue, i, j);
            }
            cellValue.clear();
            j++;
            while (j < excelColumns) {
                excel.setElementInMatrix(i, j, "", CellType::Default);
                j++;                
            }
            i++;
            j = 0;
        }
        
        else if (symbol == ',' && !specialQuoteOpened && !quoteOpened) {
            if (cellValue.empty()) {
                excel.setElementInMatrix(i, j, "", CellType::Default);
                j++;
            }
            else {
                setCellBasedOnType(excel, cellValue, i, j);
                j++;
                cellValue.clear();
            }
        }
        
        else if (symbol == '\\') {
            if (iFile.peek() == '\"') {
                specialQuoteOpened = !specialQuoteOpened;
                iFile.ignore(1);
                cellValue += '\"';
            }
        }
        
        else if (symbol == '\"' && !specialQuoteOpened) {
            quoteOpened = !quoteOpened;
        }

        else if (symbol == ' ' && (quoteOpened || specialQuoteOpened)) {
        cellValue += symbol;
        }

        else if (symbol != ' ') {
            cellValue += symbol;
        }

        else {
            cellValue += symbol;
        }
    }

    if (iFile.eof()) {
        if (cellValue.empty()) {
            excel.setElementInMatrix(i, j, "", CellType::Default);
            j++;
        }
        else {
            setCellBasedOnType(excel, cellValue, i, j);
            j++;
        }

        int remainingCells = excelColumns - j;

        for (size_t k = 0; k < remainingCells; ++k) {
            excel.setElementInMatrix(i, j, "", CellType::Default);
            j++;
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

void Terminal::setCellBasedOnType(Excel& _excel, const std::string& cellValue, const int _rows, const int _columns) {
    const char* charValue = cellValue.c_str();
    if (isValidNumber(charValue)) {
        if (isDouble(charValue)) {
            _excel.setElementInMatrix(_rows , _columns, charValue, CellType::Double);
        }
        else {
            _excel.setElementInMatrix(_rows , _columns, charValue, CellType::Integer);
        }
    }
    else {
        _excel.setElementInMatrix(_rows , _columns, charValue, CellType::String);
    }
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