#include "lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(std::istream& in) : in(in), curChar(0) {
    nextChar();
}

void Lexer::nextChar() {
    curChar = in.get();
}

Token Lexer::nextToken() {
    // Skip spaces and tabs
    while (curChar == ' ' || curChar == '\t' || curChar == '\r') {
        nextChar();
    }
    if (curChar == '\n') {
        nextChar();
        return {TOK_NEWLINE, "\\n"};
    }
    if (in.eof()) {
        return {TOK_EOF, ""};
    }
    if (std::isalpha(curChar)) {
        std::string id;
        while (std::isalpha(curChar)) {
            id.push_back(curChar);
            nextChar();
        }
        if (id == "print") return {TOK_PRINT, id};
        return {TOK_ID, id};
    }
    if (std::isdigit(curChar)) {
        std::string num;
        while (std::isdigit(curChar)) {
            num.push_back(curChar);
            nextChar();
        }
        return {TOK_NUMBER, num};
    }
    // Single-character tokens
    switch (curChar) {
        case '=': nextChar(); return {TOK_ASSIGN, "="};
        case '+': nextChar(); return {TOK_PLUS, "+"};
        case '-': nextChar(); return {TOK_MINUS, "-"};
        case '*': nextChar(); return {TOK_MUL, "*"};
        case '/': nextChar(); return {TOK_DIV, "/"};
        case '(': nextChar(); return {TOK_LPAREN, "("};
        case ')': nextChar(); return {TOK_RPAREN, ")"};
        default:
            std::cerr << "Unknown character: " << curChar << std::endl;
            nextChar();
            return nextToken();
    }
}
