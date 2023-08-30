#include "excel.h"
#include "string.h"
#include "vector.h"

#ifndef _TERMINAL_HEADER_INCLUDED_
#define _TERMINAL_HEADER_INCLUDED_

class Terminal {
private:
    Excel currentExcel;

    bool editMode;

private:


public:
    Terminal() : editMode(false) {};

    void changeEditMode(const bool _editMode) { editMode = _editMode; }
    const bool getEditMode() const { return editMode; }

    //sets the corresponding cell type to the cell
    void setCellBasedOnType(Excel& _excel, const std::string& cellValue, const int _rows, const int _columns);
    //reads the opened textFile
    void importDataIntoExcel(std::ifstream& iFile);
    //processes an input into a char** array where each separate word is a different pointer
    char** processInputIntoArray(const char* input);

    const std::string isLineValid(const std::string& line, const unsigned rowNumber);

    const bool isFileValid(std::ifstream& iFile);
    //opens a text file, and returns the name of the text file
    const char* openTextFile(const char* input);

    const char* createTextFile();

    const bool isStringValidForCell(std::string& string);

    void operateExcel(Excel& excel, std::ifstream& iFile, const Vector _commaArray);

    void editExcel(const char* firstNumber, const char* secondNumber, const char* value);
    //processes and sorts each corresponding command to its function
    void processCommand(const char* string, bool& flag);
};
//removes all extra spaces from the string
std::string removeExtraSpacesFromString(const std::string& string);
//removes all spaces from the string
std::string removeSpacesFromString(const std::string& string);
//checks whether the input from the terminal is valid
const bool validInput(const char* input);
//frees the "processInputIntoArray"'s function dynamicly allocated memory
void freeInputArray(char** inputArray, const size_t numberOfWords);

size_t wordsCounter(const char* input);
//checks whether a char arr ends correctly
bool charArrEndsCorrectly(const char* input, const char* endInput);

bool isEmptyFile(std::ifstream& iFile);

bool fileExists(const std::string& filename);

CellType getStringCellType(const char* string);

#endif