#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

enum class TokenType {
    _return,
    int_lit,
    semi,
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens {};
    std::string buf;
    for(int i = 0;i < str.length();i++) {
        char c = str.at(i);
        if (std::isalpha(c)){
            buf.push_back(c);
            i++;
            while (std::isalnum(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            if (buf == "return"){
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
                continue;
            }else{
                std::cerr<<"your mesed up"<<std::endl;
                exit(EXIT_FAILURE);
            }
        }else if (std::isdigit(c)) {
            buf.push_back(c);
            i++;
            while (std::isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
        }else if ( c == ';') {
            tokens.push_back({.type = TokenType::semi});
        }else if (std::isspace(c)) {
            continue;
        }else{
            std::cerr<<"your mesed up"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

std::string token_to_asm(const std::vector<Token> &tokens) {
    std::stringstream out;
    out << "global _start\n_start:\n";
    for(int i =0;i<tokens.size();i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::_return){
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    out << "    mov rax, 60\n";
                    out << "    mov rdi, "<< tokens.at(i +1).value.value() << '\n';
                    out << "    syscall";
                }
            }
        }
    }
    return out.str();
}

int main(int argc, char *argv[]){
    if (argc != 3){
        std::cerr << "File not passed" << std::endl;
        return EXIT_FAILURE;
    }

    std::fstream input(argv[1], std::ios::in);
    std::stringstream content_stream;
    content_stream << input.rdbuf();
    input.close();

    std::string contents = content_stream.str();

    std::vector<Token> tokens = tokenize(contents);
    std::string str = token_to_asm(tokens);

    std::fstream file(argv[2], std::ios::out);
    file << str;
    file.close();

    
    system("nasm -felf64 ./demo/test.asm");
    system("ld -o ./demo/out ./demo/test.o");

    return EXIT_SUCCESS;
}
