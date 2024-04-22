#include "../include/Parser.hpp"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>

AST_Node::~AST_Node(){
    if (left_child_ != nullptr) left_child_->~AST_Node();
    if (middle_child_ != nullptr) middle_child_->~AST_Node();
    if (right_child_ != nullptr) right_child_->~AST_Node();

    parent_ = nullptr;
    left_child_ = nullptr;
    middle_child_ = nullptr;
    right_child_ = nullptr;
    children_ = 0;
    type_ = NonTerminals::UNKNOWN;
    token_ = 0;

    delete left_child_;
    delete right_child_;
    delete  middle_child_;
}

AST_Node::AST_Node(NonTerminals type, AST_Node* left_down_node, AST_Node* middle_down_node, AST_Node* right_down_node) : type_(type){
    left_child_ = left_down_node;
    right_child_ = right_down_node;
    middle_child_ = middle_down_node;
    children_ = 0;

    if (left_down_node != nullptr) {
        left_down_node->parent_ = this;
        children_++;
    }
    if (right_down_node != nullptr){
        right_down_node->parent_ = this;
        children_++;
    }
    if (middle_down_node != nullptr){
        middle_down_node->parent_ = this;
        children_++;
    }
}

NonTerminals AST_Node::GetType(){
    return type_;
}

std::unique_ptr<BaseToken>* AST_Node::GetToken(){
    return token_;
}

// хардкод таблиц Actions и GoTo для арифметического языка. В общем случае в инициализацию нужна грамматика,
// таблица переменного размера в динамической памяти и алгоритмы из DragonBook
Parser<GrammarType::LR0>::Parser(){
    // по умолчанию Error
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 9; j++)
            actions_[i][j] = ActionType::ERROR;
    }

    // State 0
    actions_[static_cast<int>(States::I0)][static_cast<int>(Terminals::SUB)] = Action(States::I6);
    actions_[static_cast<int>(States::I0)][static_cast<int>(Terminals::OPEN_BRACK)] = Action(States::I9);
    actions_[static_cast<int>(States::I0)][static_cast<int>(Terminals::NUMBER)] = Action(States::I8);
    actions_[static_cast<int>(States::I0)][static_cast<int>(Terminals::VAR)] = Action(States::I8);

    // State 1
    actions_[static_cast<int>(States::I1)][static_cast<int>(Terminals::END)] = Action(ActionType::ACCEPT);

    // State 2
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I2)][i] = Action(NonTerminals::E, 1);
    }
    actions_[static_cast<int>(States::I2)][static_cast<int>(Terminals::ADD)] = Action(States::I5);
    actions_[static_cast<int>(States::I2)][static_cast<int>(Terminals::SUB)] = Action(States::I5);

    // State 3
    actions_[static_cast<int>(States::I3)][static_cast<int>(Terminals::SUB)] = Action(States::I6);
    actions_[static_cast<int>(States::I3)][static_cast<int>(Terminals::OPEN_BRACK)] = Action(States::I9);
    actions_[static_cast<int>(States::I3)][static_cast<int>(Terminals::NUMBER)] = Action(States::I8);
    actions_[static_cast<int>(States::I3)][static_cast<int>(Terminals::VAR)] = Action(States::I8);

    // State 4
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I4)][i] = Action(NonTerminals::T1, 1);
    }
    actions_[static_cast<int>(States::I4)][static_cast<int>(Terminals::MUL)] = Action(States::I5);
    actions_[static_cast<int>(States::I4)][static_cast<int>(Terminals::DIV)] = Action(States::I5);

    // State 5
    actions_[static_cast<int>(States::I5)][static_cast<int>(Terminals::SUB)] = Action(States::I6);
    actions_[static_cast<int>(States::I5)][static_cast<int>(Terminals::OPEN_BRACK)] = Action(States::I9);
    actions_[static_cast<int>(States::I5)][static_cast<int>(Terminals::NUMBER)] = Action(States::I8);
    actions_[static_cast<int>(States::I5)][static_cast<int>(Terminals::VAR)] = Action(States::I8);

    // State 6
    actions_[static_cast<int>(States::I6)][static_cast<int>(Terminals::OPEN_BRACK)] = Action(States::I9);
    actions_[static_cast<int>(States::I6)][static_cast<int>(Terminals::NUMBER)] = Action(States::I8);
    actions_[static_cast<int>(States::I6)][static_cast<int>(Terminals::VAR)] = Action(States::I8);

    // State 7
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I7)][i] = Action(NonTerminals::T2, 2);
    }

    // State 8
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I8)][i] = Action(NonTerminals::T3, 1);
    }

    // State 9
    actions_[static_cast<int>(States::I9)][static_cast<int>(Terminals::SUB)] = Action(States::I6);
    actions_[static_cast<int>(States::I9)][static_cast<int>(Terminals::OPEN_BRACK)] = Action(States::I9);
    actions_[static_cast<int>(States::I9)][static_cast<int>(Terminals::NUMBER)] = Action(States::I8);
    actions_[static_cast<int>(States::I9)][static_cast<int>(Terminals::VAR)] = Action(States::I8);

    // State 10
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I10)][i] = Action(NonTerminals::T2, 1);
    }

    // State 11
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I11)][i] = Action(NonTerminals::E, 3);
    }

    // State 12
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I12)][i] = Action(NonTerminals::T1, 3);
    }

    // State 13
    actions_[static_cast<int>(States::I13)][static_cast<int>(Terminals::CLOSE_BRACK)] = Action(States::I14);

    // State 14
    for (int i = static_cast<int>(Terminals::ADD); i <= static_cast<int>(Terminals::END); i++){
        actions_[static_cast<int>(States::I14)][i] = Action(NonTerminals::T3, 3);
    }

    //GoTo
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 4; j++){
            goto_[i][j] = States::ERR;
        }
    }

    goto_[static_cast<int>(States::I0)][static_cast<int>(NonTerminals::E)] = States::I1;
    goto_[static_cast<int>(States::I0)][static_cast<int>(NonTerminals::T1)] = States::I2;
    goto_[static_cast<int>(States::I0)][static_cast<int>(NonTerminals::T2)] = States::I4;
    goto_[static_cast<int>(States::I0)][static_cast<int>(NonTerminals::T3)] = States::I10;

    goto_[static_cast<int>(States::I3)][static_cast<int>(NonTerminals::E)] = States::I11;
    goto_[static_cast<int>(States::I3)][static_cast<int>(NonTerminals::T1)] = States::I2;
    goto_[static_cast<int>(States::I3)][static_cast<int>(NonTerminals::T2)] = States::I4;
    goto_[static_cast<int>(States::I3)][static_cast<int>(NonTerminals::T3)] = States::I10;

    goto_[static_cast<int>(States::I5)][static_cast<int>(NonTerminals::T1)] = States::I12;
    goto_[static_cast<int>(States::I5)][static_cast<int>(NonTerminals::T2)] = States::I4;
    goto_[static_cast<int>(States::I5)][static_cast<int>(NonTerminals::T3)] = States::I10;

    goto_[static_cast<int>(States::I6)][static_cast<int>(NonTerminals::T3)] = States::I7;

    goto_[static_cast<int>(States::I9)][static_cast<int>(NonTerminals::E)] = States::I13;
    goto_[static_cast<int>(States::I9)][static_cast<int>(NonTerminals::T1)] = States::I2;
    goto_[static_cast<int>(States::I9)][static_cast<int>(NonTerminals::T2)] = States::I4;
    goto_[static_cast<int>(States::I9)][static_cast<int>(NonTerminals::T3)] = States::I10;
}

