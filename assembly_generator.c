#include "assembly_generator.h"
#include "binary_generator.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define ARQUIVO_SAIDA_ASM "saida_asm.txt"

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

typedef struct VariavelEscopo {
    char* nome;
    int deslocamento;
    struct VariavelEscopo* prox;
} VariavelEscopo;

typedef struct EscopoVariavel {
    char* nomeEscopo;
    VariavelEscopo* variaveis;
    int proximoDeslocamento;
    struct EscopoVariavel* prox;
} EscopoVariavel;

static LabelLinha* listaLabels = NULL;
static MapaRegistrador* mapaRegistradores = NULL;
static EscopoVariavel* listaEscoposVariaveis = NULL;
static int proximoRegistradorGeral = 4;
static char escopoTraducaoAtual[128] = "global";
static FILE* arquivoSaidaAssembly = NULL;

static void asmPrint(const char* formato, ...) {
    va_list args;

    va_start(args, formato);
    if (arquivoSaidaAssembly != NULL)
        vfprintf(arquivoSaidaAssembly, formato, args);
    va_end(args);
}

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

static void liberarMapaEscoposVariaveis(void) {
    EscopoVariavel* escopoAtual = listaEscoposVariaveis;

    while (escopoAtual != NULL) {
        EscopoVariavel* proxEscopo = escopoAtual->prox;
        VariavelEscopo* variavelAtual = escopoAtual->variaveis;

        while (variavelAtual != NULL) {
            VariavelEscopo* proxVariavel = variavelAtual->prox;
            free(variavelAtual->nome);
            free(variavelAtual);
            variavelAtual = proxVariavel;
        }

        free(escopoAtual->nomeEscopo);
        free(escopoAtual);
        escopoAtual = proxEscopo;
    }

    listaEscoposVariaveis = NULL;
}

static int escopoEhGlobal(const char* nomeEscopo) {
    return nomeEscopo == NULL || strcmp(nomeEscopo, "global") == 0;
}

static EscopoVariavel* buscarEscopoVariavel(const char* nomeEscopo) {
    EscopoVariavel* atual;

    for (atual = listaEscoposVariaveis; atual != NULL; atual = atual->prox) {
        if (strcmp(atual->nomeEscopo, nomeEscopo) == 0)
            return atual;
    }

    return NULL;
}

static EscopoVariavel* adicionarEscopoVariavel(const char* nomeEscopo) {
    EscopoVariavel* novo;

    novo = (EscopoVariavel*)malloc(sizeof(EscopoVariavel));
    if (novo == NULL)
        return NULL;

    novo->nomeEscopo = duplicarTexto(nomeEscopo);
    if (novo->nomeEscopo == NULL) {
        free(novo);
        return NULL;
    }

    novo->variaveis = NULL;
    
    /* MUDANÇA: Se não for escopo global, o offset inicia em 2. 
       Isso reserva: $fp+0 para Old FP, e $fp+1 para PC Return Address ($ra) */
    novo->proximoDeslocamento = escopoEhGlobal(nomeEscopo) ? 0 : 2;
    novo->prox = listaEscoposVariaveis;
    listaEscoposVariaveis = novo;

    return novo;
}

static int inserirVariavelNoEscopo(const char* nomeEscopo, const char* nomeVariavel) {
    EscopoVariavel* escopo;
    VariavelEscopo* variavel;
    VariavelEscopo* novaVariavel;

    if (nomeEscopo == NULL || *nomeEscopo == '\0' || nomeVariavel == NULL || *nomeVariavel == '\0')
        return 0;

    escopo = buscarEscopoVariavel(nomeEscopo);
    if (escopo == NULL)
        escopo = adicionarEscopoVariavel(nomeEscopo);
    if (escopo == NULL)
        return 0;

    for (variavel = escopo->variaveis; variavel != NULL; variavel = variavel->prox) {
        if (strcmp(variavel->nome, nomeVariavel) == 0)
            return 1;
    }

    novaVariavel = (VariavelEscopo*)malloc(sizeof(VariavelEscopo));
    if (novaVariavel == NULL)
        return 0;

    novaVariavel->nome = duplicarTexto(nomeVariavel);
    if (novaVariavel->nome == NULL) {
        free(novaVariavel);
        return 0;
    }

    novaVariavel->deslocamento = escopo->proximoDeslocamento++;
    novaVariavel->prox = escopo->variaveis;
    escopo->variaveis = novaVariavel;
    return 1;
}

