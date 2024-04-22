#include "Node.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <memory>
#include <string>
#include <vector>
#include <fstream>

template <GrammarType G>
class Frontend{
    Lexer* Lexer_;
    Parser<G>* Parser_;
    std::vector<std::unique_ptr<BaseToken>> Tokens;
    std::ifstream input_;

    public:
        Frontend(const char* file_name): Lexer_{new Lexer}, Parser_(new Parser<G>), Tokens() {
            input_.open(file_name);
            if (!input_.is_open()){
                std::cout << "ERROR while oppening file " << file_name << std::endl;
                std::terminate();
            }

            Lexer_->switch_streams(input_, std::cout);
        };

        ~Frontend(){
            delete Lexer_;
            if (input_.is_open()) input_.close();
        };

        void Tokenise(){
            Lexer_->Tokenise(Tokens);
        }

        void TokenSeqPrint(){
            Lexer_->TokenSeqPrint(Tokens);
        }

        void Parse(){
            Parser_->Parse(Tokens);
        }

        void DumpParseHistory(std::string file_name = ""){
            Parser_->DumpHistory(Tokens, file_name);
        }

        void DumpParseTree(std::string file_name = ""){
            Parser_->DumpTree(file_name);
        }

        Frontend& operator=(const Frontend&) = delete;
};