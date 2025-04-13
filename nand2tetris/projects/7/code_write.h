#pragma once
#include<unordered_map>
#include<bitset>
#include"parser.h"
#include<iostream>
#include<assert.h>

class code_write
{
private:
    ofstream* out;
    string output_command;
    unordered_map<string,string>umap;
public:
    code_write(ofstream* out);
    void write(parser p);
    string get_output_command();
    ~code_write();
private:
    void write_arithmetic_op(string op);
    void write_arithmetic(parser p);
    void write_push_pop(parser p);
    void write_push();
    void write_pop();
    void memory_access(string segname,int arg2);
    void write_bool_op();
};


