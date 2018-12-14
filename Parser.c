

#include "Parser.h"
#define BUFSIZE 500



int read_line_by_line(FILE*f,char*line){
    char* readlen;
    int bufSize = BUFSIZE;
    if ((readlen = fgets(line, bufSize, f)) != NULL) {
        //fseek(f, 2, SEEK_CUR);
        return 0;
    }
    return 1;
}

void findLabels(FILE *file,char ** Labels,char * line) {
    char *label, *word;
    int pc = 0;
    while (!read_line_by_line(file, line)) {
        word = strtok(line, " \r\n\t:");
        if (word!=NULL && (memcmp(word, ".word", 5) != 0) && (parse_opcode(word) == -1)) {
            label = calloc(strlen(word), sizeof(char));
            strcpy(label, word);
            Labels[pc]=label;
        }
        if(word!=NULL && (memcmp(word, ".word", 5) != 0) && (parse_opcode(word) != -1)){
            pc++;
        }
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
    int isbranch=0;
    word=strtok(newline," \t\r\n,#");
    if(word==NULL)
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
        while (word != NULL && parameter<6) {
            if (parameter == 0){
                int opcode = parse_opcode(word);
                if(opcode==7){
                    isbranch=1;
                }
                command[parameter++] = opcode;

            }
            else if (parameter < 4)
                command[parameter++] = parse_register(word);
            else if (parameter==4){
                if(isbranch){
                    command[parameter++] = atoi(word);
                }
                else{
                    command[parameter++] = parse_register(word);
                }
            }
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
        if(line[i]=='#'){
            return line;
        }
        if (line[i]==':'){
            if(i<strlen(line)-1)
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
        return (int)strtol(immediate, NULL, 16);
    return (int)strtol(immediate, NULL,10);
}
