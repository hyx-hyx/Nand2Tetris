#include"parser.h"

parser::parser(string path):type(C_NULL),command(NULL){
    in=new ifstream(path,ios::in);
}

bool parser::has_more_commands(){
    while(getline(*in,command)){
        // 找到第一个非空格字符的位置
        size_t first = command.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) {
            continue;
        }
        // 找到最后一个非空格字符的位置
        size_t last = command.find_last_not_of(" \t\n\r\f\v");
        command=command.substr(first, last - first + 1);
        if(command[0]=='/'&&command[1]=='/'){
            continue;
        }
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
    }else{
       type=C_ARITHMETIC; 
       arg1=tokens[0];
    }
}



