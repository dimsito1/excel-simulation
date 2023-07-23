#include "excel.h"
#include "terminal.h"
#include "string.h"
#include "vector.h"

using std::endl;
using std::cin;
using std::cout;
using std::cerr;


int main() {
    Terminal terminal;
    char input[256];
    bool exit = false;

    while(!exit) {
        cout << '>';
        cin.getline(input, 256);
        terminal.processCommand(input, exit);
    }

    return 0;
}