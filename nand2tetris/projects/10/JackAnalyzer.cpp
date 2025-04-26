#include<iostream>
#include<fstream>
#include<filesystem>
#include<vector>
#include<JackTokenizer.h>
#include<assert.h>
#include<CompilationEngine.h>
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
                &&entry.path().filename().extension().string()==".jack") { // 判断是否是普通文件
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

string get_output_filepath(string filepath,int type){
    assert(type<=2);
    string output_filepath=filepath;
    string filename=filepath.substr(filepath.find_last_of("\\")+1,filepath.find_last_of(".")-filepath.find_last_of('\\')-1);
    if(type==1){
        output_filepath.replace(output_filepath.begin()+output_filepath.find_last_of("\\"),output_filepath.end(),"\\out\\"+filename+"T_output.xml");
    }else{
        output_filepath.replace(output_filepath.begin()+output_filepath.find_last_of("\\"),output_filepath.end(),"\\out\\"+filename+"_output.xml");
    }
    return output_filepath;
}

int main(int argc,char** argv){
    vector<string> filepath=listFiles(string(argv[1]));
    for(int i=0;i<filepath.size();++i){
        JackTokenizer tokenizer(filepath[i]);
        tokenizer.parse();

        vector<Token> tokens=tokenizer.get_tokens();
        
        ofstream out_token(get_output_filepath(filepath[i],1),ios::out);
        tokenizer.print_tokens(out_token);

        CompilationEngine ce(get_output_filepath(filepath[i],2),tokenizer.get_tokens());
        ce.compile();
    }
    return 0;
}