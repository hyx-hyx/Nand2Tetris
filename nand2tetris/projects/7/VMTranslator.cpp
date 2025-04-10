#include<iostream>
#include<fstream>
#include"./parser.h"
#include"code_write.h"
using namespace std;
int main(){
    string path="MemoryAccess\\BasicTest\\BasicTest.vm";
    ifstream in(path,ios::in);
    parser p;
    string filename=path.substr(path.find_last_of('\\')+1,path.find_last_of('.')-path.find_last_of('\\')-1);
    p.set_filename(filename);
    string output_path=path.substr(0,path.find_last_of('.'))+".asm";
    ofstream out(output_path,ios::out);
    code_write cw(&out);
    while(p.has_more_commands(in)){
        p.parse();
        cw.write(p);
    }
    in.close();
    return 0;
}