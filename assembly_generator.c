#include "assembly_generator.h"

#include <stdlib.h>
#include <string.h>

typedef struct LabelLinha {
    char* nome;
    int linha;
    struct LabelLinha* prox;
} LabelLinha;

typedef struct MapaRegistrador {
    char* simbolo;
    int indice;
    char nomeAsm[8];
    struct MapaRegistrador* prox;
} MapaRegistrador;

static LabelLinha* listaLabels = NULL;
static MapaRegistrador* mapaRegistradores = NULL;
static int proximoRegistradorGeral = 4;
static int labelOffset = 0; /* offset para ajustar destinos de salto (instrucoes iniciais) */

static char* duplicarTexto(const char* texto) {
    char* copia;

    if (texto == NULL)
        texto = "";

    copia = (char*)malloc(strlen(texto) + 1);
    if (copia == NULL)
        return NULL;

    strcpy(copia, texto);
    return copia;
}

static void liberarLabels(void) {
    LabelLinha* atual = listaLabels;

    while (atual != NULL) {
        LabelLinha* prox = atual->prox;
        free(atual->nome);
        free(atual);
        atual = prox;
    }

    listaLabels = NULL;
}

static void liberarMapaRegistradores(void) {
    MapaRegistrador* atual = mapaRegistradores;

    while (atual != NULL) {
        MapaRegistrador* prox = atual->prox;
        free(atual->simbolo);
        free(atual);
        atual = prox;
    }

    mapaRegistradores = NULL;
    proximoRegistradorGeral = 4;
}

static void adicionarLabel(const char* nome, int linha) {
    LabelLinha* atual;
    LabelLinha* novo;

    if (nome == NULL || *nome == '\0')
        return;

    for (atual = listaLabels; atual != NULL; atual = atual->prox) {
        if (strcmp(atual->nome, nome) == 0) {
            atual->linha = linha;
            return;
        }
    }

    novo = (LabelLinha*)malloc(sizeof(LabelLinha));
    if (novo == NULL)
        return;

    novo->nome = duplicarTexto(nome);
    if (novo->nome == NULL) {
        free(novo);
        return;
    }

    novo->linha = linha;
    novo->prox = listaLabels;
    listaLabels = novo;
}

static int buscarLinhaLabel(const char* nome) {
    LabelLinha* atual;

    for (atual = listaLabels; atual != NULL; atual = atual->prox) {
        if (strcmp(atual->nome, nome) == 0)
            return atual->linha;
    }

    return -1;
}

static int ehNumero(const char* texto) {
    size_t i;

    if (texto == NULL || *texto == '\0')
        return 0;

    i = 0;
    if (texto[0] == '-')
        i = 1;

    for (; texto[i] != '\0'; i++) {
        if (texto[i] < '0' || texto[i] > '9')
            return 0;
    }

    return 1;
}

static const char* mapearParaRegistradorGeral(const char* operando) {
    MapaRegistrador* atual;
    MapaRegistrador* novo;
    int indice;

    if (operando == NULL || *operando == '\0')
        return operando;

    if (strcmp(operando, "___") == 0 || strcmp(operando, "_") == 0)
        return operando;

    if (ehNumero(operando))
        return operando;

    if (operando[0] == '$')
        return operando;

    for (atual = mapaRegistradores; atual != NULL; atual = atual->prox) {
        if (strcmp(atual->simbolo, operando) == 0)
            return atual->nomeAsm;
    }

    novo = (MapaRegistrador*)malloc(sizeof(MapaRegistrador));
    if (novo == NULL)
        return operando;

    novo->simbolo = duplicarTexto(operando);
    if (novo->simbolo == NULL) {
        free(novo);
        return operando;
    }

    indice = proximoRegistradorGeral;
    if (indice > 63)
        indice = 63;
    else
        proximoRegistradorGeral++;

    novo->indice = indice;
    snprintf(novo->nomeAsm, sizeof(novo->nomeAsm), "$r%d", indice);
    novo->prox = mapaRegistradores;
    mapaRegistradores = novo;

    return novo->nomeAsm;
}

static int ehQuadrupla(const char* opr, const char* esperado) {
    return opr != NULL && strcmp(opr, esperado) == 0;
}