static const VariavelEscopo* buscarVariavelNoEscopo(const char* nomeEscopo, const char* nomeVariavel) {
    EscopoVariavel* escopo;
    VariavelEscopo* variavel;

    if (nomeVariavel == NULL || *nomeVariavel == '\0')
        return NULL;

    escopo = buscarEscopoVariavel(nomeEscopo);
    if (escopo == NULL && !escopoEhGlobal(nomeEscopo))
        escopo = buscarEscopoVariavel("global");
    if (escopo == NULL)
        return NULL;

    for (variavel = escopo->variaveis; variavel != NULL; variavel = variavel->prox) {
        if (strcmp(variavel->nome, nomeVariavel) == 0)
            return variavel;
    }

    return NULL;
}

static const char* baseParaEscopo(const char* nomeEscopo) {
    if (escopoEhGlobal(nomeEscopo))
        return "$zero";
    return "$fp";
}

static void adicionarLabel(const char* nome, int linha) {
    LabelLinha* novo;

    if (nome == NULL || *nome == '\0')
        return;

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
    if (ehQuadrupla(opr, "STOREVAR")) return Q_STOREVAR;
    if (ehQuadrupla(opr, "LOADVAR")) return Q_LOADVAR;
    if (ehQuadrupla(opr, "LOADCONST")) return Q_LOADCONST;
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
        case move: return "move";
        case beq: return "beq";
        case bge: return "bge";
        case bgt: return "bgt";
        case ble: return "ble";
        case blt: return "blt";
        case bne: return "bne";
        case j: return "j";
        case jr: return "jr";
        case jal: return "jal";
        case lwd: return "lwd";
        case swd: return "swd";
        case push: return "push";
        case pop: return "pop";
        case in: return "in";
        case out: return "out";
        default: return "unk";
    }
}

static int operandoValido(const char* operando) {
    return operando != NULL && strcmp(operando, "___") != 0 && strcmp(operando, "_") != 0;
}

