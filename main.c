//
//  main.c
//  Assembler
//
//  main.c
//  Assembler
//
//  Created by Yotam Manne on 18/11/2018.
//  Copyright © 2018 Yotam Manne. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "Parser.h"

#define MEMSIZE 4096
#define LINESIZE 500


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
//        parsed_instruction = {6,12,4,10,7,1023};
//        parsed_instruction[0]=6; parsed_instruction[1]=12; parsed_instruction[2]=4;
//        parsed_instruction[3]=10;parsed_instruction[4]=7;parsed_instruction[5]=1023;
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
                else
                    temp=temp&0xfff;
                inst |= temp;
            }

            memory[count] = inst;
            count++;
        }

    }
    for(int i=0;i<MEMSIZE;i++){
        fprintf(mem_file, "%08X\n",memory[i]);
    }
    fclose(mem_file);
    fclose(asm_file);
    return 0;
}
