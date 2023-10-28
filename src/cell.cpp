#include "cell.h"
#include <iostream>
#include <cstring>

void Cell::copyFrom(const Cell& other) {
    currCellType = other.currCellType;
    value = new char[strlen(other.value) + 1];
    strcpy(value, other.value);
}

void Cell::erase() {
    delete[] value;
}

Cell::Cell() : currCellType(CellType::String) {
    value = new char[1];
    value[0] = '\0';
}

Cell::Cell(const Cell& other) {
    copyFrom(other);
}

Cell& Cell::operator=(const Cell& other) {
    if (this != &other) {
        erase();
        copyFrom(other);
    }
    return *this;
}

Cell::~Cell() {
    erase();
}

void Cell::setValueAndType(const char* _value, const CellType& _type) {
    if (!_value) {
        throw std::invalid_argument("ERROR CANNOT ASSIGN NULL TO VALUE ( Cell::setValue() )");
        return;
    }
    delete[] value;
    value = new char[strlen(_value) + 1];
    strcpy(value, _value);
    currCellType = _type;
}

void Cell::changeCellType(const CellType& _cellType) {
    currCellType = _cellType;
}

const char* Cell::getCellTypeChar() const {
    switch (currCellType) {
    case CellType::Integer:
        return "Integer";
    case CellType::Double:
        return "Double";  
    case CellType::String:
        return "String";
    case CellType::Formula:
        return "Formula";
    case CellType::ERROR:
        return "Error";
    default:
        return "Default";
    }
}