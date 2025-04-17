#include<iostream>
#include<fstream>
#include<filesystem>
#include"parser.h"
#include"code_write.h"
using namespace std;

vector<string> listFiles(const filesystem::path& directory) {
    vector<string> filepath;
    // 检查目录是否存在
    if (!filesystem::exists(directory) || !filesystem::is_directory(directory)) {
        std::cerr << "指定路径不存在或不是一个目录：" << directory << std::endl;
        return filepath;
    }

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
    return filepath;
}

int main(){
    vector<string> filepath=listFiles(".\\8\\FunctionCalls\\StaticsTest");
    string path=filepath[0];
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