Terminals GetTerminalFromToken(std::unique_ptr<BaseToken>& token){
    switch (token->GetType()) {
        case TokenType::END:
            return Terminals::END;
        case TokenType::NUMBER:
            return Terminals::NUMBER;
        case TokenType::VAR:
            return Terminals::VAR;
        case TokenType::BIN_OP:
            switch (static_cast<BinOpToken*>(token.get())->GetBinOpType()) {
                case BinOpType::ADD:
                    return Terminals::ADD;
                case BinOpType::CLOSE_BRACK:
                    return Terminals::CLOSE_BRACK;
                case BinOpType::OPEN_BRACK:
                    return Terminals::OPEN_BRACK;
                case BinOpType::DIV:
                    return Terminals::DIV;
                case BinOpType::MUL:
                    return Terminals::MUL;
                case BinOpType::SUB:
                    return Terminals::SUB;
                default:
                    return Terminals::UNKNOWN;
            }
        default:
            return Terminals::UNKNOWN;
    }
}

static std::string NonTerminalToStr(NonTerminals type){
    switch (type) {
        case NonTerminals::E:
            return "E";
        case NonTerminals::T1:
            return "T1";
        case NonTerminals::T2:
            return "T2";
        case NonTerminals::T3:
            return "T3";
        case NonTerminals::S:
            return "S";
        default:
            return "UNT";
    }
}

