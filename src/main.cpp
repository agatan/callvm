#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <iostream>
#include <fstream>
#include <iterator>

#include "callvm/lib.h"
#include "callvm/codegen.h"

void parse_and_print_from_file(std::string const&);
void parse_and_print_from_stdin();

int main(int argc, char const* argv[]) {
    if (argc > 1) {
        parse_and_print_from_file(argv[1]);
    }
    return 0;
}

void parse_and_print_from_file(std::string const& name) {
    std::ifstream in(name, std::ios_base::in);
    if (!in) {
        std::cerr << "Error: Couldn't open file: " << name << std::endl;
        return;
    }

    std::string storage;
    in.unsetf(std::ios::skipws);
    std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(),
              std::back_inserter(storage));

    std::cout << "parsed: " << callvm::parse_and_stringize(storage) << std::endl;

    callvm::codegen::llvm_codegenerator gen("toplevel", llvm::getGlobalContext());
    callvm::parse_and_codegen(storage, gen);
    auto mod = gen.get_module();
    std::cout << mod->getName().str() << std::endl;
    if (!llvm::verifyModule(*mod)) {
        std::cerr << "something goes wrong" << std::endl;
        return;
    }
    mod->dump();
}
