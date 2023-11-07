#include "excel.h"
#include "terminal.h"
#include "validator.h"
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

void Terminal::printWelcomeMessage() const {
    cout << "Welcome to-" << endl;
    cout << "  ______              _  _____ _                 _       _   _              "         << endl;
    cout << " |  ____|            | |/ ____(_)               | |     | | (_)             "         << endl;
    cout << " | |__  __  _____ ___| | (___  _ _ __ ___  _   _| | __ _| |_ _  ___  _ __   "         << endl;
    cout << " |  __| \\ \\/ / __/ _ \\ |\\___ \\| | '_ ` _ \\| | | | |/ _` | __| |/ _ \\| '_ \\  " << endl;
    cout << " | |____ >  < (_|  __/ |____) | | | | | | | |_| | | (_| | |_| | (_) | | | | "         << endl;
    cout << " |______/_/\\_\\___\\___|_|_____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__|_|\\___/|_| |_| "  << endl;
    cout << endl;
    cout << "Use command \"open <filename.txt>\" to get started." << endl;
    cout << "Use command \"exit\" to exit the program at any time." << endl;
}

void Terminal::printCommands() const {
    cout << "Text file opened successfully.\n" << endl;
    cout << "Available commands:" << endl;
    cout << "- print                           : Display the contents." << endl;
    cout << "- print spreadsheet               : Show the spreadsheet format." << endl;
    cout << "- print types                     : Display data types of the contents." << endl;
    cout << "- edit <row> <column> <new_value> : To edit a cell's value using integers for rows and columns." << endl;
    cout << "- save                            : Save the current file and its changes." << endl;
    cout << "- close                           : Close the current file." << endl;
    cout << "- exit                            : To fully exit the whole program." << endl;
    cout << "\nPlease enter a command to proceed." << endl;
}

const char* Terminal::createTextFile() {
    cout << "Enter a name for a new text file: ";

    char newInput[256];
    cin.getline(newInput, 256);

    size_t numberOfWords = utility::wordsCounter(newInput);

    if (!validate::isValidInput(newInput) || (!validate::cStringEndsCorrectly(newInput, ".txt")) || numberOfWords != 1) {
        cout << "Invalid input. (file should be .txt)" << endl;
        return nullptr;
    }

    std::string filePath = "../data/";
    filePath += newInput;
    std::ofstream newFile(filePath);
    cout << "New text file \"" << newInput << "\" created successfully." << endl;
    newFile.close();

    char* result = new char[strlen(newInput) + 1];
    strcpy(result, newInput);

    return result;        
}

void Terminal::openTextFile(const char* input) {
    std::string filePath;
    // manage root of the project so it has correct execution
#if defined(_WIN32) || defined(_WIN64)
    // Windows specific code
    filePath = "data/";
#elif defined(__linux__) || defined(__APPLE__)
    // Linux specific code
    const char *projectRoot = std::getenv("EXCEL_SIMULATION_ROOT");
    if (projectRoot) {
        filePath = projectRoot;
        filePath += "/data/";
    } else {
        filePath = "../data/";
    }
#else

    #error "Unknown compiler"

#endif

    filePath += input;
    std::ifstream iFile(filePath);
    if (!iFile) {
        cerr << "The file you provided does not exist."<< endl;
        iFile.close();

        const char* tempFileName = createTextFile();
        if (!tempFileName) {
            return;
        }
        currFileName = tempFileName;
    }
    
    if (validate::isEmptyFile(iFile)) {
        cerr << "The file you provided is empty." << endl;
        cerr << "Fill it accordingly then proceed." << endl;
        return;
    }

    currFileName = input;

    em.createExcelFromFile(iFile);
    
    printCommands();
    
    iFile.close();
}

void Terminal::processCommand(const char* string, bool& flag) {
    const size_t numberOfWords = utility::wordsCounter(string);
    char** InputArray = utility::processInputIntoArray(string);
    bool currEditMode = em.getEditMode();

    if (strcmp(InputArray[0], "open") == 0 && numberOfWords == 2 && validate::isValidInput(string) && currEditMode == false) {
        openTextFile(InputArray[1]);
        
        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "edit") == 0 && numberOfWords > 3 && validate::isDigitInteger(InputArray[1]) && validate::isDigitInteger(InputArray[2])) {
        if (!currEditMode) {
            cerr << "Excel edit failed. (you are not in edit mode)" << endl;
            utility::freeInputArray(InputArray, numberOfWords);
            return;
        }
        
        std::string tempString = "";

        if (InputArray[3][0] == '\"') {
            const char* newString = utility::getStringAfterQuote(string);
            tempString = newString;
            delete[] newString;
        }

        else {
            size_t remainingWords = numberOfWords - 3;

            for (size_t i = 0; i < remainingWords; ++i) {
                tempString += InputArray[3 + i];
                tempString += ' ';
            }
            tempString[tempString.size() - 1] = '\0';
        }

        if (!validate::isStringValidForCell(tempString)) {
            cerr << "In \"edit <row> <column> <new_value>\" the value should be valid for a cell. (Strings should be inside quotation marks)" << endl;
            utility::freeInputArray(InputArray, numberOfWords);
            return;
        }

        em.editExcel(InputArray[1], InputArray[2], tempString.c_str());

        utility::freeInputArray(InputArray, numberOfWords);
    }
    else if (strcmp(InputArray[0], "exit") == 0 && numberOfWords == 1 && validate::isValidInput(string)) {
        utility::freeInputArray(InputArray, numberOfWords);
        flag = true;
    }

    else if (strcmp(InputArray[0], "close") == 0 && numberOfWords == 1 && validate::isValidInput(string) && em.getEditMode() == true) {
        em.changeEditMode(false);        
        cout << "Excel closed successfully." << endl;

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "print") == 0 && numberOfWords == 1 && validate::isValidInput(string)) {
        if (em.getEditMode()) {
            em.getExcel().printExcel();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else  if (strcmp(InputArray[0], "print") == 0 && strcmp(InputArray[1], "spreadsheet") == 0 && numberOfWords == 2 && validate::isValidInput(string)) {
        if (currEditMode) {
            em.getExcel().printSpreadsheet();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "print") == 0 && strcmp(InputArray[1], "types") == 0 && numberOfWords == 2 && validate::isValidInput(string)) {
        if (currEditMode) {
            em.getExcel().printTypes();
        }
        else {
            cerr << "Excel print failed. (you are not in edit mode)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else if (strcmp(InputArray[0], "save") == 0 && numberOfWords == 1) {
        if (currEditMode) {
            em.getExcel().saveExcelToFile(currFileName.c_str());
            cout << "File saved successfully." << endl;
        }
        else {
            cerr << "Excel save failed. (you have not edited anything)" << endl;
        }

        utility::freeInputArray(InputArray, numberOfWords);
    }

    else {
        currEditMode == true ? cerr << "You are in edit mode. (use \"close\" to stop edit mode)" << endl : cerr << "Incorrect input. " << endl;
        utility::freeInputArray(InputArray, numberOfWords);
    }
}