
#include <cctype>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>

using std::vector;
using std::string;
using std::stringstream;
using std::map;

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

        
        if (expr_vec.size() == 1) { // Handles case of a 1 expression
            return length_one(expr_vec[0], result);
        } else if (expr_vec.size() == 3) { // Handles case of a 3 expression
            return length_three(expr_vec, result);

        } else if (expr_vec.size() == 5) {
        
        } else {
            return 0;
        }
    }

private:
    // private member functions


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
        if (op == "+" || op == "-" || op == "*" || op == "/") {
            return 1;
        }
        return 0;
    }



    int operation(int operand1, int operand2, char op) {
        switch(op) {
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

    int length_one(string arg1, int &result) {
        if (is_all_letters(arg1)) {
            map<string, int>::iterator it;
            it = variables.find(arg1);
            if (it != variables.end()) {
                result = it->second;
                return 1;

            }
        } else {
            try {
                int val = std::stoi(arg1);
                return 1;
            } catch (std::invalid_argument e) {
                return 0;
            }
        }
    }

    int length_three(vector<string> expr_vec, int &result) {
        // case first is a variable
        if (expr_vec[1] == "+") {
            

        } else if (valid_op(expr_vec[1])) {

        } else {
            return 0;
        }





        return 0;
    }

    int length_five(vector<string> expr_vec, int &result) {
        return 0;
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