static void emitirComentarioMemoria(const quadrupla* quad) {
    asmPrint("# TODO memoria: (%s, %s, %s, %s)\n",
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

static int traduzirQuadrupla(const quadrupla* quad) {
    QuadruplaOp op;
    const char* rd;
    const char* rs;
    const char* rt;
    const VariavelEscopo* variavel;
    const char* base;

    if (quad == NULL || quad->opr == NULL)
        return 0;

    op = operadorQuadrupla(quad->opr);

    switch (op) {
        case Q_NOP:
            asmPrint("%s\n", nomeInstrucaoAssembly(nop));
            return 1;

        case Q_HALT:
            asmPrint("%s\n", nomeInstrucaoAssembly(hlt));
            return 1;

        case Q_LABEL:
            int linha = buscarLinhaLabel(quad->op1);
            adicionarLabel(quad->op1, linha);

            return 1;

        case Q_ADD:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            if (ehNumero(quad->op2))
                asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(addi), rd, rs, quad->op2);
            else
                asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(add), rd, rs, mapearParaRegistradorGeral(quad->op2));
            return 1;

        case Q_SUB:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            if (ehNumero(quad->op2))
                asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(subi), rd, rs, quad->op2);
            else
                asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(sub), rd, rs, mapearParaRegistradorGeral(quad->op2));
            return 1;

        case Q_MULT:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            if (ehNumero(quad->op2))
                asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(multi), rd, rs, quad->op2);
            else
                asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(mult), rd, rs, mapearParaRegistradorGeral(quad->op2));
            return 1;

        case Q_DIV:
            rd = mapearParaRegistradorGeral(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            rt = mapearParaRegistradorGeral(quad->op2);
            asmPrint("%s %s, %s, %s\n", nomeInstrucaoAssembly(divisao), rd, rs, rt);
            return 1;
        
        case Q_ARG:
            inserirVariavelNoEscopo(quad->op2, quad->op1);
            asmPrint("%s $sp, $sp, 1\n", nomeInstrucaoAssembly(addi));
            return 1;
        
        case Q_LOADCONST:
            rd = mapearParaRegistradorGeral(quad->op1);
            asmPrint("li %s, %d\n", rd, atoi(quad->op2));

            return 1;


        case Q_LOADVAR:
            variavel = buscarVariavelNoEscopo(quad->op1, quad->op2);
            rd = mapearParaRegistradorGeral(quad->op3);
            base = baseParaEscopo(quad->op1);
            asmPrint("lwd %s %s %d\n", rd, base, variavel ? variavel->deslocamento : 0);
            return 1;

        case Q_STOREVAR:
            variavel = buscarVariavelNoEscopo(quad->op3, quad->op2);
            rs = mapearParaRegistradorGeral(quad->op1);
            base = baseParaEscopo(quad->op3);
            asmPrint("swd %s %s %d\n", rs, base, variavel ? variavel->deslocamento : 0);
            return 1;

        case Q_BEQ:
        case Q_BGE:
        case Q_BGT:
        case Q_BLE:
        case Q_BLT:
        case Q_BNE: {
            AssemblyOp asmOp = branchOuSaltoAsm(op);
            int linhaDestino = buscarLinhaLabel(quad->op3);
            rs = mapearParaRegistradorGeral(quad->op1);
            rt = mapearParaRegistradorGeral(quad->op2);
            asmPrint("%s %s, %s, %d\n", nomeInstrucaoAssembly(asmOp), rs, rt, linhaDestino);
            return 1;
        }
        
        case Q_JUMP: {
            int linhaDestino = buscarLinhaLabel(quad->op1);
            if (linhaDestino >= 0) {
                asmPrint("%s %d\n", nomeInstrucaoAssembly(j), linhaDestino);
            } else {
                asmPrint("# label %s nao encontrada; salto omitido\n", quad->op1 ? quad->op1 : "(null)");
            }
            return 1;
        }

        case Q_ALLOCAMEMVAR:
            inserirVariavelNoEscopo(quad->op1, quad->op2);
            if (!escopoEhGlobal(quad->op1))
                asmPrint("%s $sp, $sp, 1\n", nomeInstrucaoAssembly(addi));
            return 1;
            
        case Q_ALLOCAMEMVET: {
            int tamanho = atoi(quad->op3);
            inserirVariavelNoEscopo(quad->op1, quad->op2);
            if (!escopoEhGlobal(quad->op1)) {
                EscopoVariavel* esc = buscarEscopoVariavel(quad->op1);
                if (esc) esc->proximoDeslocamento += (tamanho - 1);
                asmPrint("%s $sp, $sp, %d\n", nomeInstrucaoAssembly(addi), tamanho);
            } else {
                EscopoVariavel* esc = buscarEscopoVariavel("global");
                if (esc) esc->proximoDeslocamento += (tamanho - 1);
            }
            return 1;
        }

        case Q_LOADVET: {
            asmPrint("# TODO: Instrucao LOADVET\n"); 
            return 1;
        }

        case Q_STOREVET: {
            asmPrint("# TODO: Instrucao STOREVET\n"); 
            return 1;
        }
        
        case Q_PARAM:
            // O chamador empilha o argumento
            rs = mapearParaRegistradorGeral(quad->op1);
            asmPrint("%s $sp, $sp, 1\n", nomeInstrucaoAssembly(addi));
            asmPrint("%s %s, $sp, 0\n", nomeInstrucaoAssembly(swd), rs);
            
            return 1;

        case Q_CALL:
            if (strcmp(quad->op1, "input") == 0) {
                asmPrint("%s $rf\n", nomeInstrucaoAssembly(in));
            }
            else if (strcmp(quad->op1, "output") == 0) {
                asmPrint("%s $ro\n", nomeInstrucaoAssembly(pop));
                asmPrint("%s $ro\n", nomeInstrucaoAssembly(out));
            }
            else {
                int nParams = quad->op2 ? atoi(quad->op2) : 0;
                int linhaFunc = buscarLinhaLabel(quad->op1);
                
                // 1. Reserva slot acima dos argumentos empilhados por PARAM
                asmPrint("%s $sp, $sp, 1\n", nomeInstrucaoAssembly(addi));
                
                // 2. Salva o $fp atual do chamador na base do frame
                asmPrint("%s $fp, $sp, 0\n", nomeInstrucaoAssembly(swd));
                
                // 3. Transfere a base do novo frame para o $fp
                asmPrint("%s $fp, $sp\n", nomeInstrucaoAssembly(move));
                
                // 4. Reposiciona $sp no topo dos argumentos empilhados
                asmPrint("%s $sp, $sp, 1\n", nomeInstrucaoAssembly(subi));
                
                // 5. Pega os parâmetros do Q_PARAM e salva no novo frame
                for (int i = 0; i < nParams; i++) {
                    asmPrint("%s $rf, $sp, 0\n", nomeInstrucaoAssembly(lwd));
                    asmPrint("%s $sp, $sp, 1\n", nomeInstrucaoAssembly(subi));

                    // Os argumentos iniciam no offset 2 em diante
                    asmPrint("%s $rf, $fp, %d\n", nomeInstrucaoAssembly(swd), (nParams - i + 1));
                }
                
                // 6. Chamada de função
                if (linhaFunc >= 0) {
                    asmPrint("%s %d\n", nomeInstrucaoAssembly(jal), linhaFunc);
                } else {
                    asmPrint("# label %s nao encontrada; chamada omitida\n", quad->op1);
                }
                
                // 7. Restaura a pilha apagando as variáveis locais e argumentos (Frame Teardown)
                asmPrint("%s $sp, $fp\n", nomeInstrucaoAssembly(move));
                
                // 8. Restaura o Frame Pointer original do chamador
                asmPrint("%s $fp, $fp, 0\n", nomeInstrucaoAssembly(lwd));

                // 9. Reposiciona $sp acima das variaveis locais do chamador
                {
                    EscopoVariavel* escopoChamador = buscarEscopoVariavel(escopoTraducaoAtual);
                    if (escopoChamador != NULL)
                        asmPrint("%s $sp, $fp, %d\n", nomeInstrucaoAssembly(addi), escopoChamador->proximoDeslocamento);
                }
            }
            return 1;

        case Q_FUNC:
            if (quad->op2 != NULL)
                strncpy(escopoTraducaoAtual, quad->op2, sizeof(escopoTraducaoAtual) - 1);
            escopoTraducaoAtual[sizeof(escopoTraducaoAtual) - 1] = '\0';
            // Salva $ra e posiciona $sp logo apos o cabecalho do frame ($fp+0 e $fp+1)
            asmPrint("%s $ra, $fp, 1\n", nomeInstrucaoAssembly(swd));
            asmPrint("%s $sp, $fp, 2\n", nomeInstrucaoAssembly(addi));
            return 1;
            
        case Q_RETURN:
            // Verifica se a função possui retorno válido
            if (operandoValido(quad->op1)) {
                rs = mapearParaRegistradorGeral(quad->op1);
                asmPrint("%s $rf, %s\n", nomeInstrucaoAssembly(move), rs);
            }
            
            return 1;
        
        case Q_ENDFUNC:
            strcpy(escopoTraducaoAtual, "global");
            // Restaura o Return Address ($ra) e volta pro chamador caso atinja fim da função sem return
            asmPrint("%s $ra, $fp, 1\n", nomeInstrucaoAssembly(lwd));
            asmPrint("%s $ra\n", nomeInstrucaoAssembly(jr));
            return 1;


        default:
            break;
    }

    asmPrint("(%s, %s, %s, %s)\n",
           operandoValido(quad->opr) ? quad->opr : "___",
           operandoValido(quad->op1) ? quad->op1 : "___",
           operandoValido(quad->op2) ? quad->op2 : "___",
           operandoValido(quad->op3) ? quad->op3 : "___");
    return 0;
}

