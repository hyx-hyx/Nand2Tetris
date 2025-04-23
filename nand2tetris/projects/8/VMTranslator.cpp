#include<iostream>
#include<fstream>
#include<filesystem>
#include"parser.h"
#include"code_write.h"
using namespace std;

vector<string> listFiles(const filesystem::path& directory) {
    vector<string> filepath;
    if (filesystem::is_regular_file(directory)
        &&directory.filename().extension().string()==".jack") { // 判断是否是普通文件
        filepath.push_back(directory.string());
    }else{
        // 遍历目录
        for (const auto& entry : filesystem::directory_iterator(directory)) {
            if (filesystem::is_regular_file(entry)
                &&entry.path().filename().extension().string()==".vm") { // 判断是否是普通文件
                filepath.push_back(entry.path().string());
            } else if (filesystem::is_directory(entry)) { // 判断是否是子目录
                vector<string> subfilepath=listFiles(entry);
                filepath.reserve(filepath.size() + subfilepath.size());
                filepath.insert(filepath.end(), subfilepath.begin(), subfilepath.end());
            }
        }
    }
    return filepath;
}
void parse_file(code_write& cw,string path,ios::openmode mode){
    ifstream in(path,ios::in);
    parser p;
    
    int last=path.find_last_of("\\");
    string filename=path.substr(last+1,path.find_last_of(".")-1-last);
    p.set_filename(filename);

    while(p.has_more_commands(in)){
        p.parse();
        cw.write(p);
    }
    in.close();
}

int main(int argc,char** argv){
    // get output file path
    string path(argv[1]);
    int last=path.find_last_of("\\");
    string filename=path.substr(last+1,path.size());
    string output_path=path+"\\"+filename+".asm";
    
    ofstream out(output_path,ios::out);
    code_write cw(&out);

    vector<string> filepath=listFiles(string(argv[1]));
    for(int i=0;i<filepath.size();++i){
        if(i==0){
            parse_file(cw,filepath[i],ios::out);
        }else{
            parse_file(cw,filepath[i],ios::out|ios::app);
        }
    }
    return 0;
}