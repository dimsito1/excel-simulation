
#ifndef _VALIDATOR_HEADER_INCLUDED_
#define _VALIDATOR_HEADER_INCLUDED_

#include <string>
#include <fstream>

namespace validate {
    std::string isLineValid(const std::string& line, const unsigned rowNumber);
    bool isFileValid(std::ifstream& iFile);
    inline bool isEmptyFile(std::ifstream& iFile) { return iFile.peek() == std::ifstream::traits_type::eof(); }
    bool isValidInput(const char* input);
    bool isStringValidForCell(std::string& string);

    bool fileExists(const std::string& filename);
    bool cStringEndsCorrectly(const char* input, const char* endInput);

    bool isDigitInteger(const char* number);
    bool isValidNumber(const char* number);
    bool isDouble(const char* charNumber);

    bool boolArrIsAllFalseOrAllTrue(const bool* array, const size_t size);

    inline bool isDigit(const char ch) { return ch >= '0' && ch <= '9'; }
    inline bool isLetter(const char ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z'); }
}

#endif