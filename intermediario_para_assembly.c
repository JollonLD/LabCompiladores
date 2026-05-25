#include "intermediario_para_assembly.h"

#include <stdlib.h>
#include <string.h>

typedef struct MapaRegistrador {
    char* nome;
    RegistradorAsm reg;
    struct MapaRegistrador* prox;
} MapaRegistrador;

typedef struct MapaLabel {
    char* nome;
    int endereco;
    struct MapaLabel* prox;
} MapaLabel;

static InstrucaoAssembly* listaAssembly = NULL;
static InstrucaoAssembly* ultimaAssembly = NULL;
static MapaRegistrador* mapaRegistradores = NULL;
static MapaLabel* mapaLabels = NULL;
static int totalRegistradoresSimbolicos = 0;
static RegistradorAsm ultimoParametro = REG_INVALIDO;

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

static void liberarMapaRegistradores(void) {
    MapaRegistrador* atual = mapaRegistradores;

    while (atual != NULL) {
        MapaRegistrador* proximo = atual->prox;
        free(atual->nome);
        free(atual);
        atual = proximo;
    }

    mapaRegistradores = NULL;
    totalRegistradoresSimbolicos = 0;
}

static void liberarMapaLabels(void) {
    MapaLabel* atual = mapaLabels;

    while (atual != NULL) {
        MapaLabel* proximo = atual->prox;
        free(atual->nome);
        free(atual);
        atual = proximo;
    }

    mapaLabels = NULL;
}

