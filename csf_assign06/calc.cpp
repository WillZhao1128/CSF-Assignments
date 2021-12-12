/*
 * Implementation of the calculator program
 * CSF Assignment 5
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include <cctype>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <mutex>
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
    std::mutex m;
    int size;
    
public:
    // public member functions

    /*
    * Evaluates the string expression provided 
    * and stores in pass-by-ref result
    *
    * Parameters:
    *   &expr - the string expression
    *   &result - location to store result of expression
    *
    * Returns:
    *   1 if success; 0 if failed
    */
    int evalExpr(const string &expr, int &result) {
        int is_valid = 0;
        vector<string> expr_vec = tokenize(expr);

        // Check to see if the expression is in 1 of 4 formats
        if (!valid_expression(expr_vec)) {
            return 0;
        }

        m.lock(); 

        // Determine what to do based on number arguments provide
        if (expr_vec.size() == 1) {  // vector size = 1: int literal or string
            if (valid_operand(expr_vec[0])) {
                result = determine_val(expr_vec[0]);
                is_valid = 1;
            }
        } else if (expr_vec.size() == 3) { // vector size = 3
            if (expr_vec[1].compare("=") == 0) { // var = operand
                if (valid_operand(expr_vec[2])) {
                    result = determine_val(expr_vec[2]);
                    is_valid = assign_variable(expr_vec, result);
                }
            } else { // operand + operand
                if (valid_operand(expr_vec[2]) && valid_operand(expr_vec[0])) {
                    is_valid = operation(expr_vec, result);
                }
            }
        } else if (expr_vec.size() == 5) { // vector size = 5
            vector<string>::const_iterator first = expr_vec.begin() + 2;
            vector<string>::const_iterator last = expr_vec.end();
            vector<string> temp_vec(first, last); // create temporary vector with expression
            if (valid_operand(temp_vec[2]) && valid_operand(temp_vec[0])) {
                is_valid = operation(temp_vec, result);
            } else {
                m.unlock();
                return 0;
            }
            is_valid = assign_variable(expr_vec, result);
        }

        m.unlock();
        return is_valid;
    }

private:
    // private member functions

    /*
    * Creates a vector of strings based on the provided expression
    *
    * Parameters:
    *   &expr - the string expression
    *
    * Returns:
    *   a vector of tokenized inputs based on the expression
    */
    vector<string> tokenize(const string &expr) {
        vector<string> vec;
        stringstream s(expr);
        string tok;
        while (s >> tok) {
            vec.push_back(tok);
        }
        return vec;
    }

    /*
    * Determines if the variable exists in the calculator
    *
    * Parameters:
    *   s - the string to evaluate (from expression vector)
    *
    * Returns:
    *   1 if the variable exists in the map
    */
    bool var_exists(string s) {
        map<string, int>::iterator it;
        it = variables.find(s);
        if (it != variables.end()) {
            return 1;
        }
        return 0;
    }

    /*
    * Determines/returns the value of either variable or int literal 
    * based on the provided string
    *
    * Parameters:
    *   s - the string to evaluate (from expression vector)
    *
    * Returns:
    *   the value of the integer or int literal
    */
    int determine_val(string s) {
        map<string, int>::iterator it;
        it = variables.find(s);
        if (it != variables.end()) {
            return it->second;
        } else {
            return stoi(s);
        }
    }

    /*
    * Determines if the operand is valid (var stored in calculator,
    * or is an int literal
    *
    * Parameters:
    *   s - the string to evaluate (from expression vector)
    *
    * Returns:
    *   1 if the operand is in the map or is an integer litera
    */
    bool valid_operand(string s) {
        if ((is_all_letters(s) && var_exists(s)) || is_all_num(s)) {

            return 1;
        }
        cout << is_all_num(s) << endl;
        return 0;
    }

    /*
    * Determines if the operation is valid (addition, subtraction,
    * multiplication, or division)
    *
    * Parameters:
    *   op - the string to evaluate (from expression vector)
    *
    * Returns:
    *   1 if the operation is valid
    */
    bool valid_op(string op) {
        if (op.compare("+") || op.compare("-") || op.compare("*") || op.compare("/")) {
            return 1;
        }
        return 0;
    }

    /*
    * Determines if mathematical expression is valid
    *
    * Parameters:
    *   expr_vec - the vector of tokenized version of input string
    *
    * Returns:
    *   1 if the expression is acceptable (one of four possibilities)
    */
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

    /*
    * Determines if a string represents an integer (each character is a number)
    *
    * Parameters:
    *   s - the string to evaluate (from expression vector)
    *
    * Returns:
    *   1 if the string is all digits and/or has negative sign
    */
    bool is_all_num(string s) {
        int num_char = s.length();
        int start = 0;
        if (s[0] == '-') {
            start = 1;
        }
        for (int i = start; i < num_char; i++) {
            if (!std::isdigit(s[i])) {
                return 0;

            }
        }
        return 1;
    }

    /*
    * Determine if a variable is all letters or not
    *
    * Parameters:
    *   s - the string to evaluate (from expression vector)
    *
    * Returns:
    *   1 if the operand is all letters
    */
    bool is_all_letters(string s) {
        int len = s.length();
        for (int i = 0; i < len; i++) {
            if (isalpha(!s[i])) {
                return 0;
            }
        }
        return 1;
    }

    /*
    * Performs the operation defined by operand op operand
    *
    * Parameters:
    *   expr_vec - the tokenized vector of the input string expression
    *   result - the location to store the result of the operation
    *
    * Returns:
    *   1 if the operation requested was sucessful
    */
    bool operation(vector<string> expr_vec, int& result) {
        int operand1 = determine_val(expr_vec[0]);
        int operand2 = determine_val(expr_vec[2]);
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

    /*
    * Assigns a variable with new value or creates new variable
    *
    * Parameters:
    *   expr_vec - the tokenized vector of the input string expression
    *   result - the location to store the result of the operation
    *
    * Returns:
    *   1 if the variable assignment or addition to map was sucessful
    */
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
};



/*
 * Creates a new instance of a calculator struct
 *
 * Parameters:
 *
 * Returns:
 *   the created calculator object
 */
extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

/*
 * Removes/destroys the calculator object
 *
 * Parameters:
 *      *calc - pointer to the calc struct to be deleted
 *
 * Returns:
 */
extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

/*
 * Evaluates an expression
 *
 * Parameters:
 *   *calc - pointer to the calc struct to be deleted
 *   *expr - string specified by user to denote a mathematical expression/equation
 *   *result - the location to store the result of the operation
 *
 * Returns:
 *   1 if the evaluation of the expression was successful; 0 otherwise
 */
extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);
}