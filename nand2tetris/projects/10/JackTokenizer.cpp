#include "JackTokenizer.h"
#include<regex>

Token::Token(LEXICAL_ELEMENTS type,string val) : type(type),val(val){}
Token::Token(){}

JackTokenizer::JackTokenizer(string filename){
    this->filename=filename;
    keyword=set<string>{"class","constructor","function","method",
                    "field","static","var","int",
                    "char", "boolean","void" ,"true",
                    "false","null","this","let",
                    "do","if","else","while","return"};
    symbol=set<string>{"{","}","(",")","[","]",".",",",";","+","-","*",
                        "/","&",",","<",">","=","~"};
    in=ifstream(filename);
}
void JackTokenizer::match_token(string line){
    // 开始匹配关键字
    string token_val;
    for(int i=0;i<line.size();++i){
        char c=line[i];
        if(c!=' '&&c!='\t'){
            token_val+=c;
        }

        if(c=='"'){
            token_val.erase(token_val.size()-1);
            int nextquote=i+1;
            while(nextquote<line.size()&&line[nextquote]!='"'){nextquote++;}
            tokens.emplace_back(STRINGCONSTANT,line.substr(i+1,nextquote-i-1));
            i=nextquote;
        }

        if(isdigit(c)){
            int nextquote=i+1;
            while(nextquote<line.size()&&isdigit(line[nextquote])){nextquote++;}
            tokens.emplace_back(INTEGERCONSTANT,line.substr(i,nextquote-i));
            i=nextquote-1;
        }

        if(symbol.count(token_val)){
            tokens.emplace_back(SYMBOL,token_val);
        }   
        if(isalpha(c)||c=='_'){
            int endpos=i+1;
            c=line[endpos];
            while(isdigit(c)||isalpha(c)||c=='_'){
                endpos++;
                token_val+=c;
                if(keyword.count(token_val)){
                    tokens.emplace_back(KEYWORD,token_val);
                    token_val="";
                    break;
                }
                c=line[endpos];
            }
            if(token_val!=""){
                tokens.emplace_back(IDENTIFIER,token_val);
            }
            i=endpos-1;
        }
        token_val="";
    }
}
bool JackTokenizer::has_more_token()
{
    char c;
    string line;
    while(getline(in,line)){
        //去除空格
        if(line.find_first_not_of(" \t")!=string::npos){
            line=line.substr(line.find_first_not_of(' '));
        }

        //去除//单行注释
        int pos;
        if(pos=line.find("//")!=string::npos){
            line.erase(line.begin()+pos-1,line.end());
        }
        if(line.empty()){continue;}
        
        //去除/* */ 和 /** */ 注释
        if(pos=line.find("/*")!=string::npos){
            int endpos=line.find("*/");
            line=line.substr(endpos+2,line.size()-endpos-2);
        }
        if(line.empty()){continue;}
        match_token(line);
        for(int i=0;i<tokens.size();++i){
            cout<<tokens[i].type<<" "<<tokens[i].val<<endl;
        }
    }   
    return false;
}

Token JackTokenizer::get_token(){
    return cur_token;
}

LEXICAL_ELEMENTS JackTokenizer::get_type()
{
    return cur_token.type;
}

string JackTokenizer::get_val()
{
    return cur_token.val;
}

JackTokenizer::~JackTokenizer()
{
    in.close();
}
