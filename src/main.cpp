#include "lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>

static std::string read_file(const char *filename) {
  std::ifstream ifs(filename);
  std::ostringstream buffer;
  buffer << ifs.rdbuf();
  return buffer.str();
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: lokust <file>\n";
  }

  for (int i = 1; i < argc; i++) {
    Lexer lexer;
    std::string file = read_file(argv[i]);
    std::vector<Token> tokes = lexer.lex_file(file.c_str());
  }
}
