/*
Assignment: Program 8 with honors contract.
Written by: Max DeSantis
Instructor: Dr. Churchill

NOTES:

The infix calculator has most of the same functions as the rpn calculator. It does not feature the swap, copy, peek, and dump functionality.
It also does not feature the ' and ~ operators, for inversion and negation respectively, as the lack of stack diminishes their value.

Another change is to constants and variables. While in infix mode, constants and variables can only be declared with entirely alphabetic names. I did this to
simplify the parsing performed on the stringstream passed to the function.

When a user calls -infix, the program begins parsing all input within the function infixInit(). Once an '=' has been parsed, infixInit() calls the infix() function
with a stringstream of a modified version of the user input. Infix is then called recursively until it reads the '=' and returns.
There is likely a much more efficient/clean method of doing this, but this works.

*/
#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cctype>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

class calc {
private:
    /* Utility */
    char* programName = NULL;
    string useText[65] = {
        "Usage: ", //file name
        "\n\n\"", //file name
        " --help\"\n",
        "     Display this usage material\n",
        "\n\n\"",
        " -rpn\"\n", //file name
        "     The program accepts input from standard input as a sequence of numbers and operators.\n",
        "     The numbers (operands, as integers or floating point numbers) read are pushed on a stack until needed.\n",
        "     When an operator is read, the required operands are popped from the stack and used to perform the calculation,\n"
        "     with the result placed on the stack.\n",
        "     Valid operators are +, -, *, and /, interpreted as addition, subtraction, multiplication, and division,\n"
        "     respectively, as described below.\n",
        "     The ** operator is exponentiation. The sin, cos, asin, and acos functions are the standard trigonometric functions.\n",
        "     The swap and copy functions cause the top two stack elements to be exchanged and the top element to be duplicated\n"
        "     on the stack, respectively.\n",
        "     The ~ operator negates the operand, while the ' operator inverts it.\n"
        "     An additional operator is =, which indicates that the value at the top of the stack is popped from the stack\n",
        "     and displayed along with the number of values remaining on the stack, whereupon the program terminates.\n\n",
        "     The peek and dump functions dislay the top-of-stack value and the contents of the stack,\n"
        "     top to bottom, respectively,",
        "     and are provided to assist in debugging complex expressions.\n\n",
        "     Stack underflows generate an error message and halt the program.\n",
        "     Unrecognized input tokens produce error messages and result in program termination, as do unrecognized command line arguments.\n\n",
        "     Stack operations are performed so as to produce results identically as indicated here:\n",
        "     + : push(pop() + pop());\n",
        "     - : temp = pop(); push(pop() - temp);\n",
        "     * : push(pop() * pop());\n",
        "     / : temp = pop(); push(pop() / temp);\n",
        "    ** : temp = pop(); push(pow(pop(),temp));\n",
        "     ' : push(1.0 / pop());\n",
        "     ~ : push(-pop());\n",
        "   sin : push(sin(pop()));\n"
        "   cos : push(cos(pop()));\n",
        "  asin : push(asin(pop()));\n",
        "  acos : push(acos(pop()));\n",
        "  swap : t1 = pop(); t2 = pop(); push(t1); push(t2);\n",
        "  copy : temp = pop(); push(temp); push(temp);\n",
        "  peek : display the value at the top of the stack, without altering stack contents.\n",
        "  dump : display stack contents, one value per line, ffrom stack top to bottom, without altering stack contents.\n",
        "     = : pop the stack top and display it as the result with the number of items remaining on the stack.\n\n",
        "    Constants may be defined by use of the :: prefix followed by an alphabetic character and zero or more alphanumeric characters.\n",
        "    Reference to the constants declared must follow the definition, and consists o the characters following the :: prefix.\n",
        "    Definition of a constant consumes the current stack-top value.\n\n",
        "    Variables may be defined by use of the :$ prefix followed by an alphabetic character and zero or more alphanumeric\n",
        "    characters in a manner simiilar to defining constants. The behavior of variables is similar to constants, as definition uses\n",
        "    the value of the variable, and reference to the variable's value is by the name used in its definition. A variable must also\n",
        "    be declared before it is referenced. The primary difference is that variables may be reassigned values by use of the same prefix/name token.\n",
        "    Note that variables cannot be converted into constants, nor constants into variables.\n\n",
        "    Upon encountering an error in the expression being evaluated, this program will report the error and ask whether to reattempt an evaluation.\n",
        "    Y or y indicates to try again, N or n terminates the program.\n\n",
        "    Additional Command-Line Control Flags\n",
        "    -i : indicates that the next command-line argument is an input file name. The file must contain a properly formatted expression\n",
        "         for the expression type indicated by an -rpn, -infix or -prefix argument, using the ascii character set.\n",
        "    -o : indicates that the next command-line argument is an output file name. If the file already eists, it will be overwritten.\n",
        "\n\n\"",
        " -infix\"\n",
        "    The infix calculator accepts input in the traditional format of 'operand operator operand'.\n",
        "    The calculator is implemented with a recursive descent parser that increases in depth when an '(' is encountered.\n",
        "    For simplicity, proper order of operations is not supported. Expressions will need to be separated with parentheses unless left-to-right evaluation\n",
        "    is acceptable. Additionally, a space between numbers will be ignored if there is no operator. For example: '5 5 - 1 2 =' is the same as '55 - 12 ='\n",
        "    Supported operators are as follows: +, -, *, /, sin, cos, asin, acos, ^ (exponentiation), :: (constant definition), :$ (variable definition).\n",
        "    The execution will result in an error if an '=' is detected within a set of parentheses or if an '(' or ')' aren't a proper pair.\n\n",
        "    Declaring constants and variables is similar to the -rpn functionality. The operator will use the last read value to its left and store that under\n",
        "    a variable of the given name. A major difference is that valid names can only be made of alphabetic characters.\n",
        "    Example names: a, ab, abcdef. Variables can be reassigned values; constants cannot. Variables and constants cannot share names.\n\n",
        "    Upon encountering an error in the expression being evaluated, this program will report the error and ask whether to reattempt an evaluation.\n",
        "    Y or y indicates to try again, N or n terminates the program.\n\n",
        "    Additional Command-Line Control Flags\n",
        "    -i : indicates that the next command-line argument is an input file name. The file must contain a properly formatted expression\n",
        "         for the expression type indicated by an -rpn, -infix or -prefix argument, using the ascii character set.\n",
        "    -o : indicates that the next command-line argument is an output file name. If the file already eists, it will be overwritten.\n"
    };

