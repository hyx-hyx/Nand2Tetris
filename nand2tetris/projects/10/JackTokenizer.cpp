#include "JackTokenizer.h"

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

bool JackTokenizer::has_more_token()
{
    Token token;
    char c;
    string line;
    getline(in,line);
    
    while(in.get(c)){
        if(c=='/'){
            
        }
        if(c==' '){continue;}
        
        if(c=='"'){
            while(in.get(c)&&c!='"'){ token.val+=c;}
            token.type=STRINGCONSTANT;
            return true;
        }

        if(isdigit(c)){
            token.val+=c;
            while(in.get(c)&&isdigit(c)){ token.val+=c;}
            token.type=INTEGERCONSTANT;
            return true;
        }

        if(symbol.count(token.val)){
            cur_token=Token(SYMBOL,token.val);
            return true;
        }   
        if(isalpha(c)||c=='_'){
            token.val+=c;
            while(in.get(c)&&(isdigit(c)||isalpha(c)||c=='_')){ 
                token.val+=c;
                if(keyword.count(token.val)){
                    cur_token=Token(KEYWORD,token.val);
                    token.val="";
                    return true;
                }
            }
            cur_token=Token(IDENTIFIER,token.val);
            return true;
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
