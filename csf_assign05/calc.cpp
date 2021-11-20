
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
using std::cout;
using std::endl;

struct Calc {
private:
    // fields
    map<string, int> variables;
    int size;
    
public:
    // public member functions

    int is_valid = 1;
    int evalExpr(const string &expr, int &result) {
        vector<string> expr_vec = tokenize(expr);

        if (!valid_expression(expr_vec)) {
            return 0;
        } 
        if (expr_vec.size() == 1) {
            if ((is_all_letters(expr_vec[0]) && var_exists(expr_vec[0])) || is_all_num(expr_vec[0])) {
                result = determine_val(expr_vec[0]);
                return 1;
            } else {
                return 0;
            }
        
        } else if (expr_vec.size() == 3) {
            if (expr_vec[1].compare("=") == 0) {
                if ((is_all_letters(expr_vec[2]) && var_exists(expr_vec[2])) || is_all_num(expr_vec[2])) {
                    result = determine_val(expr_vec[2]);
                    return assign_variable(expr_vec, result);
                } else {
                    return 0;
                }
            } else {
                if ((is_all_letters(expr_vec[2]) && var_exists(expr_vec[2])) || is_all_num(expr_vec[2])) {
                    if (((is_all_letters(expr_vec[0]) && var_exists(expr_vec[0])) || is_all_num(expr_vec[0]))) {
                        return operation(expr_vec, result);
                        //return 1;
                    }
                }
                return 0;
            }
        } else if (expr_vec.size() == 5) {
            vector<string>::const_iterator first = expr_vec.begin() + 2;
            vector<string>::const_iterator last = expr_vec.end();
            vector<string> temp_vec(first, last);
            cout << temp_vec[1];
            if ((is_all_letters(temp_vec[2]) && var_exists(temp_vec[2])) || is_all_num(temp_vec[2])) {
                if (((is_all_letters(temp_vec[0]) && var_exists(temp_vec[0])) || is_all_num(temp_vec[0]))) {
                    is_valid = operation(temp_vec, result);
                }
            } else {
                return 0;
            }
            if (!is_valid) {
                return 0;
            }
            return assign_variable(expr_vec, result);
        }
        return 0;
    }

private:
    // private member functions
    int determine_val(string s) {
        map<string, int>::iterator it;
        it = variables.find(s);
        if (it != variables.end()) {
            return it->second;
        } else {
            return stoi(s);
        }
    }

    bool var_exists(string s) {
        map<string, int>::iterator it;
        it = variables.find(s);
        if (it != variables.end()) {
            return 1;
        }
        return 0;
    }

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

        if ((expr_vec.size() == 3 || expr_vec.size() == 5) && valid_op(expr_vec[1])) {
            for (int i = 0; i < 3; i+=2) {
                if (!is_all_letters(expr_vec[i]) && !is_all_num(expr_vec[i])) {
                    return 0;
                }
            }
        }

        if ((expr_vec.size() == 3 || expr_vec.size() == 5)&& expr_vec[1].compare("=") == 0) {
            if (!is_all_letters(expr_vec[0])) {
                return 0;
            }

            if (!var_exists(expr_vec[2]) && !is_all_num(expr_vec[2])) {
                return 0;
            }
        }

        if (expr_vec.size() == 5 && (expr_vec[3].compare("=") == 0 || !is_all_letters(expr_vec[0]))) {
            return 0;
        }

        return 1;

    }

    bool is_all_num(string s) {
        int num_char = s.length();
        for (int i = 0; i < num_char; i++) {
            if (!std::isdigit(s[i])) {
                return 0;

            }
        }
        return 1;
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
        if (op.compare("+") || op.compare("-") || op.compare("*") || op.compare("/")) {
            return 1;
        }
        return 0;
    }

    int operation(vector<string> expr_vec, int& result) {
        int operand1;
        int operand2;
        if (is_all_num(expr_vec[0])) {
            operand1 = stoi(expr_vec[0]);
        } else {
            map<string, int>::iterator it;
            it = variables.find(expr_vec[0]);
            if (it != variables.end()) {
                operand1 = it->second;
            } else {
                return 0;
            }
        }

        if (is_all_num(expr_vec[2])) {
            operand2 = stoi(expr_vec[2]);
        } else {
            map<string, int>::iterator it;
            it = variables.find(expr_vec[2]);
            if (it != variables.end()) {
                operand2 = it->second;
            } else {
                return 0;
            }
        }

        char c = expr_vec[1][0];

        switch(c) {
            case '+':
                result = operand1 + operand2;
                break;

            case '-':
                result = operand1 - operand2;
                break;

            case '*':
                result = operand1 * operand2;
                break;

            case '/':
                if (operand2 == 0) {
                    return 0;
                }
                result = operand1 / operand2;
                break;
            
            default:
                return 0;
        }
        return 1;

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