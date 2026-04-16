# Tabela de quadruplas do gerador atual

Objetivo: documentar as quadruplas realmente emitidas por code_generator.c e facilitar a validacao do que ja esta implementado e do que ainda falta.

## Convencoes

- Formato de saida: (OP, arg1, arg2, arg3)
- Temporarios: t0, t1, ...
- Labels: L0, L1, ...
- Escopo: global ou nome da funcao (funcaoAtual)
- Retorno de chamada em expressao: $rf

## Operacoes emitidas no gerador

| Categoria | Construcao em C- | Quadruplas emitidas | Status | Observacoes |
|---|---|---|---|---|
| Declaracao | int x; (global) | (ALLOCAMEMVAR, global, x, ___) | Implementado | Declaracao de escalar global. |
| Declaracao | int x; (local) | (ALLOCAMEMVAR, funcao, x, ___) | Implementado | Declaracao de escalar local. |
| Declaracao | int v[10]; (global) | (ALLOCAMEMVET, global, v, 10) | Implementado | Declaracao de vetor global. |
| Declaracao | int v[10]; (local) | (ALLOCAMEMVET, funcao, v, 10) | Implementado | Declaracao de vetor local. |
| Funcao | int f(...) { ... } | (FUNC, int, f, _) ... (ENDFUNC, f, ___, ___) | Implementado | Para void: (FUNC, void, f, _). |
| Parametro formal | int a em f(int a) | (PARAM, a, f, _) | Implementado | PARAM tambem e usado para parametros formais. |
| Constante | x = 5; | (LOADCONST, tN, 5, ___) | Implementado | Em EXPRESSAO/CONSTK. |
| Operacao aritmetica | x+y, x-y, x*y, x/y | (ADD/SUB/MULT/DIV, op1, op2, tN) | Implementado | Operandos passam por cache de LOADVAR quando necessario. |
| Acesso de variavel | uso de x em expressao | (LOADVAR, escopo, x, tN) | Implementado | Emitido sob demanda por carregarOuReusarTemp(). |
| Acesso de vetor (endereco) | v[i] | (ADD, tIndice, tBase, tAddr) | Implementado | Endereco calculado por assignVetor(). |
| Leitura de vetor | x = v[i] | (LOADVET, escopo, endereco, tN) | Implementado | Assinatura atual usa escopo no arg1. |
| Atribuicao escalar | x = expr; | (STOREVAR, tValor, x, escopo) | Implementado | Invalida cache da variavel destino. |
| Atribuicao em vetor | v[i] = expr; | (STOREVET, tValor, endereco, escopo) | Implementado | Assinatura atual inclui escopo no arg3. |
| If sem else | if (a < b) S; | (BGE/BGT/BLE/BLT/BNE/BEQ, esq, dir, Lfalso), ..., (LABEL, Lfalso, ___, ___) | Implementado | Branch invertido para salto quando condicao falha. |
| If com else | if (...) S1; else S2; | branch condicional, then, (JUMP, Lfim, ___, ___), (LABEL, Lfalso,...), else, (LABEL, Lfim,...) | Implementado | Fluxo completo de IFK. |
| While | while (cond) S; | (LABEL, Linicio,...), branch para Lfim, corpo, (JUMP, Linicio,...), (LABEL, Lfim,...) | Implementado | Fluxo padrao de laco. |
| Return com valor | return expr; | (RETURN, valor, ___, ___) | Implementado | Se expr for chamada, valor pode ser $rf. |
| Return vazio | return; | (RETURN, ___, ___, ___) | Implementado | Sem operando de retorno. |
| Chamada de funcao (statement) | f(a,b); | (PARAM, a, ___, ___), (PARAM, b, ___, ___), (CALL, f, 2, _) | Implementado | Limpa cache de temporarios apos CALL. |
| Chamada de funcao (expressao) | x = f(a,b); | (PARAM, a, ___, ___), (PARAM, b, ___, ___), (CALL, f, 2, _), depois uso de $rf | Implementado | Assinatura de CALL esta padronizada com statement. |

## Mapeamento dos relacionais para branch

| Condicao em C- | Branch emitido |
|---|---|
| a < b | BGE (salta quando a >= b) |
| a <= b | BGT (salta quando a > b) |
| a > b | BLE (salta quando a <= b) |
| a >= b | BLT (salta quando a < b) |
| a == b | BNE (salta quando a != b) |
| a != b | BEQ (salta quando a == b) |

## Cobertura em relacao a Estudos/tabela.csv

| Operacao da referencia | Situacao no gerador atual | Observacao |
|---|---|---|
| add, sub, mult | Implementado | Nomes atuais: ADD, SUB, MULT. |
| divisao | Implementado com nome diferente | Nome atual: DIV. |
| slt, sgt, slet, sget, set, sdt | Nao implementado como quadrupla propria | Comparacao vai direto para branch condicional. |
| ifFalso | Implementado com nome diferente | Equivalente por branch invertido + label. |
| jump | Implementado com nome diferente | Nome atual: JUMP. |
| label_op | Implementado com nome diferente | Nome atual: LABEL. |
| funInicio, funFim | Implementado com nome diferente | Nomes atuais: FUNC, ENDFUNC. |
| param | Implementado | Usado para formal e atual. |
| call | Implementado | Assinatura atual unica: (CALL, funcao, nArgs, _). |
| move | Nao implementado | Retorno usa RETURN e $rf para call em expressao. |
| allocaMemVar, allocaMemVet | Implementado com nome semelhante | Nomes atuais: ALLOCAMEMVAR, ALLOCAMEMVET. |
| loadVar | Implementado com nome semelhante | Nome atual: LOADVAR. |
| loadVet | Implementado (com assinatura propria) | Atual: (LOADVET, escopo, endereco, destino). |
| storeVar | Implementado com nome semelhante | Nome atual: STOREVAR. |
| storeVet | Implementado (com assinatura propria) | Atual: (STOREVET, valor, endereco, escopo). |
| empilha, desempilha | Nao implementado | Ainda nao ha convencao explicita de pilha em quadruplas. |

## Pontos pendentes

| Item | Prioridade | Impacto |
|---|---|---|
| Definir convencao de pilha em quadruplas (empilha/desempilha/prologo/epilogo) | Alta | Necessario para chamadas robustas e recursao. |
| Padronizar assinatura de LOADVET/STOREVET com backend final | Media | Evita adaptadores extras no gerador de assembly. |
| Decidir se comparacoes viram ops relacionais proprias (slt, sgt, etc.) | Media | Facilita analise e otimizacao de IR. |
| Definir se RETURN permanece unico ou migra para move + registrador de retorno | Baixa/Media | Alinhamento com pipeline alvo. |
