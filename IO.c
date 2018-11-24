//
//  IO.c
//  Assembler
//
//  Created by Yotam Manne on 18/11/2018.
//  Copyright © 2018 Yotam Manne. All rights reserved.
//

#include "IO.h"
#define BUFSIZE 500

int read_line_by_line(FILE*f,char*line){
    char* readlen;
    int bufsize = BUFSIZE;
    if ((readlen = fgets(line, bufsize, f)) != NULL) {
        return 0;
    }
    return 1;

}
