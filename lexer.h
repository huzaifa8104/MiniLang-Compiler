#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <string>

// Token types for the toy language
enum TokenType {
    TOK_EOF, TOK_NEWLINE, TOK_PRINT, TOK_ID, TOK_NUMBER,
    TOK_ASSIGN, TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV,
    TOK_LPAREN, TOK_RPAREN
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
    std::istream& in;
    char curChar;
public:
    Lexer(std::istream& in);
    Token nextToken();
private:
    void nextChar();
};

#endif // LEXER_H
