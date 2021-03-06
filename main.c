

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "Parser.h"




/**
 Assembler - the programs reads an assembly program and translates it to instruction encodinds in
 8 hexadecimal digits format. The output is written to a text file that represents the memory of the
 computer.

 @param argv -  [1]: the assembly file (a program in assembly language)
                [2]: the output file - the memory of the computer/
 @return 0
 */
int main(int argc, const char * argv[]) {
    assert(argc==3);
    int count = 0;
    FILE*asm_file = fopen(argv[1], "r");
    FILE*mem_file = fopen(argv[2], "w");
    int parsed_instruction[6]; //opcode, rd, rs, rt, rm, imm
    char buf[LINESIZE];
    char ** labels=calloc(MEMSIZE, sizeof(char *));
    int memory[MEMSIZE]={0};
    findLabels(asm_file,labels,buf);
    rewind(asm_file);
    while(!read_line_by_line(asm_file, buf)){
        unsigned int inst = 0;
        if(parser(buf,parsed_instruction,labels)==1)
            continue;
        if(parsed_instruction[0]==8){ // if "word" instruction
            memory[parsed_instruction[1]] = parsed_instruction[2];
        }
        else{ //build the bits of an int according to the parsed instruction
            inst =0;
            for(int i=0;i<6;i++){
                int temp=parsed_instruction[i];
                if(i>=0 && i<=4){
                    temp<<=(7-i)*4;
                }
                if(i==5){
                    temp &= 0xfff;
                }
                inst |= temp;
            }
            memory[count] = inst;
            count++;
        }

    }
    for(int i=0;i<MEMSIZE;i++){
        fprintf(mem_file, "%08X\r\n",memory[i]);
    }
    fclose(asm_file);
    fclose(mem_file);
    return 0;
}
