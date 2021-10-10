#include "helper.h"
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <cassert>
#include <string.h>

using std::cout;
using std::endl;
using std::cerr;
using std::isdigit;

int main(int argc, char *argv[]) {
    assert(argc > 1);

    if (argc != 9) {
        cerr << "Invalid number of arguments!" << endl;
        exit(1);
    }
    if (handle_arguments(argv, argc) == 0) {
        cerr << "Invalid argument!" << endl;
        exit(1);
    }

    cout << "hello " << argv[1] << "!" << endl;
    return 0;
}



