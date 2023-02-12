#ifndef _PARSER_HPP
#define _PARSER_HPP
#include <algorithm>
#include <iostream>
#include <vector>

enum class TokenType : unsigned char {  
    number,
    string,
    unknown,
    error
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

void parser_newline ();
void parser_new_cell ();
void parser_new_token (const std::string &token, const TokenType type);

#endif
