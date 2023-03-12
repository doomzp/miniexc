#include "lexer.hpp"

void usage () {
    puts("U: Pass the table file as argument.");
    exit(0);
}

int main (int argc, char** argv) {
    std::ifstream file (argv[1]);
    if ( !file.good() ) { usage(); }

    lexer_read(&file);
    return 0;
}
