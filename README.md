# Description

This project was completed as an honors contract for one of my programming courses. The Infix evaluation is implemented with a recursive parser that moves through the inputted expression. It cannot handle the order of operations, so parentheses are required to evaluate expressions as intended.

# Features

## Two Operating Modes
#### 1) Infix Notation
* 5 - 5 = 0
* sin (6.28 - 3.14) = 1

#### 2) Postfix Notation (RPN - Reverse Polish Notation)
* 5 5 * = 25
* 3.14 sin = 1

## Command Line Parsing
#### 1) Using specified input and output files with -i and -o tags.
#### 2) Documentation with --help.
#### 3) Error checking to ensure no illegal arguments are provided.
#### 4) Infix and RPN notation can be selected using proper -infix and -rpn arguments.

## Other

#### 1) Variables and constants can be declared and used later with the :$ and :: operators, respectively.
#### 2) The following trigonometric operations are allowed: sin, cos, asin, acos. Domains are checked to ensure proper operation.
#### 3) RPN allows stack manipulation with dump, peek, negation, inversion, and swap.

## My Notes

Infix calculators are tricky! The RPN calculator was very straightforward to implement. The stack started as a simple array written in C, then moved to a linked-list and is now a C++ vector. The different methods were done to learn about various data structures.

The infix implementation put up a fight. I'd never created a complex recursive function before, so this was quite the daunting introduction. I'm proud to have figured it out (with little help, to boot). In the future I'd like to add in the support for PEMDAS, but who knows if that'll ever happen. Things get busy!