    /* Infix specific*/
    double infixResult = 0;
    string opList[5] = {"+", "-", "*", "/", "^"};
    string trigOpList[4] = {"sin", "cos", "asin", "acos"};

    /* Rpn specific */
    vector<double> stack;

    struct cvNode {
        double val;
        bool cvtype;
    };

    /* General - Used by rpn and infix */
    istream* inStream;
    ostream* outStream;
    unordered_map<string, struct cvNode*> valMap;
    int errorCode = 0;

    double infixEval(double val1, string op, double val2) {
        if(op.compare("+") == 0) {
            return (val1 + val2);
        }
        else if(op.compare("-") == 0) {
            return (val1 - val2);
        }
        else if(op.compare("*") == 0) {
            return (val1 * val2);
        }
        else if(op.compare("/") == 0) {
            if(val2 != 0) {
                return (val1 / val2);
            }
            else {
                throw 6; //Divide by zero error
            }
        }
        else if(op.compare("^") == 0) {
            //Check for negative to fraction power
            if( (fabs(val1) < 1 && fabs(val1 > 0)) && val2 < 0) {
                throw 11;
            }
            else {
                return pow(val1, val2);
            }
        }
        else {
            throw 13;
        }
    }
    double infixEvalTrig(string op, double val) {
        if(op.compare("sin") == 0) {
            return sin(val);
        }
        else if(op.compare("cos") == 0) {
            return cos(val);
        }
        else if(op.compare("asin") == 0) {
            if(val <= 1 && val >= -1) {
                return asin(val);
            }
            else {
                throw 4; //Out of domain.
            }
            
        }
        else if(op.compare("acos") == 0) {
            if(val <= 1 && val >= -1) {
                return (acos(val));
            }
            else {
                throw 5; //Out of domain.
            }
        }
        else {
            throw 13;
        }
    }
    double infix(stringstream& buffer, int depth) {
    
        double rollingData = 0;
        double newData = 0;
        bool defaultOperatorFound = false;
        bool trigOperatorFound = false;
        string temp;
        string op = " ";
        string trigOp;
        
        while(true) {
            buffer.clear();
            buffer >> temp;

            /* Look for variables with value stored in buffer */
            unordered_map<string, struct cvNode*>::iterator savedVal = valMap.find(temp);

            /* Begin item parsing */
            if( (savedVal != valMap.end()) || sscanf(temp.c_str(), "%lf", &newData) == 1) {

                /* Store saved value into newData variable*/
                if(savedVal != valMap.end()) {
                    newData = savedVal->second->val;
                }

                /* If item is numeric, check if operator has been found */
                if(trigOperatorFound) {
                    newData = infixEvalTrig(trigOp, newData);
                    trigOperatorFound = false;
                }

                if(defaultOperatorFound) {
                    /* If operator has been found, value is on right hand side. Evaluate. */
                    rollingData = infixEval(rollingData, op, newData);

                    defaultOperatorFound = false;
                }
                else {
                    /* If operator has not been found, value is on left hand side*/
                    rollingData = newData;
                }
            }

            /* An equals should only be found in depth of 0, in which overall result is returned */
            else if(temp.compare("=") == 0){
                if(depth == 0) {
                    return rollingData;
                }
                else {
                    throw 1; //Improper nesting of "="
                }
            }

            /* An ")" should return the value and exit its recursion. If at depth 0, nesting error. */
            else if(temp.compare(")") == 0) {
                if(depth != 0) {
                    return rollingData;
                }
                else {
                    throw 2; //Improper nesting of ")"
                }
            }

            /* An "(" should begin a new recursion */
            else if(temp.compare("(") == 0) {
                newData = infix(buffer, depth + 1);

                if(trigOperatorFound) {
                    newData = infixEvalTrig(trigOp, newData);
                    trigOperatorFound = false;
                }

                if(defaultOperatorFound) {
                    rollingData = infixEval(rollingData, op, newData);
                    defaultOperatorFound = false;
                }
                else {
                    rollingData = newData;
                }
            }
            
            /* Check for constants (no error checking - it's performed in infixInit() function */
            else if(temp.substr(0, 2).compare("::") == 0) {

                /* Look for saved value in valMap*/
                unordered_map<string, struct cvNode*>::iterator con = valMap.find(temp.substr(2));

                /* Check if saved value of that name already exists*/
                if(con == valMap.end()) {

                    //Saved value of that name does not exist; construct it.
                    struct cvNode* newConstant = (struct cvNode*) malloc(sizeof(struct cvNode));
                    newConstant->val = rollingData;
                    newConstant->cvtype = true;
                    valMap.emplace(temp.substr(2), newConstant);
                }
                else {
                    //Saved value exists; constants cannot be reassigned; constants cannot have same name as variable; throw error.
                    if(con->second->cvtype) {
                        throw 8;
                    }
                    else {
                        throw 9;
                    }
                }
            }

            /* Check for variables (no error checking - it's performed in infixInit() function */
            else if(temp.substr(0, 2).compare(":$") == 0) {

                /* Look for saved value in valMap*/
                unordered_map<string, struct cvNode*>::iterator var = valMap.find(temp.substr(2));

                /* Check if value of that name already exists*/
                if(var == valMap.end()) { //Saved value of that name does not exist yet; construct it.
                    struct cvNode* newVariable = (struct cvNode*) malloc(sizeof(struct cvNode));
                    newVariable->val = rollingData;
                    newVariable->cvtype = false;
                    valMap.emplace(temp.substr(2), newVariable);
                }
                else if(!var->second->cvtype){ //Saved value of that name exists; check if its a variable.
                    var->second->val = rollingData;
                }
                else { //Saved value is a constant, not variable; throw error.
                    throw 7;
                }
            }
            
            /* Check for all operators at once by comparing to known lists. */
            else {
                for(string tempOp : opList) {
                    if(temp.compare(tempOp) == 0) {
                        op = temp;
                        defaultOperatorFound = true;
                    }
                }
                for(string tempOp : trigOpList) {
                    if(temp.compare(tempOp) == 0) {
                        trigOp = temp;
                        trigOperatorFound = true;
                    }
                }
                
                if(!defaultOperatorFound && !trigOperatorFound) {
                    throw 13; //Unrecognized item. Not a number nor operator.
                }
            }
        }
    }

public:

