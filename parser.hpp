#ifndef _PARSER_HPP
#define _PARSER_HPP
#include <algorithm>
#include <iostream>
#include <vector>

typedef signed char sigchr;
enum class TokenType : sigchr {
    number,
    string,
    unknown,
    error,
    copies,
    add,
    sub,
    div,
    mul,
    equ,
};

typedef struct Token {
    const std::string data;
    const TokenType type;  
} Token;

typedef struct Cell {
    std::vector<Token> info;
    std::string data;
    TokenType type;
} Cell;

class Copies {  
    public:
    static void start (Cell* cell);
    static Cell* get_cell (const std::string &pos);
};

class BasicMath {
    public:
    static void start (Cell* cell);
    static double number (Cell* cell, const Token token);
    static void moperator (Cell* cell, const TokenType type);
    static double do_higher (Cell* cell, double p1, double p2, TokenType type);
};

void parser_newline ();
void parser_new_cell ();
void parser_new_token (const std::string &token, const TokenType type);
void parser_run ();

#endif
