// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.
(LOOP)
@0          
D=A;
@v
M=D;        //set v value that will be filled in screen

@KBD
D=M;        


@BLACK      //if keyborad>0 then v=-1;
D;JGT

(FILL)      
@SCREEN
D=A;
@address
M=D;        //set pointer address = screen

@8192
D=A;
@c
M=D;        // set count=8192(256*512/16)

(FILLLOOP)
@v 
D=M;
@address
A=M;        
M=D;          //RAM[address]=v;

@1
D=A;
@address
M=M+D;        //address=address+32

@c
MD=M-1;           
@FILLLOOP
D;JGT         // if(c>0) jump to whiteloop

@LOOP
0;JMP

(BLACK)
@0
D=A;
@v
M=D-1;
@FILL
0;JMP         //set v=-1;