
#ifndef _EXCEL_HEADER_INCLUDED_
#define _EXCEL_HEADER_INCLUDED_

#include <iostream>
#include <cstring>
#include <fstream>
#include "cell.h"

class Excel {
public:
    Excel();
    Excel(const Excel& other);
    Excel& operator=(const Excel& other);
    ~Excel();

    Excel(const int _rows, const int _columns);
    void saveExcelToFile(const char* filename);

    void setElementInMatrix(const int& _rows, const int& _columns, const char* _value, const CellType& _type); //rowPos and columnPos is rows + 1 and columnPos is columns + 1
    const char* getElementFromMatrix(const int& _rows, const int& _columns) const;

    const unsigned getNumberOfRows() const { return rows; };
    const unsigned getNumberOfColumns() const {return columns; };

    void printRow(const int index) const;
    void printCellTypeRow(const int index) const;
    void printColumn(const int index) const;

    void printSpreadsheet() const;
    void printExcel() const;
    void printTypes() const;

private:    
    Cell** matrix;
    unsigned rows;
    unsigned columns;

private:
    void copyFrom(const Excel& other);
    void erase();
    void setRows(const int _rows);
    void setColumns(const int _columns);
    size_t getHighestLengthColumn(const int index) const;
};


#endif