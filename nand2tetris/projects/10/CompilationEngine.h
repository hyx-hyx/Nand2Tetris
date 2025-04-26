#pragma once
#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include"Token.h"
#include<vector>
#include<set>

using namespace std;
class CompilationEngine
{
private:
    /* data */
    string output_filepath;
    ostream& out;
    vector<Token> tokens;
    int token_index=0;
    Token cur_token;
    
public:
    CompilationEngine(string output_filepath,vector<Token> tokens);
    CompilationEngine(vector<Token> tokens);

    bool has_more_token();
    void advance();
    void compile();
    void compile_class(int depth);
    void compile_class_vardec(int depth);
    void compile_subroutine_dec(int depth);
    void compile_parameter_list(int depth);
    void compile_subroutine_body(int depth);
    void compile_vardec(int depth);
    void compile_statements(int depth);
    void compile_let(int depth);
    void compile_if(int depth);
    void compile_while(int depth);
    void compile_do(int depth);
    void compile_return(int depth);
    void compile_expression(int depth);
    void compile_term(int depth);
    void compile_expression_list(int depth);
    
    void process(string str,int depth);
    void process(set<string> set,int depth);
    void print_token_with_indent(Token token,int indent);

    ~CompilationEngine();
};

