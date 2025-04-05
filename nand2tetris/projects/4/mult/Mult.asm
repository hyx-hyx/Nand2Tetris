// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.
@0
D=A
@R2
M=D       //RAM[2]=0

@R1
D=M;
@18
D;JLE     // test if R1=0

@R0
D=M
@R2
M=M+D     //RAM[2]=RAM[2]+RAM[0]
@R1
D=M
M=D-1     //RAM[1]=RAM[1]-1
@4
D=D-1;JGT // test if R1=0
@18
0;JMP     //infinite loop


