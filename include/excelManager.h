#include "excel.h"

#ifndef EXCEL_MANAGER_HEADERS_INCLUDED
#define EXCEL_MANAGER_HEADERS_INCLUDED

class ExcelManager {
private:
    Excel& excel;

public:
    Excel& operator=(const ExcelManager& em) = delete;
    ExcelManager(const Excel& other) = delete;
    ~ExcelManager() = default;

    explicit ExcelManager(Excel& _excel) : excel(_excel) {};
    
    void editExcel(const char* firstNumber, const char* secondNumber, const char* value);
    void runFormulasExcel();
    void setCellBasedOnType(const int _rows, const int _columns, const std::string& cellValue);
    void setExcelDataFromFile(std::ifstream &iFile);
    void createExcelFromFile(std::ifstream& iFile);

};


#endif