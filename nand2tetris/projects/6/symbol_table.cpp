#include"symbol_table.h"

symbol_table::symbol_table(){
    st["R0"]=0;
    st["R1"]=1;
    st["R2"]=2;
    st["R3"]=3;
    st["R4"]=4;
    st["R5"]=5;
    st["R6"]=6;
    st["R7"]=7;
    st["R8"]=8;
    st["R9"]=9;
    st["R10"]=10;
    st["R11"]=11;
    st["R12"]=12;
    st["R13"]=13;
    st["R14"]=14;
    st["R15"]=15;
    st["SCREEN"]=16384;
    st["KBD"]=24576;
    st["SP"]=0;
    st["LCL"]=1;
    st["ARG"]=2;
    st["THIS"]=3;
    st["THAT"]=4;
}
void symbol_table::lable_map(ifstream& in){
    in.seekg(0,ios::beg);
    string instruction;
    int line_cnt=0;
    while(getline(in,instruction)){
        size_t first = instruction.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) {
            continue;
        }
        // 找到最后一个非空格字符的位置
        size_t last = instruction.find_last_not_of(" \t\n\r\f\v");
        instruction=instruction.substr(first, last - first + 1);

        if(instruction[0]=='/'&&instruction[1]=='/'){
            continue;
        }else if(instruction[0]=='('&&instruction[instruction.size()-1]==')'){
            string label=instruction.substr(1,instruction.size()-2);
            st[label]=line_cnt;
        }else{
            line_cnt++;
        }
    }
    in.clear();
    in.seekg(0,ios::beg);
}
bool symbol_table::is_exist(string s){
    return st.find(s)!=st.end();
}
void symbol_table::add_variable(string s,int line){
    st[s]=line;
}
int symbol_table::get_variable(string s){
    if(st.find(s)==st.end()){return -1;}
    return st[s];
}
symbol_table::~symbol_table(){

}