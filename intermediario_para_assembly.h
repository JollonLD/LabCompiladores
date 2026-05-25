#ifndef INTERMEDIARIO_PARA_ASSEMBLY_H
#define INTERMEDIARIO_PARA_ASSEMBLY_H

#include <stdio.h>

typedef enum {
    REG_ZERO = 0,
    REG_RF,
    REG_T0, REG_T1, REG_T2, REG_T3, REG_T4, REG_T5, REG_T6, REG_T7, REG_T8, REG_T9,
    REG_T10, REG_T11, REG_T12, REG_T13, REG_T14, REG_T15, REG_T16, REG_T17, REG_T18, REG_T19,
    REG_T20, REG_T21, REG_T22, REG_T23, REG_T24, REG_T25,
    REG_S0, REG_S1, REG_S2, REG_S3, REG_S4, REG_S5, REG_S6, REG_S7, REG_S8, REG_S9,
    REG_S10, REG_S11, REG_S12, REG_S13, REG_S14, REG_S15, REG_S16, REG_S17, REG_S18, REG_S19,
    REG_S20, REG_S21, REG_S22, REG_S23, REG_S24, REG_S25,
    REG_INVALIDO
} RegistradorAsm;

typedef enum {
    ASM_ADD,
    ASM_ADDI,
    ASM_SUB,
    ASM_SUBI,
    ASM_MULT,
    ASM_MULTI,
    ASM_DIV,
    ASM_BEQ,
    ASM_BGT,
    ASM_BLT,
    ASM_BGE,
    ASM_BLE,
    ASM_BNE,
    ASM_NOP,
    ASM_HLT,
    ASM_IN,
    ASM_OUT,
    ASM_J,
    ASM_MOV,
    ASM_LOAD,
    ASM_STORE,
    ASM_PARAM,
    ASM_CALL,
    ASM_RETURN,
    ASM_FUNC,
    ASM_ENDFUNC,
    ASM_ARG,
    ASM_ALLOCVAR,
    ASM_ALLOCVET
} InstrucaoAsm;

typedef enum {
    FORMATO_OPR = 0,
    FORMATO_OPI,
    FORMATO_B,
    FORMATO_J,
    FORMATO_C,
    FORMATO_M
} FormatoAsm;

typedef struct InstrucaoAssembly {
    InstrucaoAsm instrucao;
    RegistradorAsm rd;
    RegistradorAsm rs;
    RegistradorAsm rt;
    int imediato;
    int endereco;
    int formato;
    struct InstrucaoAssembly* prox;
} InstrucaoAssembly;

typedef struct quadrupla {
    char* opr;
    char* op1;
    char* op2;
    char* op3;
} quadrupla;

typedef struct quadList {
    quadrupla quad;
    struct quadList* prox;
} quadList;

void traduzirQuadruplasParaAssembly(const quadList* listaQuadruplas);

#endif /* INTERMEDIARIO_PARA_ASSEMBLY_H */