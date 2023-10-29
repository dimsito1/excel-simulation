#include "terminal.h"
#include "excelManager.h"
#include "excel.h"

using std::endl;
using std::cin;
using std::cout;
using std::cerr;

int main() {
    Excel excel;
    ExcelManager em(excel);
    Terminal terminal(em);
    terminal.printWelcomeMessage();

    char input[256];
    bool exit = false;

    while(!exit) {
        cout << '>';
        cin.getline(input, 256);

        if(cin.fail()) {
            cin.clear();
            while (cin.get() != '\n');
            cout << "Error: Input too long. Please enter a shorter command." << endl;
        }
        else {
            try {
                terminal.processCommand(input, exit);
            }
            catch(...) {
                cerr << "Problem occurred during runtime." << endl;
            }
        }
    }

    return 0;
}