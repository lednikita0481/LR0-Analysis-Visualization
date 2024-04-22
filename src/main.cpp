#include "../include/FrontendCommon.hpp"

int main(const int argc, const char* argv[]){
    if (argc != 2){
        std::cout << "ERROR: Write input file as an argument" << std::endl;
        return 1;
    }

    Frontend<GrammarType::LR0> frontend(argv[1]);
    frontend.Tokenise();
    frontend.Parse();
    frontend.DumpParseHistory();
    frontend.DumpParseTree();
}