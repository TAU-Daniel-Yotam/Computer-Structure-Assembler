//
//  Parser.c
//  Assembler
//
//  Created by Yotam Manne on 18/11/2018.
//  Copyright © 2018 Yotam Manne. All rights reserved.
//

#include "Parser.h"
#include "stdlib.h"
#define BUFSIZE 500


// TODO CHECK if we need to change \n to \r\n
int read_line_by_line(FILE*f,char*line){
    char* readlen;
    char * word;
    int bufSize = BUFSIZE;
    if ((readlen = fgets(line, bufSize, f)) != NULL) {
        word=strtok(line,"\n");  //remove \n from line .need to check if we need to change to \r\n
        if(!word) //because strtok on line "\n" not remove '\n' i check if it returns null(only if
            // line  was "\n"  ) and then remove \n by myself-return NULL;
            line[0]='\0';
        return 0;
    }
    return 1;
}

void findLabels(FILE *file,char ** Labels,char * line) {
    char *label, *word;
    int pc = 0;
    while (!read_line_by_line(file, line)) {
        word = strtok(line, " \t:");
        if(!word)//check if it was empty line . if it is emtpy line we skip to next iteration and read the next line
            continue;
        else if ((memcmp(word, ".word", 5) != 0) && (parse_opcode(word) == -1)) {//check if it is label
            label = calloc(strlen(word), sizeof(char));
            strcpy(label, word);
            Labels[pc]=label;
            word=strtok(NULL,",");//check if the label is in the same line of the command .if it's not in the same line
            //then we don't promote pc because the next line refer the same pc.
            if(!word)
                pc++;
        }
        else if((memcmp(word, ".word", 5)))//check if it is command .if it is command we promote pc if not-it is .word  and it's not count
            //as a command so we don't promote pc.
            pc++;
    }
}


int findAddress(char ** Labels,char * label){
    for (int i=0;i<4096;i++){
        if (Labels[i]){
            if(!(strcmp(label,Labels[i]))) {
                return i;
            }
        }
    }
    return 0;
}


int parser(char * line,int * command,char ** Labels){
    char * word;
    int parameter=0;
    char * newline=removeLabel(line);
    if(newline==NULL||!(word=strtok(newline," \t,#")))
        return 1;
    if(!(strcmp(word,".word"))){
        while(parameter<3){
            if (parameter==0)
                command[parameter++]=8;
            else
                command[parameter++]=parseImmediate(word);
            word=strtok(NULL," \t#");
        }
    }
    else {
        while (parameter<6) {
            if (parameter == 0)
                command[parameter++] = parse_opcode(word);
            else if (parameter <= 4)
                command[parameter++] = parse_register(word);
            else {
                if ((word[0] < '0' || word[0] > '9') && word[0] != '-')
                    command[parameter++] = findAddress(Labels, word);
                else command[parameter++] = parseImmediate(word);
            }

            word=strtok(NULL,", \t#");
        }

    }
    return 0;
}

char * removeLabel(char * line){
    for (int i=0; i<strlen(line);i++) {
        if (line[i]==':'){
            if(i<strlen(line)-1)//it is not the end of the line -after the : there is a command-we return the command only
                return &line[i+1];
            else
                return NULL;
        }
    }
    return line;
}





int  parse_opcode(char * opcode){
    if (!strcmp(opcode,"add"))
        return ADD;
    if (!strcmp(opcode,"sub"))
        return SUB;
    if (!strcmp(opcode,"and"))
        return AND;
    if (!strcmp(opcode,"or"))
        return OR;
    if (!strcmp(opcode,"sll"))
        return SLL;
    if (!strcmp(opcode,"sra"))
        return SRA;
    if (!strcmp(opcode,"mac"))
        return MAC;
    if (!strcmp(opcode,"branch"))
        return BRANCH;
    if (!strcmp(opcode,"jal"))
        return JAL;
    if (!strcmp(opcode,"lw"))
        return LW;
    if (!strcmp(opcode,"sw"))
        return SW;
    if (!strcmp(opcode,"jr"))
        return JR;
    if (!strcmp(opcode,"halt"))
        return HALT;
    return -1;
}

int parse_register(char * reg){
    if (!strcmp(reg,"$zero"))
        return ZERO;
    if (!strcmp(reg,"$at"))
        return AT;
    if (!strcmp(reg,"$v0"))
        return V0;
    if (!strcmp(reg,"$a0"))
        return A0;
    if (!strcmp(reg,"$a1"))
        return A1;
    if (!strcmp(reg,"$t0"))
        return T0;
    if (!strcmp(reg,"$t1"))
        return T1;
    if (!strcmp(reg,"$t2"))
        return T2;
    if (!strcmp(reg,"$t3"))
        return T3;
    if (!strcmp(reg,"$s0"))
        return S0;
    if (!strcmp(reg,"$s1"))
        return S1;
    if (!strcmp(reg,"$s2"))
        return S2;
    if (!strcmp(reg,"$gp"))
        return GP;
    if (!strcmp(reg,"$sp"))
        return SP;
    if (!strcmp(reg,"$fp"))
        return FP;
    if (!strcmp(reg,"$ra"))
        return RA;
    return -1;

}

int parseImmediate(char * immediate) {
    if (strlen(immediate) > 1 && immediate[1] == 'x')
        return strtol(immediate, NULL, 16);
    return strtol(immediate, NULL,10);
}
