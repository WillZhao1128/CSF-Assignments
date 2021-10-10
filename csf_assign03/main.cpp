#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    assert(argc > 1);
    cout << "hello " << argv[1] << "!" << endl;
    return 0;
}