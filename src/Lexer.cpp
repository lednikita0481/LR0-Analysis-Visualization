#include "../include/Lexer.hpp"


BinOpType GetBinOpTypeFromText(const char* yyline){
    switch (*yyline) {
        case '(':
            return BinOpType::OPEN_BRACK;
        case ')':
            return BinOpType::CLOSE_BRACK;
        case '*':
            return BinOpType::MUL;
        case '+':
            return BinOpType::ADD;
        case '-':
            return BinOpType::SUB;
        case '/':
            return BinOpType::DIV;
        default:
            std::cout << "No such binary type:" << *yyline << std::endl;
            return BinOpType::UNKNOWN;
    }
}

static const char* enum_type_str(TokenType type){
    #define CASE_STR(type) \
        case(type):          \
        return #type

    switch (type) {
        CASE_STR(TokenType::NUMBER);
        CASE_STR(TokenType::VAR);
        CASE_STR(TokenType::BIN_OP);
        default:
            return "unknown";
    };

    #undef CASE_STR
}

void Lexer::Tokenise(std::vector<std::unique_ptr<BaseToken>>& Tokens){
    TokenType type = TokenType::BIN_OP;
    int offset = 0;
    int yylineno_prev = yylineno;

    while (type != TokenType::END) {
        type = static_cast<TokenType>(yylex());
        std::cout << enum_type_str(type) << " " << *YYText() << std::endl;

        if (yylineno_prev != yylineno){
            offset = 0;
            yylineno_prev = yylineno;
        }

        switch (type) {
            case TokenType::BIN_OP:
                //Tokens.push_back(BinOpToken(GetBinOpTypeFromText(Lexer_->YYText())));
                Tokens.push_back(std::unique_ptr<BaseToken>(new BinOpToken(GetBinOpTypeFromText(YYText()), lineno(), offset)));
                break;
            case TokenType::NUMBER:
                //Tokens.push_back(NumberToken(std::stoi(Lexer_->YYText())));
                Tokens.push_back(std::unique_ptr<BaseToken>(new NumberToken(std::stoi(YYText()), lineno(), offset)));
                break;
            case TokenType::VAR:
                //Tokens.push_back(VarToken(*Lexer_->YYText()));
                Tokens.push_back(std::unique_ptr<VarToken>(new VarToken(*YYText(), lineno(), offset)));
                break;
            case TokenType::ERROR:
                std::cout << "No such syntaxis: " << YYText() << "in line " << lineno() << std::endl;
                break;
            case TokenType::END:
                Tokens.push_back(std::unique_ptr<BaseToken>(new BaseToken(END, lineno(), offset)));
                break;
            default:
                std::cout << "Reached unreachable" << std::endl;
        }

        offset += yyleng;
    }
}

void Lexer::TokenSeqPrint(std::vector<std::unique_ptr<BaseToken>>& Tokens){
  for (int i = 0; i < Tokens.size(); i++){
    std::cout << "Token " << i << ", type: " << enum_type_str(Tokens[i]->GetType());

    if (Tokens[i]->GetType() == TokenType::NUMBER) std::cout << ", int: " << static_cast<NumberToken*>(Tokens[i].get())->GetNumber();
    if (Tokens[i]->GetType() == TokenType::VAR) std::cout << ", var: " << static_cast<VarToken*>(Tokens[i].get())->GetName();
    if (Tokens[i]->GetType() == TokenType::BIN_OP) std::cout << ", op: " << (int)static_cast<BinOpToken*>(Tokens[i].get())->GetBinOpType();

    std::cout << std::endl;
  }
}