#include "helper.h"
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <cassert>
#include <string.h>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::cerr;
using std::isdigit;
using std::string;


int main(int argc, char *argv[]) {
    if (argc != 7) {
        cerr << "Invalid number of arguments!" << endl;
        exit(1);
    }
    if (handle_number_arguments(argv) == 0 || handle_string_arguments(argv) == 0) {
        cerr << "Invalid argument!" << endl;
        exit(1);
    }
    string s;
    while (std::getline(std::cin, s)){
        cout << s << endl;
    }
    
    
    cout << "hello " << argv[1] << "!" << endl;
    return 0;
}



