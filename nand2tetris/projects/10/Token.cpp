#include"Token.h"

Token::Token(LEXICAL_ELEMENTS type,string val) : type(type),val(val){}
Token::Token(){}

LEXICAL_ELEMENTS Token::get_type()
{
    return type;
}

string Token::get_type_str()
{
    switch(type){
        case KEYWORD:return "keyword";
        case SYMBOL:return "symbol";
        case INTEGERCONSTANT:return "integerConstant";
        case STRINGCONSTANT:return "stringConstant";
        case IDENTIFIER:return "identifier";
        default: return "NULL";
    }
}

string Token::get_val()
{
    return val;
}
ostream &operator<<(ostream &os, Token &t)
{
    if(t.get_type()==NULLELEMENT){
        return os<<t.get_val();
    }else{
        return os<<"<"<<t.get_type_str()<<"> "
                <<t.get_val()<<" "
                <<"</"<<t.get_type_str()<<"> ";
    }
    
}

