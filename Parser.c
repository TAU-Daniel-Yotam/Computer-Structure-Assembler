

#include "Parser.h"
#define BUFSIZE 500



/**
 Reads one line from the given file into a given buffer

 @param f - a file descriptor
 @param line - a buffer to read one line to
 @return 0 if one line was read or 1 if nothing was read (due to error or EOF)
 */
int read_line_by_line(FILE*f,char*line){
    char* readlen;
    int bufSize = BUFSIZE;
    if ((readlen = fgets(line, bufSize, f)) != NULL) {
        //fseek(f, 2, SEEK_CUR);
        return 0;
    }
    return 1;
}


/**
 Goes through the assembly program's lines and saves for each label the corresponding address

 @param file - a file descriptor
 @param Labels - a string array of size 4096, each cell will contain it's corresponding label or NULL o.w
 @param line - a buffer to read one line to
 */
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
            pc++; //if current line is a valid assembly command (not .word command) increment pc
        }
    }
}


/**
 Finds the memory address of a given label

 @param Labels - the labels-addresses array
 @param label - a label to search for
 @return the memory address of the given label or -1 if not found (should never happen)
 */
int findAddress(char ** Labels,char * label){
    for (int i=0;i<MEMSIZE;i++){
        if (Labels[i]){
            if(!(strcmp(label,Labels[i]))) {
                return i;
            }
        }
    }
    return -1;
}


/**
 Parses one line of assembly code to an int array of size 6 - each cell corresponds to one instruction argument

 @param line - a line to parse
 @param command - an array to save parsed values in
 @param Labels - the memory label index
 @return 0
 */
int parser(char * line,int * command,char ** Labels){
    char * word;
    int parameter=0;
    char * newline=removeLabel(line);
    int isbranch=0;
    word=strtok(newline," \t\r\n,#");
    if(word==NULL)
        return 1;
    if(!(strcmp(word,".word"))){ //special structure of word command
        while(parameter<3){
            if (parameter==0)
                command[parameter++]=8;
            else
                command[parameter++]=parseImmediate(word);
            word=strtok(NULL," \t#");
        }
    }
    else { // valid assembly command
        while (word != NULL && parameter<6) {
            if (parameter == 0){
                int opcode = parse_opcode(word);
                if(opcode==7){ // if branch command - different meaning to rm argument
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
                    command[parameter++] = findAddress(Labels, word); //if immidiate is a label - translate to number
                else command[parameter++] = parseImmediate(word);
            }

            word=strtok(NULL,", \t#");
        }

    }
    return 0;
}


/**
 Removes the label from a given assembly code line

 @param line - an assembly code line
 @return line
 */
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





/**
 Translates an assembly opperation word to an opcode number

 @param opcode - the first word in an assembly instruction, represents the desired operation
 @return the instruction number if valid, or -1 o.w
 */
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


/**
 Translates a register name in assembly language to it's corresponding number

 @param reg - the register name
 @return the register number if valid or -1 o.w
 */
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


/**
 Translates a string that represents a number (in decimal or hexa) to it's integer value

 @param immediate - a string to convert to a number
 @return the integer value of the string
 */
int parseImmediate(char * immediate) {
    if (strlen(immediate) > 1 && immediate[1] == 'x')
        return (int)strtol(immediate, NULL, 16);
    return (int)strtol(immediate, NULL,10);
}
