#include "file_handler.hpp"
#include <iostream>


int main() {
    using json = nlohmann::json;
    // get_json from user
    std::string input{
        std::istreambuf_iterator<char>{std::cin}, // Beginning of stdin
        std::istreambuf_iterator<char>{} // default constructed stands for EOF
    };
    json original = json::parse(input);
    Persistence::FileHandler::save(original);

    json loaded = Persistence::FileHandler::load();

    if (original != loaded) {
        std::cerr << "original differs from loaded\n";
        std::cout << "Original:\n"
          << original.dump(4)
          << "\nLoaded:\n"
          << loaded.dump(4)
          << '\n';
        return 1;

    }

    std::cout << "FileHandler round-trip passed\n";
    return 0;
}