    /* Initialize calculator and point name at argv[0] of main function */
    calc(char* name) {
        programName = name;
    }

    /* Store input and output streams as pointers */
    void setup(istream* inStream, ostream* outStream) {
        this->inStream = inStream;
        this->outStream = outStream;
    }

    /* Return infix-specific expression result */
    double expResultInfix() {
        return infixResult;
    }

    /* Return error code for future parsing with errorText */
    int evalError() {
        return errorCode;
    }

    /* Return infix-specific errors using provided error code */
    string errorTextInfix(int error) {
        string returnText = "\n***ERROR: ";
        switch(error) {
            case 1:
                returnText += "Improper nesting of \"=\"";
                break;
            case 2:
                returnText += "Improper nesting of \")\"";
                break;
            case 4:
                returnText += "Specified value is not in arcsines domain of [-1, 1].";
                break;
            case 5:
                returnText += "Specified value is not in arccosines's domain of [-1, 1].";
                break;
            case 6:
                returnText += "Cannot divide by zero.";
                break;
            case 7:
                returnText += "A constant already has that name.";
                break;
            case 8:
                returnText += "The value of a constant cannot be redefined.";
                break;
            case 9:
                returnText += "A variable already has that name.";
                break;
            case 10:
                returnText = "***INVALID DECLARATION: Non-alphabetic character detected in constant/variable name.";
                break;
            case 11:
                returnText += "Proper fraction exponents cannot have a negative base.";
                break;
            case 12:
                returnText += "Improper usage of constant/variable declaration operator \":\"";
                break;
            case 13:
                returnText += "Unrecognized tokens entered.";
                break;
            default:
                returnText += "Unknown error code.";
                break;
        }
        return returnText;
    }

