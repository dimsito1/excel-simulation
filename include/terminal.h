#ifndef _TERMINAL_HEADER_INCLUDED_
#define _TERMINAL_HEADER_INCLUDED_

#include "excel.h"
#include "string.h"
#include "utility.h"
#include "formulaType.h"
#include "excelManager.h"

class Terminal {

public:
    Terminal(ExcelManager& _em) : em(_em) {};
    
    Terminal& operator=(const Terminal& other) = delete;
    Terminal(const Terminal& other) = delete;
    ~Terminal() = default;

    void processCommand(const char* string, bool& flag);
    void printCommands() const;
    void printWelcomeMessage() const;

private:
    void openTextFile(const char* input);
    const char* createTextFile();

private:
    ExcelManager& em;
    std::string currFileName;
};

#endif