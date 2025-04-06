#include"parser.h"

parser::parser() : type(-1) {
}

bool parser::has_more_commands(ifstream& in){
    while(getline(in,instruction)){
        // 找到第一个非空格字符的位置
        size_t first = instruction.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) {
            continue;
        }
        // 找到最后一个非空格字符的位置
        size_t last = instruction.find_last_not_of(" \t\n\r\f\v");
        instruction=instruction.substr(first, last - first + 1);
        if(instruction[0]=='/'&&instruction[1]=='/'){
            continue;
        }
        return true;
    }
    return false;
}
vector<string> parser::split_instruction(const std::string& str, const std::string& delimiters){
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find_first_of(delimiters, start)) != std::string::npos) {
        token = str.substr(start, end - start);
        if (!token.empty()) {
            tokens.push_back(token);
        }
        start = end + 1;
    }

    // 添加最后一部分
    token = str.substr(start);
    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}
void parser::parse(symbol_table symtab){
    set_address("");set_comp("");set_dest("");set_jmp("");
    if(instruction[0]=='@'){
        type=0;
        string s=instruction.substr(1,instruction.size()-1);
        if(isdigit(s[0])){
            set_address(s);
        }else{
            //query to symbol table
            if(symtab.is_exist(s)){

            }else{
                symtab.add_variable(s,line);
            }
        }
    }else{
        type=1;
        std::string token;
        size_t start = 0;
        size_t end = 0;
        int stage=1;
        while(end<instruction.size()){
            if(instruction[end]=='='){
                set_dest(instruction.substr(start,end-start));
                start=end+1;
                stage=2;
            }
            if(instruction[end]==';'){
                set_comp(instruction.substr(start,end-start));
                start=end+1;
                stage=3;
            }
            end++;
        }
        if(stage==3){
            set_jmp(instruction.substr(start,end-start));
        }else{
            set_comp(instruction.substr(start,end-start));
        }
    }
}
string parser::get_dest(){
    return dest;
}
void parser::set_dest(string dest){
    this->dest=dest;
}
string parser::get_comp(){
    return comp;
}
void parser::set_comp(string comp){
    this->comp=comp;
}
string parser::get_jmp(){
    return jmp;
}
void parser::set_jmp(string jmp){
    this->jmp=jmp;
}
string parser::get_address(){
    return address;
}
void parser::set_address(string address){
    this->address=address;
}

parser::~parser()
{
    // in->close();
    // delete in;
}

string parser::to_string(){
    char c;
    itoa(type,&c,10);
    return "instruction: "+instruction+'\n'
    +"type: "+c+'\n'
    +"dest: "+dest+'\n'
    +"comp: "+comp+'\n'
    +"jmp: "+jmp+'\n'
    +"address: "+address+'\n';
}

int parser::get_type(){
    return type;
}