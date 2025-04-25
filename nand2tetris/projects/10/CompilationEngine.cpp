#include<iostream>
#include "CompilationEngine.h"
using namespace std;

CompilationEngine::CompilationEngine(string output_filepath,vector<Token> tokens)
    :output_filepath(output_filepath),tokens(tokens),token_index(0){
    out=ofstream(output_filepath,ios::out);
}

bool CompilationEngine::has_more_token()
{
    if(token_index<tokens.size()-1){
        return true;
    }
    return false;
}

void CompilationEngine::advance()
{
    token_index++;
    cur_token=tokens[token_index];
}

void CompilationEngine::compile()
{
    cur_token=tokens[token_index];
}

void CompilationEngine::compile_class(int depth)
{
    for(int i=0;i<depth;++i){
        cout<<"\t"<<endl;
    }
    cout<<"<class>"<<endl;
    advance();
    print_token_with_indent(cur_token,depth+1);
    advance();
    print_token_with_indent(cur_token,depth+1);
    advance();
    while(cur_token.get_val()=="static"||cur_token.get_val()=="field"){
        compile_class_vardec(depth+1);
    }
    while(cur_token.get_val()=="static"||cur_token.get_val()=="field"||cur_token.get_val()=="field"){
        compile_subroutine_dec(depth+1);
    }
    print_token_with_indent(cur_token,depth+1);

    for(int i=0;i<depth;++i){
        cout<<"\t"<<endl;
    }
    cout<<"</class>"<<endl;
}

void CompilationEngine::compile_class_vardec(int depth)
{
    for(int i=0;i<depth;++i){
        cout<<"\t"<<endl;
    }
    cout<<"<classVarDec>"<<endl;

    //('static'|'field')
    print_token_with_indent(cur_token,depth+1);
    
    //type
    advance();
    if(cur_token.get_val()=="int"||cur_token.get_val()=="char"||cur_token.get_val()=="boolean"||cur_token.get_type()==LEXICAL_ELEMENTS::IDENTIFIER){
        print_token_with_indent(cur_token,depth+1);
    }
    
    //varName
    advance();
    print_token_with_indent(cur_token,depth+1);

    //(','varName)*
    advance();
    while(cur_token.get_val()==","){
        print_token_with_indent(cur_token,depth+1);
        advance();
        print_token_with_indent(cur_token,depth+1);
    }

    //';'
    advance();
    print_token_with_indent(cur_token,depth+1);

    for(int i=0;i<depth;++i){
        cout<<"\t"<<endl;
    }
    cout<<"</classVarDec>"<<endl;
}

void CompilationEngine::compile_subroutine_dec(int depth)
{
    for(int i=0;i<depth;++i){
        cout<<"\t"<<endl;
    }
    cout<<"<subroutineDec>"<<endl;

    //('constructor'|'function'|'method')
    advance();
    if(cur_token.get_val()=="constructor"||cur_token.get_val()=="function"||cur_token.get_val()=="method"){
        print_token_with_indent(cur_token,depth+1);
    }

    //('void'|type)
    advance();
    if(cur_token.get_val()=="void"||cur_token.get_val()=="int"||cur_token.get_val()=="char"
        ||cur_token.get_val()=="boolean"||cur_token.get_type()==LEXICAL_ELEMENTS::IDENTIFIER){
        print_token_with_indent(cur_token,depth+1);
    }

    // subroutineName
    advance();
    print_token_with_indent(cur_token,depth+1);

    // '('
    advance();
    print_token_with_indent(cur_token,depth+1);

    // parameter_list
    compile_parameter_list(depth+1);
    
    // ')'
    print_token_with_indent(cur_token,depth+1);

    // subroutineBody
    compile_subroutine_body(depth+1);
    
    for(int i=0;i<depth;++i){
        cout<<"\t"<<endl;
    }
    cout<<"</subroutineDec>"<<endl;
}

void CompilationEngine::compile_parameter_list(int depth)
{
    if(cur_token.get_val()=="int"||cur_token.get_val()=="char"
        ||cur_token.get_val()=="boolean"||cur_token.get_type()==LEXICAL_ELEMENTS::IDENTIFIER){
        print_token_with_indent(cur_token,depth+1);
        advance();
        print_token_with_indent(cur_token,depth+1);

        advance();
        while(cur_token.get_val()==","){
            print_token_with_indent(cur_token,depth+1);

            advance();
            if(cur_token.get_val()=="int"||cur_token.get_val()=="char"
                ||cur_token.get_val()=="boolean"||cur_token.get_type()==LEXICAL_ELEMENTS::IDENTIFIER){
                print_token_with_indent(cur_token,depth+1);
            }
            advance();
            print_token_with_indent(cur_token,depth+1);

            advance();
        }
    }
}

