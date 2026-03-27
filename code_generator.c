#include "code_generator.h"
#include "cminus.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Contadores globais para temporários e labels */
static int contadorTemporarios = 0;
static int contadorLabels = 0;

/* Nome da Função Atual para salvar contexto */
static const char* funcaoAtual = NULL;


static char* gerarExpressao(TreeNode* no);
static void gerarComando(TreeNode* no);
static void gerarComandoExpressao(TreeNode* no);

static char* novoTemporario(void) {
    char* nomeTemp = (char*)malloc(10 * sizeof(char));
    sprintf(nomeTemp, "t%d", contadorTemporarios++);
    return nomeTemp;
}

static char* novoLabel(void) {
    char* nomeLabel = (char*)malloc(10 * sizeof(char));
    sprintf(nomeLabel, "L%d", contadorLabels++);
    return nomeLabel;
}

// Função para que seja feito a atribuição de um vetor para uma variável
static char* assignVetor(char* indice, char* vetor){
    if (indice == NULL) 
        return NULL;
    
    char* temp1 = novoTemporario();
    char* temp2 = novoTemporario();
    char* temp3 = novoTemporario();
    printf("(LOADVAR, %s, %s, %s)\n", funcaoAtual, indice, temp1);
    printf("(LOADVAR, %s, %s, %s)\n", funcaoAtual, vetor, temp2);
    printf("(ADD, %s, %s, %s)\n", temp1, temp2, temp3);
    
    return temp3;
}

// salva endereços do no esquerda e direita para usar nas condições
typedef struct {
    char* esq;
    char* dir;
} CondAddrs;

static char* resolverOperandoCond(TreeNode* no) {
    if (no == NULL) return NULL;

    if (no->nodekind == VARK) {
        if (no->kind.var.varKind == KIND_ARRAY && no->child[0] != NULL) {
            char* indice = gerarExpressao(no->child[0]);
            return assignVetor(indice, no->kind.var.attr.name);
        }

        char* temp = novoTemporario();
        printf("(LOADVAR, %s, %s, %s)\n", funcaoAtual, no->kind.var.attr.name, temp);
        return temp;
    }

    return gerarExpressao(no);
}

static CondAddrs gerarCondicaoExpressao(TreeNode* esq, TreeNode* dir) {
    CondAddrs out;
    out.esq = resolverOperandoCond(esq);
    out.dir = resolverOperandoCond(dir);
    return out;
}

// Função para gerar salto condicional If-else
static void gerarCondicao(TreeNode* condicao, char* label) {
    if (condicao == NULL || condicao->child[0] == NULL || condicao->child[1] == NULL) return;

    CondAddrs addrs = gerarCondicaoExpressao(condicao->child[0], condicao->child[1]);
    if (addrs.esq == NULL || addrs.dir == NULL) return;

    char* operador = "?";
    switch (condicao->op) {
        case LT: operador = "BGE"; break;
        case LE: operador = "BGT"; break;
        case GT: operador = "BLE"; break;
        case GE: operador = "BLT"; break;
        case EQ: operador = "BNE"; break;
        case NE: operador = "BEQ"; break;
        default: break;
    }

    printf("(%s, %s, %s, %s)\n", operador, addrs.esq, addrs.dir, label);
}

/* Gera código para expressões e retorna o temporário onde o resultado está armazenado */
static char* gerarExpressao(TreeNode* no) {
    char* temp;
    char* esquerda;
    char* direita;
    char* operador;

    if (no == NULL)
        return NULL;

    if (no->nodekind == EXPK) {
        switch (no->kind.exp) {
            case CONSTK:
                temp = novoTemporario();
                printf("(LOADCONST, %s, %d, ___)\n", temp, no->kind.var.attr.val);
                return temp;

            case IDK: 
                return no->kind.var.attr.name;

            case OPK: 
                esquerda = gerarExpressao(no->child[0]);
                direita = gerarExpressao(no->child[1]);
                char* temp_rs = novoTemporario();
                char* temp_rt = novoTemporario();
                char* temp_rd = novoTemporario();

                printf("(LOADVAR, %s, %s, %s)\n", funcaoAtual, esquerda, temp_rs);
                printf("(LOADVAR, %s, %s, %s)\n", funcaoAtual, direita, temp_rt);


                switch (no->op) {
                    case PLUS:    operador = "ADD"; break;
                    case MINUS:   operador = "SUB"; break;
                    case TIMES:   operador = "MULT"; break;
                    case DIVIDE:  operador = "DIV"; break;
            
                    default:      operador = "?"; break;
                }
                // add RS, RT, RD
                printf("(%s, %s, %s, %s)\n", operador, temp_rs, temp_rt, temp_rd);
                return temp_rd;

            case CALLK: 
                {
                    TreeNode* argumento = no->child[0];
                    int numArgumentos = 0;

                    // Processa argumentos
                    while (argumento != NULL) {
                        char* tempArg = gerarExpressao(argumento);
                        printf("(PARAM, %s, ___, ___)\n", tempArg);
                        numArgumentos++;
                        argumento = argumento->sibling;
                    }

                    printf("(CALL, %s, %s, %d)\n", temp, no->kind.var.attr.name, numArgumentos);
                    return temp;
                }

            case ASSIGNK: 
                return NULL; // Será tratado em gerarComandoExpressao

            default:
                return NULL;
        }
    } else if (no->nodekind == VARK) {
        // Acesso a variável ou array
        if (no->kind.var.varKind == KIND_ARRAY && no->child[0] != NULL) {
            // Acesso a array com índice
            char* indice = gerarExpressao(no->child[0]);
            char* vetor = no->kind.var.attr.name;
            temp = assignVetor(indice, vetor);

            return temp;
        } else {
            // Variável simples
            return no->kind.var.attr.name;
        }
    }

    return NULL;
}

