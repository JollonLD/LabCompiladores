#ifndef ASSEMBLY_GENERATOR_H
#define ASSEMBLY_GENERATOR_H

#include <stdio.h>

typedef enum {
    QOP_INVALIDO = 0,
    Q_NOP,
    Q_HALT,
    Q_ADD,
    Q_SUB,
    Q_MULT,
    Q_DIV,
    Q_BEQ,
    Q_BGE,
    Q_BGT,
    Q_BLE,
    Q_BLT,
    Q_BNE,
    Q_JUMP,
    Q_LABEL,
    Q_IN,
    Q_INPUT,
    Q_OUT,
    Q_OUTPUT,
    Q_STOREVAR,
    Q_LOADVAR,
    Q_LOADCONST,
    Q_STOREVET,
    Q_LOADVET,
    Q_FUNC,
    Q_ARG,
    Q_ALLOCAMEMVAR,
    Q_ALLOCAMEMVET,
    Q_CALL,
    Q_RETURN,
    Q_ENDFUNC,
    Q_PARAM
} QuadruplaOp;

typedef enum {
    invalido = 0,
    nop,
    hlt,
    add,
    addi,
    sub,
    subi,
    mult,
    multi,
    divisao,
    move,
    beq,
    bge,
    bgt,
    ble,
    blt,
    bne,
    j,
    jr,
    jal,
    lwd,
    swd,
    li,
    push,
    pop,
    in,
    out
} AssemblyOp;


typedef enum {
    // Registradores Específicos mapeados no hardware
    $zero,  // $zero - Constante zero
    $ra,  // $ra   - Endereço de retorno (Return Address)
    $rf,  // $rf   - Registrador de Retorno de Função
    $rp,  // $rp   - Ponteiro de Pilha (Reg. de função específica Srp)
    
    // Registradores de Propósito Geral ($r4 a $r63)
    $r4,
    $r5,
    $r6,
    $r7,
    $r8,
    $r9,
    $r10,
    $r11,
    $r12,
    $r13,
    $r14,
    $r15,
    $r16,
    $r17,
    $r18,
    $r19,
    $r20,
    $r21,
    $r22,
    $r23,
    $r24,
    $r25,
    $r26,
    $r27,
    $r28,
    $r29,
    $r30,
    $r31,
    $r32,
    $r33,
    $r34,
    $r35,
    $r36,
    $r37,
    $r38,
    $r39,
    $r40,
    $r41,
    $r42,
    $r43,
    $r44,
    $r45,
    $r46,
    $r47,
    $r48,
    $r49,
    $r50,
    $r51,
    $r52,
    $r53,
    $r54,
    $r55,
    $r56,
    $r57,
    $r58,
    $r59,
    $r60,
    $r61,
    $r62,
    $r63,
    
    NUM_REGISTRADORES = 64
} regs;


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

#endif /* ASSEMBLY_GENERATOR_H */