static int contarInstrucoesAssembly(const quadrupla* quad) {
    QuadruplaOp op = operadorQuadrupla(quad->opr);

    switch (op) {
        case Q_NOP:
        case Q_HALT:
        case Q_ADD:
        case Q_SUB:
        case Q_MULT:
        case Q_DIV:
        case Q_ARG:
        case Q_LOADCONST:
        case Q_LOADVAR:
        case Q_STOREVAR:
        case Q_BEQ:
        case Q_BGE:
        case Q_BGT:
        case Q_BLE:
        case Q_BLT:
        case Q_BNE:
        case Q_JUMP:
        case Q_LOADVET:
        case Q_STOREVET:
            return 1;

        case Q_PARAM:
            return 2; // addi $sp + swd argumento

        case Q_FUNC:
            return 2; // swd $ra + addi $sp, $fp, 2

        case Q_ENDFUNC:
            return 2; // lwd $ra + jr

        case Q_RETURN:
            return operandoValido(quad->op1) ? 1 : 0; // move $rf (se houver)

        case Q_ALLOCAMEMVAR:
            return escopoEhGlobal(quad->op1) ? 0 : 1;

        case Q_ALLOCAMEMVET:
            return escopoEhGlobal(quad->op1) ? 0 : 1;

        case Q_CALL:
            if (strcmp(quad->op1, "input") == 0)
                return 1;
            if (strcmp(quad->op1, "output") == 0)
                return 2;

            {
                int nParams = quad->op2 ? atoi(quad->op2) : 0;
                /* addi + swd + move + subi + (lwd + subi + swd) * n + jal + move + lwd + addi */
                return 8 + (nParams * 3);
            }

        case Q_LABEL:
            return 0;

        default:
            return 0;
    }
}

