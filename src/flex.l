%option c++
%option nounput
%option noinput
%option noyywrap
%option yylineno

%{
  using std::cout;
  using std::endl;

  #include "../include/Node.hpp"
%}

%%
[ \t\n\v]+                  {}
"("                         {return TokenType::BIN_OP;}
")"                         {return TokenType::BIN_OP;}
"*"                         {return TokenType::BIN_OP;}
"/"                         {return TokenType::BIN_OP;}
"+"                         {return TokenType::BIN_OP;}
"-"                         {return TokenType::BIN_OP;}

[0-9]+                      {return TokenType::NUMBER;}
[xyz]                       {return TokenType::VAR;}



.                           {return TokenType::ERROR;}

%%