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
    int nextlabel=0;
    int ret_label=0;
    int static_label=0;
public:
    code_write(ofstream* out);
    void write(parser p);
    string get_output_command();
    ~code_write();
private:
    void write_arithmetic(parser p);
    void write_push_pop(parser p);
    void write_push();
    void write_pop();
    void write_label(string label);
    void write_goto(string label);
    void write_if(string label);
    void write_function(string fn_name,int nvars);
    void write_call(string fn_name,int nargs);
    void write_return();
    void memory_access(string segname,int arg2);
};

