
#ifndef EXCEL_MANAGER_HEADER_INCLUDED
#define EXCEL_MANAGER_HEADER_INCLUDED

#include "excel.h"

class ExcelManager {
public:
    Excel& operator=(const ExcelManager& em) = delete;
    ExcelManager(const Excel& other) = delete;
    ~ExcelManager() = default;

    explicit ExcelManager(Excel& _excel) : excel(_excel), editMode(false) {};

    void createExcelFromFile(std::ifstream& iFile);
    void editExcel(const char* firstNumber, const char* secondNumber, const char* value);
    Excel& getExcel() const { return excel; }

    void changeEditMode(bool _editMode) { editMode = _editMode; }
    bool getEditMode() const { return editMode; }

private:
    void runFormulasExcel();
    void setExcelDataFromFile(std::ifstream &iFile);
    void setExcelCellBasedOnType(const int _rows, const int _columns, const std::string& cellValue);

protected:
    Excel& excel;
    bool editMode;
};


#endif