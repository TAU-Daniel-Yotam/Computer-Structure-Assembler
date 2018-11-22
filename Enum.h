//
//  Enum.h
//  Assembler
//
//  Created by Yotam Manne on 22/11/2018.
//  Copyright © 2018 Yotam Manne. All rights reserved.
//

#ifndef Enum_h
#define Enum_h

#include <stdio.h>

typedef enum reg{
    ZERO= 0x0,
    AT  = 0x1,
    V0  = 0x2,
    A0  = 0x3,
    A1  = 0x4,
    T0  = 0x5,
    T1  = 0x6,
    T2  = 0x7,
    T3  = 0x8,
    S0  = 0x9,
    S1  = 0xa,
    S2  = 0xb,
    GP  = 0xc,
    SP  = 0xd,
    FP  = 0xe,
    RA  = 0xf
} reg;

typedef enum instrucion{
    ADD     = 0x0,
    SUB     = 0x1,
    AND     = 0x2,
    OR      = 0x3,
    SLL     = 0x4,
    SRA     = 0x5,
    MAC     = 0x6,
    BRANCH  = 0x7,
    RES1    = 0x8,
    RES2    = 0x9,
    RES3    = 0xa,
    JAL     = 0xb,
    LW      = 0xc,
    SW      = 0xd,
    JR      = 0xe,
    HALT    = 0xf
}inst;

#endif /* Enum_h */
