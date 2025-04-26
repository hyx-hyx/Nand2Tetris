#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<vector>
#include<set>
#include"Token.h"
//#include"JackTokenizerIterator.h"

using namespace std;
class JackTokenizer
{
private: /* data */
    string filename;
    set<string> keyword;
    set<string> symbol;
    vector<Token> tokens;
    ifstream in;
    bool multi_line=false; //multi line comment
public:
    JackTokenizer(string filename);
    void parse();
    void pre_process(string& line);
    void match_token(string line);
    vector<Token> get_tokens();

    // JackTokenizerIterator begin() const { return JackTokenizerIterator(&tokens[0]); }
    // JackTokenizerIterator end() const { return JackTokenizerIterator() }

    // 提供对元素的访问
    // Token& operator[](size_t index) { return tokens[index]; }
    // const Token& operator[](size_t index) const { return tokens[index]; }

    void print_tokens(ostream& out=std::cout);
    
    ~JackTokenizer();
};