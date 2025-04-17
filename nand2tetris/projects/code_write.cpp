#include "code_write.h"


code_write::code_write(ofstream* out) : out(out){
    umap["local"]="LCL";
    umap["argument"]="ARG";
    umap["this"]="THIS";
    umap["that"]="THAT";
}

void code_write::write(parser p)
{
    *out<<"//"<<p.get_command()<<endl;
    if(p.get_command_type()==C_ARITHMETIC){
        write_arithmetic(p);
    }else if(p.get_command_type()==C_PUSH||p.get_command_type()==C_POP){
        write_push_pop(p);
    }else{}
    *out<<endl;
}

void code_write::write_push(){
    *out<<"@SP"<<endl;
    *out<<"A=M"<<endl;
    *out<<"M=D"<<endl;
    *out<<"@SP"<<endl;
    *out<<"M=M+1"<<endl;
}

void code_write::write_pop(){
    *out<<"@SP"<<endl;
    *out<<"AM=M-1"<<endl;
    *out<<"D=M"<<endl;
}

// 操作数1 放在R13,操作数2放在R14
    // @SP
    // AM=M-1;
    // D=M;
    // @R13
    // M=D;

    // @SP
    // AM=M-1;
    // D=M;
    // @R14
    // M=D;

    //@R13
    //D=M
    //@R14
    //D=D OP M
    
    //@SP
    //A=M
    //M=D
    //@SP
    //M=M+1
void code_write::write_arithmetic(parser p)
{
    assert(p.get_command_type()==C_ARITHMETIC);
    string op=p.get_arg1();
    if(op=="add"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        
        *out<<"D=M+D"<<endl;
        write_push();
    }else if(op=="sub"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M-D"<<endl;
        
        write_push();
    }else if(op=="neg"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        *out<<"D=-D"<<endl;
        write_push();
    }else if(op=="eq"){
        nextlabel+=1;

        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;

        *out<<"@SP"<<endl;
        *out<<"A=M-1"<<endl;
        *out<<"D=M-D"<<endl;
        *out<<"M=-1"<<endl;
        *out<<"@eqtrue"<<nextlabel<<endl;
        *out<<"D;JEQ"<<endl;
        *out<<"@SP"<<endl;
        *out<<"A=M-1"<<endl;
        *out<<"M=0"<<endl;
        *out<<"(eqtrue"<<nextlabel<<")"<<endl;

    }else if(op=="gt"){
        nextlabel+=1;

        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;

        *out<<"@SP"<<endl;
        *out<<"A=M-1"<<endl;
        *out<<"D=M-D"<<endl;
        *out<<"M=-1"<<endl;
        *out<<"@gttrue"<<nextlabel<<endl;
        *out<<"D;JGT"<<endl;
        *out<<"@SP"<<endl;
        *out<<"A=M-1"<<endl;
        *out<<"M=0"<<endl;
        *out<<"(gttrue"<<nextlabel<<")"<<endl;

    }else if(op=="lt"){
        nextlabel+=1;

        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;

        *out<<"@SP"<<endl;
        *out<<"A=M-1"<<endl;
        *out<<"D=M-D"<<endl;
        *out<<"M=-1"<<endl;
        *out<<"@lttrue"<<nextlabel<<endl;
        *out<<"D;JLT"<<endl;
        *out<<"@SP"<<endl;
        *out<<"A=M-1"<<endl;
        *out<<"M=0"<<endl;
        *out<<"(lttrue"<<nextlabel<<")"<<endl;
    }else if(op=="and"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;

        *out<<"D=D&M"<<endl;
        write_push();
    }else if(op=="or"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;

        *out<<"D=D|M"<<endl;
        write_push();
    }else if(op=="not"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        *out<<"D=!D"<<endl;
        write_push();
    }else{
        throw std::invalid_argument("arithmetic operation is not allowed."); // 抛出异常
    }
    
}

//access segment data,assign to D Register
void code_write::memory_access(string segname,int arg2){
    *out<<"@"<<arg2<<endl;
    *out<<"D=A"<<endl;
    *out<<"@"<<umap[segname]<<endl;
    *out<<"A=M+D"<<endl;
}
void code_write::write_push_pop(parser p)
{
    command_type type=p.get_command_type();
    assert(type==C_PUSH||type==C_POP);
    string arg1=p.get_arg1();
    int arg2=p.get_arg2();
    unordered_map<string,string>umap;
    
    if(arg1=="local"||arg1=="argument"||arg1=="this"||arg1=="that"){
        if(type==C_PUSH){   
            memory_access(arg1,arg2);
            *out<<"D=M"<<endl;
            write_push();
        }else{
            *out<<"@"<<arg2<<endl;
            *out<<"D=A"<<endl;
            *out<<"@"<<this->umap[arg1]<<endl;
            *out<<"D=M+D"<<endl;
            *out<<"@addr"<<endl;
            *out<<"M=D"<<endl;
            write_pop();
            *out<<"@addr"<<endl;
            *out<<"A=M"<<endl;
            *out<<"M=D"<<endl;
        }
    }else if(arg1=="constant"){
        *out<<"@"<<arg2<<endl;
        *out<<"D=A"<<endl;
        write_push();
    }else if(arg1=="static"){
        if(type==C_PUSH){   
            *out<<"@"<<p.get_filename()+"."+to_string(arg2)<<endl;
            *out<<"D=M"<<endl;
            write_push();
        }else{
            write_pop();
            *out<<"@"<<p.get_filename()+"."+to_string(arg2)<<endl;
            *out<<"M=D"<<endl;
        }
    }
    else if(arg1=="pointer"){
        if(arg2==0){
            arg1="THIS";
        }else{
            arg1="THAT";
        }
        if(type==C_PUSH){  
            *out<<"@"<<arg1<<endl;
            *out<<"D=M"<<endl; 
            write_push();
        }else{
            write_pop();
            *out<<"@"<<arg1<<endl;
            *out<<"M=D"<<endl;
        }
    }else if(arg1=="temp"){
        if(type==C_PUSH){ 
            *out<<"@5"<<endl;
            *out<<"D=A"<<endl;
            *out<<"@"<<arg2<<endl;
            *out<<"A=A+D"<<endl;
            *out<<"D=M"<<endl;
            write_push();
        }else{
            *out<<"@5"<<endl;
            *out<<"D=A"<<endl;
            *out<<"@"<<arg2<<endl;
            *out<<"D=A+D"<<endl;
            *out<<"@addr"<<endl;
            *out<<"M=D"<<endl;
            write_pop();
            *out<<"@addr"<<endl;
            *out<<"A=M"<<endl;
            *out<<"M=D"<<endl;
           
        }
    }
    
}

string code_write::get_output_command(){
    return output_command;
}
code_write::~code_write()
{
    out->close();
}