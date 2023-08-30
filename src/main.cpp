#include "terminal.h"

using std::endl;
using std::cin;
using std::cout;
using std::cerr;


int main() {

    Terminal terminal;
    terminal.printWelcomeMessage();

    char input[256];
    bool exit = false;

    while(!exit) {
        cout << '>';
        cin.getline(input, 256);

        if(cin.fail()) {
            cin.clear();
            //идеята за while-a със cin е от интернета
            while (cin.get() != '\n');
            cout << "Error: Input too long. Please enter a shorter command." << endl;
        }
        else {
            try {
                terminal.processCommand(input, exit);
            }
            catch(...) {
                cerr << "Problem occured during runtime." << endl;
            }
        }
    }

    return 0;
}