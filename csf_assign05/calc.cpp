
#include <cctype>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include "calc.h"

using std::vector;
using std::string;
using std::stringstream;
using std::map;
using std::stoi;
using std::pair;

struct Calc {
private:
    // fields
    map<string, int> variables;
    
public:
    // public member functions
    Calc();
    ~Calc();

    int evalExpr(const string &expr, int &result) {
        vector<string> expr_vec = tokenize(expr);
        if (!valid_expression(expr_vec)) {
            return 0;
        }
        if (expr_vec.size() == 1) {
            result = stoi(expr_vec[0]);
        } else if (expr_vec.size() == 3) {
            if (expr_vec[1].compare("=")) {
                result = stoi(expr_vec[0]);
                return assign_variable(expr_vec, result);

            } else {
                result = operation(expr_vec);
                return 1;
            }
        } else if (expr_vec.size() == 5) {
            vector<string>::const_iterator first = expr_vec.begin() + 2;
            vector<string>::const_iterator last = expr_vec.end();
            vector<string> temp_vec(first, last);
            result = operation(temp_vec);

            return assign_variable(expr_vec, result);
        }

        return 0;

        
    }

private:
    // private member functions

    bool assign_variable(vector<string> expr_vec, int result) {
        map<string, int>::iterator it;
        it = variables.find(expr_vec[0]);
        if (it != variables.end()) {
            it->second = result;

        } else if (is_all_letters(expr_vec[0])) {
            variables.insert(pair<string,int>(expr_vec[0],result));
        } else {
            return 0;
        }
        return 1;

    }

    bool valid_expression(vector<string> expr_vec) {
        int len = expr_vec.size();
        for (int i = 0; i < len; i+=2) {
            if (!is_all_letters(expr_vec[i]) || !is_all_num(expr_vec[i])) {
                return 0;
            }
        }

        for (int i = 1; i < len; i+=2) {
            if (!valid_op(expr_vec[i])) {
                return 0;
            }
        }

        if (expr_vec.size() >= 3 && expr_vec[1].compare("=")) {
            if (!is_all_letters(expr_vec[0])) {
                return 0;
            }
        }

        if (expr_vec.size() == 5 && (expr_vec[3].compare("=") || !is_all_letters(expr_vec[0]))) {
            return 0;
        }

        return 1;

    }

    bool is_all_num(string s) {
        try {
            std::stoi(s);
            return 1;
        } catch(std::invalid_argument e) {
            return 0;
        }
    }


    vector<string> tokenize(const string &expr) {
        vector<string> vec;
        stringstream s(expr);

        string tok;
        while (s >> tok) {
            vec.push_back(tok);
        }

        return vec;
    }

    int valid_op(string op) {
        if (op.compare("+") || op.compare("-") || op.compare("*") || op.compare("/") || op.compare("=")) {
            return 1;
        }
        return 0;
    }

    int operation(vector<string> expr_vec) {
        int operand1 = stoi(expr_vec[0]);
        int operand2 = stoi(expr_vec[1]);

        char c = expr_vec[1][0];

        switch(c) {
            case '+':
                return operand1 + operand2;

            case '-':
                return operand1 - operand2;

            case '*':
                return operand1 * operand2;

            case '/':
                return operand1 / operand2;
            
            default:
                std::cerr << "Error";
                exit(1);
        }

    }

    // Determine if a variable is all letters or not
    bool is_all_letters(string s) {
        int len = s.length();
        for (int i = 0; i < len; i++) {
            if (isalpha(!s[i])) {
                return 0;
            }
        }
        return 1;
    }

};



extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);
}