    /* Parsing and formatting user input to be easily usable by infix() function*/
    bool infixInit() {
        string buffer;
        stringstream streamBuf;
        bool strWritten = false;
        char temp;
        char prevTemp = ' ';

        string tempStr;

        while(prevTemp != '=') {
            *inStream >> tempStr;

            strWritten = false;

            /* Check for known multi-character values */
            for(string str : trigOpList) {
                if(tempStr.compare(str) == 0) {
                    buffer += " ";
                    buffer += tempStr;
                    strWritten = true;
                    break;
                }
            }

            /* Check for potential constants/variables */
            if(tempStr.at(0) == ':') {
                if(tempStr.size() > 2) {
                    if(tempStr.at(1) == ':' || tempStr.at(1) == '$') {
                        //Potential constant or variable. Begin error checking name.

                        for(int i = 2; i < tempStr.size(); ++i) {
                            if(!isalpha(tempStr.at(i))) {
                                throw 10;
                            }
                        }

                        buffer+= " ";
                        buffer+= tempStr;
                        buffer += " ";
                        strWritten = true;
                    }
                    else {
                        throw 12; //Not an actual operator.
                    }
                }
                else {
                    throw 12; //Not long enough.
                }
            }

            /* Check for numbers, single-character operators, and parentheses */
            if(!strWritten) {
                for(int i = 0; i < tempStr.size(); ++i) {
                    if(tempStr.at(i) == '\n' || tempStr.at(i) == ' ') {}    //Don't place newlines and spaces into the buffer.

                    else if(isdigit(tempStr.at(i)) || tempStr.at(i) == '.') {
                        if(isdigit(prevTemp) || prevTemp == '.' || prevTemp == '-') {          //Don't put spaces between a continuous number.
                            buffer += tempStr.at(i);
                        }
                        else {                                              //Place space before new number
                            buffer += " ";
                            buffer += tempStr.at(i);
                        }
                    }

                    else if(isalpha(tempStr.at(i))) {                       //Don't separate alphanumeric strings. They may be variable calls.
                        if(isalpha(prevTemp)) {
                            buffer+= tempStr.at(i);
                        }
                        else {
                            buffer += " ";
                            buffer += tempStr.at(i);
                        }
                    }
                    else if(tempStr.compare("-") == 0) {
                        buffer = buffer + " " + tempStr + " ";
                    }

                    else {
                        buffer += " ";
                        buffer += tempStr.at(i);
                    }

                    prevTemp = tempStr.at(i);
                }
            }
            
            
        }

        /* Convert to stringstream and begin function */
        streamBuf.str(buffer);
        try {
            infixResult = infix(streamBuf, 0);
            return true;
        }
        catch (int e) {
            errorCode = e;
            return false;
        }
    }

    double expResultRpn() { //Return evaluation answer
        return pop();
    }

    int stackSize() { //Return number of elements remaining on stack
        return stack.size();
    }

    /* Reset calculator for use after previous error */
    void resetCalc() {
        valMap.clear();
        stack.clear();

        return;
    }

