#include "excel.h"
#include "terminal.h"
#include "vector.h"
#include <string>
#include <cstring>

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

bool isDigit(const char ch) {
    return ch >= '0' && ch <= '9';
}

bool isDigitInteger(const char* number) {
    for (size_t i = 0; i < strlen(number); ++i) {
        if (!isDigit(number[i])) {
            return false;
        }
    }
    return true;
}

bool isLetter(const char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z');
    
}

bool isValidNumber(const char* charNumber) {
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

bool isDouble(const char* charNumber){
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

CellType getStringCellType(const char* string) {
    //assuming the Cell has correct values.
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] == '\"') {
            return CellType::String;
        }
        if (string[i] == '.') {
            return CellType::Double;
        }
    }
    return CellType::Integer;
}

const char* removeQuotationMarks(const char* string) { //fix for \"  \"
    int len = std::strlen(string);
    char* result = new char[len + 1];
    int j = 0;

    for (int i = 0; i < len; i++) {
        if (string[i] != '"') {
            result[j] = string[i];
            j++;
        }
    }
    result[j] = '\0';

    return result;
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

const std::string Terminal::isLineValid(const std::string& line, const unsigned rowNumber) { 
    bool quoteOpened = false;
    bool dotFound = false;
    bool specialQuoteOpened = false;
    bool quoteShouldNotBeOpened = false;

    unsigned currentRow = rowNumber;
    unsigned currentColumn = 0;

    bool isValid = true;
    std::string errorCharacter;
    std::string errorMessage;
    std::string tempString;

    for (size_t i = 0; i < line.size(); ++i) {
        char current = line[i];
        // cout << "current: " << current << endl;
        char next = i + 1 < line.size() ? line[i + 1] : '\0';
        // std::cout << "Current char: " << current << endl;
        switch (current)
        {
        case ',':
            dotFound = false;
            // if (quoteShouldNotBeOpened)
            // if (next != '\"' && next != '\\') {
            //     cout << "Setting quoteShouldNotBeOpened to true." << endl;
            //     quoteShouldNotBeOpened = true;
            // }
            if (!quoteOpened && !specialQuoteOpened) {
                if (isValid) {
                    currentColumn++;
                }
                break;
            }
            if (quoteOpened) {
                break;
            }
            if (specialQuoteOpened) {
                break;
            }
            if (!(next == '=' || next == '+' || next == '-' || next == '\\' ||next == '\"' || isDigit(next) || next == ',' || next == '\0' || next == '\n')) {
                errorMessage = "Wrong use of character \"";
                errorMessage += next;
                errorMessage += "\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            break;
        case '\"' :
            if (quoteShouldNotBeOpened) {
                errorMessage = "Quote should not be opened at: ";
                isValid = false;
                goto endOfLoop;
            }
            if (specialQuoteOpened) {
                break;
            }
            if (!specialQuoteOpened && line[i - 1] == '\\') {
                break;
            }

            if (quoteOpened) {
                quoteOpened = false;
                if (next != ',' && i + 1 < line.size() && next != '\n') {
                    cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
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
            if (quoteOpened || specialQuoteOpened) {
                errorMessage = "Quote not closed at: ";
                isValid = false;
                goto endOfLoop;
            }
            if (isValid) {
                currentColumn = 0;
            }

            break;

        case '\\' :
            if (i + 1 < line.size() && next == '\"') {
                // special \" case
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
                errorMessage = "Wrong use of character \"";
                errorMessage += next;
                errorMessage += "\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            break;

        case '.' :
            if (quoteOpened) {
                break;
            }
            if (specialQuoteOpened) {
                break;
            }
            if (dotFound) {
                errorMessage = "Wrong use of an extra dot \".\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            
            if (!quoteOpened && !isDigit(next) && !specialQuoteOpened) {
                errorMessage = "Wrong use of character \"";
                errorMessage += next;
                errorMessage += "\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            dotFound = true;
            break;
        default:
            if (!isDigit(current) && !specialQuoteOpened && !quoteOpened &&current != ',' && current != '\"' &&
                current != '=' && current != '+' && current != '-' &&
                current != '<' && current != '>' && current != '.' &&
                current != '\0' && current != '\n') {
                    errorMessage = "Wrong use of character \"";
                    errorMessage += current;
                    errorMessage += "\" at: ";
                    isValid = false;
                    goto endOfLoop;
                }
            break;
        }
    }
    endOfLoop:
    // The quote should be closed and no dot should be found
    // at the end of a valid line
    if (!quoteOpened && !dotFound && !specialQuoteOpened && isValid) {
        return "1";
    }

    else {
        tempString += errorMessage + 
                    '\n' + "Row: " + std::to_string(currentRow + 1) + ", " +
                    "Column: " + std::to_string(currentColumn + 1);
        return tempString;
    }
}

const bool Terminal::isFileValid(std::ifstream& iFile) {
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
        newInput = removeSpacesFromString(input);
        newInput += '\n';
        isLineValidString = isLineValid(newInput, rowNumber);
        rowNumber++;
        if (isLineValidString != "1") {
            cerr << "The spreadsheet you provided was invalid. Please make sure it is valid." << endl;
            cerr << "Error message- " << isLineValidString << endl;
            return false;// HANDLE RETURN ERROR MESSAGE
        }
    }

    cout << "Text file opened successfully.\n" << endl;
    cout << "Available commands:" << endl;
    cout << "- print                           : Display the contents." << endl;
    cout << "- print spreadsheet               : Show the spreadsheet format." << endl;
    cout << "- print types                     : Display data types of the contents." << endl;
    cout << "- edit <row> <column> <new_value> : To edit a cell's value." << endl;
    cout << "- save                            : Save the current file and it's changes." << endl;
    cout << "- close                           : Close the current file." << endl;
    cout << "- exit                            : To fully exit the whole program." << endl;
    cout << "\nPlease enter a command to proceed." << endl;

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

    currentExcel = excel;
}

const bool Terminal::isStringValidForCell(std::string& string) {
    bool dotFound = false;
    size_t size = string.size() - 1;

    if (string[0] == '+' || string[0] == '-' || isDigit(string[0])) {
        for (size_t i = 1; i < size; ++i) {
            if (string[i + 1] == '.') {
                if (i == 0) {
                    return false;
                }
                if (dotFound) {
                    return false;
                }
                dotFound = true;
            }
            if (!isDigit(string[i]) && string[i] != '.') {
                return false;   
            }
        }
        return true;
    }

    else if (string[0] == '\"' && string[size - 1] == '\"') {
        cout << "I GOT HERE" << endl;
        for (size_t i = 1; i < size - 1; ++i) {
            if (string[i] == '\\' && string[i + 1] == '\"') {
                i += 2;
                continue;
            }
            else if (string[i] == '\"' && i != size && string[i - 1] != '\\') {
                return false;
            }
        }
        return true;
    }

    return false;
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

void Terminal::editExcel(const char* firstNumber, const char* secondNumber, const char* value) {
    int firstNumberInt;
    int secondNumberInt;

    cout << "First number: " << firstNumber << endl;
    cout << "Second number: " << secondNumber << endl;

    try {
        firstNumberInt = std::stoi(firstNumber);
        secondNumberInt = std::stoi(secondNumber);
    } catch(...) {
        cerr << "In \"edit <row> <column> <new_value>\" row and column should be integers." << endl;
        return; 
    }

    if (firstNumberInt > currentExcel.getNumberOfRows() || firstNumberInt < 1
    || secondNumberInt > currentExcel.getNumberOfColumns() || secondNumberInt < 1) {
        cerr << "In \"edit <row> <column> <new_value>\" invalid numbers for row and column." << endl;
        return;
    }

    if (getStringCellType(value) == CellType::String) {
        const char* newValue = removeQuotationMarks(value);
       
        currentExcel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, newValue, CellType::String);
       
        delete newValue;
    }
    else {
        currentExcel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, value, getStringCellType(value));

    }
}

void Terminal::processCommand(const char* string, bool& flag) {
    //TO DO: 
    //fix quoteShouldNotBeOpened
    //continue the edit command (use openTextFile())
    //remove specialQuoteOpened
    const size_t numberOfWords = wordsCounter(string);
    char** InputArray = processInputIntoArray(string);

    if (strcmp(InputArray[0], "open") == 0 && numberOfWords == 2 && validInput(string) && editMode == false) {
        openTextFile(InputArray[1]);
        
        freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "edit") == 0 && numberOfWords > 3 && isDigitInteger(InputArray[1]) && isDigitInteger(InputArray[2])) {
        if (!editMode) {
            cerr << "Excel edit failed. (you are not in edit mode)" << endl;
            freeInputArray(InputArray, numberOfWords);
            return;
        }
        
        size_t remainingWords = numberOfWords - 3;
        std::string tempString = "";

        for (size_t i = 0; i < remainingWords; ++i) {
            tempString += InputArray[3 + i];
            tempString += ' ';
        }
        
        cout << "TEMPSTRING: " << tempString << endl;
        cout << std::boolalpha << isStringValidForCell(tempString) << endl;

        editExcel(InputArray[1], InputArray[2], InputArray[3]);

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

        freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "print") == 0 && numberOfWords == 1 && validInput(string)) {
        if (editMode) {
            // cout << "----------Excel----------" << endl;
            currentExcel.printExcel();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        freeInputArray(InputArray, numberOfWords);
    }

    else  if (strcmp(InputArray[0], "print") == 0 && strcmp(InputArray[1], "spreadsheet") == 0 && numberOfWords == 2 && validInput(string)) {
        if (editMode) {
            // cout << "-------SPREADSHEET-------" << endl;
            currentExcel.printSpreadsheet();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "print") == 0 && strcmp(InputArray[1], "types") == 0 && numberOfWords == 2 && validInput(string)) {
        if (editMode) {
            // cout << "---------Types---------" << endl;
            currentExcel.printTypes();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        freeInputArray(InputArray, numberOfWords);
    }

    else {
        editMode == true ? cerr << "You are in edit mode. (use \"close\" to stop edit mode)" << endl : cerr << "Incorrect input. " << endl;
        freeInputArray(InputArray, numberOfWords);
    }
}