#pragma once
#include<string>
#include<iostream>
using namespace std;
enum LEXICAL_ELEMENTS{KEYWORD,SYMBOL,INTEGERCONSTANT,STRINGCONSTANT,IDENTIFIER,NULLELEMENT};
class Token
{
private:
    LEXICAL_ELEMENTS type;
    string val;
public:
    Token(LEXICAL_ELEMENTS type,string val);
    Token();
    LEXICAL_ELEMENTS get_type();
    string get_type_str();
    string get_val();
    friend ostream& operator<<(ostream& os, Token& t);
};