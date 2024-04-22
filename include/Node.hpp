#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif


enum TokenType{
    END,
    NUMBER,
    VAR,
    BIN_OP,
    ERROR
};

enum class BinOpType{
    ADD,
    SUB,
    MUL,
    DIV,
    OPEN_BRACK,
    CLOSE_BRACK,
    UNKNOWN
};

class BaseToken{
    TokenType type_;
    int cur_line_;
    int cur_pos_;

    public:
        BaseToken(TokenType type, int cur_line, int cur_pos);
        //BaseToken(const BaseToken&) = delete;
        ~BaseToken();
        TokenType GetType();
        int GetCurLine();
        int GetCurPos();

        //BaseToken& operator=(const BaseToken&) = delete;
};

class NumberToken final: public BaseToken{
    int number_;

    public:
        NumberToken(int number, int cur_line, int cur_pos);
        ~NumberToken();
        int GetNumber();
};

class BinOpToken final: public BaseToken{
    BinOpType bin_op_;

    public:
        BinOpToken(BinOpType bin_op_type, int cur_line, int cur_pos);
        ~BinOpToken();
        BinOpType GetBinOpType();
};

class VarToken final : public BaseToken{
    char name_;

    public:
        VarToken(char name, int cur_line, int cur_pos);
        ~VarToken();
        char GetName();
};



#endif