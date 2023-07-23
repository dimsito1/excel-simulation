#include "excel.h"
#include "string.h"
#include "vector.h"

#ifndef _TERMINAL_HEADER_INCLUDED_
#define _TERMINAL_HEADER_INCLUDED_


class Terminal {
private:
    Excel currentExcel;

    bool editMode;

public:
    Terminal() : editMode(false) {};

    void changeEditMode(const bool _editMode) { editMode = _editMode; }
    const bool getEditMode() const { return editMode; }

    //reads the opened textFile
    void processTextFileIntoExcel(std::ifstream& iFile);
    //processes an input into a char** array where each separate word is a different pointer
    char** processInputIntoArray(const char* input);

    const bool isLineValid(const std::string& line) const;

    const bool isFileValid(std::ifstream& iFile) const;
    //opens a text file, and returns the name of the text file
    const char* openTextFile(const char* input);

    const char* createTextFile();

    void operateExcel(Excel& excel, std::ifstream& iFile, const Vector _commaArray);
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

#endif