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
#include "IO.h"
#include "Parser.h"

#define MEMSIZE 4096
#define LINESIZE 4096

int main(int argc, const char * argv[]) {
    assert(argc==3);
    int count = 0;
    FILE*asm_file = fopen(argv[1], "r");
    FILE*mem_file = fopen(argv[2], "w");
    int parsed_instruction[6]; //opcode, rd, rs, rt, rm, imm
    char buf[LINESIZE];
    int memory[MEMSIZE]={0};
    while(!read_line_by_line(asm_file, buf)){
        unsigned int inst = 0;
        parse(parsed_instruction,buf);
        if(parsed_instruction[0]==8){ // if "word" instruction
            memory[parsed_instruction[1]] = parsed_instruction[2];
        }
        else{ //build the bits of an int according to the parsed instruction
            inst = parsed_instruction[0];
            inst = inst << 6;
            for(int i=1;i<4;i++){
                inst += parsed_instruction[1];
                inst = inst << 4;
            }
            inst += parsed_instruction[5];
            inst = inst << 12;
            memory[count] = inst;
            count++;
        }
        
    }
    for(int i=0;i<MEMSIZE;i++){
        fprintf(mem_file, "%32X\n",memory[i]);
    }
    fclose(mem_file);
    fclose(asm_file);
    return 0;
}
