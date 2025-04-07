#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"symbol_table.h"
using namespace std;

class parser
{
private:
    string instruction;
    int line_count=0;
    int type;
    string dest;
    string comp;
    string jmp;
    string address;   // for A instruction
public:
    parser();
    bool has_more_commands(ifstream& in);
    void parse(symbol_table& symtab);
    string get_dest();
    void set_dest(string dest);
    string get_comp();
    void set_comp(string comp);
    string get_jmp();
    void set_jmp(string jmp);
    string get_address();
    void set_address(string address);
    
    string to_string();

    int get_type();
    ~parser();
private:
};
