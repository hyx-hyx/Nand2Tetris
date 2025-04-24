#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<vector>
#include<set>
using namespace std;
enum LEXICAL_ELEMENTS{KEYWORD,SYMBOL,INTEGERCONSTANT,STRINGCONSTANT,IDENTIFIER};
class Token
{
public:
    LEXICAL_ELEMENTS type;
    string val;
public:
    Token(LEXICAL_ELEMENTS type,string val);
    Token();
};


class JackTokenizer
{
private: /* data */
    string filename;
    set<string> keyword;
    set<string> symbol;
    vector<Token> tokens;
    ifstream in;
public:
    JackTokenizer(string filename);
    bool has_more_token();
    Token get_token();
    LEXICAL_ELEMENTS get_type();
    string get_val();
    void match_token(string line);
    ~JackTokenizer();
};