/* Gera código para comandos (statements) */
static void gerarComando(TreeNode* no) {
    char* labelFalso;
    char* labelFim;
    char* labelInicio;
    char* valor;

    if (no == NULL)
        return;

    if (no->nodekind == STMTK) {
        switch (no->kind.stmt) {
            case IFK: // if ou if-else
                labelFalso = novoLabel();
                labelFim = novoLabel();

                // monta condição 
                gerarCondicao(no->child[0], labelFalso);
                // monta codigo dentro
                gerarComando(no->child[1]);
                
                // Tem else
                if (no->child[2] != NULL) {
                    // salto para Fim dentro do If
                    printf("(JUMP, %s, ___, ___)\n", labelFim);
                    // começo do Else
                    printf("(LABEL, %s, ___, ___)\n", labelFalso);
                    gerarComando(no->child[2]);
                    printf("(LABEL, %s, ___, ___)\n", labelFim);
                } else {
                    // Sem else
                    printf("(LABEL, %s, ___, ___)\n", labelFalso);
                }
                break;

            case WHILEK: // Loop while
                labelInicio = novoLabel();
                labelFim = novoLabel();
                // para o caso de condição ter expressão
                TreeNode* condicao = no->child[0];

                printf("(LABEL, %s, ___, ___)\n", labelInicio);
                gerarCondicao(condicao, labelFim);
                gerarComando(no->child[1]);
                printf("(JUMP, %s, ___, ___)\n", labelInicio);
                printf("(LABEL, %s, ___, ___)\n", labelFim);

                break;

            case RETURNK: // Return
                if (no->child[0] != NULL) {
                    valor = gerarExpressao(no->child[0]);
                    printf("(RETURN, %s, ___, ___)\n", valor);
                } else {
                    printf("(RETURN, ___, ___, ___)\n");
                }
                break;

            case COMPK: // Bloco composto { ... }
                if (no->child[0] != NULL) {
                    // Declaração de variáveis e arrays dentro de função
                    TreeNode* declaracao = no->child[0];
                    while (declaracao != NULL) {
                        gerarComando(declaracao);
                        declaracao = declaracao->sibling;
                    }
                }

                if (no->child[1] != NULL) {
                    TreeNode* comando = no->child[1];
                    while (comando != NULL) {
                        if (comando->nodekind == STMTK) {
                            gerarComando(comando);
                        } else if (comando->nodekind == EXPK) {
                            gerarComandoExpressao(comando);
                        }
                        comando = comando->sibling;
                    }
                }
                break;
            
            // INTEGERK e VOIDK possuem a mesma lógica de declaração
            case INTEGERK:
            case VOIDK:
                // Declarações de variáveis ou funções
                if (no->child[0] != NULL && no->child[0]->nodekind == VARK) {
                    TreeNode* noIdentificador = no->child[0];

                    if (noIdentificador->kind.var.varKind == KIND_FUNC) {
                        // Declaração de função
                        char* tipofunc = (no->kind.stmt == INTEGERK) ? "int" : "void";
                        
                        const char* funcaoAnterior = funcaoAtual;
                        funcaoAtual = noIdentificador->kind.var.attr.name;
                        
                        printf("(FUNC, %s, %s, _)\n", tipofunc, noIdentificador->kind.var.attr.name);

                        // Processa parâmetros (child[0] do nó de função)
                        if (noIdentificador->child[0] != NULL) {
                            TreeNode* parametro = noIdentificador->child[0];
                            while (parametro != NULL) {
                                if (parametro->nodekind == STMTK && parametro->child[0] != NULL) {
                                    printf("(PARAM, %s, %s, _)\n", parametro->child[0]->kind.var.attr.name, funcaoAtual);
                                }
                                parametro = parametro->sibling;
                            }
                        }
                        // Processa corpo da função (child[1] do nó de função)
                        if (noIdentificador->child[1] != NULL) {
                            gerarComando(noIdentificador->child[1]);
                        }

                        printf("(ENDFUNC, %s, ___, ___)\n", funcaoAtual);
                        
                        funcaoAtual = funcaoAnterior;

                    // Declaração de array
                    } else if (noIdentificador->kind.var.varKind == KIND_ARRAY && noIdentificador->kind.var.acesso == DECLK) {
                        
                        if (noIdentificador->child[0] != NULL) {
                            // escopo global
                            if (funcaoAtual == NULL) {
                               printf("(ALLOCAMEMVET, global, %s, %d)\n", noIdentificador->kind.var.attr.name,
                                    noIdentificador->child[0]->kind.var.attr.val); 
                            }
                            // escopo de função
                            else {
                                printf("(ALLOCAMEMVET, %s, %s, %d)\n", funcaoAtual, noIdentificador->kind.var.attr.name,
                                        noIdentificador->child[0]->kind.var.attr.val);
                            }
                        }

                    // Declaração de variáveis
                    } else if (noIdentificador->kind.var.varKind == KIND_VAR && noIdentificador->kind.var.acesso == DECLK) {
                        // escopo global
                            if (funcaoAtual == NULL) {
                               printf("(ALLOCAMEMVAR, global, %s, ___)\n", noIdentificador->kind.var.attr.name);; 
                            }
                            // escopo de função
                            else {
                                printf("(ALLOCAMEMVAR, %s, %s, ___)\n", funcaoAtual, noIdentificador->kind.var.attr.name);
                            }    
                    }
                }
                break;

            default:
                break;
        }
    } else if (no->nodekind == EXPK) {
        gerarComandoExpressao(no);
    }
}

