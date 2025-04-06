#pragma once
#include<unordered_map>
#include<bitset>
#include"parser.h"
#include<iostream>
using namespace std;

class code
{
private:
    unordered_map<string,bitset<7>> comp_table;
    unordered_map<string,bitset<3>> dest_table;
    unordered_map<string,bitset<3>> jmp_table;
    string binary_code;
public:
    code();
    string get_binary_code(parser p);
    ~code();
private:
    // string get_comp_code(string comp);
    // string get_dest_code(string dest);
    // string get_jmp_code(string jmp);
};
