#include "excel.h"
#include "cell.h"
#include "terminal.h"
#include "utility.h"

using std::cerr;
using std::cout;
using std::endl;

void Excel::copyFrom(const Excel& other) {
    setRows(other.rows);
    setColumns(other.columns);

    matrix = new Cell*[rows];

    for (size_t i = 0; i < rows; i++){
        matrix[i] = new Cell[columns];
    }
    
    for (size_t i = 0; i < rows; ++i){
        for (size_t j = 0; j < columns; ++j){
            matrix[i][j] = other.matrix[i][j];
        }
    }
}

void Excel::erase() {
    for (size_t i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

Excel::Excel() : rows(1), columns(1) {
    matrix = new Cell*[1];
    matrix[0] = new Cell[1];
};

Excel::Excel(const Excel& other) {
    copyFrom(other);
}

Excel& Excel::operator=(const Excel& other) {
    if (this != &other) {
        erase();
        copyFrom(other);
    }
    return *this;
}

Excel::~Excel() {
    erase();
}

Excel::Excel(const int _rows, const int _columns) {
    setRows(_rows);
    setColumns(_columns);

    matrix = new Cell*[_rows];

    for (size_t i = 0; i < rows; ++i){
        matrix[i] = new Cell[columns];
    }
}

void Excel::setRows(const int _rows) {
    if (_rows <= 0) {
        throw std::invalid_argument("COLUMNS OUT OF BOUNDS ( Excel::setRows )");
    }
    rows = _rows;
}

void Excel::setColumns(const int _columns) {
    if (_columns <= 0) {
        throw std::invalid_argument("COLUMNS OUT OF BOUNDS ( Excel::setColumns )");
    }
    columns = _columns;
}

void Excel::setElementInMatrix(const int& _rows, const int& _columns, const char* _value, const CellType& _type) {
    if ( _rows < 0 || _columns < 0 || _rows >= rows || _columns >= columns) {
        throw std::invalid_argument("INDEX OUT OF BOUNDS ( Excel::setElementInMatrix ) EDIT MODE");
    }
    matrix[_rows][_columns].setValueAndType(_value, _type);
}

const char* Excel::getElementFromMatrix(const int& _rows, const int& _columns) const {
    if (_rows >= rows || _columns >= columns || rows < 0 || _columns < 0) {
        throw std::out_of_range("INDEX OUT OF BOUNDS ( Excel::getElementFromMatrix )");
    }
    return matrix[_rows][_columns].getValue();
}

void Excel::printRow(const int index) const {
    if (index >= rows || index < 0) {
        throw std::out_of_range("INDEX OUT OF BOUND( Excel::printRow )");
    }
    for (size_t i = 0; i < columns; i++){
        cout << matrix[index][i].getValue() << ' ';
    }
    cout << endl;
}

void Excel::printColumn(const int index) const {
    if (index >= columns || index < 0) {
        throw std::out_of_range("INDEX OUT OF BOUND( Excel::printColumn )");
    }
    for (size_t i = 0; i < rows; i++){
        cout << matrix[i][index].getValue() << endl;
    }
}

void Excel::printCellTypeRow(const int index) const {
    if (index >= rows || index < 0) {
        throw std::out_of_range("INDEX OUT OF BOUND( Excel::printCellTypeRow )");
    }
    for (size_t i = 0; i < columns; ++i) {
        cout << matrix[index][i].getCellTypeChar() << ' ';
    }
    cout << endl;
}

size_t Excel::getHighestLengthColumn(const int index) const {
    if (index >= columns || index < 0) {
        throw std::out_of_range("ERROR INDEX OUT OF RANGE ( Excel::getHighestLengthColumn )");
    }
    size_t currLength = 0;
    size_t maxLength = 0;

    for (size_t i = 0; i < rows; ++i) {
        currLength = strlen(matrix[i][index].getValue());
        if (currLength > maxLength) {
            maxLength = currLength;
        }
    }
    return maxLength;
}

void Excel::printSpreadsheet() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            cout << matrix[i][j].getValue() << ' ';
        }
        cout << endl;
    }
}

void Excel::printExcel() const {
    size_t highestLengthRow = 0;
    for (size_t i = 0; i < columns; ++i) {
        highestLengthRow += getHighestLengthColumn(i);
    }
    highestLengthRow += 3 * columns + 1;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < highestLengthRow; ++j) {
            cout << '-';
        }
        cout << endl;
        for (size_t k = 0; k < columns; ++k) {
            size_t highestLengthColumn = getHighestLengthColumn(k);
            size_t remainingColumnSize = highestLengthColumn - strlen(matrix[i][k].getValue());
            cout << "| " << matrix[i][k].getValue();
            if (k != columns - 1) {
                for (size_t n = 0; n < remainingColumnSize + 1; ++n) {
                    cout << ' ';
                }
            }
            else {
                for (size_t m = 0; m < remainingColumnSize; ++m) {
                    cout << ' ';
                }
            }

            if (k == columns - 1) {
                cout << " |";
            }
        }
        cout << endl;
    }
    for (size_t z = 0; z < highestLengthRow; ++z) {
        cout << '-';
    }
    cout << endl;
}

void Excel::printTypes() const {
    for (size_t i = 0; i < rows; ++i) {
        printCellTypeRow(i);
    }
}

void Excel::saveToFile(const char* filename) {
    std::ofstream oFile(filename);

    if (!oFile) {
        cerr << "Something wrong with file to be saved" << endl;
        return;
    }

    const unsigned numberOfRows = getNumberOfRows();
    const unsigned numberOfColumns = getNumberOfColumns();

    for (size_t i = 0; i < numberOfRows; i++) {
        for (size_t j = 0; j < numberOfColumns; j++) {
            const char* tempElement = getElementFromMatrix(i, j);
            
            if (matrix[i][j].getCellType() == CellType::String) {
                const char* fixedTempElement = utility::fixFileString(tempElement);
                oFile << "\"" << fixedTempElement << "\"";
                delete fixedTempElement;
            } else {
                oFile << tempElement;
            }

            if (j != numberOfColumns - 1) {
                oFile << ", ";
            } else if (i != numberOfRows - 1) {
                oFile << '\n';
            }
        }
    }
    
    oFile.close();
}