    /* Compare thrown error code to database of known errors, then return error text */
    string errorTextRpn(int error) {
        string returnText = "***ERROR: ";
        switch(error) {
            case 1:
                returnText += "Cannot divide by zero.";
                break;
            case 2:
                returnText += "Proper fraction exponents cannot have a negative base.";
                break;
            case 3:
                returnText += "Specified value is not in arcsines domain of [-1, 1].";
                break;
            case 4:
                returnText += "Specified value is not in arccosines's domain of [-1, 1].";
                break;
            case 5:
                returnText += "Stack underflow - insufficient operands.";
                break;
            case 6:
                returnText = "***INVALID DECLARATION: Non-alphanumeric character detected in constant/variable name.";
                break;
            case 7:
                returnText += "Constant of that name is already defined.";
                break;
            case 8:
                returnText += "A variable already has that name.";
                break;
            case 9:
                returnText = "***INVALID DECLARATION: Non-alphabetic character detected at start of constant/variable name.";
                break;
            case 10:
                returnText += "A constant already has that name.";
                break;
            case 11:
                returnText += "No variables/constants defined to be used. "; //Allow fallthrough because this can be a double error.
            case 12:
                returnText += "Unrecognized tokens entered.";
                break;
            default:
                returnText += "Unknown error code.";
                break;
        }
        return returnText;
    }

