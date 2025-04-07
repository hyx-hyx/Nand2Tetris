#include<iostream>
#include"parser.h"
#include"code.h"
#include"symbol_table.h"
using namespace std;
int main(){
    ifstream in("max\\MaxL.asm",ios::in);
    if(!in.is_open()){
        cout<<"open file fail"<<endl;
        return 0;
    }
    symbol_table symtab;
    symtab.lable_map(in);
    parser p;
    code c;
    ofstream out("a.hack",ios::out);
    while(p.has_more_commands(in)){
        p.parse(symtab);
        cout<<p.to_string()<<endl;

        string binary_code=c.get_binary_code(p);
        cout<<binary_code<<endl;
        
        out<<binary_code<<endl;
    }
    in.close();
    out.close();
    return 0;
}