#pragma once
#include<iostream>
#include<unordered_map>
#include<fstream>
using namespace std;
class symbol_table
{
private:
    unordered_map<string,int>st;
    
public:
    int var_addr=16;
    symbol_table();
    void lable_map(ifstream& in);
    bool is_exist(string s);
    void add_variable(string s,int line);
    int get_variable(string s);
    ~symbol_table();
};