/* Gera código para expressões usadas como comandos (ex: atribuição, chamada de função) */
static void gerarComandoExpressao(TreeNode* no) {
    char* valor;

    if (no == NULL)
        return;

    if (no->nodekind == EXPK) {
        switch (no->kind.exp) {
            case ASSIGNK: 
                if (no->child[0] != NULL && no->child[1] != NULL) {
                    valor = gerarExpressao(no->child[1]);
                    char* temporario = novoTemporario();
                    if (no->child[0]->nodekind == VARK) {
                        if (no->child[0]->kind.var.varKind == KIND_ARRAY &&
                            no->child[0]->child[0] != NULL) {
                            char* indice = gerarExpressao(no->child[0]->child[0]);
                            printf("%s[%s] = %s\n", no->child[0]->kind.var.attr.name,
                                   indice, valor);
                        } else {
                            // caso de assign pra variavel normal
                            if (no->child[1]->kind.var.varKind == KIND_VAR && no->child[1] != NULL){
                                printf("(LOADVAR, %s, %s, %s)\n", funcaoAtual, valor, temporario);
                                printf("(STOREVAR, %s, %s, %s)\n", temporario, no->child[0]->kind.var.attr.name, funcaoAtual);
                            }
                            // caso de assign pra vetor
                            if (no->child[1]->kind.var.varKind == KIND_ARRAY && no->child[1] != NULL){
                                printf("(LOADVET, %s, %s, %s)\n", funcaoAtual, valor, temporario);
                                printf("(STOREVAR, %s, %s, %s)\n", temporario, no->child[0]->kind.var.attr.name, funcaoAtual);
                            }
                        }
                    }
                }
                break;

            case CALLK: // Chamada de função (como statement)
                {
                    TreeNode* argumento = no->child[0];
                    int numArgumentos = 0;

                    while (argumento != NULL) {
                        char* tempArg = gerarExpressao(argumento);
                        printf("(PARAM, %s, ___, ___)\n", tempArg);
                        numArgumentos++;
                        argumento = argumento->sibling;
                    }

                    printf("(CALL, %s, %d, _)\n", no->kind.var.attr.name, numArgumentos);
                }
                break;

            default:
                break;
        }
    }
}

/* Percorre a árvore sintática gerando código */
static void percorrerArvore(TreeNode* no) {
    while (no != NULL) {
        if (no->nodekind == STMTK) {
            gerarComando(no);
        } else if (no->nodekind == EXPK) {
            gerarComandoExpressao(no);
        }
        no = no->sibling;
    }
}

void codeGen(TreeNode* arvoreSintatica) {
    printf("\n*** CODIGO INTERMEDIARIO QUADRUPLAS ***\n\n");
    percorrerArvore(arvoreSintatica);
    printf("\n******************************************\n\n");
}
