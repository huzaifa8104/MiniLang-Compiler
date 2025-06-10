// main.cpp

#include "parser.h"
#include <fstream>
#include <iostream>
#include <set>
#include <filesystem>   // C++17 filesystem

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: compiler <inputfile>\n";
        return 1;
    }

    // Convert the user-supplied path into an absolute path
    fs::path inputPath = fs::absolute(argv[1]);
    if (!fs::exists(inputPath)) {
        std::cerr << "Input file not found: " << inputPath << "\n";
        return 1;
    }

    // Open the input file
    std::ifstream infile(inputPath);
    if (!infile) {
        std::cerr << "Failed to open input file: " << inputPath << "\n";
        return 1;
    }

    // Parse the source into AST statements
    Parser parser(infile);

    // Collect variable names for data section
    std::set<std::string> vars;
    for (auto& stmt : parser.statements) {
        if (auto as = dynamic_cast<AssignStmt*>(stmt.get())) {
            vars.insert(as->name);
        }
    }

    // Determine output directory and assembly file path
    fs::path outDir  = inputPath.parent_path();      // e.g. /…/mycompiler/test
    fs::path asmFile = outDir / "out.s";             // …/mycompiler/test/out.s

    // Write generated assembly to out.s
    std::ofstream out(asmFile);
    if (!out) {
        std::cerr << "Failed to create assembly file: " << asmFile << "\n";
        return 1;
    }

    out << ".globl main\n";
    out << "main:\n";
    out << "    push %rbp\n";
    out << "    mov %rsp, %rbp\n";

    for (auto& stmt : parser.statements) {
        stmt->codegen(out);
    }

    out << "    pop %rbp\n";
    out << "    xor %rax, %rax\n";
    out << "    ret\n\n";

    out << ".data\n";
    for (auto& var : vars) {
        out << var << ": .quad 0\n";
    }
    out << "fmt: .string \"%d\\n\"\n";

    out.close();

    std::cout << "Wrote assembly to: " << asmFile << "\n";
    return 0;
}
