#include <iostream>
#include <fstream>
#include <iterator>

#include "callvm/lib.h"

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

    std::cout << callvm::parse_and_stringize(storage) << std::endl;
}
