#include "excelManager.h"
#include "utility.h"
#include "validator.h"
#include <vector>
#include <algorithm>

using std::cerr;
using std::endl;
using std::cout;

void ExcelManager::editExcel(const char* firstNumber, const char* secondNumber, const char* value) {
    int firstNumberInt;
    int secondNumberInt;

    try {
        firstNumberInt = std::stoi(firstNumber);
        secondNumberInt = std::stoi(secondNumber);
    } catch(...) {
        cerr << "In \"edit <row> <column> <new_value>\" row and column should be integers." << endl;
        return; 
    }

    if (firstNumberInt > excel.getNumberOfRows() || firstNumberInt < 1
    || secondNumberInt > excel.getNumberOfColumns() || secondNumberInt < 1) {
        cerr << "In \"edit <row> <column> <new_value>\" invalid numbers for row and column." << endl;
        return;
    }

    if (utility::getStringCellType(value) == CellType::String) {
        const char* updatedString = utility::makeStringValueForCell(value);
       
        excel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, updatedString, CellType::String);
        cout << "Excel edited successfully." << endl;

        delete[] updatedString;
    }
    else {
        excel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, value, utility::getStringCellType(value));
        cout << "Excel edited successfully." << endl;
    }
}

void ExcelManager::setExcelCellBasedOnType(const int _rows, const int _columns, const std::string& cellValue) {
    const char* charValue = cellValue.c_str();
    if (validate::isValidNumber(charValue)) {
        if (validate::isDouble(charValue)) {
            excel.setElementInMatrix(_rows , _columns, charValue, CellType::Double);
        }
        else {
            excel.setElementInMatrix(_rows , _columns, charValue, CellType::Integer);
        }
    }
    else if (utility::getStringCellType(charValue) == CellType::String){
        excel.setElementInMatrix(_rows , _columns, charValue, CellType::String);
    }
    else if (utility::getStringCellType(charValue) == CellType::Formula) {
        excel.setElementInMatrix(_rows, _columns, charValue, CellType::Formula);
    }
}

void ExcelManager::createExcelFromFile(std::ifstream& iFile) { //TO DO readTextFile
    if (!iFile) {
        cerr << "ERROR OPENING FILE ( readTextFile )" << endl;
        cerr << "Error state: " << iFile.rdstate() << endl;
        return;
    }

    if (!validate::isFileValid(iFile)) {
        return; 
    }

    iFile.clear(); // Clear any flags
    iFile.seekg(0, std::ios::beg); 

    size_t commaCounter = 0;
    std::vector<size_t> commaArray;
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
            commaArray.push_back(commaCounter);
            commaCounter = 0;
        }
    }

    if (iFile.eof()) {
        commaArray.push_back(commaCounter);
    }
    else {
        cerr << "ERROR END OF FILE NOT REACHED ( Terminal::readTextFile )" << endl;
        return;
    }

    const int newRows = commaArray.size();
    const int newColumns = *std::max_element(commaArray.begin(), commaArray.end()) + 1;
    
    Excel newExcel(newRows, newColumns);

    excel = newExcel;

    setExcelDataFromFile(iFile);
}

void ExcelManager::setExcelDataFromFile(std::ifstream &iFile) {
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
                setExcelCellBasedOnType(i, j, cellValue);
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
                setExcelCellBasedOnType(i, j, cellValue);
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
            setExcelCellBasedOnType(i, j, cellValue);
            j++;
        }

        int remainingCells = excelColumns - j;

        for (size_t k = 0; k < remainingCells; ++k) {
            excel.setElementInMatrix(i, j, "", CellType::Default);
            j++;
        }
    }

    runFormulasExcel();
}

void ExcelManager::runFormulasExcel() {

    unsigned originalRowNum = excel.getNumberOfRows();
    unsigned originalColNum = excel.getNumberOfColumns();

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
            if (utility::getStringCellType(excel.getElementFromMatrix(i, j)) == CellType::Formula) {
                formulaString = excel.getElementFromMatrix(i, j);
 
                std::string extractFormula = utility::extractFormulaFromString(formulaString);
                std::string extractedNum = utility::extractNumbersFromString(formulaString);
                const char* extractedNumChar = extractedNum.c_str();
                char** numbersArray = utility::processInputIntoArray(extractedNumChar);
                size_t numberOfWords = utility::wordsCounter2(extractedNumChar);

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
                    firstCellValueChar = new char[strlen(excel.getElementFromMatrix(firstRowNum - 1, firstColumnNum - 1)) + 1];
                    strcpy(firstCellValueChar, excel.getElementFromMatrix(firstRowNum - 1, firstColumnNum - 1));
                    firstCellType = utility::getStringCellType(firstCellValueChar);

                    if (firstCellType == CellType::String) {
                        if (validate::isValidNumber(firstCellValueChar)) {
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
                    secondCellValueChar = new char[strlen(excel.getElementFromMatrix(secondRowNum - 1, secondColumnNum - 1)) + 1];
                    strcpy(secondCellValueChar, excel.getElementFromMatrix(secondRowNum - 1, secondColumnNum - 1));
                    secondCellType = utility::getStringCellType(secondCellValueChar);
                    
                    if (secondCellType == CellType::String) {
                        if (validate::isValidNumber(secondCellValueChar)) {
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

                FormulaType formulaType = utility::getFormulaType(extractFormula.c_str()[0], extractFormula.c_str()[1]);
                std::string finalValue = utility::getResultFromFormula(firstCellValue, secondCellValue, formulaType);
                if (finalValue == "ERROR") {
                    excel.setElementInMatrix(i, j, finalValue.c_str(), CellType::ERROR);
                    return;
                }
                CellType finalCellType = utility::getStringCellType(finalValue.c_str());

                excel.setElementInMatrix(i, j, finalValue.c_str(), finalCellType);
            }
        }
    }
}
