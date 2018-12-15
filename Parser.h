
/**
 This module is responsible for parsing assembly commands into machine language
 */
#include "Enum.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef Parser_h
#define Parser_h
#define MEMSIZE 4096
#define LINESIZE 500

int read_line_by_line(FILE*f,char*line);
void findLabels(FILE *file,char ** Labels,char * line);
int findAddress(char ** Labels,char * label);
int parser(char * line,int * command,char ** Labels);
int  parse_opcode(char * opcode);
int parse_register(char * reg);
char * removeLabel(char * line);
int parseImmediate(char * immediate);

#endif


