
#ifndef EXCEL_MANAGER_HEADER_INCLUDED
#define EXCEL_MANAGER_HEADER_INCLUDED

#include "excel.h"

class ExcelManager {
protected:
    Excel& excel;
    bool editMode;

public:
    Excel& operator=(const ExcelManager& em) = delete;
    ExcelManager(const Excel& other) = delete;
    ~ExcelManager() = default;

    explicit ExcelManager(Excel& _excel) : excel(_excel), editMode(false) {};
    
    Excel& getExcel() const { return excel; }

    void changeEditMode(bool _editMode) { editMode = _editMode; }
    bool getEditMode() const { return editMode; }

    void editExcel(const char* firstNumber, const char* secondNumber, const char* value);
    void runFormulasExcel();
    void setExcelDataFromFile(std::ifstream &iFile);
    void setExcelCellBasedOnType(const int _rows, const int _columns, const std::string& cellValue);
    void createExcelFromFile(std::ifstream& iFile);
};


#endif