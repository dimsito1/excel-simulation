#include "excelManager.h"
#include "utility.h"

using std::cerr;

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

    if (getStringCellType(value) == CellType::String) {
        const char* updatedString = utility::makeStringValueForCell(value);
       
        excel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, updatedString, CellType::String);

        delete updatedString;       
    }
    else {
        excel.setElementInMatrix(firstNumberInt - 1, secondNumberInt - 1, value, getStringCellType(value));

    }
}