    bool rpn() {
        
        double val = 0.0;
        string buffer;
        string dump;
        stringstream ss;

        while(true) {

            /*---- Checking for inputs ----*/
            *inStream >> buffer;

            /* Clean stream and attempt to store in val*/
            ss >> dump;
            ss.clear();
            ss.str(buffer);
            ss >> val;

            try {

                /* If something was stored in val, push it onto the stack */
                if(ss) {
                    push(val);
                }

                /*---- Begin checking for operators ----*/
                else if(buffer.compare("+") == 0) { push(pop() + pop()); }                  //Addition

                else if(buffer.compare("-") == 0) {                                         //Subtraction
                    val = pop();
                    push(pop() - val);
                }

                else if(buffer.compare("*") == 0) { push(pop() * pop()); }                  //Multiplication

                else if(buffer.compare("/") == 0) {                                         //Division
                    val = pop();
                    if(val == 0.0) { //Check for divide-by-zero
                        throw 1;
                    }
                    else {
                        push(pop() / val);
                    }
                }
                else if(buffer.compare("**") == 0) {                                        //Exponentiation
                    val = pop();
                    double val2 = pop();
                    if( (fabs(val) < 1 && fabs(val > 0)) && val2 < 0) {
                        throw 2;
                    }
                    else {
                        push(pow(val2, val));
                    }
                    
                }
                else if(buffer.compare("'") == 0) {                                         //Inversion
                    val = pop();
                    if(val != 0) {
                        push(1.0 / val);
                    }
                    else {
                        throw 1; //Check for divide-by-zero
                    }
                    
                }
                else if(buffer.compare("~") == 0) {                                         //Negation
                    push(-pop());
                }
                else if(buffer.compare("sin") == 0) { push(sin(pop())); }                   //Sine function

                else if(buffer.compare("cos") == 0) { push(cos(pop())); }                   //Cosine function

                else if(buffer.compare("asin") == 0) {                                      //Arcsine function
                    val = pop();
                    if(val >= -1 && val <= 1) { //Check if in arcsine's domain
                        push(asin(val));
                    }
                    else {
                        throw 4;
                    }
                }

                else if(buffer.compare("acos") == 0) {                                      //Arccosine function
                    val = pop();

                    if(val >= -1 && val <= 1) { //Check if in arccos's domain
                        push(acos(val));
                    }
                    else {
                        throw 5;
                    }
                }

                else if(buffer.compare("swap") == 0) {                                      //Swap top of data stack
                    double temp1 = pop();
                    double temp2 = pop();
                    push(temp1);
                    push(temp2);
                }

                else if(buffer.compare("copy") == 0) {                                      //Copy top of data stack
                    val = pop();
                    push(val);
                    push(val);
                }

                else if(buffer.compare("peek") == 0) {                                      //Print top of stack
                    if(!stack.empty()) {
                        *outStream << "Top value: " << stack.back() << "\n";
                    }
                    else {
                        *outStream << "Stack is empty.\n";
                    }
                }
                
                else if(buffer.compare("dump") == 0) {
                    if(!stack.empty()) {
                        int i = 0;
                        for(i = 0; i < stack.size(); ++i) {
                            *outStream << "Top+" << setw(2) << right << setfill('0') << i << left << setfill(' ') << " : " << stack.at(stack.size() - i - 1) << "\n"; //output a formatted label for stack.
                        }
                    }
                    else {
                        *outStream << "\nStack is empty.\n";
                    }
                    
                }

                else if(buffer.size() > 2 && buffer.substr(0, 2).compare("::") == 0) {//Create a constant
                    if(isalpha(buffer.at(2))) { //Check for alphabetic character at start of name

                        for(int i = 3; i < buffer.size(); ++i) { //Check for alphanumeric characters in rest of name
                            if(!isalnum(buffer.at(i))) {
                                throw 6;
                            }
                        }

                        /* Look for saved value in valMap*/
                        unordered_map<string, struct cvNode*>::iterator con = valMap.find(buffer.substr(2));

                        /* Check if saved value of that name already exists*/
                        if(con == valMap.end()) {

                            //Saved value of that name does not exist; construct it.
                            struct cvNode* newConstant = (struct cvNode*) malloc(sizeof(struct cvNode));
                            newConstant->val = pop();
                            newConstant->cvtype = true;
                            valMap.emplace(buffer.substr(2), newConstant);
                        }
                        else {
                            //Saved value exists; constants cannot be reassigned; constants cannot have same name as variable; throw error.
                            if(con->second->cvtype) {
                                throw 7;
                            }
                            else {
                                throw 8;
                            }
                        }
                    }
                    else {
                        throw 9;
                    }
                }

                else if(buffer.size() > 2 && buffer.substr(0, 2).compare(":$") == 0) {      //Create or change a variable
                    if(isalpha(buffer.at(2))) { //Check for alphabetic character at start of name

                        for(int i = 3; i < buffer.size(); ++i) { //Check for alphanumeric characters in the rest of name
                            if(!isalnum(buffer.at(i))) {
                                throw 6;
                            }
                        }

                        /* Look for saved value in valMap*/
                        unordered_map<string, struct cvNode*>::iterator var = valMap.find(buffer.substr(2));

                        /* Check if value of that name already exists*/
                        if(var == valMap.end()) { //Saved value of that name does not exist yet; construct it.
                            struct cvNode* newVariable = (struct cvNode*) malloc(sizeof(struct cvNode));
                            newVariable->val = pop();
                            newVariable->cvtype = false;
                            valMap.emplace(buffer.substr(2), newVariable);
                        }
                        else if(!var->second->cvtype){ //Saved value of that name exists; check if its a variable.
                            var->second->val = pop();
                        }
                        else { //Saved value is a constant, not variable; throw error.
                            throw 10;
                        }
                        
                    }
                    else {
                        throw 9;
                    }
                }

                else if(buffer.compare("=") == 0) {                                  //Equals (return top of stack and display node counts)
                    return true;
                }

                else if(buffer.compare("\n") != 0 && buffer.compare(" ") != 0) {            //Ignore white space and check for constants
                    
                    /* Check if any variables or constants exist */
                    if(!valMap.empty()) { //Some exist

                        /* Look for variables with value stored in buffer */
                        unordered_map<string, struct cvNode*>::iterator savedVal = valMap.find(buffer);

                        /* If variable/constant has been found, push value onto stack */
                        if(!(savedVal == valMap.end())) { //found
                            push(savedVal->second->val);
                        }
                        else { //not found; throw error
                            printUsage();
                            throw 12;
                        }
                    }
                    else { //No variables or constants exist; throw error.
                        printUsage();
                        throw 11;
                    }
                }
            }
            catch (int e) {
                errorCode = e;
                return false;
            }
        }
    }

    void push(double newVal) {
        stack.push_back(newVal);
        return;
    }

    double pop() {
        try {
            if(!stack.empty()) {
                double val = stack.back();
                stack.pop_back();
                return val;
            }
            else {
                throw 5; //Stack underflow error
            }
        }
        catch (int e) {
            errorCode = e;
            return false;
        }
        
    }

    void printUsage() {
        for (int i = 0; i < sizeof(useText) / sizeof(*useText); i++) { //determine length of array by comparing to size of string.
            if(i == 1 || i == 2 || i == 5 || i == 49) { //Print program name at specified lines.
                cout << programName;
            }
            cout << useText[i];
        }
        return;
    }
};

#endif //CALC_H