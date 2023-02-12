#include "parser.hpp"

namespace Table {
    std::vector<unsigned> marks { 0 };
    std::vector<Cell> cells;
}

void parser_newline () {
    if ( Table::cells.empty() ) { return; }
    Table::marks.push_back(Table::cells.size() - 1);
}

void parser_new_cell () {
    Table::cells.push_back(Cell {
        .data = "unknown!",
        .type = TokenType::unknown
    });
}

void parser_new_token (const std::string &token, const TokenType type) {
    Cell* back = &Table::cells.back();
    /* The first token of every cell defines what that cell will do. */
    if ( back->info.empty() ) {
        back->data = token;
        back->type = type;
    }

    back->info.push_back(Token {
        .data = token,
        .type = type 
    });
}
