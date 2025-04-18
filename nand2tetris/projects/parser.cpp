#include"parser.h"

parser::parser():type(C_NULL){
}

bool parser::has_more_commands(ifstream& in){
    while(getline(in,command)){
        // 找到第一个非空格字符的位置
        size_t first = command.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) {
            continue;
        }
        if(command[first]=='/'&&command[first+1]=='/'){
            continue;
        }
        
        size_t last = command.find_first_of("//");
        command=command.substr(first, last - first);

        // 找到最后一个非空格字符的位置
        last=command.find_last_not_of(" \t\n\r\f\v");
        if (last == std::string::npos) {
            continue;
        }
        
        command=command.substr(0, last + 1);
        
        return true;
    }
    return false;
}


std::vector<std::string> parser::split_string(const std::string& str) {
    std::vector<std::string> tokens;
    std::string token;
    
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += str[i];
        }
    }
    
    // 添加最后一个单词（如果有的话）
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void parser::parse(){
    std::vector<std::string> tokens = split_string(command);
    if(tokens[0]=="push"){
        type=C_PUSH;
        arg1=tokens[1];
        arg2=stoi(tokens[2]);
    }else if(tokens[0]=="pop"){
        type=C_POP;
        arg1=tokens[1];
        arg2=stoi(tokens[2]);
    }else if(tokens[0]=="label"){
        type=C_LABEL;
        arg1=tokens[1];
    }else if(tokens[0]=="goto"){
        type=C_GOTO;
        arg1=tokens[1];
    }else if(tokens[0]=="if-goto"){
        type=C_IF;
        arg1=tokens[1];
    }else if(tokens[0]=="function"){
        type=C_FUNCTION;
        arg1=tokens[1];
        arg2=stoi(tokens[2]);
    }else if(tokens[0]=="call"){
        type=C_CALL;
        arg1=tokens[1];
        arg2=stoi(tokens[2]);
    }else if(tokens[0]=="return"){
        type=C_RETURN;
    }else{
       type=C_ARITHMETIC; 
       arg1=tokens[0];
    }
}

string parser::get_command()
{
    return command;
}

command_type parser::get_command_type()
{
    return type;
}

string parser::get_arg1()
{
    return arg1;
}

int parser::get_arg2()
{
    return arg2;
}

void parser::set_filename(string filename){
    this->filename=filename;
}

string parser::get_filename()
{
    return filename;
}
