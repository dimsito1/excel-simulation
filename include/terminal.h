#include "excel.h"
#include "string.h"
#include "utility.h"

#ifndef _TERMINAL_HEADER_INCLUDED_
#define _TERMINAL_HEADER_INCLUDED_

enum class FormulaType {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    SmallerThan,
    SmallerOrEqualThan,
    BiggerThan,
    BiggerOrEqualThan,
    None
};

class Terminal {
private:
    Excel currentExcel;
    std::string currentFileName;
    bool editMode;

private:
    void changeEditMode(bool _editMode) { editMode = _editMode; }
    void setCellBasedOnType(Excel& _excel, const std::string& cellValue, const int _rows, const int _columns);
    void createExcelFromFile(std::ifstream& iFile);
    const std::string isLineValid(const std::string& line, const unsigned rowNumber) const;
    bool isFileValid(std::ifstream& iFile) const;
    void openTextFile(const char* input);
    const char* createTextFile();
    bool getEditMode() const { return editMode; }
    bool isStringValidForCell(std::string& string);
    void setExcelDataFromFile(Excel& excel, std::ifstream& iFile);
    void runFormulasExcel(Excel& _excel);
    void editExcel(const char* firstNumber, const char* secondNumber, const char* value);
    void getErrorMessage(std::string& errorMessage, const char errorChar) const;
    const std::string getResultFromFormula(const long double& firstValue, const long double& secondValue, const FormulaType& formulaType) const;
    const FormulaType getFormulaType(const char firstChar, const char secondChar) const;
    const CellType getStringCellType(const char* string) const;

public:
    Terminal() : editMode(false) {};
    Terminal& operator=(const Terminal& other) = delete;
    Terminal(const Terminal& other) = delete;
    
    void processCommand(const char* string, bool& flag);
    void printWelcomeMessage() const;
};

#endif