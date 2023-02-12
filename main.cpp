#include "lexer.hpp"

void usage () {
    puts("Usage: This programs just takes one argument.");
    puts("    * The path to the file which contains the table.");
    exit(0);
}

int main (int argc, char** argv) {
    std::ifstream file (argv[1]);
    if ( !file.good() ) { usage(); }

    lexer_read(&file);
    return 0;
}