static void liberarListaAssembly(void) {
    InstrucaoAssembly* atual = listaAssembly;

    while (atual != NULL) {
        InstrucaoAssembly* proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    listaAssembly = NULL;
    ultimaAssembly = NULL;
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

static int numeroAposPrefixo(const char* texto, char prefixo) {
    const char* resto;

    if (texto == NULL || texto[0] != prefixo)
        return -1;

    resto = texto + 1;
    if (!ehNumero(resto))
        return -1;

    return atoi(resto);
}

static RegistradorAsm registradorTemporal(const char* nome) {
    int indice;

    indice = numeroAposPrefixo(nome, 't');
    if (indice < 0 || indice > 25)
        return REG_INVALIDO;

    return (RegistradorAsm)(REG_T0 + indice);
}

static const char* nomeRegistrador(RegistradorAsm reg) {
    static const char* nomes[] = {
        "ZERO", "RF",
        "T0", "T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8", "T9",
        "T10", "T11", "T12", "T13", "T14", "T15", "T16", "T17", "T18", "T19",
        "T20", "T21", "T22", "T23", "T24", "T25",
        "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9",
        "S10", "S11", "S12", "S13", "S14", "S15", "S16", "S17", "S18", "S19",
        "S20", "S21", "S22", "S23", "S24", "S25",
        "INV"
    };

    if (reg < REG_ZERO || reg > REG_INVALIDO)
        return "INV";

    return nomes[reg];
}

static RegistradorAsm registrarSimbolo(const char* nome) {
    MapaRegistrador* atual;
    MapaRegistrador* novo;

    if (nome == NULL || *nome == '\0')
        return REG_INVALIDO;

    for (atual = mapaRegistradores; atual != NULL; atual = atual->prox) {
        if (strcmp(atual->nome, nome) == 0)
            return atual->reg;
    }

    if (totalRegistradoresSimbolicos >= 26)
        return REG_INVALIDO;

    novo = (MapaRegistrador*)malloc(sizeof(MapaRegistrador));
    if (novo == NULL)
        return REG_INVALIDO;

    novo->nome = duplicarTexto(nome);
    if (novo->nome == NULL) {
        free(novo);
        return REG_INVALIDO;
    }

    novo->reg = (RegistradorAsm)(REG_S0 + totalRegistradoresSimbolicos++);
    novo->prox = mapaRegistradores;
    mapaRegistradores = novo;

    return novo->reg;
}

static RegistradorAsm registradorOperando(const char* operando) {
    if (operando == NULL || strcmp(operando, "___") == 0 || strcmp(operando, "_") == 0)
        return REG_INVALIDO;

    if (strcmp(operando, "$rf") == 0)
        return REG_RF;

    if (operando[0] == 't')
        return registradorTemporal(operando);

    return registrarSimbolo(operando);
}

static int valorInteiro(const char* texto) {
    if (texto == NULL || *texto == '\0')
        return 0;
    return atoi(texto);
}

static void adicionarLabel(const char* nome, int endereco) {
    MapaLabel* novo;

    if (nome == NULL)
        return;

    novo = (MapaLabel*)malloc(sizeof(MapaLabel));
    if (novo == NULL)
        return;

    novo->nome = duplicarTexto(nome);
    if (novo->nome == NULL) {
        free(novo);
        return;
    }

    novo->endereco = endereco;
    novo->prox = mapaLabels;
    mapaLabels = novo;
}

static int enderecoLabel(const char* nome) {
    MapaLabel* atual;

    for (atual = mapaLabels; atual != NULL; atual = atual->prox) {
        if (strcmp(atual->nome, nome) == 0)
            return atual->endereco;
    }

    return -1;
}

static InstrucaoAssembly* criarInstrucao(InstrucaoAsm instrucao,
                                         RegistradorAsm rd,
                                         RegistradorAsm rs,
                                         RegistradorAsm rt,
                                         int imediato,
                                         int endereco,
                                         int formato) {
    InstrucaoAssembly* novo;

    novo = (InstrucaoAssembly*)malloc(sizeof(InstrucaoAssembly));
    if (novo == NULL)
        return NULL;

    novo->instrucao = instrucao;
    novo->rd = rd;
    novo->rs = rs;
    novo->rt = rt;
    novo->imediato = imediato;
    novo->endereco = endereco;
    novo->formato = formato;
    novo->prox = NULL;

    return novo;
}

static void anexarInstrucao(InstrucaoAssembly* instrucao) {
    if (instrucao == NULL)
        return;

    if (listaAssembly == NULL) {
        listaAssembly = instrucao;
        ultimaAssembly = instrucao;
    } else {
        ultimaAssembly->prox = instrucao;
        ultimaAssembly = instrucao;
    }
}

static const char* nomeInstrucao(InstrucaoAsm instrucao) {
    switch (instrucao) {
        case ASM_ADD: return "add";
        case ASM_ADDI: return "addi";
        case ASM_SUB: return "sub";
        case ASM_SUBI: return "subi";
        case ASM_MULT: return "mult";
        case ASM_MULTI: return "multi";
        case ASM_DIV: return "div";
        case ASM_BEQ: return "beq";
        case ASM_BGT: return "bgt";
        case ASM_BLT: return "blt";
        case ASM_BGE: return "bge";
        case ASM_BLE: return "ble";
        case ASM_BNE: return "bne";
        case ASM_NOP: return "nop";
        case ASM_HLT: return "hlt";
        case ASM_IN: return "in";
        case ASM_OUT: return "out";
        case ASM_J: return "j";
        case ASM_MOV: return "mov";
        case ASM_LOAD: return "load";
        case ASM_STORE: return "store";
        case ASM_PARAM: return "param";
        case ASM_CALL: return "call";
        case ASM_RETURN: return "return";
        case ASM_FUNC: return "func";
        case ASM_ENDFUNC: return "endfunc";
        case ASM_ARG: return "arg";
        case ASM_ALLOCVAR: return "allocavar";
        case ASM_ALLOCVET: return "allocavet";
        default: return "unk";
    }
}

static int ehQuadrupla(const char* opr, const char* esperado) {
    return opr != NULL && strcmp(opr, esperado) == 0;
}

static int contarInstrucao(const quadrupla* quad) {
    if (quad == NULL || quad->opr == NULL)
        return 0;

    if (ehQuadrupla(quad->opr, "LABEL"))
        return 0;

    return 1;
}

static void imprimirInstrucao(const InstrucaoAssembly* instrucao) {
    if (instrucao == NULL)
        return;

    switch ((FormatoAsm)instrucao->formato) {
        case FORMATO_OPR:
            printf("%s %s, %s, %s\n",
                   nomeInstrucao(instrucao->instrucao),
                   nomeRegistrador(instrucao->rd),
                   nomeRegistrador(instrucao->rs),
                   nomeRegistrador(instrucao->rt));
            break;

        case FORMATO_OPI:
            printf("%s %s, %s, %d\n",
                   nomeInstrucao(instrucao->instrucao),
                   nomeRegistrador(instrucao->rd),
                   nomeRegistrador(instrucao->rs),
                   instrucao->imediato);
            break;

        case FORMATO_B:
            printf("%s %s, %s, %d\n",
                   nomeInstrucao(instrucao->instrucao),
                   nomeRegistrador(instrucao->rs),
                   nomeRegistrador(instrucao->rt),
                   instrucao->endereco);
            break;

        case FORMATO_J:
            printf("%s %d\n",
                   nomeInstrucao(instrucao->instrucao),
                   instrucao->endereco);
            break;

        case FORMATO_M:
            printf("%s %s\n",
                   nomeInstrucao(instrucao->instrucao),
                   nomeRegistrador(instrucao->rd != REG_INVALIDO ? instrucao->rd : instrucao->rs));
            break;

        case FORMATO_C:
        default:
            switch (instrucao->instrucao) {
                case ASM_CALL:
                    printf("call %s, %d\n",
                           nomeRegistrador(instrucao->rd),
                           instrucao->imediato);
                    break;
                case ASM_FUNC:
                    printf("func %s\n", nomeRegistrador(instrucao->rd));
                    break;
                case ASM_ENDFUNC:
                    printf("endfunc\n");
                    break;
                case ASM_PARAM:
                    printf("param %s\n", nomeRegistrador(instrucao->rd));
                    break;
                case ASM_ARG:
                    printf("arg %s\n", nomeRegistrador(instrucao->rd));
                    break;
                case ASM_ALLOCVAR:
                    printf("allocavar %s\n", nomeRegistrador(instrucao->rd));
                    break;
                case ASM_ALLOCVET:
                    printf("allocavet %s, %d\n", nomeRegistrador(instrucao->rd), instrucao->imediato);
                    break;
                case ASM_RETURN:
                    if (instrucao->rs != REG_INVALIDO)
                        printf("return %s\n", nomeRegistrador(instrucao->rs));
                    else
                        printf("return\n");
                    break;
                default:
                    printf("%s\n", nomeInstrucao(instrucao->instrucao));
                    break;
            }
            break;
    }
}

static void imprimirListaAssembly(void) {
    InstrucaoAssembly* atual = listaAssembly;

    while (atual != NULL) {
        imprimirInstrucao(atual);
        atual = atual->prox;
    }
}

static void traduzirQuadrupla(const quadrupla* quad) {
    RegistradorAsm rd;
    RegistradorAsm rs;
    RegistradorAsm rt;
    int endereco;
    int imediato;

    if (quad == NULL || quad->opr == NULL)
        return;

    if (ehQuadrupla(quad->opr, "LABEL"))
        return;

    rd = REG_INVALIDO;
    rs = REG_INVALIDO;
    rt = REG_INVALIDO;
    imediato = 0;
    endereco = -1;

    if (ehQuadrupla(quad->opr, "NOP")) {
        anexarInstrucao(criarInstrucao(ASM_NOP, REG_INVALIDO, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "HALT")) {
        anexarInstrucao(criarInstrucao(ASM_HLT, REG_INVALIDO, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "LOADCONST")) {
        rd = registradorOperando(quad->op1);
        imediato = valorInteiro(quad->op2);
        anexarInstrucao(criarInstrucao(ASM_ADDI, rd, REG_ZERO, REG_INVALIDO, imediato, -1, FORMATO_OPI));
        return;
    }

    if (ehQuadrupla(quad->opr, "LOADVAR")) {
        rd = registradorOperando(quad->op3);
        rs = registrarSimbolo(quad->op2);
        anexarInstrucao(criarInstrucao(ASM_MOV, rd, rs, REG_INVALIDO, 0, -1, FORMATO_M));
        return;
    }

    if (ehQuadrupla(quad->opr, "LOADVET")) {
        rd = registradorOperando(quad->op3);
        rs = registrarSimbolo(quad->op1);
        rt = registradorOperando(quad->op2);
        anexarInstrucao(criarInstrucao(ASM_LOAD, rd, rs, rt, 0, -1, FORMATO_OPR));
        return;
    }

    if (ehQuadrupla(quad->opr, "STOREVAR")) {
        rd = registrarSimbolo(quad->op2);
        rs = registradorOperando(quad->op1);
        anexarInstrucao(criarInstrucao(ASM_MOV, rd, rs, REG_INVALIDO, 0, -1, FORMATO_M));
        return;
    }

    if (ehQuadrupla(quad->opr, "STOREVET")) {
        rd = registradorOperando(quad->op1);
        rs = registradorOperando(quad->op2);
        rt = registrarSimbolo(quad->op3);
        anexarInstrucao(criarInstrucao(ASM_STORE, rd, rs, rt, 0, -1, FORMATO_OPR));
        return;
    }

    if (ehQuadrupla(quad->opr, "ADD") || ehQuadrupla(quad->opr, "SUB") ||
        ehQuadrupla(quad->opr, "MULT") || ehQuadrupla(quad->opr, "DIV")) {
        rd = registradorOperando(quad->op3);
        rs = registradorOperando(quad->op1);
        rt = registradorOperando(quad->op2);

        if (ehQuadrupla(quad->opr, "ADD")) {
            if (ehNumero(quad->op2)) {
                imediato = valorInteiro(quad->op2);
                anexarInstrucao(criarInstrucao(ASM_ADDI, rd, rs, REG_INVALIDO, imediato, -1, FORMATO_OPI));
            } else {
                anexarInstrucao(criarInstrucao(ASM_ADD, rd, rs, rt, 0, -1, FORMATO_OPR));
            }
            return;
        }

        if (ehQuadrupla(quad->opr, "SUB")) {
            if (ehNumero(quad->op2)) {
                imediato = valorInteiro(quad->op2);
                anexarInstrucao(criarInstrucao(ASM_SUBI, rd, rs, REG_INVALIDO, imediato, -1, FORMATO_OPI));
            } else {
                anexarInstrucao(criarInstrucao(ASM_SUB, rd, rs, rt, 0, -1, FORMATO_OPR));
            }
            return;
        }

        if (ehQuadrupla(quad->opr, "MULT")) {
            if (ehNumero(quad->op2)) {
                imediato = valorInteiro(quad->op2);
                anexarInstrucao(criarInstrucao(ASM_MULTI, rd, rs, REG_INVALIDO, imediato, -1, FORMATO_OPI));
            } else {
                anexarInstrucao(criarInstrucao(ASM_MULT, rd, rs, rt, 0, -1, FORMATO_OPR));
            }
            return;
        }

        anexarInstrucao(criarInstrucao(ASM_DIV, rd, rs, rt, 0, -1, FORMATO_OPR));
        return;
    }

    if (ehQuadrupla(quad->opr, "BGE") || ehQuadrupla(quad->opr, "BGT") ||
        ehQuadrupla(quad->opr, "BLT") || ehQuadrupla(quad->opr, "BLE") ||
        ehQuadrupla(quad->opr, "BEQ") || ehQuadrupla(quad->opr, "BNE")) {
        rs = registradorOperando(quad->op1);
        rt = registradorOperando(quad->op2);
        endereco = enderecoLabel(quad->op3);

        if (ehQuadrupla(quad->opr, "BGE"))
            anexarInstrucao(criarInstrucao(ASM_BGE, REG_INVALIDO, rs, rt, 0, endereco, FORMATO_B));
        else if (ehQuadrupla(quad->opr, "BGT"))
            anexarInstrucao(criarInstrucao(ASM_BGT, REG_INVALIDO, rs, rt, 0, endereco, FORMATO_B));
        else if (ehQuadrupla(quad->opr, "BLT"))
            anexarInstrucao(criarInstrucao(ASM_BLT, REG_INVALIDO, rs, rt, 0, endereco, FORMATO_B));
        else if (ehQuadrupla(quad->opr, "BLE"))
            anexarInstrucao(criarInstrucao(ASM_BLE, REG_INVALIDO, rs, rt, 0, endereco, FORMATO_B));
        else if (ehQuadrupla(quad->opr, "BEQ"))
            anexarInstrucao(criarInstrucao(ASM_BEQ, REG_INVALIDO, rs, rt, 0, endereco, FORMATO_B));
        else
            anexarInstrucao(criarInstrucao(ASM_BNE, REG_INVALIDO, rs, rt, 0, endereco, FORMATO_B));
        return;
    }

    if (ehQuadrupla(quad->opr, "JUMP")) {
        endereco = enderecoLabel(quad->op1);
        anexarInstrucao(criarInstrucao(ASM_J, REG_INVALIDO, REG_INVALIDO, REG_INVALIDO, 0, endereco, FORMATO_J));
        return;
    }

    if (ehQuadrupla(quad->opr, "FUNC")) {
        rd = registrarSimbolo(quad->op2);
        anexarInstrucao(criarInstrucao(ASM_FUNC, rd, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "ENDFUNC")) {
        rd = registrarSimbolo(quad->op1);
        anexarInstrucao(criarInstrucao(ASM_ENDFUNC, rd, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "ARG")) {
        rd = registrarSimbolo(quad->op1);
        anexarInstrucao(criarInstrucao(ASM_ARG, rd, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "ALLOCAMEMVAR")) {
        rd = registrarSimbolo(quad->op2);
        anexarInstrucao(criarInstrucao(ASM_ALLOCVAR, rd, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "ALLOCAMEMVET")) {
        rd = registrarSimbolo(quad->op2);
        imediato = valorInteiro(quad->op3);
        anexarInstrucao(criarInstrucao(ASM_ALLOCVET, rd, REG_INVALIDO, REG_INVALIDO, imediato, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "PARAM")) {
        rd = registradorOperando(quad->op1);
        ultimoParametro = rd;
        anexarInstrucao(criarInstrucao(ASM_PARAM, rd, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    if (ehQuadrupla(quad->opr, "CALL")) {
        if (quad->op1 != NULL && strcmp(quad->op1, "input") == 0) {
            anexarInstrucao(criarInstrucao(ASM_IN, REG_RF, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_M));
            ultimoParametro = REG_INVALIDO;
            return;
        }

        if (quad->op1 != NULL && strcmp(quad->op1, "output") == 0) {
            rd = (ultimoParametro != REG_INVALIDO) ? ultimoParametro : registradorOperando(quad->op1);
            anexarInstrucao(criarInstrucao(ASM_OUT, rd, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_M));
            ultimoParametro = REG_INVALIDO;
            return;
        }

        rd = registrarSimbolo(quad->op1);
        imediato = valorInteiro(quad->op2);
        anexarInstrucao(criarInstrucao(ASM_CALL, rd, REG_INVALIDO, REG_INVALIDO, imediato, -1, FORMATO_C));
        ultimoParametro = REG_INVALIDO;
        return;
    }

    if (ehQuadrupla(quad->opr, "RETURN")) {
        rd = registradorOperando(quad->op1);
        anexarInstrucao(criarInstrucao(ASM_RETURN, REG_INVALIDO, rd, REG_INVALIDO, 0, -1, FORMATO_C));
        return;
    }

    anexarInstrucao(criarInstrucao(ASM_NOP, REG_INVALIDO, REG_INVALIDO, REG_INVALIDO, 0, -1, FORMATO_C));
}

static int contarInstrucaoTotal(const quadList* listaQuadruplas) {
    int total = 0;

    while (listaQuadruplas != NULL) {
        total += contarInstrucao(&listaQuadruplas->quad);
        listaQuadruplas = listaQuadruplas->prox;
    }

    return total;
}

void traduzirQuadruplasParaAssembly(const quadList* listaQuadruplas) {
    const quadList* atual;
    int enderecoAtual;

    liberarListaAssembly();
    liberarMapaRegistradores();
    liberarMapaLabels();
    ultimoParametro = REG_INVALIDO;

    enderecoAtual = 0;
    for (atual = listaQuadruplas; atual != NULL; atual = atual->prox) {
        if (atual->quad.opr != NULL && strcmp(atual->quad.opr, "LABEL") == 0) {
            adicionarLabel(atual->quad.op1, enderecoAtual);
        } else {
            enderecoAtual += 1;
        }
    }

    for (atual = listaQuadruplas; atual != NULL; atual = atual->prox) {
        traduzirQuadrupla(&atual->quad);
    }

    printf("\n*** CODIGO ASSEMBLY ***\n\n");
    imprimirListaAssembly();
    printf("\nTotal de instrucoes: %d\n", contarInstrucaoTotal(listaQuadruplas));
    printf("\n************************\n\n");

    liberarListaAssembly();
    liberarMapaRegistradores();
    liberarMapaLabels();
}