static QuadruplaOp operadorQuadrupla(const char* opr) {
    if (ehQuadrupla(opr, "NOP")) return Q_NOP;
    if (ehQuadrupla(opr, "HALT")) return Q_HALT;
    if (ehQuadrupla(opr, "ADD")) return Q_ADD;
    if (ehQuadrupla(opr, "SUB")) return Q_SUB;
    if (ehQuadrupla(opr, "MULT")) return Q_MULT;
    if (ehQuadrupla(opr, "DIV")) return Q_DIV;
    if (ehQuadrupla(opr, "BEQ")) return Q_BEQ;
    if (ehQuadrupla(opr, "BGE")) return Q_BGE;
    if (ehQuadrupla(opr, "BGT")) return Q_BGT;
    if (ehQuadrupla(opr, "BLE")) return Q_BLE;
    if (ehQuadrupla(opr, "BLT")) return Q_BLT;
    if (ehQuadrupla(opr, "BNE")) return Q_BNE;
    if (ehQuadrupla(opr, "JUMP")) return Q_JUMP;
    if (ehQuadrupla(opr, "LABEL")) return Q_LABEL;
    if (ehQuadrupla(opr, "IN")) return Q_IN;
    if (ehQuadrupla(opr, "INPUT")) return Q_INPUT;
    if (ehQuadrupla(opr, "OUT")) return Q_OUT;
    if (ehQuadrupla(opr, "OUTPUT")) return Q_OUTPUT;
    if (ehQuadrupla(opr, "STOREVAR")) return Q_STOREVAR;
    if (ehQuadrupla(opr, "LOADVAR")) return Q_LOADVAR;
    if (ehQuadrupla(opr, "STOREVET")) return Q_STOREVET;
    if (ehQuadrupla(opr, "LOADVET")) return Q_LOADVET;
    if (ehQuadrupla(opr, "FUNC")) return Q_FUNC;
    if (ehQuadrupla(opr, "ARG")) return Q_ARG;
    if (ehQuadrupla(opr, "ALLOCAMEMVAR")) return Q_ALLOCAMEMVAR;
    if (ehQuadrupla(opr, "ALLOCAMEMVET")) return Q_ALLOCAMEMVET;
    if (ehQuadrupla(opr, "CALL")) return Q_CALL;
    if (ehQuadrupla(opr, "RETURN")) return Q_RETURN;
    if (ehQuadrupla(opr, "ENDFUNC")) return Q_ENDFUNC;
    if (ehQuadrupla(opr, "PARAM")) return Q_PARAM;
    return QOP_INVALIDO;
}

static const char* nomeInstrucaoAssembly(AssemblyOp op) {
    switch (op) {
        case nop: return "nop";
        case hlt: return "hlt";
        case add: return "add";
        case addi: return "addi";
        case sub: return "sub";
        case subi: return "subi";
        case mult: return "mult";
        case multi: return "multi";
        case divisao: return "div";
        case beq: return "beq";
        case bge: return "bge";
        case bgt: return "bgt";
        case ble: return "ble";
        case blt: return "blt";
        case bne: return "bne";
        case j: return "j";
        case in: return "in";
        case out: return "out";
        default: return "unk";
    }
}

static int geraInstrucaoDireta(QuadruplaOp op) {
    switch (op) {
        case Q_NOP:
        case Q_HALT:
        case Q_ADD:
        case Q_SUB:
        case Q_MULT:
        case Q_DIV:
        case Q_BEQ:
        case Q_BGE:
        case Q_BGT:
        case Q_BLE:
        case Q_BLT:
        case Q_BNE:
        case Q_JUMP:
        case Q_IN:
        case Q_INPUT:
        case Q_OUT:
        case Q_OUTPUT:
            return 1;
        default:
            return 0;
    }
}

static int operandoValido(const char* operando) {
    return operando != NULL && strcmp(operando, "___") != 0 && strcmp(operando, "_") != 0;
}

static void emitirComentarioMemoria(const quadrupla* quad) {
    printf("# TODO memoria: (%s, %s, %s, %s)\n",
           operandoValido(quad->opr) ? quad->opr : "___",
           operandoValido(quad->op1) ? quad->op1 : "___",
           operandoValido(quad->op2) ? quad->op2 : "___",
           operandoValido(quad->op3) ? quad->op3 : "___");
}

static AssemblyOp branchOuSaltoAsm(QuadruplaOp op) {
    switch (op) {
        case Q_BEQ: return beq;
        case Q_BGE: return bge;
        case Q_BGT: return bgt;
        case Q_BLE: return ble;
        case Q_BLT: return blt;
        case Q_BNE: return bne;
        case Q_JUMP: return j;
        default: return invalido;
    }
}

