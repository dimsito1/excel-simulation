#include <iostream>
#include <cstring>
#include <fstream>
#include "cell.h"

#ifndef _EXCEL_HEADER_INCLUDED_
#define _EXCEL_HEADER_INCLUDED_

class Excel {
private:    
    Cell** matrix;
    unsigned rows;
    unsigned columns;

private:
    void copyFrom(const Excel& other);
    void erase();

    size_t getHighestLengthColumn(const int index) const;

public:
    Excel();
    Excel(const Excel& other);
    Excel& operator=(const Excel& other);
    ~Excel();

    Excel(const int _rows, const int _columns);

    void setRows(const int _rows); //fix setters
    void setColumns(const int _columns);

    void setElementInMatrix(const int _rows, const int _columns, const char* _value, const CellType _type); //rowPos and columnPos is rows + 1 and columnPos is columns + 1
    const char* getElementFromMatrix(const int _rows, const int _columns) const;

    void printRow(const int index) const;
    void printCellTypeRow(const int index) const;
    void printColumn(const int index) const;

    void printSpreadsheet() const;
    void printExcel() const;
    void printTypes() const;

    const unsigned getNumberOfRows() const { return rows; };
    const unsigned getNumberOfColumns() const {return columns; };
};


#endif