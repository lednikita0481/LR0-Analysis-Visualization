#include "../include/Node.hpp"


BaseToken::BaseToken(TokenType type, int cur_line, int cur_pos) : type_(type), cur_line_(cur_line), cur_pos_(cur_pos) {};
BaseToken::~BaseToken() {};
TokenType BaseToken::GetType() {return type_;};
int BaseToken::GetCurLine() {return cur_line_;}
int BaseToken::GetCurPos() {return cur_pos_;}

NumberToken::NumberToken(int number, int cur_line, int cur_pos) : BaseToken(TokenType::NUMBER, cur_line, cur_pos), number_(number) {};
NumberToken::~NumberToken() {};
int NumberToken::GetNumber() {return number_;}

BinOpToken::BinOpToken(BinOpType bin_op_type, int cur_line, int cur_pos) : BaseToken(TokenType::BIN_OP, cur_line, cur_pos), bin_op_(bin_op_type) {};
BinOpToken::~BinOpToken() {};
BinOpType BinOpToken::GetBinOpType() {return bin_op_;}

VarToken::VarToken(char name, int cur_line, int cur_pos) : BaseToken(TokenType::VAR, cur_line, cur_pos), name_(name) {std::cout << "name is " << name_ << std::endl;};
VarToken::~VarToken() {};
char VarToken::GetName() {return name_;}