#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"symbol_table.h"
using namespace std;

enum command_type{C_NULL,C_ARITHMETIC,C_PUSH,C_POP,C_LABEL,C_GOTO,C_IF,C_FUNCTION,C_RETURN,C_CALL};
class parser
{
private:
    ifstream* in=nullptr;
    command_type type;
    string command;
    string arg1;
    int arg2;
public:
    parser(string path);
    bool has_more_commands();
    void parse();
    int get_command_type();
    string get_arg1();
    int get_arg2();
private:
    std::vector<std::string> split_string(const std::string& str);
};