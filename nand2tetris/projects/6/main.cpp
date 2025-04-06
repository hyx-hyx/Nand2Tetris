#include<iostream>
#include"parser.h"
#include"code.h"
#include"symbol_table.h"
using namespace std;
int main(int argc,char** argv){
    ifstream in("max\\Max.asm",ios_base::in);
    symbol_table symtab;
    symtab.lable_map(in);
    parser p;
    code c;
    ofstream out("a.hack",ios_base::out);
    while(p.has_more_commands(in)){
        p.parse();
        cout<<p.to_string()<<endl;

        string binary_code=c.get_binary_code(p);
        cout<<binary_code<<endl;
        
        out<<binary_code<<endl;
    }
    in.close();
    return 0;
}