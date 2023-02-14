#define __a_ascii_pos 97
#define __0_ascii_pos 48
#include "parser.hpp"

namespace Table {
    std::vector<unsigned> marks { 0 };
    std::vector<Cell> cells;

    bool savesConstant (const TokenType type) {
        return type == TokenType::number || type == TokenType::string || type == TokenType::unknown || type == TokenType::error;
    }

    void doThis (Cell* cell) {
        switch ( cell->type ) {
            case TokenType::equ : { BasicMath::start(cell); break; }
            case TokenType::copies : { Copies::start(cell); break; }
        }
    }

    void setError (Cell* cell, const std::string &err) {
        cell->data = err;
        cell->type = TokenType::error;
    }
}

void parser_newline () {
    if ( Table::cells.empty() ) { return; }
    Table::marks.push_back(Table::cells.size());
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

void parser_run () {
    unsigned midx = 0;
    for (unsigned cidx = 0; cidx < Table::cells.size(); cidx++) {
        if ( cidx == Table::marks[midx] ) {
            std::cout << '\n';
            midx++;
        }
        Table::doThis(&Table::cells.at(cidx));
        std::cout << Table::cells.at(cidx).data << " | ";
    }
}

void Copies::start (Cell* cell) {
    static Cell* beginsin = nullptr;
    if ( !beginsin ) { beginsin = cell; }

    Cell* copy = Copies::get_cell(cell->data);
    if ( copy == beginsin && copy->type == TokenType::copies ) {
        Table::setError(copy, "loop_copying!");
    }

    Table::doThis(copy);
    cell->data = copy->data;
    cell->type = copy->type;
    beginsin = nullptr;
}

Cell* Copies::get_cell (const std::string &pos) {    
    unsigned row = 0, col = 0;
    for (char c : pos) {
        isdigit(c) ? row += (c - __0_ascii_pos) : col += (c - __a_ascii_pos);
    }

    /* The parser saves the cells in a single vector, that means ain't rows, just "columns".
     * However when a new row is readed by the lexer the parser will do a mark to the
     * number of cell which starts that new row (parser_newline function).
     *
     * So if there's a table that looks like this:
     *      a0 b1 c2
     *      d3 e4 f5 g6
     *      h7
     * The marks will be: <0, 3, 7>
     * And to get one cell the position must be given as @<ROW (NUMBERS)><COL (LOWER_LETTERS)>, no matter the order.
     * So for example:
     *      Get(e4) = @1b = marks[1] + 1 = 4 :: cells[4] = e4.
     *      Get(a0) = @0a = marks[0] + 0 = 0 :: cells[0] = a0.
     *      Get(h7) = @2a = marks[2] + 0 = 7 :: cells[7] = h7.
     * */
    unsigned pointingto = Table::marks[row] + col;
    if ( pointingto >= Table::cells.size() ) {
        std::cout << "E: Trying to get '@" << pos << "' but it's out of range :c.\n";
        exit(1);
    }
    return &Table::cells.at(pointingto);
}

void BasicMath::start (Cell* cell) {
    if ( cell->info.size() == 1 ) {
        Table::setError(cell, "empty_expression!");
        return;
    }
    
    std::vector<double> numbers;
    std::vector<TokenType> operators;
    bool isnumber = true;
    TokenType lastop = TokenType::unknown;

    for (Token const &token : cell->info) {
        if ( token.type == TokenType::equ ) { continue; }
        if ( isnumber ) {
            double newnum = BasicMath::number(cell, token);
            if ( lastop ==  TokenType::div || lastop == TokenType::mul ) {
                newnum = BasicMath::do_higher(cell, numbers.back(), newnum, lastop);
                numbers.pop_back();
                operators.pop_back();
            }
            numbers.push_back(newnum);
            lastop = TokenType::unknown;
        }
        else {
            BasicMath::moperator(cell, token.type);
            operators.push_back(token.type);
            lastop = token.type;
        }

        if ( cell->type == TokenType::error ) { return; }
        isnumber = !isnumber;
    }

    for (unsigned i = 1; i < numbers.size(); i++) {
        if ( operators.at(i - 1) == TokenType::add ) { numbers.front() += numbers.at(i); }
        else { numbers.front() -= numbers.at(i); }
    }

    cell->type = TokenType::number;
    cell->data = std::to_string(numbers.front());
}

double BasicMath::number (Cell* cell, const Token token) {   
    if ( token.type == TokenType::number ) { return stod(token.data); }
    if ( token.type == TokenType::copies ) {
        Cell* thatcell = Copies::get_cell(token.data);
        if ( thatcell->type == TokenType::number ) {
            return stod(thatcell->data);
        }
    }
    
    Table::setError(cell, "number_ref!");
    return 0;
}

void BasicMath::moperator (Cell* cell, const TokenType type) {
    switch ( type ) {
        case TokenType::sub : case TokenType::div :
        case TokenType::mul : case TokenType::add : { return; }
    }
    Table::setError(cell, "operator_ref!");
}

double BasicMath::do_higher (Cell* cell, double p1, double p2, TokenType type) {
    if ( type == TokenType::mul ) { return p1 * p2; }
    if ( !p2 ) { Table::setError(cell, "divby_zero!"); }
    return p1 / p2;
}
