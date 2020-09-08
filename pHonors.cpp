/*
pHonors.cpp

Course:                 CS 2433, C/C++ Programming
Semester:               Summer 2020
Assignment:             Program 8 with honors contract (infix calculator)
Instructor:             Richard Churchill
Due Date/Time:          30 July 2020, 11:59 PM
Submitted:          	
Written by:             Max DeSantis

Build Instructions:
g++ -o pHonors pHonors.cpp

Compiled and working with MinGW 6.3.0 (on Windows) and GCC 9.1.1 (on csx3)

Usage:
Use <program_name> --help to display usage information.
Use <program_name> -rpn to begin a Reverse Polish Notation calculator.
Operators +, -, *, /, **, sin, cos, asin, acos, swap, copy, ~ (negation),
' (inversion), peek, dump, and = are supported.
Define constants with ::<letter><letters and numbers>
Define variables with :$<letter><letters and numbers>

Example input: 1.0 2.0 + =
Example output: 3.0000000
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "honorsCalc.h"

using namespace std;

int main(int argc, char* argv[]) {

    /* Initializatiion */
    calc calculator(argv[0]);
    bool continuing = true;

    /* Track what operation the user input on command line*/
    enum Operation {none, rpn, infix};
    Operation specifiedOp = none;
    string keyWords[4] = {"-rpn", "-infix", "-i", "-o"};

    /* Modular I/O streams. Cin and Cout by default. */
    istream* inStream = &cin;
    ostream* outStream = &cout;

    /* Ensure only one file of each type is opened*/
    bool inputDefined = false;
    bool outputDefined = false;

    try {

        /* Check for sufficient number of arguments */
        if(argc < 2) {
            throw 1;
        }
        else {
            /* Parse through command-line arguments looking for inputs/outputs/operation type */
            for(int i = 1; i < argc; ++i) {

                /* Check for which main operation should be performed. */
                if(strcmp(argv[i], "--help") == 0) {
                    throw 3;
                }
                else if(strcmp(argv[i], "-rpn") == 0) {
                    if(specifiedOp == none) {
                        specifiedOp = rpn;
                    }
                    else {
                        throw 10;
                    }
                        
                }
                else if(strcmp(argv[i], "-infix") == 0) {
                    if(specifiedOp == none) {
                        specifiedOp = infix;
                    }
                    else {
                        throw 10;
                    }
                }

                /* Check for a user-specified input file. */
                else if(strcmp(argv[i], "-i") == 0) {
                    if(argc > i && !inputDefined) {
                        
                        /* Check for reserved keywords following input specifier */
                        for(string temp : keyWords) {
                            if(strcmp(argv[i + 1], temp.c_str()) == 0) {
                                throw 5;
                            }
                        }
                        
                        /* Will only execute if allowable name is present */
                        ifstream* newInStream = new ifstream(argv[i + 1]);
                        if(newInStream->is_open()) {
                            inStream = newInStream;
                        }
                        else {
                            throw 8; //Cannot find input file.
                        }
                        ++i; //Skip past he file name in arguments list.
                        
                    }
                    else {
                        throw 1; //Not enough arguments
                    }
                }

                /* Check for a user-specified output file. */
                else if(strcmp(argv[i], "-o") == 0) {
                    if(argc > i) {
                        /* Check for reserved keywords following input specifier */
                        for(string temp : keyWords) {
                            if(strcmp(argv[i + 1], temp.c_str()) == 0) {
                                throw 6;
                            }
                        }
                        
                        /* Will only execute if allowable name is present */
                        ofstream* newOutStream = new ofstream(argv[i + 1]);
                        if(newOutStream->is_open()) {
                            outStream = newOutStream;
                        }
                        else {
                            throw 9; //Cannot open output file.
                        }
                        ++i; //Skip past the file name in arguments list.
                    }
                    else {
                        throw 1; //Not enough arguments
                    }
                }
                else {
                    throw 2;
                }
            }

            calculator.setup(inStream, outStream);

            if(specifiedOp == rpn) {
                while(continuing) {
                    if(calculator.rpn()) { //Calculator reached execution end.

                        /* Gather and print execution results */
                        double result = calculator.expResultRpn();
                        *outStream << "Result = " << fixed << setprecision(5) << result << "\n";

                        /* Gather and print stack outcome*/
                        int size = calculator.stackSize();
                        *outStream << size << " values remain on the stack.\n";

                        continuing = false;
                    }
                    else { //Calculator error
                        char input;
                        string temp;

                        /* Gather and print error */
                        int errorCode = calculator.evalError();
                        cout << calculator.errorTextRpn(errorCode) << "\n";

                        /* "flush" input buffer if cin is being used as input */
                        if(inStream = &cin) {
                            cin.clear();
                            getline(cin, temp);
                        }
                        
                        
                        cout << "Calculation failed. Try again? [Y / N]: ";

                        /* Gather user response and act accordingly */
                        getline(cin, temp);

                        /* Check for valid input */
                        if(temp.size() > 0) {
                            input = tolower(temp.at(0));

                            if(input == 'y') {
                                calculator.resetCalc();
                            }
                            else if(input == 'n') {
                                continuing = false;
                            }
                            else {
                                throw 4;
                            }
                        }
                        else {
                            throw 4;
                        }
                        
                        
                    }
                }
            }
            else if(specifiedOp == infix) {
                while(continuing) {
                    if(calculator.infixInit()) {

                        /* Gather and print execution results */
                        double result = calculator.expResultInfix();
                        *outStream << "Result = " << fixed << setprecision(5) << result << endl;

                        continuing = false;
                    }
                    else {
                        char input;
                        string temp;

                        /* Gather and print error */
                        int errorCode = calculator.evalError();
                        cerr << calculator.errorTextInfix(errorCode) << "\n";

                        /* "flush" input buffer if cin is being used as input */
                        if(inStream = &cin) {
                            cin.clear();
                            getline(cin, temp);
                        }
                        cerr << "Calculation failed. Try again? [Y / N]: ";

                        /* Gather user response and act accordingly */
                        getline(cin, temp);

                        /* Check for valid input */
                        if(temp.size() > 0) {
                            input = tolower(temp.at(0));

                            if(input == 'y') {
                                calculator.resetCalc();
                            }
                            else if(input == 'n') {
                                continuing = false;
                            }
                            else {
                                throw 4;
                            }
                        }
                        else {
                            throw 4;
                        }
                    }
                }
            }
            else { //No operation was specified.
                throw 7;
            }
        }

        /* Calculator exited cleanly.
        Cleanup streams and terminate. */
        return 0;
    }
    catch (int e) {
        string returnText = "***ERROR: ";
        switch (e) {
            case 1:
                returnText += "Insufficient number of command-line arguments.";
                break;
            case 2:
                returnText += "Unrecognized command-line arguments.";
                break;
            case 3:
                returnText = ""; //User input --help
                break;
            case 4:
                returnText += "Unrecognized response - terminating.";
                break;
            case 5:
                returnText += "Invalid input file name.";
                break;
            case 6:
                returnText += "Invalid output file name.";
                break;
            case 7:
                returnText += "Invalid operation specified.";
                break;
            case 8:
                returnText += "Unable to locate specified input file.";
                break;
            case 9:
                returnText += "Unable to open specified output file.";
                break;
            case 10:
                returnText += "Too many operations described.";
                break;
            default:
                returnText += "Unrecognized error code.";
                break;
        }

        calculator.printUsage();
        cerr << "\n" << returnText << endl;
        
        /* Calculator exiting.
        Cleanup streams and terminate. */
        exit(-1);
    }
}