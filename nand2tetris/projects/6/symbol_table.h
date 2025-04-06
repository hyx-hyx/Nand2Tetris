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
    symbol_table();
    void lable_map(ifstream& in);
    bool is_exist(string s);
    void add_variable(string s,int line);
    ~symbol_table();
};
