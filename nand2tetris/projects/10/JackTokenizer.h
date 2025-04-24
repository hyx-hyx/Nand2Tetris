#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<vector>
#include<set>
#include"JackTokenizerIterator.h"
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
    void match_token(string line);

    
    JackTokenizerIterator begin() const { return JackTokenizerIterator(&tokens[0]); }
    JackTokenizerIterator end() const { return JackTokenizerIterator() }

    // 提供对元素的访问
    Token& operator[](size_t index) { return tokens[index]; }
    const T& operator[](size_t index) const { return tokens[index]; }
    ~JackTokenizer();
};