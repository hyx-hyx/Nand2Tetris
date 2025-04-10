#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

enum command_type{C_NULL,C_ARITHMETIC,C_PUSH,C_POP,C_LABEL,C_GOTO,C_IF,C_FUNCTION,C_RETURN,C_CALL};
class parser
{
private:
    string filename;
    command_type type;
    string command;
    string arg1;
    int arg2;
public:
    parser();
    bool has_more_commands(ifstream& in);
    void parse();
    string get_command();
    command_type get_command_type();
    string get_arg1();
    int get_arg2();
    void set_filename(string filename);
    string get_filename();
private:
    std::vector<std::string> split_string(const std::string& str);
};