void CompilationEngine::compile_subroutine_body(int depth)
{
    for(int i=0;i<depth+1;++i){
        cout<<"\t"<<endl;
    }
    cout<<"<subroutineBody>"<<endl;

    //'{'
    advance();
    print_token_with_indent(cur_token,depth+1);

    //varDec*
    advance();
    while(cur_token.get_val()=="var"){
        compile_vardec(depth+1);
        advance();
    }

    // statements
    compile_statements(depth+1);

    //'}'
    advance();
    print_token_with_indent(cur_token,depth+1);

    for(int i=0;i<depth+1;++i){
        cout<<"\t"<<endl;
    }
    cout<<"</subroutineBody>"<<endl;
}

void CompilationEngine::compile_vardec(int depth)
{
    // 'var'
    print_token_with_indent(cur_token,depth+1);

    //'type'
    advance();
    if(cur_token.get_val()=="int"||cur_token.get_val()=="char"
        ||cur_token.get_val()=="boolean"||cur_token.get_type()==LEXICAL_ELEMENTS::IDENTIFIER){
        print_token_with_indent(cur_token,depth+1);
    }

    //varName
    advance();
    print_token_with_indent(cur_token,depth+1);

    //(','varName)*
    while(cur_token.get_val()==","){
        print_token_with_indent(cur_token,depth+1);

        // varName
        advance();
        print_token_with_indent(cur_token,depth+1);

        advance();
    }

    //';'
    print_token_with_indent(cur_token,depth+1);
}

void CompilationEngine::compile_statements(int depth)
{
    string token_val=cur_token.get_val();
    while(token_val!="}"){
        if(token_val=="let"){
            compile_let(depth+1);
        }else if(token_val=="if"){
            compile_if(depth+1);
        }else if(token_val=="while"){
            compile_while(depth+1);
        }else if(token_val=="do"){
            compile_do(depth+1);
        }else {
            compile_return(depth+1);
        }
        
        advance();
    }

}

void CompilationEngine::compile_let(int depth)
{
    print_token_with_indent(cur_token,depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);

    advance();
    if(cur_token.get_val()=="["){
        print_token_with_indent(cur_token,depth+1);

        //expression
        compile_expression(depth+1);

        advance();
        print_token_with_indent(cur_token,depth+1);

    }

    //'='
    advance();
    print_token_with_indent(cur_token,depth+1);

    compile_expression(depth+1);

    //';'
    advance();
    print_token_with_indent(cur_token,depth+1);
}

void CompilationEngine::compile_if(int depth)
{
    print_token_with_indent(cur_token,depth+1);

    //'('
    advance();
    print_token_with_indent(cur_token,depth+1);

    //expression
    compile_expression(depth+1);

    //')'
    advance();
    print_token_with_indent(cur_token,depth+1);

    //'{'
    advance();
    print_token_with_indent(cur_token,depth+1);

    compile_statements(depth+1);

    //'}'
    advance();
    print_token_with_indent(cur_token,depth+1);

    // ('else''{'statements'}')?
    advance();
    if(cur_token.get_val()=="else"){
        print_token_with_indent(cur_token,depth+1);

        advance();
        print_token_with_indent(cur_token,depth+1);

        compile_statements(depth+1);

        advance();
        print_token_with_indent(cur_token,depth+1);
    }
}

void CompilationEngine::compile_while(int depth)
{
    print_token_with_indent(cur_token,depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);

    compile_expression(depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);

    compile_statements(depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);
}

void CompilationEngine::compile_do(int depth)
{
    print_token_with_indent(cur_token,depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);
    
    advance();
    print_token_with_indent(cur_token,depth+1);
    if(cur_token.get_val()=="."){
        advance();
        print_token_with_indent(cur_token,depth+1);
        
        advance();
        print_token_with_indent(cur_token,depth+1);

    }

    advance();
    compile_expression_list(depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);

    advance();
    print_token_with_indent(cur_token,depth+1);
}

void CompilationEngine::compile_return(int depth)
{
    print_token_with_indent(cur_token,depth+1);

    advance();
    if(cur_token.get_val()!=";"){
        compile_expression(depth+1);
        advance();
    }

    print_token_with_indent(cur_token,depth+1);
}

void CompilationEngine::compile_expression(int depth)
{
    compile_term(depth+1);
}

void CompilationEngine::compile_term(int depth)
{
    
}

void CompilationEngine::compile_expression_list(int depth)
{

}

void CompilationEngine::print_token_with_indent(Token token, int indent)
{
    for(int i=0;i<indent;++i){cout<<"\t"<<endl;}
    cout<<token<<endl;
}

CompilationEngine::~CompilationEngine() { out.close(); }