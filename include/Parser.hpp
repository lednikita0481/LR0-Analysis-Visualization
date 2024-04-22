#ifndef PARSER_HPP
#define PARSER_HPP

#include "Node.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

enum class GrammarType {
    LR0, 
    LR1,
    LALR,
    NONE
};

enum class NonTerminals{
    E, 
    T1, 
    T2, 
    T3,

    S, 

    UNKNOWN,
    TERMINAL,
};

enum class Terminals{
    ADD,
    SUB,
    MUL,
    DIV,
    OPEN_BRACK,
    CLOSE_BRACK,
    NUMBER,
    VAR,
    END,
    UNKNOWN,
    NONTERMINAL,
};

enum class ActionType{
    SHIFT,
    REDUCE,
    ACCEPT,
    ERROR,
};

enum class States{
    I0, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, ERR
};

class Action final {
    public:
    ActionType type_;

    union{
        struct{
            NonTerminals ReduceTo_;
            int operands_;
        } Reduce_;
        States ShiftTo_;
    } what_do_;

    Action() : type_(ActionType::ERROR) {};
    Action(ActionType type) : type_(type) {};
    Action(NonTerminals ReduceTo, int operands) : type_(ActionType::REDUCE) {what_do_.Reduce_.ReduceTo_= ReduceTo; what_do_.Reduce_.operands_ = operands;};
    Action(States ShiftTo) : type_(ActionType::SHIFT) {what_do_.ShiftTo_ = ShiftTo;};

    Action(const Action& other){
        type_ = other.type_;
        what_do_ = other.what_do_;
    }

    Action& operator=(Action&& other){
        std::swap(type_, other.type_);
        std::swap(what_do_, other.what_do_);
        return *this;
    }
};

class AST_Node final {
    AST_Node* parent_;
    AST_Node* left_child_;
    AST_Node* middle_child_;
    AST_Node* right_child_;

    int children_;

    NonTerminals type_;

    std::unique_ptr<BaseToken>* token_;

    public:
        AST_Node(NonTerminals type = NonTerminals::UNKNOWN, std::unique_ptr<BaseToken>* token = nullptr, int children = 0, AST_Node* parent = nullptr, 
                AST_Node* left_child = nullptr, AST_Node* right_child = nullptr, AST_Node* middle_child = nullptr):
                children_(children), type_(type), token_(token),
                parent_(parent), left_child_(left_child), right_child_(right_child), middle_child_(middle_child)
                {};

        AST_Node(NonTerminals type, AST_Node* down_node) : AST_Node(type, nullptr, down_node, nullptr) {};
        AST_Node(NonTerminals type, AST_Node* left_down_node, AST_Node* right_down_node) : AST_Node(type, left_down_node, nullptr, right_down_node) {};
        AST_Node(NonTerminals type, AST_Node* left_down_node, AST_Node* middle_down_node, AST_Node* right_down_node);
        ~AST_Node();

        NonTerminals GetType();
        std::unique_ptr<BaseToken>* GetToken();
        void Dump(std::ofstream& dump_file);
};


template <GrammarType G>
class Parser{
public:
    Parser(){std::cout << "No such Grammar type implemented" << std::endl;};
    void Parse(std::vector<std::unique_ptr<BaseToken>>& Tokens){
        std::cout << "No such Grammar type implemented" << std::endl;
    };

    void DumpHistory(){
        std::cout << "No such Grammar type implemented" << std::endl;
    }

    void DumpTree(){
            std::cout << "No such Grammar type implemented" << std::endl;
        }
};

struct HistoryEntry{
    std::vector<States> cond_stack_;
    std::vector<AST_Node*> symbols_;
    int cur_inp_token_;
    Action action_;

    HistoryEntry(std::vector<States>& cond_stack_, std::vector<AST_Node*>& symbols, int cur_inp_token, Action& action) :
                cond_stack_(cond_stack_), symbols_(symbols), cur_inp_token_(cur_inp_token), action_(action) {};
};

template <>
class Parser<GrammarType::LR0>{
    std::vector<States> cond_stack_;
    std::vector<AST_Node*> symbols_;
    AST_Node* root_;

    Action actions_[15][9] = {};
    States goto_[15][4] = {};

    std::vector<HistoryEntry> history_;
    std::string dump_hist_file_name_common_ = "HistDump";
    std::string dump_tree_file_name_common_ = "TreeDump";

    public:
        Parser();
        ~Parser(){delete root_;};

        void Parse(std::vector<std::unique_ptr<BaseToken>>& Tokens);
        void DumpHistory(std::vector<std::unique_ptr<BaseToken>>& Tokens, std::string dump_file_name = "");
        void DumpTree(std::string dump_file_name = "");

        Parser operator=(Parser& other) = delete;
}; 

#endif