static std::string TerminalToStr(std::unique_ptr<BaseToken>* token){
    switch (token->get()->GetType()) {
        case NUMBER:
            return std::to_string(static_cast<NumberToken*>(token->get())->GetNumber());
        case VAR:
        {
            char x = static_cast<VarToken*>(token->get())->GetName();
            return std::string(1, x);
        }
        case BIN_OP:
            switch (static_cast<BinOpToken*>(token->get())->GetBinOpType()) {
                case BinOpType::ADD:
                    return "+";
                case BinOpType::SUB:
                    return "-";
                case BinOpType::MUL:
                    return "*";
                case BinOpType::DIV:
                    return "/";
                case BinOpType::OPEN_BRACK:
                    return "(";
                case BinOpType::CLOSE_BRACK:
                    return ")";
                default:
                    return "UB";
            }

        case END:
            return "$";
        
        default:
            return "UT";
    };
}

void Parser<GrammarType::LR0>::DumpHistory(std::vector<std::unique_ptr<BaseToken>>& Tokens, std::string dump_file_name){
    std::ofstream dump_file{};
    if (dump_file_name == "") dump_file_name = dump_hist_file_name_common_;
    std::string dump_file_name_md = dump_file_name + ".md";

    dump_file.open(dump_file_name_md);
    if (!dump_file.is_open()){
        std::cout << "Impossible to open dump file " << dump_file_name_md << std::endl;
        return;
    }

    dump_file << "| Number |  Stack  |  Symbols  | Input  | Acts  |" << std::endl;
    dump_file << "|--------|---------|-----------|--------|-------|" << std::endl;
    for (int entry = 0; entry < history_.size(); entry++){
        dump_file << "|" << entry << "|";

        for (int i = 0; i < history_[entry].cond_stack_.size(); i++){
            std::string str = std::to_string(static_cast<int>(history_[entry].cond_stack_[i]));
            dump_file << str << " ";
        }

        dump_file << "|";


        for (int i = 0; i < history_[entry].symbols_.size(); i++){
            std::string str = "";

            if (history_[entry].symbols_[i]->GetType() != NonTerminals::TERMINAL){
                str = std::string(NonTerminalToStr(history_[entry].symbols_[i]->GetType()));
            }
            else{
                str = TerminalToStr(history_[entry].symbols_[i]->GetToken());
            }

            dump_file << str << " ";
        }

        dump_file << "|";


        for (int i = history_[entry].cur_inp_token_; i < Tokens.size(); i++){
            std::string str = TerminalToStr(&Tokens[i]);
            dump_file << str << " ";
        }
        dump_file << "|";

        switch (history_[entry].action_.type_) {
            case ActionType::SHIFT:
                dump_file << "Shift " << static_cast<int>(history_[entry].action_.what_do_.ShiftTo_);
                break;
            case ActionType::REDUCE:
                dump_file << "Reduce";
                break;
            case ActionType::ACCEPT:
                dump_file << "Accept";
                break;
            default:
                dump_file << "Shit"; 
                break;
        }
        dump_file << std::endl;
    }

    dump_file.close();

    std::string md_to_pdf = "pandoc " + dump_file_name_md + " -o " + dump_file_name + ".pdf";
    system(md_to_pdf.data());

    std::string rm_md = "rm -f " + dump_file_name_md;
    system(rm_md.data());
}

void AST_Node::Dump(std::ofstream& dump_file){
    dump_file << "   \"" << this << "\"[shape = Mrecord, color = \"black\", style = filled, fontcolor = \"black\", fillcolor = ";

    if (type_ == NonTerminals::TERMINAL){
        dump_file << "\"red\"";
        dump_file << "   label = \" {" << TerminalToStr(token_) << "}\"];\n";
    }
    else{
        dump_file << ((type_ == NonTerminals::S) ? "\"blueviolet\"" : "\"green\"");
        dump_file << "   label = \" {" << NonTerminalToStr(type_) << "}\"];\n";
    }

    if (left_child_ != nullptr) 
        dump_file << "  \"" << this << "\" -> \"" << left_child_ << "\" [color = \"black\"];\n";
    if (middle_child_ != nullptr) 
        dump_file << "  \"" << this << "\" -> \"" << middle_child_ << "\" [color = \"black\"];\n";
    if (right_child_ != nullptr) 
        dump_file << "  \"" << this << "\" -> \"" << right_child_ << "\" [color = \"black\"];\n";

    if (left_child_ != nullptr) 
        left_child_->Dump(dump_file);
    if (middle_child_ != nullptr)
        middle_child_->Dump(dump_file);
    if (right_child_ != nullptr)
        right_child_->Dump(dump_file);
}

