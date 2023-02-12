#ifndef _LEXER_HPP
#define _LEXER_HPP
#include "parser.hpp"
#include <fstream>

void lexer_read (std::ifstream *table);
void lexer_seek ();
TokenType lexer_type (std::string *data, std::size_t* pos);

#endif
