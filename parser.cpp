#include "parser.h"
#include <iostream>
#include <cctype>

// Expr implementations

NumberExpr::NumberExpr(int value) : value(value) {}

void NumberExpr::codegen(std::ostream& out) {
    out << "    movq $" << value << ", %rax\n";
}

VarExpr::VarExpr(const std::string &name) : name(name) {}

void VarExpr::codegen(std::ostream& out) {
    out << "    movq " << name << "(%rip), %rax\n";
}

BinaryExpr::BinaryExpr(char op, ExprPtr lhs, ExprPtr rhs)
    : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

void BinaryExpr::codegen(std::ostream& out) {
    lhs->codegen(out);
    out << "    push %rax\n";
    rhs->codegen(out);
    out << "    movq %rax, %rbx\n";
    out << "    pop %rax\n";
    switch (op) {
        case '+': out << "    addq %rbx, %rax\n"; break;
        case '-': out << "    subq %rbx, %rax\n"; break;
        case '*': out << "    imulq %rbx, %rax\n"; break;
        case '/':
            out << "    cqto\n";
            out << "    idivq %rbx\n";
            break;
        default: break;
    }
}

// Stmt implementations

AssignStmt::AssignStmt(const std::string &name, ExprPtr expr)
    : name(name), expr(std::move(expr)) {}

void AssignStmt::codegen(std::ostream& out) {
    expr->codegen(out);
    out << "    movq %rax, " << name << "(%rip)\n";
}

PrintStmt::PrintStmt(ExprPtr expr) : expr(std::move(expr)) {}

void PrintStmt::codegen(std::ostream& out) {
    expr->codegen(out);
    out << "    movq %rax, %rsi\n";
    out << "    leaq fmt(%rip), %rdi\n";
    out << "    xor %rax, %rax\n";
    out << "    call printf\n";
}

// Parser implementation

Parser::Parser(std::istream& in) : lexer(in) {
    nextToken();
    parseStatements();
}

void Parser::nextToken() {
    tok = lexer.nextToken();
}

void Parser::parseStatements() {
    while (tok.type != TOK_EOF) {
        if (tok.type == TOK_NEWLINE) {
            nextToken();
            continue;
        }
        auto stmt = parseStmt();
        statements.push_back(std::move(stmt));
    }
}

StmtPtr Parser::parseStmt() {
    if (tok.type == TOK_ID) {
        std::string id = tok.text;
        nextToken();
        if (tok.type == TOK_ASSIGN) {
            nextToken();
            ExprPtr expr = parseExpr();
            return std::make_unique<AssignStmt>(id, std::move(expr));
        }
    } else if (tok.type == TOK_PRINT) {
        nextToken();
        ExprPtr expr = parseExpr();
        return std::make_unique<PrintStmt>(std::move(expr));
    }
    std::cerr << "Unknown statement start: " << tok.text << std::endl;
    nextToken();
    return nullptr;
}

ExprPtr Parser::parseExpr() {
    ExprPtr lhs = parseTerm();
    while (tok.type == TOK_PLUS || tok.type == TOK_MINUS) {
        char op = tok.text[0];
        nextToken();
        ExprPtr rhs = parseTerm();
        lhs = std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
    }
    return lhs;
}

ExprPtr Parser::parseTerm() {
    ExprPtr lhs = parseFactor();
    while (tok.type == TOK_MUL || tok.type == TOK_DIV) {
        char op = tok.text[0];
        nextToken();
        ExprPtr rhs = parseFactor();
        lhs = std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
    }
    return lhs;
}

ExprPtr Parser::parseFactor() {
    if (tok.type == TOK_NUMBER) {
        int val = std::stoi(tok.text);
        nextToken();
        return std::make_unique<NumberExpr>(val);
    } else if (tok.type == TOK_ID) {
        std::string id = tok.text;
        nextToken();
        return std::make_unique<VarExpr>(id);
    } else if (tok.type == TOK_LPAREN) {
        nextToken();
        ExprPtr expr = parseExpr();
        if (tok.type == TOK_RPAREN) nextToken();
        return expr;
    }
    std::cerr << "Unexpected token in expression: " << tok.text << std::endl;
    nextToken();
    return nullptr;
}
