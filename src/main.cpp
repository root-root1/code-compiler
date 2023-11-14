#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

#include "./generation.hpp"
#include "./parser.hpp"
#include "./tokenization.hpp"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "File not passed" << std::endl;
    return EXIT_FAILURE;
  }

  std::fstream input(argv[1], std::ios::in);
  std::stringstream content_stream;
  content_stream << input.rdbuf();
  input.close();

  std::string contents = content_stream.str();

  Tokenizer tokenizer(std::move(contents));
  std::vector<Token> tokens = tokenizer.tokenize();

  Parser parse(tokens);
  std::optional<NodeExit> tree = parse.parse();

  if (!tree.has_value()) {
    std::cerr << "NO Exit Found" << std::endl;
    exit(EXIT_FAILURE);
  }

  Generator generator(tree.value());
  std::string str = generator.generate();

  std::fstream file(argv[2], std::ios::out);
  file << str;
  file.close();

  system("nasm -felf64 ./demo/test.asm");
  system("ld -o ./demo/out ./demo/test.o");

  return EXIT_SUCCESS;
}