static int traduzirQuadruplaDireta(const quadrupla* quad) {
    QuadruplaOp op;
    const char* rd;
    const char* rs;
    const char* rt;

    if (quad == NULL || quad->opr == NULL)
        return 0;

    op = operadorQuadrupla(quad->opr);

    switch (op) {
        case Q_NOP:
            printf("%s\n", nomeInstrucaoAssembly(nop));
            return 1;

        case Q_HALT:
            printf("%s\n", nomeInstrucaoAssembly(hlt));
            return 1;

        case Q_ADD:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            if (ehNumero(quad->op2))
                printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(addi), rd, rs, quad->op2);
            else
                printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(add), rd, rs, mapearParaRegistradorGeral(quad->op2));
            return 1;

        case Q_SUB:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            if (ehNumero(quad->op2))
                printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(subi), rd, rs, quad->op2);
            else
                printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(sub), rd, rs, mapearParaRegistradorGeral(quad->op2));
            return 1;

        case Q_MULT:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            if (ehNumero(quad->op2))
                printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(multi), rd, rs, quad->op2);
            else
                printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(mult), rd, rs, mapearParaRegistradorGeral(quad->op2));
            return 1;

        case Q_DIV:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            rt = mapearParaRegistradorGeral(quad->op2);
            printf("%s %s, %s, %s\n", nomeInstrucaoAssembly(divisao), rd, rs, rt);
            return 1;

        case Q_BEQ:
        case Q_BGE:
        case Q_BGT:
        case Q_BLE:
        case Q_BLT:
        case Q_BNE: {
            AssemblyOp asmOp = branchOuSaltoAsm(op);
                    int linhaDestino = buscarLinhaLabel(quad->op3);
                    if (linhaDestino >= 0) linhaDestino += labelOffset;
            rs = mapearParaRegistradorGeral(quad->op1);
            rt = mapearParaRegistradorGeral(quad->op2);
            printf("%s %s, %s, %d\n", nomeInstrucaoAssembly(asmOp), rs, rt, linhaDestino);
            return 1;
        }

        case Q_JUMP: {
            int linhaDestino = buscarLinhaLabel(quad->op1);
            if (linhaDestino >= 0) {
                linhaDestino += labelOffset;
                printf("%s %d\n", nomeInstrucaoAssembly(j), linhaDestino);
            } else {
                printf("# label %s nao encontrada; pulso de salto omitido\n", quad->op1 ? quad->op1 : "(null)");
            }
            return 1;
        }

        default:
            break;
    }

    printf("(%s, %s, %s, %s)\n",
           operandoValido(quad->opr) ? quad->opr : "___",
           operandoValido(quad->op1) ? quad->op1 : "___",
           operandoValido(quad->op2) ? quad->op2 : "___",
           operandoValido(quad->op3) ? quad->op3 : "___");
    return 0;
}

static void mapearLabelsParaLinhas(const quadList* listaQuadruplas) {
    const quadList* atual;
    int linhaAtual = 1;

    liberarLabels();

    for (atual = listaQuadruplas; atual != NULL; atual = atual->prox) {
        QuadruplaOp op = operadorQuadrupla(atual->quad.opr);

        if (op == Q_LABEL) {
            adicionarLabel(atual->quad.op1, linhaAtual);
        } else if (op == Q_FUNC) {
            if (atual->quad.op2 != NULL)
                adicionarLabel(atual->quad.op2, linhaAtual);
        }

        /* conta uma linha de saída para esta quádrupla */
        linhaAtual++;
    }
}

void traduzirQuadruplasParaAssembly(const quadList* listaQuadruplas) {
    const quadList* atual;
    int totalInstrucoes = 0;

    liberarMapaRegistradores();
    mapearLabelsParaLinhas(listaQuadruplas);

    printf("\n*** CODIGO ASSEMBLY ***\n\n");
    /* Emite instruções iniciais: NOP e salto para a função main */
    /* Ajustamos labelOffset para contar essas duas instruções que aparecem antes do codigo gerado */
    labelOffset = 2; /* nop + j */
    printf("%s\n", nomeInstrucaoAssembly(nop));
    int linhaMain = buscarLinhaLabel("main");
    if (linhaMain >= 0) {
        printf("%s %d\n", nomeInstrucaoAssembly(j), linhaMain + labelOffset);
    } else {
        printf("# main nao encontrada; salto inicial omitido\n");
    }

    for (atual = listaQuadruplas; atual != NULL; atual = atual->prox) {
        totalInstrucoes += traduzirQuadruplaDireta(&atual->quad);
    }

    printf("\nTotal de instrucoes diretas: %d\n", totalInstrucoes);
    printf("\n************************\n\n");

    liberarMapaRegistradores();
    liberarLabels();
}
