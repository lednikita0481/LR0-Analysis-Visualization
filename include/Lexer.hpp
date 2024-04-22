#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "Node.hpp"

#include <vector>
#include <memory>

class Lexer : public yyFlexLexer{
  public:
    Lexer() = default;
    ~Lexer() = default;

    void Tokenise(std::vector<std::unique_ptr<BaseToken>>& Tokens);
    void TokenSeqPrint(std::vector<std::unique_ptr<BaseToken>>& Tokens);
};