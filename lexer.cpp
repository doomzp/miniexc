#include "lexer.hpp"

namespace G {
    std::string row;
}

namespace Get {
    typedef bool getif (char);

    bool strings (char c) { return c != '"'; }
    bool numbers (char c) { return isdigit(c) || c == '.'; }
    bool copies (char c ) { return isdigit(c) || islower(c); }

    void checkNumber (std::string* num) {
        unsigned npoints = (unsigned) std::count(num->begin(), num->end(), '.');
        if ( !npoints || npoints == 1 ) { return; }
        *num = "number_def!";
    }

    void checkCopies (std::string *copy) {
        *copy = copy->substr(1);    
        bool digitpart = std::any_of(copy->begin(), copy->end(), isdigit);
        bool lowerpart = std::any_of(copy->begin(), copy->end(), islower);

        if ( digitpart && lowerpart ) { return; }
        *copy = "copies_def!";
    }

    void get (std::string* data, std::size_t* pos, Get::getif condition) {
        data->clear();
        do {
            data->push_back(G::row[*pos]);
            *pos += 1;
        } while ( condition(G::row[*pos]) );

        if ( condition == Get::strings ) { *data = data->substr(1); }
        if ( condition == Get::numbers ) { Get::checkNumber(data); }
        if ( condition == Get::copies ) { Get::checkCopies(data); }
    }
}

void lexer_read (std::ifstream *table) {
    while ( !table->eof() ) {
        std::getline(*table, G::row);
        bool allspaces = std::all_of(G::row.begin(), G::row.end(), isspace);
        
        if ( !G::row.empty() && !allspaces ) {
            parser_newline();
            lexer_seek();
        }
    }

    table->close();
    parser_run();
}

void lexer_seek () {
    std::string token;
    for (std::size_t i = 0; i < G::row.size(); ++i) {
        while ( isspace(G::row[i]) ) { i++; }
        token.push_back(G::row[i]);
        
        if ( token.back() == '|' ) {
            parser_new_cell();
            token.clear();
        }
        
        TokenType thist;
        if ( (thist = lexer_type(&token, &i)) != TokenType::unknown ) {
            parser_new_token(token, thist);
            token.clear();
        }
    }
}

TokenType lexer_type (std::string *data, std::size_t* pos) {
    const char cchar = data->back();
    const char nchar = G::row[*pos + 1];

    switch ( cchar ) {
        case '+' : { return TokenType::add; break; }
        case '~' : { return TokenType::sub; break; }
        case '*' : { return TokenType::mul; break; }
        case '/' : { return TokenType::div; break; }
        case '=' : { return TokenType::equ; break; }
    }

    if ( cchar == '"' ) {
        Get::get(data, pos, Get::strings);
        return TokenType::string;
    }
    if ( isdigit(cchar) || (cchar == '-' && isdigit(nchar)) ) {
        Get::get(data, pos, Get::numbers);
        return TokenType::number;
    }
    if ( cchar == '@' && (islower(nchar) || isdigit(nchar)) ) {
        Get::get(data, pos, Get::copies);
        return TokenType::copies;
    }

    return TokenType::unknown;
}