void Parser<GrammarType::LR0>::DumpTree(std::string dump_file_name){
    std::ofstream dump_file{};
    if (dump_file_name == "") dump_file_name = dump_tree_file_name_common_;
    std::string dump_file_name_dot = dump_file_name + ".dot";

    dump_file.open(dump_file_name_dot);
    if (!dump_file.is_open()){
        std::cout << "Impossible to open dump file " << dump_file_name_dot << std::endl;
        return;
    }

    dump_file << "digraph Tree\n{\n";
    dump_file << "   rankdir = HR;\n";

    root_->Dump(dump_file);

    //dump_file << "   tree -> \"" << root_ << "\" [color = \"blueviolet\"];\n";
    dump_file << "}";
    dump_file.close();

    std::string create_png = "dot " + dump_file_name_dot + " -Tpng -o " + dump_file_name + ".png";
    system(create_png.data());


    std::string rm_dot = "rm -f " + dump_file_name_dot;
    system(rm_dot.data());
}

void PrintError(std::vector<std::unique_ptr<BaseToken>>& Tokens, int pos){
    std::cout << "Error: line " << Tokens[pos]->GetCurLine() << ", position " << Tokens[pos]->GetCurPos() << std::endl;
}

void Parser<GrammarType::LR0>::Parse(std::vector<std::unique_ptr<BaseToken>>& Tokens){
    bool accepted = false;
    cond_stack_.push_back(States::I0);

    int cur_token_num = 0;
    int max_tokens = Tokens.size();

    while(!accepted){
        States state = cond_stack_.back();
        
        Terminals next_terminal = GetTerminalFromToken(Tokens[cur_token_num]);

        if (next_terminal == Terminals::UNKNOWN){
            std::cout << "Unknown terminal" << std::endl;
            PrintError(Tokens, cur_token_num);
            std::terminate();
        }

        Action& action = actions_[static_cast<int>(state)][static_cast<int>(next_terminal)];
        
        switch (action.type_) {
            case ActionType::ACCEPT:
            {
                accepted = true;
                AST_Node* final_node = new AST_Node(NonTerminals::S, symbols_.back());
                symbols_.pop_back();
                symbols_.push_back(final_node);
                root_ = final_node;
                break;
            }

            case ActionType::SHIFT:
                cond_stack_.push_back(action.what_do_.ShiftTo_);
                symbols_.push_back(new AST_Node(NonTerminals::TERMINAL, &Tokens[cur_token_num]));
                cur_token_num++;
                break;     

            case ActionType::REDUCE:
            {
                int ops = action.what_do_.Reduce_.operands_;
                AST_Node* new_node = nullptr;
                NonTerminals gotten_term = NonTerminals::UNKNOWN;

                switch (ops) {
                    case 1:
                    {
                        new_node = new AST_Node(action.what_do_.Reduce_.ReduceTo_, symbols_.back());
                        symbols_.pop_back();
                        symbols_.push_back(new_node);
                        gotten_term = new_node->GetType();
                        break;
                    }
                    case 2:
                    {
                        AST_Node* node_right = symbols_.back();
                        symbols_.pop_back();
                        AST_Node* node_left = symbols_.back();
                        symbols_.pop_back();
                        new_node = new AST_Node(action.what_do_.Reduce_.ReduceTo_, node_left, node_right);
                        symbols_.push_back(new_node);
                        gotten_term = new_node->GetType();
                        break;
                    }
                    case 3:
                    {
                        AST_Node* node_right = symbols_.back();
                        symbols_.pop_back();
                        AST_Node* node_middle = symbols_.back();
                        symbols_.pop_back();
                        AST_Node* node_left = symbols_.back();
                        symbols_.pop_back();
                        new_node = new AST_Node(action.what_do_.Reduce_.ReduceTo_, node_left, node_middle, node_right);
                        symbols_.push_back(new_node);
                        gotten_term = new_node->GetType();
                        break;
                    }

                    default:
                    {
                        std::cout << "Operands amount invalid" << std::endl;
                        PrintError(Tokens, cur_token_num);
                        std::terminate();
                    }

                }
                
                for (int i = 0; i < ops; i++)
                    cond_stack_.pop_back();

                if (gotten_term >= NonTerminals::UNKNOWN){
                    std::cout << "Error - no terminal determined in Reduce" << std::endl;
                    PrintError(Tokens, cur_token_num);
                    std::terminate();
                }

                States goto_state = goto_[static_cast<int>(cond_stack_.back())][static_cast<int>(gotten_term)];
                if (goto_state == States::ERR){
                    std::cout << "Error - goto state invalid" << std::endl;
                    PrintError(Tokens, cur_token_num);
                    std::terminate();
                }

                cond_stack_.push_back(goto_state);
                break;
            }

            default:
                std::cout << "Action error" << std::endl;
                PrintError(Tokens, cur_token_num);
                std::terminate();

        }

        history_.push_back(HistoryEntry(cond_stack_, symbols_, cur_token_num, action));
        if (accepted) break;
    }
}

