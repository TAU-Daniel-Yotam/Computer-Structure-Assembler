//
// Created by danielpeer on 26/11/2018.
//
#include "Enum.h"
#include <string.h>
#include <stdio.h>
#ifndef Parser_h
#define Parser_h

#endif

int read_line_by_line(FILE*f,char*line);
void findLabels(FILE *file,char ** Labels,char * line);
int findAddress(char ** Labels,char * label);
int parser(char * line,int * command,char ** Labels);
int  parse_opcode(char * opcode);
int parse_register(char * reg);
char * removeLabel(char * line);
int parseImmediate(char * immediate);