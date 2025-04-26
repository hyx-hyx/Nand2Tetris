#include "JackTokenizer.h"
#include<regex>

JackTokenizer::JackTokenizer(string filename){
    this->filename=filename;
    keyword=set<string>{"class","constructor","function","method",
                    "field","static","var","int",
                    "char", "boolean","void" ,"true",
                    "false","null","this","let",
                    "do","if","else","while","return"};
    symbol=set<string>{"{","}","(",")","[","]",".",",",";","+","-","*",
                        "/","&","|","<",">","=","~"};
    in=ifstream(filename);
}
void JackTokenizer::pre_process(string& line){
    //去除空格
    if(line.find_first_not_of(" \t")!=string::npos){
        line=line.substr(line.find_first_not_of(' '));
    }

    if(line.empty()){return ;}

    //去除//单行注释
    int pos=line.find("//");
    if(pos!=string::npos){
        line=line.substr(0,line.begin()+pos-line.begin());
    }

    if(line.empty()){return ;}

    //去除/* */ 和 /** */ 注释
    if((pos=line.find("/*"))!=string::npos){
        int endpos;
        if((endpos=line.find("*/"))==string::npos){
            multi_line=true;
            line.clear();
            return ;
        }else{
            line=line.substr(endpos+2,line.size()-endpos-2);
            multi_line=false;
        }
    }
    if(line[0]=='*'&&multi_line){
        line.clear();
        return ;
    }
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
vector<Token> JackTokenizer::get_tokens()
{
    return tokens;
}
void JackTokenizer::parse()
{
    char c;
    string line;
    while(getline(in,line)){
        pre_process(line);
        match_token(line);
    }   
}

void JackTokenizer::print_tokens(ostream& out){
    out<<"<tokens>"<<endl;
    for(int i=0;i<tokens.size();++i){
        out<<"<"<<tokens[i].get_type_str()<<"> "
            <<tokens[i].get_val()<<" "
            <<"</"<<tokens[i].get_type_str()<<"> "<<endl;
    }
    out<<"</tokens>"<<endl;
}

JackTokenizer::~JackTokenizer()
{
    in.close();
}
