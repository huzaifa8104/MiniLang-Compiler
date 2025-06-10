#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>

// Forward declaration of AST classes
class Expr;
class Stmt;
using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;

// Base class for expressions
class Expr {
public:
    virtual ~Expr() = default;
    virtual void codegen(std::ostream& out) = 0;
};

// Numeric literal
class NumberExpr : public Expr {
public:
    int value;
    NumberExpr(int value);
    void codegen(std::ostream& out) override;
};

// Variable reference
class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(const std::string &name);
    void codegen(std::ostream& out) override;
};

// Binary expression (e.g., +, -, *, /)
class BinaryExpr : public Expr {
public:
    char op;
    ExprPtr lhs, rhs;
    BinaryExpr(char op, ExprPtr lhs, ExprPtr rhs);
    void codegen(std::ostream& out) override;
};

// Base class for statements
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void codegen(std::ostream& out) = 0;
};

// Assignment statement (id = expr)
class AssignStmt : public Stmt {
public:
    std::string name;
    ExprPtr expr;
    AssignStmt(const std::string &name, ExprPtr expr);
    void codegen(std::ostream& out) override;
};

// Print statement (print expr)
class PrintStmt : public Stmt {
public:
    ExprPtr expr;
    PrintStmt(ExprPtr expr);
    void codegen(std::ostream& out) override;
};

// Parser class: builds AST from tokens
class Parser {
    Lexer lexer;
    Token tok;
    void nextToken();
public:
    std::vector<StmtPtr> statements;
    Parser(std::istream& in);
private:
    void parseStatements();
    StmtPtr parseStmt();
    ExprPtr parseExpr();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
};

#endif // PARSER_H
