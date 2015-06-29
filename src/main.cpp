#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>
#include <iostream>
#include <fstream>
#include <iterator>

#include "callvm/lib.h"
#include "callvm/ast/ast.hpp"
#include "callvm/codegen.h"

bool loop(callvm::codegen::llvm_codegenerator&, int);

int main(int argc, char const* argv[]) {
    callvm::codegen::llvm_codegenerator gen("toplevel",
                                            llvm::getGlobalContext());
    int cnt = 0;
    while (loop(gen, cnt++));

    std::ofstream out(argv[1], std::ios_base::out);
    llvm::raw_os_ostream o(out);
    gen.get_module()->print(o, nullptr);
    return 0;
}

bool loop(callvm::codegen::llvm_codegenerator& gen, int cnt) {
    std::string line;
    std::cout << "user> ";
    std::flush(std::cout);
    std::getline(std::cin, line);
    if (line == "quit") return false;
    auto ast = callvm::parse(line);
    std::cout << callvm::ast::visitor::stringize(ast) << std::endl;

    if (!gen.generate(ast, "func" + std::to_string(cnt++))) {
        std::cerr << "Error: failed code generation" << std::endl;
        return true;
    }

    gen.get_module()->dump();
    return true;
}
