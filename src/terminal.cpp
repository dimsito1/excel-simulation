#include "excel.h"
#include "terminal.h"
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

const FormulaType Terminal::getFormulaType(const char firstChar, const char secondChar) const {
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

const CellType Terminal::getStringCellType(const char* string) const {
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

const std::string Terminal::getResultFromFormula(const long double& firstValue, const long double& secondValue, const FormulaType& formulaType) const {
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


void Terminal::getErrorMessage(std::string& errorMessage, const char errorChar) const {
    errorMessage = "Wrong use of character \"";
    errorMessage += errorChar;
    errorMessage += "\" at: ";
}

const std::string Terminal::isLineValid(const std::string& line, const unsigned rowNumber) const {
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
                if (!utility::boolArrIsAllFalseOrAllTrue(formulaLettersFound, 4)) {
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
            || utility::isDigit(next) || next == ',' || next == '\0' || next == '\n' || next == 'R')) {
                //cout << "1" << endl;
                getErrorMessage(errorMessage, next);

                isValid = false;
                goto endOfLoop;
            }
            break;
        case 'C':
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted || next ==',') {
                //cout << "2" << endl;
                getErrorMessage(errorMessage, current);
                    
                isValid = false;
                goto endOfLoop;
            }
            else if (formulaStarted && !utility::isDigit(next)) {
                //cout << "3" << endl;
                getErrorMessage(errorMessage, next);
                    
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
                //cout << "4" << endl;
                getErrorMessage(errorMessage, next);
                    
                isValid = false;
                goto endOfLoop;
            }
            break;
        case 'R':
            if (quoteOpened) {
                break;
            }
            else if (!utility::isDigit(next) || next == ',') {
                //cout << "5" << endl;
                getErrorMessage(errorMessage, next);

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
                //cout << "6" << endl;
                getErrorMessage(errorMessage, current);

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
            else if (!formulaStarted || !utility::isDigit(prev) || (next != 'R' && next != '=')) {
                //cout << "7" << endl;
                getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            break;
        case '=': 
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted) {
                //cout << "8" << endl;
                getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            else if ((next == '=' && !utility::isDigit(prev)) || (prev == '=' && next != 'R')) {
                //cout << "9" << endl;
                getErrorMessage(errorMessage, current);
                
                isValid = false;
                goto endOfLoop;
            }
        break;

        case '+':
        case '-':
            if (quoteOpened) {
                break;
            }
            else if (!formulaStarted && !utility::isDigit(next) && prev != ',' && prev != '\0') {
                //cout << "10" << endl;
                getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            else if (formulaStarted && (next != 'R' || !utility::isDigit(prev))) {
                //cout << "11" << endl;
                getErrorMessage(errorMessage, current);

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
                //cout << "12" << endl;
                getErrorMessage(errorMessage, current);

                isValid = false;
                goto endOfLoop;
            }
            else if (formulaStarted) {
                if (!utility::isDigit(prev) || next != 'R') {
                    //cout << "13" << endl;
                    getErrorMessage(errorMessage, current);

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
                    //cout << "14" << endl;
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
               // cout << "15" << endl;
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
                //cout << "16" << endl;
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
                //cout << "17" << endl;
                errorMessage = "Wrong use of an extra dot \".\" at: ";
                isValid = false;
                goto endOfLoop;
            }
            
            if (!quoteOpened && !utility::isDigit(next)) {
                //cout << "18" << endl;

                getErrorMessage(errorMessage, next);

                isValid = false;
                goto endOfLoop;
            }
            dotFound = true;
            break;
        default:
            if (formulaStarted) {
                if (utility::isDigit(current)) {
                    if (next != 'C' && next != '-' && next != '+' && next != '*'
                    && next != ',' && next != '=' && next != '<' && next != '>'
                    && next != '/' && prev != 'R' && prev != 'C'&& !utility::isDigit(next)) {
                        cout << "19" << endl;
                        getErrorMessage(errorMessage, next);

                        isValid = false;
                        goto endOfLoop;
                    }
                }
            }
            if (!utility::isDigit(current) && !quoteOpened &&current != ',' && current != '\"' &&
                current != '=' && current != '+' && current != '-' &&
                current != '<' && current != '>' && current != '.' &&
                current != '\0' && current != '\n' && current != 'R') {
                    cout << "20" << endl;
                    getErrorMessage(errorMessage, current);

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
    
    bool areAllSame = utility::boolArrIsAllFalseOrAllTrue(formulaLettersFound, 4);
    
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

bool Terminal::isFileValid(std::ifstream& iFile) const {
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

    cout << "Text file opened successfully.\n" << endl;
    cout << "Available commands:" << endl;
    cout << "- print                           : Display the contents." << endl;
    cout << "- print spreadsheet               : Show the spreadsheet format." << endl;
    cout << "- print types                     : Display data types of the contents." << endl;
    cout << "- edit <row> <column> <new_value> : To edit a cell's value using integers for rows and columns." << endl;
    cout << "- save                            : Save the current file and its changes." << endl;
    cout << "- close                           : Close the current file." << endl;
    cout << "- exit                            : To fully exit the whole program." << endl;
    cout << "\nPlease enter a command to proceed." << endl;

    return true;
}

const char* Terminal::createTextFile() {
    cout << "Enter a name for a new text file: ";

    char newInput[256];
    cin.getline(newInput, 256);

    size_t numberOfWords = utility::wordsCounter(newInput);

    if (!utility::validInput(newInput) || (!utility::charArrEndsCorrectly(newInput, ".txt")) || numberOfWords != 1) {
        cout << "Invalid input. (file should be .txt)" << endl;
        return nullptr;
    }

    std::string filePath = "data/";
    filePath += newInput;
    std::ofstream newFile(filePath);
    cout << "New text file \"" << newInput << "\" created successfully." << endl;
    newFile.close();

    char* result = new char[strlen(newInput) + 1];
    strcpy(result, newInput);

    return result;        
}

void Terminal::openTextFile(const char* input) {
    std::string filePath = "data/";
    filePath += input;
    std::ifstream iFile(filePath);
    if (!iFile) {
        cerr << "The file you provided does not exist."<< endl;
        iFile.close();

        const char* tempFileName = createTextFile();
        if (!tempFileName) {
            return;
        }
        currentFileName = tempFileName;
    }
    
    if (utility::isEmptyFile(iFile)) {
        cerr << "The file you provided is empty." << endl;
        cerr << "Fill it accordingly then proceed." << endl;
        return;
    }

    currentFileName = input;

    setRowsAndColsExcel(iFile);
    iFile.close();
}


void Terminal::setRowsAndColsExcel(std::ifstream& iFile) { //TO DO readTextFile
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
    std::vector<size_t> commaArray;
    // Vector commaArray;
    char symbol;
    bool quoteOpened = false;
    bool specialQuoteOpened = false;

    while (iFile.get(symbol)) {
        if (symbol == '\\') {
            char nextChar = iFile.get();
            if (nextChar == '\"' && iFile.peek() != ',') {
                specialQuoteOpened = !specialQuoteOpened;
            }
            iFile.seekg(-1, std::ios::cur); 
        }
        if (symbol == '\"') {
            quoteOpened = !quoteOpened;
        }
        if (symbol == ',' && !quoteOpened) {
            commaCounter++;
        }
        else if (symbol == '\n') {
            // commaArray.pushBack(commaCounter);
            commaArray.push_back(commaCounter);
            commaCounter = 0;
        }
    }

    if (iFile.eof()) {
        // commaArray.pushBack(commaCounter);
        commaArray.push_back(commaCounter);
    }
    else {
        cerr << "ERROR END OF FILE NOT REACHED ( Terminal::readTextFile )" << endl;
        return;
    }

    // const int newRows = commaArray.getSize();
    const int newRows = commaArray.size();
    const int newColumns = *std::max_element(commaArray.begin(), commaArray.end());
    // const int newColumns = commaArray.getMaxElement() + 1;
    // cout << "Number of Columns: " << newColumns << endl;
    
    Excel newExcel(newRows, newColumns);

    importDataIntoExcel(newExcel, iFile);
}

void Terminal::importDataIntoExcel(Excel &excel, std::ifstream &iFile) {
    changeEditMode(true);

    iFile.clear(); // Clear any flags
    iFile.seekg(0, std::ios::beg);

    if (!iFile) {
        cerr << "ERROR OPENING FILE ( Terminal::operateExcel )" << endl;
        return;
    }

    bool quoteOpened = false;
    char symbol;

    int excelRows = excel.getNumberOfRows();
    int excelColumns = excel.getNumberOfColumns();

    int j = 0;
    int i = 0;
    std::string cellValue;

    while (iFile.get(symbol)) {
        if (symbol == ' ' && !quoteOpened) {
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
        
        else if (symbol == ',' && !quoteOpened) {
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
            char nextChar = iFile.get();
            if (nextChar == '\"' && quoteOpened) {
                while (iFile.peek() == ' ') {
                    iFile.ignore(1);
                }
                if (iFile.peek() != ',') {
                    cellValue += '\"';
                } else {
                    cellValue += '\\';
                    iFile.putback(nextChar);
                }
            }
            else {
                cellValue += '\\';
                if (nextChar != '\"') {
                    iFile.putback(nextChar);
                }
            }
        }
        
        else if (symbol == '\"') {
            quoteOpened = !quoteOpened;
        }

        else if (symbol == ' ' && quoteOpened) {
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

    runFormulasExcel(currentExcel);
}

bool Terminal::isStringValidForCell(std::string& string) {
    bool dotFound = false;
    size_t size = string.size();

    if (string[0] == '\"') {
        if (string[size - 1] != '\"') {
            return false;
        }
    }

    if (string[0] == '+' || string[0] == '-' || utility::isDigit(string[0])) {
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
            if (!utility::isDigit(string[i]) && string[i] != '.') {
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

void Terminal::setCellBasedOnType(Excel& _excel, const std::string& cellValue, const int _rows, const int _columns) {
    const char* charValue = cellValue.c_str();
    if (utility::isValidNumber(charValue)) {
        if (utility::isDouble(charValue)) {
            _excel.setElementInMatrix(_rows , _columns, charValue, CellType::Double);
        }
        else {
            _excel.setElementInMatrix(_rows , _columns, charValue, CellType::Integer);
        }
    }
    else if (getStringCellType(charValue) == CellType::String){
        _excel.setElementInMatrix(_rows , _columns, charValue, CellType::String);
    }
    else if (getStringCellType(charValue) == CellType::Formula) {
        _excel.setElementInMatrix(_rows, _columns, charValue, CellType::Formula);
    }
}

void Terminal::runFormulasExcel(Excel& _excel) {

    unsigned originalRowNum = _excel.getNumberOfRows();
    unsigned originalColNum = _excel.getNumberOfColumns();

    unsigned firstRowNum;
    unsigned firstColumnNum;
    unsigned secondRowNum;
    unsigned secondColumnNum;

    char firstFormulaChar;
    char secondFormulaChar = '0';

    char* firstCellValueChar;
    char* secondCellValueChar;
    char* newCellValueChar;

    long double firstCellValue = 0.0L;
    long double secondCellValue = 0.0L;
    long double newCellValue = 0.0L;

    CellType firstCellType = CellType::Default;
    CellType secondCellType = CellType::Default;


    std::string formulaString;
    for (size_t i = 0; i < originalRowNum; i++) {
        for (size_t j = 0; j < originalColNum; j++) {
            if (getStringCellType(_excel.getElementFromMatrix(i, j)) == CellType::Formula) {
                formulaString = _excel.getElementFromMatrix(i, j);
 
                std::string extractFormula = utility::extractFormulaFromString(formulaString);
                std::string extractedNum = utility::extractNumbersFromString(formulaString);
                const char* extractedNumChar = extractedNum.c_str();
                char** numbersArray = utility::processInputIntoArray(extractedNumChar);
                size_t numberOfWords = utility::wordsCounter2(extractedNumChar);

                // cout << "FIRST NUMBER:" << numbersArray[0] << '!' << endl;
                // cout << "SECOND NUMBER:" << numbersArray[1] << '!' << endl;
                // cout << "THIRD NUMBER:" << numbersArray[2] << '!' << endl;
                // cout << "FOURTH NUMBER:" << numbersArray[3] << '!' << endl;

                firstRowNum = std::stoul(numbersArray[0]);
                firstColumnNum = std::stoul(numbersArray[1]);
                secondRowNum = std::stoul(numbersArray[2]);
                secondColumnNum = std::stoul(numbersArray[3]);

                utility::freeInputArray(numbersArray, numberOfWords);
                
                if (firstRowNum < 1 || firstRowNum > originalRowNum
                || firstColumnNum < 1 || firstColumnNum > originalColNum) {
                    firstCellValue = 0;
                }
                else {
                    firstCellValueChar = new char[strlen(_excel.getElementFromMatrix(firstRowNum - 1, firstColumnNum - 1)) + 1];
                    strcpy(firstCellValueChar, _excel.getElementFromMatrix(firstRowNum - 1, firstColumnNum - 1));
                    firstCellType = getStringCellType(firstCellValueChar);

                    if (firstCellType == CellType::String) {
                        if (utility::isValidNumber(firstCellValueChar)) {
                            firstCellValue = std::stold(firstCellValueChar);
                        }
                        else {
                            firstCellValue = 0;
                        }
                    }
                    else if (firstCellType  == CellType::Double || firstCellType == CellType::Integer) {
                        firstCellValue = std::stold(firstCellValueChar);
                    }
                    else if (firstCellType == CellType::Default) {
                        firstCellValue = 0;
                    }
                    else if (firstCellType == CellType::Formula) {
                        cerr << "Error you cannot target a Formula cell with a Formula cell." << endl;
                        delete firstCellValueChar;
                        return;
                    }

                    delete firstCellValueChar;
                }

                if (secondRowNum < 1 || secondRowNum > originalRowNum
                || secondColumnNum < 1 || secondColumnNum > originalColNum) {
                    secondCellValue = 0;
                }
                else {
                    secondCellValueChar = new char[strlen(_excel.getElementFromMatrix(secondRowNum - 1, secondColumnNum - 1)) + 1];
                    strcpy(secondCellValueChar, _excel.getElementFromMatrix(secondRowNum - 1, secondColumnNum - 1));
                    secondCellType = getStringCellType(secondCellValueChar);
                    
                    if (secondCellType == CellType::String) {
                        if (utility::isValidNumber(secondCellValueChar)) {
                            secondCellValue = std::stold(secondCellValueChar);
                        }
                        else {
                            secondCellValue = 0;
                        }
                    }
                    else if (secondCellType  == CellType::Double || secondCellType == CellType::Integer) {
                        secondCellValue = std::stold(secondCellValueChar);
                    }
                    else if (secondCellType == CellType::Default) {
                        secondCellValue = 0;
                    }
                    else if (secondCellType == CellType::Formula) {
                        cerr << "Error you cannot target a Formula cell with a Formula cell." << endl;
                        delete secondCellValueChar;
                        return;
                    }

                    delete secondCellValueChar;
                }

                FormulaType formulaType = getFormulaType(extractFormula.c_str()[0], extractFormula.c_str()[1]);
                std::string finalValue = getResultFromFormula(firstCellValue, secondCellValue, formulaType);
                if (finalValue == "ERROR") {
                    _excel.setElementInMatrix(i, j, finalValue.c_str(), CellType::ERROR);
                    return;
                }
                CellType finalCellType = getStringCellType(finalValue.c_str());

                _excel.setElementInMatrix(i, j, finalValue.c_str(), finalCellType);
            }
        }
    }
}

void Terminal::editExcel(const char* firstNumber, const char* secondNumber, const char* value) {
    int firstNumberInt;
    int secondNumberInt;

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
        const char* updatedString = utility::makeStringValueForCell(value);
       
        currentExcel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, updatedString, CellType::String);

        delete updatedString;       
    }
    else {
        currentExcel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, value, getStringCellType(value));

    }
}

void Terminal::processCommand(const char* string, bool& flag) {
    const size_t numberOfWords = utility::wordsCounter(string);
    char** InputArray = utility::processInputIntoArray(string);

    if (strcmp(InputArray[0], "open") == 0 && numberOfWords == 2 && utility::validInput(string) && editMode == false) {
        openTextFile(InputArray[1]);
        
        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "edit") == 0 && numberOfWords > 3 && utility::isDigitInteger(InputArray[1]) && utility::isDigitInteger(InputArray[2])) {
        if (!editMode) {
            cerr << "Excel edit failed. (you are not in edit mode)" << endl;
            utility::freeInputArray(InputArray, numberOfWords);
            return;
        }
        
        std::string tempString = "";

        if (InputArray[3][0] == '\"') {
            const char* newString = utility::getStringAfterQuote(string);
            tempString = newString;
            delete newString;
        }

        else {
            size_t remainingWords = numberOfWords - 3;

            for (size_t i = 0; i < remainingWords; ++i) {
                tempString += InputArray[3 + i];
                tempString += ' ';
            }
            tempString[tempString.size() - 1] = '\0';
        }
        
        // cout << std::boolalpha << isStringValidForCell(tempString) << endl;

        if (!isStringValidForCell(tempString)) {
            cerr << "In \"edit <row> <column> <new_value>\" the value should be valid for a cell." << endl;
            utility::freeInputArray(InputArray, numberOfWords);
            return;
        }

        editExcel(InputArray[1], InputArray[2], tempString.c_str());

        utility::freeInputArray(InputArray, numberOfWords);
    }
    else if (strcmp(InputArray[0], "exit") == 0 && numberOfWords == 1 && utility::validInput(string)) {
        utility::freeInputArray(InputArray, numberOfWords);
        flag = true;
    }

    else if (strcmp(InputArray[0], "close") == 0 && numberOfWords == 1 && utility::validInput(string) && editMode == true) {
        changeEditMode(false);        
        cout << "Excel closed successfully." << endl;

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "print") == 0 && numberOfWords == 1 && utility::validInput(string)) {
        if (editMode) {
            currentExcel.printExcel();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else  if (strcmp(InputArray[0], "print") == 0 && strcmp(InputArray[1], "spreadsheet") == 0 && numberOfWords == 2 && utility::validInput(string)) {
        if (editMode) {
            currentExcel.printSpreadsheet();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "print") == 0 && strcmp(InputArray[1], "types") == 0 && numberOfWords == 2 && utility::validInput(string)) {
        if (editMode) {
            currentExcel.printTypes();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "save") == 0 && numberOfWords == 1) {
        if (editMode) {
            currentExcel.saveToFile(currentFileName.c_str());
            cout << "File saved successfully." << endl;
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else {
        editMode == true ? cerr << "You are in edit mode. (use \"close\" to stop edit mode)" << endl : cerr << "Incorrect input. " << endl;
        utility::freeInputArray(InputArray, numberOfWords);
    }
}

void Terminal::printWelcomeMessage() const {
    cout << "Welcome to-" << endl;
    cout << "  ______              _  _____ _                 _       _   _              "         << endl;
    cout << " |  ____|            | |/ ____(_)               | |     | | (_)             "         << endl;
    cout << " | |__  __  _____ ___| | (___  _ _ __ ___  _   _| | __ _| |_ _  ___  _ __   "         << endl;
    cout << " |  __| \\ \\/ / __/ _ \\ |\\___ \\| | '_ ` _ \\| | | | |/ _` | __| |/ _ \\| '_ \\  " << endl;
    cout << " | |____ >  < (_|  __/ |____) | | | | | | | |_| | | (_| | |_| | (_) | | | | "         << endl;
    cout << " |______/_/\\_\\___\\___|_|_____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_| "  << endl;
    cout << endl;
    cout << "Use command \"open <filename.txt>\" to get started." << endl;
    cout << "Use command \"exit\" to exit the program at any time." << endl;
}