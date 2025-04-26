#include<iostream>
#include "CompilationEngine.h"
using namespace std;

CompilationEngine::CompilationEngine(string output_filepath,vector<Token> tokens)
    :output_filepath(output_filepath),tokens(tokens),token_index(0),cur_token(tokens[token_index])
        ,out(*new std::ofstream(output_filepath, std::ios::out)){
}

CompilationEngine::CompilationEngine(vector<Token> tokens)
    :tokens(tokens),token_index(0),cur_token(tokens[token_index]),out(cout){
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
    compile_class(0);
}

void CompilationEngine::compile_class(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<class>"),depth);

    process("class",nextdepth);

    print_token_with_indent(cur_token,nextdepth);
    advance();

    process("{",nextdepth);

    while(cur_token.get_val()=="static"||cur_token.get_val()=="field"){
        compile_class_vardec(nextdepth);
    }
    while(cur_token.get_val()=="constructor"||cur_token.get_val()=="function"||cur_token.get_val()=="method"){
        compile_subroutine_dec(nextdepth);
    }
    print_token_with_indent(cur_token,nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</class>"),depth);
}

void CompilationEngine::compile_class_vardec(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<classVarDec>"),depth);

    //('static'|'field')
    process(set<string>{"static","field"},nextdepth);
    
    //type
    process(set<string>{"int","char","boolean",""},nextdepth);
    
    //varName
    process("",nextdepth);

    //(','varName)*
    while(cur_token.get_val()==","){
        print_token_with_indent(cur_token,nextdepth);
        advance();

        process("",nextdepth);
    }

    process(";",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</classVarDec>"),depth);
}

void CompilationEngine::compile_subroutine_dec(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<subroutineDec>"),depth);

    //('constructor'|'function'|'method')
    process(set<string>{"constructor","function","method"},nextdepth);

    //('void'|type)
    process(set<string>{"int","char","boolean","void",""},nextdepth);

    // subroutineName
    process("",nextdepth);

    // '('
    process("(",nextdepth);

    // parameter_list
    compile_parameter_list(nextdepth);
    
    // ')'
    process(")",nextdepth);

    // subroutineBody
    compile_subroutine_body(nextdepth);
    
    print_token_with_indent(Token(NULLELEMENT,"</subroutineDec>"),depth);

}

void CompilationEngine::compile_parameter_list(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<parameterList>"),depth);

    if(cur_token.get_val()=="int"||cur_token.get_val()=="char"
        ||cur_token.get_val()=="boolean"||cur_token.get_type()==LEXICAL_ELEMENTS::IDENTIFIER){
        print_token_with_indent(cur_token,nextdepth);
        advance();

        process("",nextdepth);

        while(cur_token.get_val()==","){
            process(",",nextdepth);
            
            //type
            process(set<string>{"int","char","boolean",""},nextdepth);

            process("",nextdepth);
        }
    }

    print_token_with_indent(Token(NULLELEMENT,"</parameterList>"),depth);
}

void CompilationEngine::compile_subroutine_body(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<subroutineBody>"),depth);

    //'{'
    process("{",nextdepth);

    //varDec*
    while(cur_token.get_val()=="var"){
        compile_vardec(nextdepth);
    }

    // statements
    compile_statements(nextdepth);

    //'}'
    process("}",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</subroutineBody>"),depth);
}

void CompilationEngine::compile_vardec(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<varDec>"),depth);
    // 'var'
    process("var",nextdepth);

    //'type'
    process(set<string>{"int","char","boolean",""},nextdepth);

    //varName
    process("",nextdepth);

    //(','varName)*
    while(cur_token.get_val()==","){
        print_token_with_indent(cur_token,nextdepth);

        advance();
        process("",nextdepth);
    }

    //';'
    process(";",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</varDec>"),depth);
}

void CompilationEngine::compile_statements(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<statements>"),depth);
    string token_val=cur_token.get_val();
    while(token_val!="}"){
        if(token_val=="let"){
            compile_let(nextdepth);
        }else if(token_val=="if"){
            compile_if(nextdepth);
        }else if(token_val=="while"){
            compile_while(nextdepth);
        }else if(token_val=="do"){
            compile_do(nextdepth);
        }else {
            compile_return(nextdepth);
        }
        token_val=cur_token.get_val();
    }

    print_token_with_indent(Token(NULLELEMENT,"</statements>"),depth);

}

void CompilationEngine::compile_let(int depth)
{
    int nextdepth=depth+1;

    print_token_with_indent(Token(NULLELEMENT,"<letStatement>"),depth);

    process("let",nextdepth);

    //varName
    process("",nextdepth);

    if(cur_token.get_val()=="["){
        process("[",nextdepth);
        //expression
        compile_expression(nextdepth);
        process("]",nextdepth);
    }

    //'='
    process("=",nextdepth);

    compile_expression(nextdepth);

    //';'
    process(";",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</letStatement>"),depth);
}

void CompilationEngine::compile_if(int depth)
{
    int nextdepth=depth+1;

    print_token_with_indent(Token(NULLELEMENT,"<ifStatement>"),depth);

    process("if",nextdepth);
    process("(",nextdepth);

    //expression
    compile_expression(nextdepth);

    process(")",nextdepth);
    process("{",nextdepth);

    compile_statements(nextdepth);

    process("}",nextdepth);

    // ('else''{'statements'}')?
    if(cur_token.get_val()=="else"){
        process("else",nextdepth);
        process("{",nextdepth);
        compile_statements(nextdepth);
        process("}",nextdepth);
    }

    print_token_with_indent(Token(NULLELEMENT,"</ifStatement>"),depth);
}

void CompilationEngine::compile_while(int depth)
{
    print_token_with_indent(Token(NULLELEMENT,"<whileStatement>"),depth);

    int nextdepth=depth+1;

    process("while",nextdepth);
    process("(",nextdepth);
    compile_expression(nextdepth);
    process(")",nextdepth);

    process("{",nextdepth);
    compile_statements(nextdepth);
    process("}",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</whileStatement>"),depth);
}

void CompilationEngine::compile_do(int depth)
{
    print_token_with_indent(Token(NULLELEMENT,"<doStatement>"),depth);

    int nextdepth=depth+1;

    process("do",nextdepth);

    //subroutineCall
    process("",nextdepth);
    if(cur_token.get_val()=="("){
        process("(",nextdepth);
        compile_expression_list(nextdepth);
        process(")",nextdepth);
    }else if(cur_token.get_val()=="."){
        process(".",nextdepth);
        process("",nextdepth);
        process("(",nextdepth);
        compile_expression_list(nextdepth);
        process(")",nextdepth);
    }

    process(";",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</doStatement>"),depth);
}

void CompilationEngine::compile_return(int depth)
{
    print_token_with_indent(Token(NULLELEMENT,"<returnStatement>"),depth);

    int nextdepth=depth+1;
    
    process("return",nextdepth);
    if(cur_token.get_val()!=";"){
        compile_expression(nextdepth);
    }
    process(";",nextdepth);

    print_token_with_indent(Token(NULLELEMENT,"</returnStatement>"),depth);
}

void CompilationEngine::compile_expression(int depth)
{
    print_token_with_indent(Token(NULLELEMENT,"<expression>"),depth);
    
    int nextdepth=depth+1;

    compile_term(nextdepth);
    
    set<string> op{"+","-","*","/","&","|","<",">","="};
    while(op.count(cur_token.get_val())){
        process(op,nextdepth);
        compile_term(nextdepth);
    }
    
    print_token_with_indent(Token(NULLELEMENT,"</expression>"),depth);
}

void CompilationEngine::compile_term(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<term>"),depth);

    if(cur_token.get_type()==INTEGERCONSTANT||cur_token.get_type()==STRINGCONSTANT
        ||set<string>{"true","false","null","this"}.count(cur_token.get_val())){
        process("",nextdepth);
    }else if(set<string>{"~","-"}.count(cur_token.get_val())){
        process("",nextdepth);
        compile_term(nextdepth);
    }else if(cur_token.get_val()=="("){
        process("(",nextdepth);
        compile_expression(nextdepth);
        process(")",nextdepth);
    }else if(cur_token.get_type()==IDENTIFIER){
        Token old_token=cur_token;
        process("",nextdepth);

        if(cur_token.get_val()=="["){
            process("[",nextdepth);
            compile_expression(nextdepth);
            process("]",nextdepth);
        }else if(cur_token.get_val()=="("){
            process("(",nextdepth);
            compile_expression_list(nextdepth);
            process(")",nextdepth);
        }else if(cur_token.get_val()=="."){
            process(".",nextdepth);
            process("",nextdepth);
            process("(",nextdepth);
            compile_expression_list(nextdepth);
            process(")",nextdepth);
        }

    }
    print_token_with_indent(Token(NULLELEMENT,"</term>"),depth);
}

void CompilationEngine::compile_expression_list(int depth)
{
    int nextdepth=depth+1;
    print_token_with_indent(Token(NULLELEMENT,"<expressionList>"),depth);
    
    if(cur_token.get_val()!=")"){
        compile_expression(depth+1);

        while(cur_token.get_val()==","){
            process(",",depth+1);
            compile_expression(depth+1);
        }
    }
    print_token_with_indent(Token(NULLELEMENT,"</expressionList>"),depth);
}

void CompilationEngine::process(string str,int depth)
{
    if(cur_token.get_val()==str||str==""){
        print_token_with_indent(cur_token,depth);
    }else{
        out<<"syntax error!"<<endl;
    }
    advance();
}

void CompilationEngine::process(set<string> set,int depth)
{
    if(set.count(cur_token.get_val())||set.count("")){
        print_token_with_indent(cur_token,depth);
    }else{
        out<<"syntax error!"<<endl;
    }
    advance();
}

void CompilationEngine::print_token_with_indent(Token token, int indent)
{
    for(int i=0;i<indent;++i){cout<<"  ";}
    if(!token.get_val().empty()){
        out<<token<<endl;
    }
}

CompilationEngine::~CompilationEngine() { out.clear(); }