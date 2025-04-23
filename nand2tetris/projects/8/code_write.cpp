#include "code_write.h"


code_write::code_write(ofstream* out) : out(out){
    umap["local"]="LCL";
    umap["argument"]="ARG";
    umap["this"]="THIS";
    umap["that"]="THAT";
    
    *out<<"@256"<<endl;
    *out<<"D=A"<<endl;
    *out<<"@SP"<<endl;
    *out<<"M=D"<<endl;

    write_call("Sys.init",0);
}

void code_write::write(parser p)
{
    *out<<"//"<<p.get_command()<<endl;
    if(p.get_command_type()==C_ARITHMETIC){
        write_arithmetic(p);
    }else if(p.get_command_type()==C_PUSH||p.get_command_type()==C_POP){
        write_push_pop(p);
    }else if(p.get_command_type()==C_LABEL){
        write_label(p.get_arg1());
    }else if(p.get_command_type()==C_GOTO){
        write_goto(p.get_arg1());
    }else if(p.get_command_type()==C_IF){
        write_if(p.get_arg1());
    }else if(p.get_command_type()==C_FUNCTION){
        write_function(p.get_arg1(),p.get_arg2());
    }else if(p.get_command_type()==C_CALL){
        write_call(p.get_arg1(),p.get_arg2());
    }else if(p.get_command_type()==C_RETURN){
        write_return();
    }
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
        write_arithmetic_compare("EQ");
    }else if(op=="gt"){
        write_arithmetic_compare("GT");
    }else if(op=="lt"){
        write_arithmetic_compare("LT");
    }else if(op=="and"||op=="or"||op=="not"){
        *out<<"@SP"<<endl;
        *out<<"AM=M-1"<<endl;
        *out<<"D=M"<<endl;
        if(op=="and"||op=="or"){
            *out<<"@SP"<<endl;
            *out<<"AM=M-1"<<endl;
            if(op=="and"){
                *out<<"D=D&M"<<endl;
            }else{
                *out<<"D=D|M"<<endl; 
            }
        }else{
            *out<<"D=!D"<<endl;
        }
        write_push();
    }else{
        throw std::invalid_argument("arithmetic operation is not allowed."); // 抛出异常
    }
    
}

void code_write::write_arithmetic_compare(string op){
    nextlabel+=1;

    *out<<"@SP"<<endl;
    *out<<"AM=M-1"<<endl;
    *out<<"D=M"<<endl;

    *out<<"@SP"<<endl;
    *out<<"A=M-1"<<endl;
    *out<<"D=M-D"<<endl;
    *out<<"M=-1"<<endl;
    *out<<"@"<<op<<"TRUE"<<nextlabel<<endl;
    *out<<"D;J"<<op<<endl;
    *out<<"@SP"<<endl;
    *out<<"A=M-1"<<endl;
    *out<<"M=0"<<endl;
    *out<<"("<<op<<"TRUE"<<nextlabel<<")"<<endl;
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
void code_write::write_label(string label){
    *out<<"("+label+")"<<endl;
}
void code_write::write_goto(string label){
    *out<<"@"+label<<endl;
    *out<<"0;JMP"<<endl;
}
void code_write::write_if(string label){
    write_pop();
    *out<<"@"+label<<endl;
    *out<<"D;JNE"<<endl;
}
void code_write::write_function(string fn_name,int nvars){
    *out<<"("+fn_name+")"<<endl;
    for(int i=0;i<nvars;++i){
        *out<<"@0"<<endl; 
        *out<<"D=A"<<endl;  
        write_push();
    }
}
void code_write::write_call(string fn_name,int nargs){
    //Save the return address
    *out<<"@"+fn_name+"$ret."+to_string(ret_label)<<endl;
    *out<<"D=A"<<endl;
    write_push();

    //Save the caller’s segment pointers
    *out<<"@LCL"<<endl;
    *out<<"D=M"<<endl;
    write_push();

    *out<<"@ARG"<<endl;
    *out<<"D=M"<<endl;
    write_push();
    
    *out<<"@THIS"<<endl;
    *out<<"D=M"<<endl;
    write_push();

    *out<<"@THAT"<<endl;
    *out<<"D=M"<<endl;
    write_push();

    //Reposition ARG (for the callee)
    *out<<"@"<<5+nargs<<endl;
    *out<<"D=A"<<endl;
    *out<<"@SP"<<endl;
    *out<<"D=M-D"<<endl;
    *out<<"@ARG"<<endl;
    *out<<"M=D"<<endl;

    //Reposition LCL (for the callee)
    *out<<"@SP"<<endl;
    *out<<"D=M"<<endl;
    *out<<"@LCL"<<endl;
    *out<<"M=D"<<endl;
    

    //Go to execute the callee’s code
    *out<<"@"+fn_name<<endl;
    *out<<"0;JMP"<<endl;
    *out<<"("+fn_name+"$ret."+to_string(ret_label)+")"<<endl;
    ret_label++;
}
void code_write::write_restore_segment(string seg,int bias){
    *out<<"@endFrame"<<endl;
    if(bias==1){
        *out<<"D=M-1"<<endl; 
    }else{
        *out<<"D=M"<<endl; 
        *out<<"@"<<bias<<endl;
        *out<<"D=D-A"<<endl;
    }
    *out<<"A=D"<<endl;
    *out<<"D=M"<<endl;
    *out<<"@"<<seg<<endl;
    *out<<"M=D"<<endl;
}
void code_write::write_return(){

    //endFrame = LCL   gets the address at the frame’s end
    *out<<"@LCL"<<endl;
    *out<<"D=M"<<endl;
    *out<<"@endFrame"<<endl;
    *out<<"M=D"<<endl;

    //retAddr = *(endFrame – 5)  gets the return address
    write_restore_segment("retAddr",5);
    
    //*ARG = pop()
    write_pop();
    *out<<"@ARG"<<endl;
    *out<<"A=M"<<endl;
    *out<<"M=D"<<endl;

    //SP = ARG + 1
    *out<<"@ARG"<<endl;
    *out<<"D=M+1"<<endl;
    *out<<"@SP"<<endl;
    *out<<"M=D"<<endl;

    //THAT = *(endFrame – 1)
    write_restore_segment("THAT",1);
    
    //THIS = *(endFrame – 2)
    write_restore_segment("THIS",2);

    //ARG = *(endFrame – 3)
    write_restore_segment("ARG",3);

    //LCL = *(endFrame – 4)
    write_restore_segment("LCL",4);

    //goto retAddr
    *out<<"@retAddr"<<endl;
    *out<<"A=M"<<endl;
    *out<<"0;JMP"<<endl;

}
string code_write::get_output_command(){
    return output_command;
}
code_write::~code_write()
{
    out->close();
}