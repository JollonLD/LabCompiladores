# Funcoes do Contexto e da Analise Semantica

Este documento descreve em detalhes o conceito de **Contexto do Parser** e cada funcao utilizada na analise semantica, construcao da AST, tabela de simbolos e funcoes auxiliares do compilador C-Minus. Para cada funcao sao apresentados: objetivo, assinatura, parametros, retorno, logica interna, e locais onde e utilizada.

---

## Sumario

1. [O Conceito de Contexto (ParserContext)](#1-o-conceito-de-contexto-parsercontext)
2. [Funcoes do Contexto](#2-funcoes-do-contexto)
3. [Funcoes de Gerenciamento de Escopos](#3-funcoes-de-gerenciamento-de-escopos)
4. [Funcoes de Busca na Tabela de Simbolos](#4-funcoes-de-busca-na-tabela-de-simbolos)
5. [Funcoes de Insercao na Tabela de Simbolos](#5-funcoes-de-insercao-na-tabela-de-simbolos)
6. [Funcao de Verificacao de Tipos](#6-funcao-de-verificacao-de-tipos)
7. [Funcao de Exibicao da Tabela de Simbolos](#7-funcao-de-exibicao-da-tabela-de-simbolos)
8. [Funcao de Liberacao de Memoria](#8-funcao-de-liberacao-de-memoria)
9. [Funcoes de Construcao da AST](#9-funcoes-de-construcao-da-ast)
10. [Funcoes de Impressao da AST](#10-funcoes-de-impressao-da-ast)
11. [Funcoes Auxiliares do GraphViz](#11-funcoes-auxiliares-do-graphviz)
12. [Funcao de Erro Sintatico (yyerror)](#12-funcao-de-erro-sintatico-yyerror)
13. [Funcao de Traducao de Tokens](#13-funcao-de-traducao-de-tokens)
14. [Funcao Principal (main)](#14-funcao-principal-main)

---

## 1. O Conceito de Contexto (ParserContext)

### 1.1 O que e o Contexto

O `ParserContext` e uma estrutura de dados central que funciona como o **estado global compartilhado** de toda a compilacao. Ele encapsula todas as informacoes que precisam persistir e ser acessiveis durante as diferentes fases do processo de compilacao: parsing, analise semantica e preparacao para geracao de codigo.

### 1.2 Por que o Contexto existe

Em compiladores simples, e comum usar variaveis globais para armazenar o estado (escopo atual, raiz da AST, flag de erros). O problema dessa abordagem e que torna o compilador nao-reentrante: nao seria possivel executar duas compilacoes simultaneas no mesmo processo.

O `ParserContext` resolve isso agrupando todo o estado mutavel em uma unica estrutura que e passada explicitamente como parametro. Isso e viabilizado pela diretiva do Bison:

```
%parse-param {ParserContext *ctx}
```

Essa diretiva faz com que `yyparse()` aceite um parametro extra `ctx`, e todas as acoes semanticas das regras gramaticais recebem acesso a esse parametro. Assim, cada chamada a `yyparse()` pode trabalhar com seu proprio contexto independente.

### 1.3 Definicao da estrutura

A estrutura e declarada em `parser_context.h`:

```c
typedef struct ParserContext {
    struct treeNode *ast_root;
    struct Escopo *escopo_atual;
    struct Escopo *lista_escopos;
    int has_errors;
} ParserContext;
```

### 1.4 Descricao detalhada de cada campo

#### `ast_root` (`struct treeNode *`)

**O que e:** Ponteiro para o no raiz da Arvore Sintatica Abstrata (AST).

**Quando e preenchido:** Na acao semantica da regra `program -> declaration_list`, que e a ultima reducao do parsing. A linha `ctx->ast_root = $$` armazena a raiz.

**Valor inicial:** `NULL` (definido em `parser_context_create()`).

**Quem consulta:**
- A funcao `main()` em `cminusSintSem.y`, apos o parsing, para decidir se gera o arquivo DOT e o codigo intermediario: `if (!ctx->has_errors && ctx->ast_root) { ... }`.
- Passado como argumento para `printTreeDOT()` e `codeGen()`.

**Ciclo de vida:** Criado durante o parsing (nas reducoes de regras que criam nos com `newStmtNode`, `newExpNode`, `newVarNode`). Permanece em memoria ate o final do programa. A memoria dos nos da AST **nao e liberada explicitamente** -- o programa confia no encerramento do processo para liberar a memoria.

#### `escopo_atual` (`struct Escopo *`)

**O que e:** Ponteiro para o escopo ativo no momento corrente da analise. Representa o escopo mais interno em que estamos inserindo ou buscando simbolos.

**Quando e atualizado:**
- `enter_scope_ctx()`: atualiza para o novo escopo criado.
- `leave_scope_ctx()`: atualiza para o escopo pai (`ctx->escopo_atual = ctx->escopo_atual->pai`).

**Valor inicial:** `NULL` (definido em `parser_context_create()`). E atualizado para o escopo global na primeira chamada a `enter_scope_ctx()` na `main()`.

**Quem consulta:**
- Todas as funcoes de insercao (`insert_symbol_ctx`, `insert_array_ctx`, `insert_function_ctx`): usam para saber **onde** inserir o novo simbolo.
- `lookup_symbol_current_ctx()`: busca apenas neste escopo.
- `lookup_symbol_ctx()`: comeca a busca por este escopo e sobe pela cadeia `pai`.

**Ciclo de vida:** Aponta para diferentes escopos durante o parsing. Ao final, `free_all_scopes_ctx()` define como `NULL`.

#### `lista_escopos` (`struct Escopo *`)

**O que e:** Ponteiro para a cabeca de uma lista encadeada (via `next_all`) de **todos** os escopos ja criados durante a compilacao, independentemente de ainda estarem ativos ou nao.

**Quando e atualizado:**
- `enter_scope_ctx()`: insere o novo escopo no inicio da lista (`novo->next_all = ctx->lista_escopos; ctx->lista_escopos = novo`).

**Valor inicial:** `NULL`.

**Quem consulta:**
- `ExibirTabelaSimbolos_ctx()`: percorre todos os escopos para exibir a tabela completa de simbolos.
- `free_all_scopes_ctx()`: percorre todos os escopos para liberar a memoria.

**Por que e necessario:** Os escopos formam uma hierarquia pai-filho via o campo `pai`, mas essa hierarquia nao permite percorrer **todos** os escopos de forma simples (um pai pode ter multiplos filhos). A lista `next_all` fornece um mecanismo alternativo de iteracao sequencial.

**Ciclo de vida:** Cresce durante o parsing (nunca tem escopos removidos). Ao final, `free_all_scopes_ctx()` libera todos e define como `NULL`.

#### `has_errors` (`int`)

**O que e:** Flag booleana que indica se algum erro sintatico ou semantico foi detectado durante a compilacao. Valor 0 = sem erros, valor 1 = com erros.

**Quando e atualizado:**
- Definido como `1` por `yyerror()` (erros sintaticos).
- Definido como `1` por cada funcao de insercao quando detecta redeclaracao.
- Definido como `1` pelas acoes semanticas nas regras gramaticais quando verificacoes falham (tipo incompativel, variavel nao declarada, etc.).
- **Nunca** e resetado para 0 apos ser marcado como 1.

**Valor inicial:** `0` (definido em `parser_context_create()`).

**Quem consulta:**
- A regra `program`: exibe mensagem "Nenhum erro encontrado" apenas se `!ctx->has_errors`.
- A `main()`: decide se gera AST/DOT e codigo intermediario (`if (!ctx->has_errors && ctx->ast_root)`).
- A `main()`: define o codigo de retorno do processo (`ctx->has_errors ? 1 : 0`).

**Consequencia pratica:** Um unico erro em qualquer ponto da compilacao impede a geracao de qualquer saida (DOT e TAC), mas a tabela de simbolos ainda e exibida.

### 1.5 Fluxo de uso do Contexto

```
1. parser_context_create()     --> aloca e inicializa todos os campos como NULL/0
2. enter_scope_ctx(ctx)        --> cria escopo global, escopo_atual aponta para ele
3. insert_symbol_ctx(...)      --> insere built-ins (input, output) no escopo global
4. yyparse(ctx)                --> parsing completo; ctx e passado como parametro
   |-- Acoes semanticas usam ctx para:
   |   |-- enter_scope_ctx(ctx)        --> cria escopos de funcoes e blocos
   |   |-- leave_scope_ctx(ctx)        --> sai de escopos
   |   |-- insert_symbol_ctx(ctx, ...) --> insere variaveis
   |   |-- insert_array_ctx(ctx, ...)  --> insere arrays
   |   |-- insert_function_ctx(ctx, ...)  --> insere funcoes
   |   |-- lookup_symbol_ctx(ctx, ...)    --> busca variaveis/funcoes
   |   |-- lookup_symbol_current_ctx(ctx, ...) --> busca no escopo atual
   |   |-- check_expression_type_ctx(ctx, ...) --> verifica tipos
   |   |-- ctx->has_errors = 1         --> marca erros
   |   +-- ctx->ast_root = $$          --> armazena raiz da AST
5. ExibirTabelaSimbolos_ctx(ctx)  --> exibe tabela completa
6. printTreeDOT(ctx->ast_root)     --> gera GraphViz (se sem erros)
7. codeGen(ctx->ast_root)          --> gera TAC (se sem erros)
8. free_all_scopes_ctx(ctx)        --> libera escopos e simbolos
9. parser_context_destroy(ctx)     --> libera a estrutura ctx
```

---

## 2. Funcoes do Contexto

### 2.1 `parser_context_create`

**Arquivo:** `parser_context.c`

**Assinatura:**

```c
ParserContext* parser_context_create(void);
```

**Parametros:** Nenhum.

**Retorno:** Ponteiro para um `ParserContext` alocado e inicializado, ou `NULL` se a alocacao falhar.

**Objetivo:** Criar e inicializar uma nova instancia do contexto do parser com todos os campos em seus valores iniciais seguros.

**Logica interna:**

1. Aloca `sizeof(ParserContext)` bytes com `malloc()`.
2. Se a alocacao falhar, retorna `NULL`.
3. Inicializa `ast_root = NULL` (nenhuma AST ainda).
4. Inicializa `escopo_atual = NULL` (nenhum escopo ativo ainda).
5. Inicializa `lista_escopos = NULL` (nenhum escopo criado ainda).
6. Inicializa `has_errors = 0` (sem erros inicialmente).
7. Retorna o ponteiro.

**Onde e usada:** Na `main()` de `cminusSintSem.y`, como primeiro passo da compilacao:

```c
ParserContext *ctx = parser_context_create();
```

---

### 2.2 `parser_context_destroy`

**Arquivo:** `parser_context.c`

**Assinatura:**

```c
void parser_context_destroy(ParserContext *ctx);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Ponteiro para o contexto a ser destruido |

**Retorno:** Nenhum (`void`).

**Objetivo:** Liberar a memoria da estrutura `ParserContext` em si.

**Logica interna:**

1. Verifica se `ctx` nao e `NULL` (protecao contra double-free).
2. Libera a memoria com `free(ctx)`.

**Importante:** Esta funcao **nao** libera os escopos nem os simbolos -- isso e responsabilidade de `free_all_scopes_ctx()`, que deve ser chamada **antes**. Tambem **nao** libera os nos da AST.

**Onde e usada:** Na `main()`, como penultimo passo antes de fechar o arquivo de entrada:

```c
free_all_scopes_ctx(ctx);       // primeiro libera escopos
parser_context_destroy(ctx);    // depois libera o contexto
```

---

## 3. Funcoes de Gerenciamento de Escopos

### 3.1 `enter_scope_ctx`

**Arquivo:** `cminusSintSem.y` (secao de codigo C apos `%%`)

**Assinatura:**

```c
void enter_scope_ctx(ParserContext *ctx);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser onde o novo escopo sera criado |

**Retorno:** Nenhum (`void`).

**Objetivo:** Criar um novo escopo e torna-lo o escopo ativo. O novo escopo e filho do escopo anteriormente ativo.

**Logica interna passo a passo:**

1. `Escopo *novo = (Escopo*) malloc(sizeof(Escopo));` -- Aloca memoria para um novo escopo.
2. `novo->simbolos = NULL;` -- O escopo comeca vazio, sem nenhum simbolo declarado.
3. `novo->pai = ctx->escopo_atual;` -- O escopo atual vira o pai do novo escopo. Isso cria a hierarquia de escopos aninhados. Se estamos no escopo global e criamos o escopo de uma funcao, o global sera pai do escopo da funcao.
4. `novo->next_all = ctx->lista_escopos;` -- Insere o novo escopo no **inicio** da lista global de todos os escopos (prepend). Isso garante que `lista_escopos` sempre aponta para o escopo mais recentemente criado.
5. `ctx->lista_escopos = novo;` -- Atualiza a cabeca da lista global.
6. `ctx->escopo_atual = novo;` -- O novo escopo se torna o escopo ativo. Todas as insercoes e buscas de simbolos a partir deste ponto usarao este escopo.

**Onde e usada (3 contextos):**

1. **Na `main()`, para criar o escopo global:**
   ```c
   enter_scope_ctx(ctx);  // escopo global, pai = NULL
   ```
   Chamada antes de inserir as funcoes built-in (`input`, `output`) e antes de `yyparse()`.

2. **Na regra `fun_declaration`, para criar o escopo da funcao:**
   ```c
   type_specifier ID
   {
       insert_function_ctx(ctx, $2, $1, yylineno);
       Simbolo *func_sym = lookup_symbol_current_ctx(ctx, $2);
       enter_scope_ctx(ctx);  // escopo para parametros e corpo
       if (func_sym) {
           func_sym->def_scope = ctx->escopo_atual;
       }
   }
   LPAREN params RPAREN compound_stmt
   ```
   O escopo e criado **apos** inserir a funcao no escopo pai, mas **antes** de processar os parametros. Isso garante que os parametros pertencem ao escopo da funcao.

3. **Na regra `compound_stmt_with_scope`, para blocos `{ }` como statements:**
   ```c
   compound_stmt_with_scope :
       { enter_scope_ctx(ctx); }
       compound_stmt
       { leave_scope_ctx(ctx); $$ = $2; }
   ```
   Blocos compostos usados como comandos independentes recebem seu proprio escopo.

---

### 3.2 `leave_scope_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void leave_scope_ctx(ParserContext *ctx);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser de onde sairemos do escopo atual |

**Retorno:** Nenhum (`void`).

**Objetivo:** Sair do escopo atual, retornando ao escopo pai. O escopo abandonado **nao** e destruido -- permanece na lista global para consulta posterior.

**Logica interna passo a passo:**

1. `if (!ctx->escopo_atual) return;` -- Protecao: se nao ha escopo ativo (nunca deveria acontecer em execucao normal), simplesmente retorna.
2. `ctx->escopo_atual = ctx->escopo_atual->pai;` -- O escopo ativo passa a ser o pai do escopo atual. Todas as operacoes futuras de insercao e busca usarao o escopo pai.

**Por que o escopo nao e destruido:** O escopo precisa permanecer em memoria porque:
- A tabela de simbolos e exibida **apos** o parsing completo, e precisa acessar todos os escopos.
- O campo `def_scope` de funcoes aponta para o escopo do corpo da funcao, e esse escopo precisa estar acessivel para contar parametros na exibicao.

**Onde e usada (2 contextos):**

1. **Na regra `fun_declaration`, apos processar o corpo:**
   ```c
   LPAREN params RPAREN compound_stmt
   {
       leave_scope_ctx(ctx);  // volta ao escopo onde a funcao foi declarada
       // ... construcao da AST ...
   }
   ```

2. **Na regra `compound_stmt_with_scope`, apos o bloco:**
   ```c
   compound_stmt_with_scope :
       { enter_scope_ctx(ctx); }
       compound_stmt
       { leave_scope_ctx(ctx); $$ = $2; }  // volta ao escopo externo
   ```

---

## 4. Funcoes de Busca na Tabela de Simbolos

### 4.1 `lookup_symbol_current_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
Simbolo* lookup_symbol_current_ctx(ParserContext *ctx, const char *nome);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para acessar `escopo_atual`) |
| `nome` | `const char *` | Nome do identificador a buscar |

**Retorno:** Ponteiro para o `Simbolo` encontrado, ou `NULL` se nao encontrado.

**Objetivo:** Buscar um identificador **apenas** no escopo atual, sem subir para escopos pais. Usada para detectar redeclaracao no mesmo escopo.

**Logica interna passo a passo:**

1. `if (!ctx->escopo_atual) return NULL;` -- Se nao ha escopo ativo, nao ha onde buscar.
2. `Simbolo *s = ctx->escopo_atual->simbolos;` -- Comeca pela cabeca da lista de simbolos do escopo atual.
3. Loop `while (s)`:
   - `if (strcmp(s->nome, nome) == 0) return s;` -- Se o nome casa, retorna o simbolo.
   - `s = s->prox;` -- Avanca para o proximo simbolo na lista.
4. `return NULL;` -- Percorreu toda a lista sem encontrar.

**Onde e usada (4 contextos):**

1. **Em `insert_symbol_ctx()`:** Antes de inserir uma variavel, verifica se ja existe no escopo atual. Se existir, emite erro de redeclaracao.

2. **Em `insert_array_ctx()`:** Mesmo proposito -- verifica redeclaracao antes de inserir array.

3. **Em `insert_function_ctx()`:** Verifica redeclaracao antes de inserir funcao.

4. **Na regra `fun_declaration`:** Apos inserir a funcao, busca o simbolo recem-inserido para configurar `def_scope`:
   ```c
   Simbolo *func_sym = lookup_symbol_current_ctx(ctx, $2);
   ```

5. **Na regra `param`:** Apos inserir o parametro, busca o simbolo para marcar `is_param = 1`:
   ```c
   Simbolo *p = lookup_symbol_current_ctx(ctx, $2);
   if (p) p->is_param = 1;
   ```

---

### 4.2 `lookup_symbol_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
Simbolo* lookup_symbol_ctx(ParserContext *ctx, const char *nome);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para acessar `escopo_atual` e a cadeia de escopos pais) |
| `nome` | `const char *` | Nome do identificador a buscar |

**Retorno:** Ponteiro para o `Simbolo` encontrado, ou `NULL` se nao encontrado em nenhum escopo visivel.

**Objetivo:** Buscar um identificador no escopo atual e em todos os escopos ancestrais (pais), implementando a regra de visibilidade da linguagem. O primeiro casamento encontrado e retornado, o que implementa *shadowing* (variavel local com mesmo nome que global esconde a global).

**Logica interna passo a passo:**

1. `Escopo *e = ctx->escopo_atual;` -- Comeca pelo escopo mais interno (atual).
2. Loop externo `while (e)` -- percorre escopos do mais interno ao global:
   - `Simbolo *s = e->simbolos;` -- Cabeca da lista de simbolos do escopo atual.
   - Loop interno `while (s)` -- percorre todos os simbolos deste escopo:
     - `if (strcmp(s->nome, nome) == 0) return s;` -- Nome casou, retorna imediatamente (shadowing).
     - `s = s->prox;` -- Proximo simbolo.
   - `e = e->pai;` -- Sobe para o escopo pai.
3. `return NULL;` -- Percorreu todos os escopos visiveis sem encontrar.

**Ordem de busca:** Escopo atual -> escopo pai -> escopo avo -> ... -> escopo global. O primeiro casamento prevalece.

**Onde e usada (3 contextos):**

1. **Na regra `var` (acesso a variavel simples):** Verifica se a variavel existe em algum escopo visivel:
   ```c
   Simbolo *s = lookup_symbol_ctx(ctx, $1);
   if (!s) {
       fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada ...");
   }
   ```

2. **Na regra `var` (acesso a array indexado):** Verifica existencia e se e realmente um array:
   ```c
   Simbolo *s = lookup_symbol_ctx(ctx, $1);
   if (!s) { /* erro: nao declarada */ }
   else if (s->kind != KIND_ARRAY) { /* erro: nao e array */ }
   ```

3. **Na regra `call` (chamada de funcao):** Verifica existencia e se e realmente uma funcao:
   ```c
   Simbolo *s = lookup_symbol_ctx(ctx, $1);
   if (!s) { /* erro: funcao nao declarada */ }
   else if (s->kind != KIND_FUNC) { /* erro: nao e funcao */ }
   ```

---

## 5. Funcoes de Insercao na Tabela de Simbolos

### 5.1 `insert_symbol_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void insert_symbol_ctx(ParserContext *ctx, const char *nome, TipoVar tipo,
                       TipoSimbolo kind, int linha);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para acessar `escopo_atual`) |
| `nome` | `const char *` | Nome do identificador a inserir |
| `tipo` | `TipoVar` | Tipo de dado (`TYPE_INT`, `TYPE_VOID`) |
| `kind` | `TipoSimbolo` | Categoria (`KIND_VAR`, `KIND_ARRAY`, `KIND_FUNC`) |
| `linha` | `int` | Numero da linha da declaracao no codigo-fonte |

**Retorno:** Nenhum (`void`).

**Objetivo:** Inserir um novo simbolo generico (variavel, funcao) na tabela de simbolos do escopo atual. E a funcao de insercao mais geral, usada para variaveis simples, parametros simples e funcoes built-in.

**Logica interna passo a passo:**

1. **Verificacao de escopo ativo:**
   ```c
   if (!ctx->escopo_atual) {
       fprintf(stderr, "Erro interno: nenhum escopo ativo\n");
       return;
   }
   ```
   Se nao ha escopo ativo, e um erro interno do compilador. Nao deveria acontecer em execucao normal.

2. **Verificacao de redeclaracao:**
   ```c
   if (lookup_symbol_current_ctx(ctx, nome)) {
       fprintf(stderr, "ERRO SEMANTICO: identificador '%s' ja declarado neste escopo - LINHA: %d\n", nome, linha);
       ctx->has_errors = 1;
       return;
   }
   ```
   Se o identificador ja existe no escopo atual, emite erro e **nao insere** (evita duplicatas).

3. **Alocacao e preenchimento:**
   ```c
   Simbolo *s = (Simbolo*) malloc(sizeof(Simbolo));
   s->nome = strdup(nome);        // copia do nome (independente do yytext)
   s->tipo = tipo;                 // tipo de dado
   s->kind = kind;                 // categoria
   s->tamanho = 0;                 // 0 para nao-arrays
   s->num_params = 0;              // 0 inicialmente
   s->param_types = NULL;          // nao preenchido
   s->linha = linha;               // linha de declaracao
   s->is_param = 0;                // nao e parametro por padrao
   s->def_scope = NULL;            // sem escopo associado por padrao
   ```

4. **Insercao no inicio da lista (prepend):**
   ```c
   s->prox = ctx->escopo_atual->simbolos;
   ctx->escopo_atual->simbolos = s;
   ```
   O novo simbolo se torna a cabeca da lista. Isso e O(1).

**Onde e usada (5 contextos):**

1. **`var_declaration` (variavel simples):** `insert_symbol_ctx(ctx, $2, $1, KIND_VAR, yylineno);`
2. **`param` (parametro simples):** `insert_symbol_ctx(ctx, $2, $1, KIND_VAR, yylineno);`
3. **`param` (parametro array):** `insert_symbol_ctx(ctx, $2, TYPE_INT_ARRAY, KIND_ARRAY, yylineno);`
4. **`main()` (funcao built-in input):** `insert_symbol_ctx(ctx, "input", TYPE_INT, KIND_FUNC, 0);`
5. **`main()` (funcao built-in output):** `insert_symbol_ctx(ctx, "output", TYPE_VOID, KIND_FUNC, 0);`

---

### 5.2 `insert_array_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void insert_array_ctx(ParserContext *ctx, const char *nome, int tamanho, int linha);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser |
| `nome` | `const char *` | Nome do array |
| `tamanho` | `int` | Numero de elementos do array (valor entre colchetes na declaracao) |
| `linha` | `int` | Linha de declaracao |

**Retorno:** Nenhum (`void`).

**Objetivo:** Inserir um simbolo de array na tabela de simbolos, com tipo fixo `TYPE_INT_ARRAY`, categoria `KIND_ARRAY`, e o tamanho armazenado.

**Logica interna passo a passo:**

1. **Verificacao de escopo ativo:** `if (!ctx->escopo_atual) return;` -- Retorna silenciosamente.

2. **Verificacao de redeclaracao:** Identica a `insert_symbol_ctx()`. Emite erro e retorna se ja existe.

3. **Alocacao e preenchimento:**
   ```c
   s->nome = strdup(nome);
   s->tipo = TYPE_INT_ARRAY;    // tipo fixo: array de int
   s->kind = KIND_ARRAY;        // categoria fixa: array
   s->tamanho = tamanho;        // tamanho do array (ex: 10 para int x[10])
   s->num_params = 0;
   s->param_types = NULL;
   s->linha = linha;
   ```
   Diferenca em relacao a `insert_symbol_ctx()`: o tipo e `TYPE_INT_ARRAY` (fixo), a categoria e `KIND_ARRAY` (fixo), e o `tamanho` e preenchido com o valor passado.

4. **Insercao no inicio da lista:** Igual a `insert_symbol_ctx()`.

**Nota:** O campo `is_param` **nao** e inicializado explicitamente nesta funcao (fica com valor indefinido do `malloc`). Isso e uma diferenca sutil em relacao a `insert_symbol_ctx()`, onde `is_param = 0` e definido explicitamente. Na pratica, arrays declarados via `insert_array_ctx()` sao arrays locais/globais (nao parametros), entao isso nao causa problema porque parametros array usam `insert_symbol_ctx()` com `KIND_ARRAY`.

**Onde e usada (1 contexto):**

1. **`var_declaration` (array):** `insert_array_ctx(ctx, $2, $4, yylineno);` -- onde `$4` e o valor de `NUM` entre colchetes.

---

### 5.3 `insert_function_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void insert_function_ctx(ParserContext *ctx, const char *nome, TipoVar tipo_retorno, int linha);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser |
| `nome` | `const char *` | Nome da funcao |
| `tipo_retorno` | `TipoVar` | Tipo de retorno da funcao (`TYPE_INT` ou `TYPE_VOID`) |
| `linha` | `int` | Linha de declaracao |

**Retorno:** Nenhum (`void`).

**Objetivo:** Inserir um simbolo de funcao na tabela de simbolos, com categoria `KIND_FUNC` e o tipo de retorno especificado.

**Logica interna passo a passo:**

1. **Verificacao de escopo ativo:** Igual as anteriores.

2. **Verificacao de redeclaracao:** Igual as anteriores.

3. **Alocacao e preenchimento:**
   ```c
   s->nome = strdup(nome);
   s->tipo = tipo_retorno;      // int ou void
   s->kind = KIND_FUNC;         // categoria fixa: funcao
   s->tamanho = 0;              // nao aplicavel a funcoes
   s->num_params = 0;           // sera calculado dinamicamente depois
   s->param_types = NULL;       // nao preenchido nesta versao
   s->linha = linha;
   ```
   O campo `def_scope` e preenchido **depois** pela regra `fun_declaration`, nao por esta funcao.

4. **Insercao no inicio da lista:** Igual as anteriores.

**Onde e usada (1 contexto):**

1. **`fun_declaration` (acao intermediaria):**
   ```c
   insert_function_ctx(ctx, $2, $1, yylineno);
   ```
   Chamada no escopo **externo** (onde a funcao e declarada), **antes** de criar o escopo do corpo. Isso garante que a funcao e visivel no escopo onde esta definida (e nao dentro de si mesma como se fosse local).

---

## 6. Funcao de Verificacao de Tipos

### 6.1 `check_expression_type_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
TipoVar check_expression_type_ctx(ParserContext *ctx, const char *op,
                                   TipoVar t1, TipoVar t2, int linha);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para marcar `has_errors`) |
| `op` | `const char *` | String descritiva da operacao (`"relacional"`, `"aditivo"`, `"multiplicativo"`) |
| `t1` | `TipoVar` | Tipo do operando esquerdo |
| `t2` | `TipoVar` | Tipo do operando direito |
| `linha` | `int` | Linha da operacao no codigo-fonte |

**Retorno:** `TipoVar` -- o tipo resultante da operacao:
- `TYPE_INT` se ambos operandos sao inteiros (operacao valida).
- `TYPE_ERROR` se algum operando e invalido.

**Objetivo:** Verificar se os dois operandos de uma operacao binaria sao compativeis (ambos devem ser `TYPE_INT`) e retornar o tipo resultante. E o ponto central de verificacao de tipos do compilador.

**Logica interna passo a passo:**

1. **Propagacao de erro (anti-cascata):**
   ```c
   if (t1 == TYPE_ERROR || t2 == TYPE_ERROR) {
       return TYPE_ERROR;
   }
   ```
   Se algum operando ja tem tipo de erro (de uma verificacao anterior que falhou), simplesmente propaga o erro **sem emitir nova mensagem**. Isso evita que um unico erro gere dezenas de mensagens de erro em cascata.

2. **Rejeicao de void:**
   ```c
   if (t1 == TYPE_VOID || t2 == TYPE_VOID) {
       fprintf(stderr, "ERRO SEMANTICO: operacao %s com tipo void - LINHA: %d\n", op, linha);
       ctx->has_errors = 1;
       return TYPE_ERROR;
   }
   ```
   Operandos `void` nao podem participar de operacoes aritmeticas ou relacionais. Exemplo: `output(x) + 1` onde `output` retorna `void`.

3. **Exigencia de inteiros:**
   ```c
   if (t1 != TYPE_INT || t2 != TYPE_INT) {
       fprintf(stderr, "ERRO SEMANTICO: operacao %s requer operandos inteiros - LINHA: %d\n", op, linha);
       ctx->has_errors = 1;
       return TYPE_ERROR;
   }
   ```
   Captura o caso de `TYPE_INT_ARRAY` (quando um array inteiro e usado como operando sem indexacao).

4. **Caso valido:**
   ```c
   return TYPE_INT;
   ```
   Ambos operandos sao inteiros, a operacao e valida e o resultado e inteiro.

**Onde e usada (3 contextos):**

1. **`simple_expression` (operacao relacional):**
   ```c
   TipoVar result = check_expression_type_ctx(ctx, "relacional", t1, t2, yylineno);
   ```

2. **`additive_expression` (operacao aditiva +/-):**
   ```c
   TipoVar result = check_expression_type_ctx(ctx, "aditivo", t1, t2, yylineno);
   ```

3. **`term` (operacao multiplicativa */):**
   ```c
   TipoVar result = check_expression_type_ctx(ctx, "multiplicativo", t1, t2, yylineno);
   ```

---

## 7. Funcao de Exibicao da Tabela de Simbolos

### 7.1 `ExibirTabelaSimbolos_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void ExibirTabelaSimbolos_ctx(ParserContext *ctx);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para acessar `lista_escopos`) |

**Retorno:** Nenhum (`void`).

**Objetivo:** Exibir na saida padrao (`stdout`) uma tabela formatada com todos os simbolos de todos os escopos, incluindo nome, tipo, categoria, escopo, tamanho, numero de parametros e linha de declaracao.

**Logica interna passo a passo:**

1. **Impressao do cabecalho:** Imprime titulo e cabecalho da tabela com colunas alinhadas.

2. **Contagem de escopos:**
   ```c
   int count = 0;
   Escopo *e = ctx->lista_escopos;
   while (e) { count++; e = e->next_all; }
   ```
   Percorre a lista global para contar quantos escopos existem. Se zero, retorna.

3. **Criacao do array auxiliar:**
   ```c
   Escopo **arr = (Escopo**) malloc(count * sizeof(Escopo*));
   ```
   Copia os ponteiros de escopo para um array para poder percorre-los em ordem reversa (do mais antigo ao mais recente).

4. **Iteracao reversa (do global ao mais interno):**
   ```c
   for (int idx = count - 1; idx >= 0; idx--)
   ```
   A lista `next_all` tem os escopos na ordem de criacao reversa (o mais recente primeiro). Iterando de tras para frente, o escopo global (criado primeiro) e impresso primeiro.

5. **Para cada simbolo de cada escopo, exibe:**
   - **Nome:** `s->nome`
   - **Tipo:** Traduz `s->tipo` para string (`int`, `void`, `int[]`, `ERROR`)
   - **Categoria:** Traduz `s->kind` para string (`VAR`, `ARRAY`, `FUNC`)
   - **Numero do escopo:** Inteiro sequencial comecando em 0 (global)
   - **Tamanho:** `s->tamanho` se array, senao `-`
   - **Numero de parametros (para funcoes):** Calculado dinamicamente:
     ```c
     int computed_params = 0;
     if (s->def_scope) {
         Simbolo *ps = s->def_scope->simbolos;
         while (ps) {
             if (ps->is_param) computed_params++;
             ps = ps->prox;
         }
     }
     ```
     Percorre os simbolos do escopo do corpo da funcao (`def_scope`) contando aqueles marcados com `is_param = 1`.
   - **Linha:** `s->linha`

6. **Liberacao do array auxiliar:** `free(arr);`

**Onde e usada:**

Na `main()`, **sempre** apos o `yyparse()`, independentemente de erros:

```c
ExibirTabelaSimbolos_ctx(ctx);
```

Isso permite que o usuario veja a tabela de simbolos mesmo quando ha erros, facilitando a depuracao.

---

## 8. Funcao de Liberacao de Memoria

### 8.1 `free_all_scopes_ctx`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void free_all_scopes_ctx(ParserContext *ctx);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para acessar e limpar `lista_escopos`) |

**Retorno:** Nenhum (`void`).

**Objetivo:** Liberar toda a memoria alocada para escopos e simbolos durante a compilacao.

**Logica interna passo a passo:**

1. **Percorre a lista global de escopos:**
   ```c
   Escopo *e = ctx->lista_escopos;
   while (e) {
   ```

2. **Para cada escopo, percorre e libera cada simbolo:**
   ```c
   Simbolo *s = e->simbolos;
   while (s) {
       Simbolo *tmp = s;
       s = s->prox;
       free(tmp->nome);                          // libera a string do nome
       if (tmp->param_types) free(tmp->param_types); // libera array de tipos (se existir)
       free(tmp);                                 // libera o simbolo
   }
   ```

3. **Libera o escopo:**
   ```c
   Escopo *tmp_e = e;
   e = e->next_all;
   free(tmp_e);
   ```

4. **Limpa os ponteiros do contexto:**
   ```c
   ctx->lista_escopos = NULL;
   ctx->escopo_atual = NULL;
   ```

**Onde e usada:** Na `main()`, apos todo o processamento e saida, **antes** de `parser_context_destroy()`:

```c
free_all_scopes_ctx(ctx);
```

---

## 9. Funcoes de Construcao da AST

### 9.1 `newStmtNode`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
TreeNode* newStmtNode(StmtKind kind);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `kind` | `StmtKind` | Subtipo do statement: `INTEGERK`, `VOIDK`, `IFK`, `WHILEK`, `RETURNK` ou `COMPK` |

**Retorno:** Ponteiro para o `TreeNode` alocado e inicializado.

**Objetivo:** Alocar e inicializar um no da AST do tipo **statement** (comando).

**Logica interna passo a passo:**

1. `TreeNode *t = (TreeNode*) malloc(sizeof(TreeNode));` -- Aloca memoria.
2. Verificacao de falha de alocacao -- encerra o programa com `exit(1)` se falhar.
3. `for (int i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;` -- Todos os filhos comecam nulos.
4. `t->sibling = NULL;` -- Sem irmao.
5. `t->nodekind = STMTK;` -- Marca como statement.
6. `t->kind.stmt = kind;` -- Define o subtipo especifico.
7. `t->lineno = yylineno;` -- Captura a linha atual do scanner.
8. `t->type = TYPE_VOID;` -- Statements tem tipo void por padrao (nao produzem valor).
9. `t->op = 0;` -- Sem operador.

**Onde e usada:** Em varias regras gramaticais:

| Regra | Chamada | Subtipo |
|---|---|---|
| `var_declaration` | `newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK)` | `INTEGERK` ou `VOIDK` |
| `fun_declaration` | `newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK)` | `INTEGERK` ou `VOIDK` |
| `param` | `newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK)` | `INTEGERK` ou `VOIDK` |
| `compound_stmt` | `newStmtNode(COMPK)` | `COMPK` |
| `selection_stmt` | `newStmtNode(IFK)` | `IFK` |
| `iteration_stmt` | `newStmtNode(WHILEK)` | `WHILEK` |
| `return_stmt` | `newStmtNode(RETURNK)` | `RETURNK` |

---

### 9.2 `newExpNode`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
TreeNode* newExpNode(ExpKind kind);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `kind` | `ExpKind` | Subtipo da expressao: `OPK`, `CONSTK`, `IDK`, `ASSIGNK`, `CALLK` ou `VECTORK` |

**Retorno:** Ponteiro para o `TreeNode` alocado e inicializado.

**Objetivo:** Alocar e inicializar um no da AST do tipo **expressao**.

**Logica interna passo a passo:**

1. Alocacao e verificacao identicas a `newStmtNode`.
2. `t->nodekind = EXPK;` -- Marca como expressao.
3. `t->kind.exp = kind;` -- Define o subtipo de expressao.
4. `t->lineno = yylineno;`
5. `t->type = TYPE_INT;` -- Expressoes tem tipo inteiro por padrao (o tipo mais comum).
6. `t->op = 0;` -- Sera preenchido para nos `OPK`.
7. `t->kind.var.attr.name = NULL;` -- Inicializa ponteiro de nome como nulo. Isso evita que um ponteiro lixo seja acidentalmente acessado ou liberado.

**Onde e usada:**

| Regra | Chamada | Subtipo |
|---|---|---|
| `expression` (atribuicao) | `newExpNode(ASSIGNK)` | `ASSIGNK` |
| `simple_expression` | `newExpNode(OPK)` | `OPK` |
| `additive_expression` | `newExpNode(OPK)` | `OPK` |
| `term` | `newExpNode(OPK)` | `OPK` |
| `factor` (constante) | `newExpNode(CONSTK)` | `CONSTK` |
| `call` | `newExpNode(CALLK)` | `CALLK` |
| `var_declaration` (tamanho array) | `newExpNode(CONSTK)` | `CONSTK` |

---

### 9.3 `newVarNode`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
TreeNode* newVarNode(ExpKind kind);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `kind` | `ExpKind` | Subtipo visual: `IDK` (identificador simples) ou `VECTORK` (vetor/array) |

**Retorno:** Ponteiro para o `TreeNode` alocado e inicializado.

**Objetivo:** Alocar e inicializar um no da AST do tipo **variavel/identificador** (`VARK`). Este tipo de no carrega informacoes ricas sobre o identificador (nome, categoria, modo de acesso).

**Logica interna passo a passo:**

1. Alocacao e verificacao identicas.
2. `t->nodekind = VARK;` -- Marca como variavel/identificador (diferente de `EXPK`).
3. `t->kind.var.acesso = DECLK;` -- Padrao: declaracao (sera mudado para `ACCESSK` quando for acesso).
4. `t->kind.var.varKind = KIND_VAR;` -- Padrao: variavel simples (sera mudado conforme necessidade).
5. `t->kind.var.attr.name = NULL;` -- Nome sera preenchido pela regra gramatical.
6. `t->lineno = yylineno;`
7. `t->type = TYPE_INT;`
8. `t->op = 0;`

**Onde e usada:**

| Regra | Chamada | Proposito |
|---|---|---|
| `var_declaration` (simples) | `newVarNode(IDK)` | No da variavel declarada |
| `var_declaration` (array) | `newVarNode(VECTORK)` | No do array declarado |
| `fun_declaration` | `newVarNode(IDK)` | No da funcao declarada |
| `param` (simples) | `newVarNode(IDK)` | No do parametro |
| `param` (array) | `newVarNode(VECTORK)` | No do parametro array |
| `var` (acesso simples) | `newVarNode(IDK)` ou `newVarNode(VECTORK)` | No de acesso a variavel |
| `var` (acesso indexado) | `newVarNode(VECTORK)` | No de acesso a elemento de array |

---

## 10. Funcoes de Impressao da AST

### 10.1 `printTree`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void printTree(TreeNode *tree, int indent);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `tree` | `TreeNode *` | No atual da arvore a ser impresso |
| `indent` | `int` | Nivel de indentacao (0 para raiz, incrementado a cada nivel) |

**Retorno:** Nenhum (`void`).

**Objetivo:** Imprimir a AST completa em formato textual indentado no `stdout`. Mostra todos os nos sem simplificacao, incluindo nos de tipo e compound statements.

**Logica interna passo a passo:**

1. **Caso base:** Se `tree == NULL`, retorna.
2. **Indentacao:** Imprime `indent` pares de espacos.
3. **Classificacao e impressao do no por `nodekind`:**
   - `STMTK`: imprime `[int]`/`[void]` com sufixo (Function/Array/Var), `If`, `While`, `Return`, `Compound Statement`
   - `VARK`: imprime `[nome]` com sufixo `(function)`, `(array)`, `(declaration)`, `(access)`
   - `EXPK`: imprime `Op: +`, `Const: 42`, `Id: x`, `Assign (=)`, `Call: f()`, `Vector: v`
4. **Recursao nos filhos:** `printTree(tree->child[i], indent + 1)` para cada filho
5. **Recursao nos irmaos:** `printTree(tree->sibling, indent)` com mesmo nivel

**Onde e usada:** Atualmente **comentada** na `main()`. Era usada para depuracao antes da implementacao do GraphViz.

---

### 10.2 `printTreeSimplified`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void printTreeSimplified(TreeNode *tree, int indent);
```

**Parametros:** Identicos a `printTree`.

**Retorno:** Nenhum (`void`).

**Objetivo:** Imprimir a AST em formato textual simplificado, pulando nos intermediarios (`INTEGERK`/`VOIDK` e `COMPK`) para focar nos nos semanticamente relevantes.

**Logica interna:**

Semelhante a `printTree`, mas com simplificacoes:
- **Nos `INTEGERK`/`VOIDK`:** Pula o no e processa diretamente o `child[0]` (o identificador) e o `sibling`, sem incrementar indentacao.
- **Nos `COMPK`:** Pula o no e processa todos os filhos e siblings sem incrementar indentacao.
- **Demais nos:** Imprime normalmente (IF, WHILE, RETURN, funcoes, variaveis, etc.).

**Onde e usada:** Nao e chamada atualmente no codigo, mas esta disponivel como alternativa textual.

---

## 11. Funcoes Auxiliares do GraphViz

### 11.1 `escape_label`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
static char *escape_label(const char *str);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `str` | `const char *` | String a ser escapada |

**Retorno:** Nova string alocada com caracteres especiais escapados. O chamador e responsavel por liberar com `free()`.

**Objetivo:** Escapar caracteres especiais (`"` e `\`) em strings que serao usadas como labels em nos do GraphViz, evitando que quebrem a sintaxe DOT.

**Logica interna:**

1. Se `str` e `NULL`, retorna `strdup("")` (string vazia alocada).
2. Aloca buffer de tamanho `len * 2 + 1` (pior caso: todos os caracteres precisam escape).
3. Para cada caractere: se for `"` ou `\`, insere `\` antes dele.
4. Termina com `\0`.

**Onde e usada:** Em `printTreeDOT_simplified()`, sempre que um nome de identificador e usado como label:

```c
char *esc_name = escape_label(tree->kind.var.attr.name);
snprintf(label, sizeof(label), "Function\\n%s", esc_name);
free(esc_name);
```

---

### 11.2 `get_op_symbol`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
static const char* get_op_symbol(int op);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `op` | `int` | Codigo do token do operador (ex: `PLUS`, `MINUS`, `LT`, etc.) |

**Retorno:** String literal constante com o simbolo do operador (`"+"`, `"-"`, `"<"`, etc.), ou `"?"` para operadores desconhecidos.

**Objetivo:** Traduzir o codigo numerico de um token de operador para sua representacao textual.

**Mapeamento completo:**

| Codigo | Retorno |
|---|---|
| `PLUS` | `"+"` |
| `MINUS` | `"-"` |
| `TIMES` | `"*"` |
| `DIVIDE` | `"/"` |
| `LT` | `"<"` |
| `LE` | `"<="` |
| `GT` | `">"` |
| `GE` | `">="` |
| `EQ` | `"=="` |
| `NE` | `"!="` |
| default | `"?"` |

**Onde e usada:**

1. `printTreeDOT_simplified()`: para labels de nos de operacao no GraphViz.
2. `printTreeSimplified()`: para impressao textual de operadores.

---

### 11.3 `printTreeDOT_simplified`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
static int printTreeDOT_simplified(DotContext *ctx, TreeNode *tree, int parent_id);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `DotContext *` | Contexto da geracao DOT (contador de nos e ponteiro do arquivo) |
| `tree` | `TreeNode *` | No atual da arvore |
| `parent_id` | `int` | ID do no pai no grafo DOT (-1 se nao tem pai, ou seja, e raiz) |

**Retorno:** `int` -- ID do no criado, ou `-1` se o no foi pulado (simplificacao).

**Objetivo:** Gerar recursivamente a representacao DOT simplificada da AST, onde nos intermediarios (`INTEGERK`, `VOIDK`, `COMPK`) sao omitidos e seus filhos conectados diretamente ao pai.

**Logica interna passo a passo:**

1. **Caso base:** Se `tree == NULL`, retorna -1.
2. **Atribuicao de ID:** `int current_id = ctx->node_counter++;`
3. **Definicao de forma, cor e label do no** baseada em `nodekind` e subtipos:
   - **STMTK / INTEGERK ou VOIDK:** Pula o no. Processa `child[0]` e `sibling` com o `parent_id` do avo. Retorna -1.
   - **STMTK / COMPK:** Pula o no. Processa todos os filhos e siblings com o `parent_id` do avo. Retorna -1.
   - **STMTK / IFK, WHILEK, RETURNK:** Cria no com forma e cor especificas.
   - **VARK:** Cria no com label baseado no nome, distinguindo funcao/array/variavel e declaracao/acesso.
   - **EXPK:** Cria no com label baseado no subtipo (operador, constante, nome, etc.).
4. **Escrita do no:** `fprintf(fp, "  node%d [label=\"%s\", shape=%s, ...];\n", ...)`
5. **Conexao ao pai:** Se `parent_id >= 0`, escreve aresta.
6. **Recursao nos filhos:** Com `current_id` como pai.
7. **Recursao nos irmaos:** Com `parent_id` (mesmo pai) -- irmaos sao nos no mesmo nivel.

**Onde e usada:** Por `printTreeDOT()`.

---

### 11.4 `printTreeDOT`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void printTreeDOT(TreeNode *tree, const char *filename);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `tree` | `TreeNode *` | Raiz da AST |
| `filename` | `const char *` | Nome do arquivo de saida (ex: `"ast.dot"`) |

**Retorno:** Nenhum (`void`).

**Objetivo:** Gerar um arquivo no formato GraphViz DOT contendo a representacao visual da AST.

**Logica interna passo a passo:**

1. Abre o arquivo para escrita.
2. Cria `DotContext` com contador 0 e ponteiro do arquivo.
3. Escreve cabecalho DOT (`digraph AST {`, configuracoes de estilo).
4. Chama `printTreeDOT_simplified()` com a raiz e `parent_id = -1`.
5. Escreve rodape (`}`).
6. Fecha o arquivo.
7. Imprime instrucoes para gerar a imagem (`dot -Tpng ...`).

**Onde e usada:** Na `main()`, se nao houver erros:

```c
printTreeDOT(ctx->ast_root, "ast.dot");
```

---

## 12. Funcao de Erro Sintatico (yyerror)

### 12.1 `yyerror`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
void yyerror(ParserContext *ctx, const char *s);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `ctx` | `ParserContext *` | Contexto do parser (para marcar `has_errors = 1`) |
| `s` | `const char *` | Mensagem de erro gerada pelo Bison (em ingles, formato `"syntax error, unexpected TOKEN, expecting TOKEN"`) |

**Retorno:** Nenhum (`void`).

**Objetivo:** Processar mensagens de erro sintatico do Bison, traduzi-las para portugues com termos amigaveis, e apresenta-las ao usuario com indicacao de linha. Marca o contexto como tendo erros.

**Logica interna passo a passo:**

1. **Verificacao de mensagem detalhada:** `if (strstr(s, "unexpected") != NULL)` -- Se a mensagem contem "unexpected", e uma mensagem detalhada do Bison (habilitada por `%error-verbose`).

2. **Extracao de tokens:**
   - Copia a mensagem com `strdup()`.
   - Localiza `"unexpected "` e `"expecting "` na copia.
   - Extrai o token inesperado com `sscanf(unexpected_pos, "unexpected %[^,]", unexpected_raw)`.
   - Extrai os tokens esperados copiando caractere por caractere.

3. **Traduz o token inesperado:** `translate_token(unexpected_raw)`.

4. **Casos especiais:**
   - **`$end` inesperado:** Fim de arquivo prematuro, sugere delimitador nao fechado.
   - **`ELSE` inesperado:** Sugere `else` sem `if` correspondente.
   - **Contexto de statement:** Quando o esperado e `LBRACE` e o inesperado e `RBRACE`/`ELSE`/`RETURN`, fornece mensagem mais descritiva.

5. **Caso geral:** Processa multiplos tokens esperados separados por `"or"`, traduzindo cada um com `translate_token()` e juntando-os com `"ou"`.

6. **Impressao:** `fprintf(stderr, "ERRO SINTATICO: ... - LINHA: %d\n", yylineno);`

7. **Marcacao de erro:** `ctx->has_errors = 1;`

**Onde e usada:** Chamada automaticamente pelo parser gerado pelo Bison sempre que ocorre um erro sintatico (token inesperado). O Bison chama `yyerror()` internamente quando nao consegue realizar shift nem reduce.

---

## 13. Funcao de Traducao de Tokens

### 13.1 `translate_token`

**Arquivo:** `cminusSintSem.y` (bloco `%{ ... %}`)

**Assinatura:**

```c
const char* translate_token(const char *token);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `token` | `const char *` | Nome interno do token gerado pelo Bison (ex: `"SEMI"`, `"LPAREN"`, `"ID"`) |

**Retorno:** String constante com a representacao amigavel em portugues/simbolo (ex: `"';'"`, `"'('"`, `"identificador"`), ou o proprio token original se nao houver traducao.

**Objetivo:** Converter nomes internos de tokens do Bison para formas legiveis ao usuario, usadas nas mensagens de erro sintatico.

**Mapeamento completo:**

| Token interno | Traducao |
|---|---|
| `SEMI` | `';'` |
| `COMMA` | `','` |
| `LPAREN` | `'('` |
| `RPAREN` | `')'` |
| `LBRACK` | `'['` |
| `RBRACK` | `']'` |
| `LBRACE` | `'{'` |
| `RBRACE` | `'}'` |
| `ASSIGN` | `'='` |
| `PLUS` | `'+'` |
| `MINUS` | `'-'` |
| `TIMES` | `'*'` |
| `DIVIDE` | `'/'` |
| `LT` | `'<'` |
| `LE` | `'<='` |
| `GT` | `'>'` |
| `GE` | `'>='` |
| `EQ` | `'=='` |
| `NE` | `'!='` |
| `IF` | `'if'` |
| `ELSE` | `'else'` |
| `WHILE` | `'while'` |
| `RETURN` | `'return'` |
| `INT` | `'int'` |
| `VOID` | `'void'` |
| `ID` | `identificador` |
| `NUM` | `numero` |
| `$end` (contido) | `fim de arquivo` |
| (qualquer outro) | (retorna o proprio token sem traducao) |

**Onde e usada:** Exclusivamente dentro de `yyerror()`, para traduzir os tokens das mensagens do Bison.

---

## 14. Funcao Principal (main)

### 14.1 `main`

**Arquivo:** `cminusSintSem.y`

**Assinatura:**

```c
int main(int argc, char **argv);
```

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `argc` | `int` | Numero de argumentos da linha de comando |
| `argv` | `char **` | Array de strings dos argumentos |

**Retorno:** `int` -- codigo de saida do processo: 0 para sucesso, 1 para erros.

**Objetivo:** Orquestrar todo o processo de compilacao, desde a abertura do arquivo ate a liberacao de memoria.

**Logica interna passo a passo:**

1. **Abertura do arquivo de entrada:**
   ```c
   if (argc > 1) {
       yyin = fopen(argv[1], "r");
   } else {
       yyin = stdin;
   }
   ```
   Se um arquivo e passado como argumento, abre-o. Caso contrario, le da entrada padrao.

2. **Criacao do contexto:**
   ```c
   ParserContext *ctx = parser_context_create();
   ```

3. **Criacao do escopo global:**
   ```c
   enter_scope_ctx(ctx);
   ```

4. **Insercao de funcoes built-in:**
   ```c
   insert_symbol_ctx(ctx, "input", TYPE_INT, KIND_FUNC, 0);
   insert_symbol_ctx(ctx, "output", TYPE_VOID, KIND_FUNC, 0);
   ```
   Linha 0 indica que sao built-in (nao existem no codigo-fonte).

5. **Parsing (inclui analise lexica, sintatica e semantica):**
   ```c
   yyparse(ctx);
   ```

6. **Exibicao da tabela de simbolos (sempre):**
   ```c
   ExibirTabelaSimbolos_ctx(ctx);
   ```

7. **Geracao de saida (somente se sem erros):**
   ```c
   if (!ctx->has_errors && ctx->ast_root) {
       printTreeDOT(ctx->ast_root, "ast.dot");
       codeGen(ctx->ast_root);
   }
   ```

8. **Liberacao de memoria:**
   ```c
   free_all_scopes_ctx(ctx);
   parser_context_destroy(ctx);
   if (yyin != stdin) fclose(yyin);
   ```

9. **Retorno:** `ctx->has_errors ? 1 : 0`

**Onde e usada:** Ponto de entrada do programa (chamada pelo sistema operacional).

---

## Apendice: Macro de Compatibilidade

### `STRTOK_REENTRANT`

**Arquivo:** `cminusSintSem.y` (bloco `%{ ... %}`)

```c
#ifdef _WIN32
#define STRTOK_REENTRANT(str, delim, saveptr) strtok_s((str), (delim), (saveptr))
#else
#define STRTOK_REENTRANT(str, delim, saveptr) strtok_r((str), (delim), (saveptr))
#endif
```

**Objetivo:** Fornecer uma versao thread-safe de `strtok()` que funciona tanto em Windows (`strtok_s`) quanto em Linux/macOS (`strtok_r`).

**Parametros:**

| Parametro | Tipo | Descricao |
|---|---|---|
| `str` | `char *` | String a tokenizar (NULL para chamadas subsequentes) |
| `delim` | `const char *` | String de delimitadores |
| `saveptr` | `char **` | Ponteiro para estado interno (preserva posicao entre chamadas) |

**Onde e usada:** Em `yyerror()`, para separar multiplos tokens esperados na mensagem de erro do Bison (separados por espaco e `"or"`).