static void mapearLabelsParaLinhas(const quadList* listaQuadruplas) {
    const quadList* atual;
    
    /* Inicia em 2 porque as instruções 'nop' e 'j main' vão ocupar as linhas 0 e 1 */
    int linhaAtual = 2; 

    liberarLabels();

    for (atual = listaQuadruplas; atual != NULL; atual = atual->prox) {
        QuadruplaOp op = operadorQuadrupla(atual->quad.opr);

        if (op == Q_LABEL) {
            adicionarLabel(atual->quad.op1, linhaAtual);
        } else if (op == Q_FUNC) {
            if (atual->quad.op2 != NULL)
                adicionarLabel(atual->quad.op2, linhaAtual);
        }

        /* Incrementa de acordo com as instruções assembly REAIS que serão geradas */
        linhaAtual += contarInstrucoesAssembly(&atual->quad);
    }
}


void traduzirQuadruplasParaAssembly(const quadList* listaQuadruplas) {
    const quadList* atual;
    int linhaMain;

    liberarMapaRegistradores();
    liberarMapaEscoposVariaveis();
    strcpy(escopoTraducaoAtual, "global");
    mapearLabelsParaLinhas(listaQuadruplas);

    arquivoSaidaAssembly = fopen(ARQUIVO_SAIDA_ASM, "w");
    if (arquivoSaidaAssembly == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel criar %s\n", ARQUIVO_SAIDA_ASM);
        return;
    }

    asmPrint("%s\n", nomeInstrucaoAssembly(nop));
    linhaMain = buscarLinhaLabel("main");
    if (linhaMain >= 0)
        asmPrint("%s %d\n", nomeInstrucaoAssembly(j), linhaMain);
    else
        asmPrint("# main nao encontrada; salto inicial omitido\n");

    for (atual = listaQuadruplas; atual != NULL; atual = atual->prox)
        traduzirQuadrupla(&atual->quad);

    fclose(arquivoSaidaAssembly);
    arquivoSaidaAssembly = NULL;

    liberarMapaRegistradores();
    liberarMapaEscoposVariaveis();
    liberarLabels();

    printf("\n*** CODIGO ASSEMBLY ***\n");
    printf("Arquivo gerado: %s\n", ARQUIVO_SAIDA_ASM);

    if (traduzirArquivoAssemblyParaBinario(ARQUIVO_SAIDA_ASM, "saida_bin.txt"))
        printf("Arquivo binario gerado: saida_bin.txt\n");
    else
        fprintf(stderr, "ERRO: falha ao gerar saida_bin.txt\n");
}
