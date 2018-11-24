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


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
(byte & 0x80 ? '1' : '0'), \
(byte & 0x40 ? '1' : '0'), \
(byte & 0x20 ? '1' : '0'), \
(byte & 0x10 ? '1' : '0'), \
(byte & 0x08 ? '1' : '0'), \
(byte & 0x04 ? '1' : '0'), \
(byte & 0x02 ? '1' : '0'), \
(byte & 0x01 ? '1' : '0')

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
        //parse(parsed_instruction,buf);
        //parsed_instruction = {6,12,4,10,7,1023};
        parsed_instruction[0]=6; parsed_instruction[1]=12; parsed_instruction[2]=4;
        parsed_instruction[3]=10;parsed_instruction[4]=7;parsed_instruction[5]=1023;
        if(parsed_instruction[0]==8){ // if "word" instruction
            memory[parsed_instruction[1]] = parsed_instruction[2];
        }
        else{ //build the bits of an int according to the parsed instruction
            inst = 0;
            for(int i=0;i<6;i++){
                int temp=parsed_instruction[i];
                if(i>=0 && i<=3){
                    temp<<=(7-i)*4;
                }
                else if (i==4){
                    temp<<=12;
                }
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
