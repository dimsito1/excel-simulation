
#ifndef _CELL_HEADERS_INCLUDED
#define _CELL_HEADERS_INCLUDED

enum class CellType {
    Integer,
    Double,
    String,
    Formula,
    Default,
    ERROR
};

class Cell {
protected:
    CellType currCellType;
    char* value;

private:
    void copyFrom(const Cell& other);
    void erase();

public:
    Cell();
    Cell(const Cell& other);
    Cell& operator=(const Cell& other);
    ~Cell();

    void setValueAndType(const char* _value, const CellType& _type);
    void changeCellType(const CellType& _cellType);

    CellType getCellType() const { return currCellType; }
    const char* getValue() const { return value; }

    const char* getCellTypeChar() const;
};


#endif