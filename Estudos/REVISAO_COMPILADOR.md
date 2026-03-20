# Revisao Tecnica Completa do Compilador C-Minus

Este documento apresenta uma revisao tecnica detalhada de cada etapa do processo de compilacao implementado neste projeto, desde a analise lexica ate a geracao de codigo intermediario de tres enderecos. Todas as estruturas de dados, variaveis, estrategias e decisoes de implementacao sao documentadas com base no codigo-fonte real.

---

## Sumario

1. [Visao Geral da Arquitetura](#1-visao-geral-da-arquitetura)
2. [Analise Lexica](#2-analise-lexica)
3. [Estruturas de Dados Fundamentais](#3-estruturas-de-dados-fundamentais)
4. [Analise Sintatica](#4-analise-sintatica)
5. [Arvore Sintatica Abstrata (AST)](#5-arvore-sintatica-abstrata-ast)
6. [Tabela de Simbolos e Gerenciamento de Escopos](#6-tabela-de-simbolos-e-gerenciamento-de-escopos)
7. [Analise Semantica](#7-analise-semantica)
8. [Geracao de Codigo Intermediario](#8-geracao-de-codigo-intermediario)
9. [Fluxo Completo de Execucao](#9-fluxo-completo-de-execucao)

---

## 1. Visao Geral da Arquitetura

O compilador e composto pelos seguintes arquivos-fonte:

| Arquivo | Funcao |
|---|---|
| `cminusLex.l` | Especificacao lexica (Flex) |
| `cminusSintSem.y` | Gramatica, acoes sintaticas, semanticas, AST, tabela de simbolos (Bison) |
| `parser_context.h` / `parser_context.c` | Contexto global do parser (`ParserContext`) |
| `code_generator.h` / `code_generator.c` | Geracao de codigo intermediario de tres enderecos |
| `cminus.tab.h` / `cminus.tab.c` | Codigo C gerado pelo Bison |
| `lex.yy.c` | Codigo C gerado pelo Flex |

A compilacao segue o pipeline classico:

```
Codigo-fonte (.cm)
    |
    v
[Analise Lexica] -- Flex (cminusLex.l)
    |  tokens
    v
[Analise Sintatica + Semantica] -- Bison (cminusSintSem.y)
    |  AST + Tabela de Simbolos
    v
[Geracao de Codigo Intermediario] -- code_generator.c
    |
    v
Codigo de Tres Enderecos (stdout)
```

A particularidade deste compilador e que a analise sintatica e a analise semantica sao realizadas de forma integrada: as verificacoes de tipo, escopo e declaracao acontecem dentro das acoes semanticas das regras gramaticais do Bison, durante o processo de reducao (bottom-up). Nao ha uma fase separada de travessia da AST para checagem semantica.

---

## 2. Analise Lexica

### 2.1 Arquivo de especificacao

A analise lexica e definida em `cminusLex.l` e processada pelo Flex para gerar `lex.yy.c`.

### 2.2 Configuracoes

```
%option noyywrap
%option yylineno
```

- **`noyywrap`**: desabilita a funcao `yywrap()`, indicando que nao ha multiplos arquivos de entrada. Quando o scanner chega ao fim do arquivo, ele simplesmente para.
- **`yylineno`**: ativa a contagem automatica de linhas. O Flex incrementa a variavel global `yylineno` a cada `\n` encontrado. Essa variavel e usada em todas as mensagens de erro do compilador.

### 2.3 Definicao do scanner

```c
#define YY_DECL int yylex()
```

Define a assinatura da funcao do scanner. Retorna um `int` correspondendo ao token reconhecido.

### 2.4 Estados exclusivos

```
%x COMMENT
```

O scanner possui um estado exclusivo `COMMENT` para tratar comentarios de bloco `/* ... */`. Quando `/*` e reconhecido, o scanner entra neste estado e ignora todos os caracteres ate encontrar `*/`. Se o fim do arquivo for alcancado dentro do comentario, um erro lexico e emitido:

```
ERRO LEXICO: 'comentario nao fechado' - LINHA: X
```

### 2.5 Tokens reconhecidos

#### Espacos e quebras de linha

```
[ \t\r]+    ;   /* ignorados */
\n          ;   /* ignorado, mas yylineno e incrementado */
```

#### Comentarios

| Padrao | Acao |
|---|---|
| `"/*"` | Entra no estado `COMMENT` |
| `<COMMENT>"*/"` | Sai do estado `COMMENT` |
| `<COMMENT>.` | Consome caracteres dentro do comentario |
| `<COMMENT>\n` | Consome quebra de linha dentro do comentario |
| `<COMMENT><<EOF>>` | Erro: comentario nao fechado |
| `"//".*` | Comentario de linha unica, ignorado |

#### Palavras reservadas

| Padrao | Token retornado |
|---|---|
| `"else"` | `ELSE` |
| `"if"` | `IF` |
| `"int"` | `INT` |
| `"return"` | `RETURN` |
| `"void"` | `VOID` |
| `"while"` | `WHILE` |

As palavras reservadas sao casadas **antes** da regra geral de identificadores porque aparecem primeiro no arquivo `.l`. O Flex prioriza a regra que aparece primeiro quando ha empate de comprimento.

#### Identificadores e numeros

| Padrao | Token | Valor semantico |
|---|---|---|
| `[a-zA-Z][a-zA-Z0-9]*` | `ID` | `yylval.id = strdup(yytext)` |
| `[0-9]+` | `NUM` | `yylval.ival = atoi(yytext)` |

- Para `ID`, o texto e copiado com `strdup()` para que o ponteiro sobreviva alem da chamada a `yylex()`.
- Para `NUM`, o texto e convertido a inteiro com `atoi()`.

#### Operadores relacionais

| Padrao | Token |
|---|---|
| `"<="` | `LE` |
| `"<"` | `LT` |
| `">"` | `GT` |
| `">="` | `GE` |
| `"=="` | `EQ` |
| `"!="` | `NE` |

Os operadores de dois caracteres (`<=`, `>=`, `==`, `!=`) aparecem antes dos de um caractere para garantir casamento maximo (*maximal munch*).

#### Operadores aritmeticos e pontuacao

| Padrao | Token |
|---|---|
| `"="` | `ASSIGN` |
| `";"` | `SEMI` |
| `","` | `COMMA` |
| `"("` | `LPAREN` |
| `")"` | `RPAREN` |
| `"["` | `LBRACK` |
| `"]"` | `RBRACK` |
| `"{"` | `LBRACE` |
| `"}"` | `RBRACE` |
| `"+"` | `PLUS` |
| `"-"` | `MINUS` |
| `"*"` | `TIMES` |
| `"/"` | `DIVIDE` |

#### Caractere nao reconhecido

```
.   { fprintf(stderr, "ERRO LEXICO: caractere nao reconhecido: '%s' - LINHA: %d\n", yytext, yylineno); }
```

Qualquer caractere que nao case com nenhuma regra anterior gera um erro lexico, mas a analise continua.

### 2.6 Uniao de valores semanticos

A comunicacao entre lexer e parser e feita pela uniao `%union` definida no Bison:

```c
%union {
    int ival;          /* valor numerico para NUM */
    char *id;          /* string para ID */
    TipoVar tipo;      /* tipo para type_specifier */
    struct {
        TipoVar tipo;
        int is_array;
    } var_info;        /* informacao composta de variavel */
    TreeNode *node;    /* ponteiro para no da AST */
}
```

| Campo | Tipo | Uso |
|---|---|---|
| `ival` | `int` | Armazena o valor inteiro de tokens `NUM` e os codigos de operadores (`relop`, `addop`, `mulop`) |
| `id` | `char *` | Armazena o nome de identificadores (`ID`) como string alocada dinamicamente |
| `tipo` | `TipoVar` | Armazena o tipo retornado por `type_specifier` (`TYPE_INT` ou `TYPE_VOID`) |
| `var_info` | struct | Estrutura composta para informacao de variavel (tipo + flag de array) |
| `node` | `TreeNode *` | Ponteiro para nos da AST, usado pela maioria dos nao-terminais da gramatica |

---

## 3. Estruturas de Dados Fundamentais

Todas as estruturas de dados centrais estao definidas no bloco `%code requires { ... }` do arquivo `cminusSintSem.y`, o que permite que sejam incluidas no header gerado (`cminus.tab.h`) e, portanto, acessiveis por todos os modulos do compilador.

### 3.1 Enumeracoes de Tipos

#### `TipoVar` - Tipos de dados da linguagem

```c
typedef enum {
    TYPE_INT,        /* tipo inteiro */
    TYPE_VOID,       /* tipo void */
    TYPE_INT_ARRAY,  /* tipo array de inteiros */
    TYPE_ERROR       /* tipo de erro (propagacao) */
} TipoVar;
```

| Valor | Descricao | Onde e usado |
|---|---|---|
| `TYPE_INT` | Representa o tipo `int`. Variaveis inteiras, constantes numericas, resultados de operacoes aritmeticas e relacionais, retorno de funcoes `int`. | Atribuido a variaveis `int`, constantes `NUM`, resultados de `check_expression_type_ctx()`, retorno de funcoes `int` |
| `TYPE_VOID` | Representa o tipo `void`. Funcoes sem retorno, lista de parametros vazia. | Atribuido a funcoes `void`, usado na verificacao de operacoes (operandos `void` geram erro) |
| `TYPE_INT_ARRAY` | Representa um array de inteiros. Diferencia arrays de variaveis simples no sistema de tipos. | Atribuido a declaracoes e parametros de array, permite verificar se um identificador pode ser indexado |
| `TYPE_ERROR` | Tipo sentinela para propagacao de erros. Quando uma expressao contem erro, seu tipo e marcado como `TYPE_ERROR` para evitar cascata de mensagens de erro. | Retornado por `check_expression_type_ctx()` quando detecta erro; em `lookup` quando variavel nao existe; verificado antes de emitir novos erros para nao duplicar mensagens |

#### `TipoSimbolo` - Categorias de simbolos

```c
typedef enum {
    KIND_VAR,    /* variavel simples */
    KIND_ARRAY,  /* array */
    KIND_FUNC    /* funcao */
} TipoSimbolo;
```

| Valor | Descricao | Onde e usado |
|---|---|---|
| `KIND_VAR` | Simbolo e uma variavel escalar. | Inserido por `insert_symbol_ctx()` para variaveis simples e parametros simples |
| `KIND_ARRAY` | Simbolo e um array. | Inserido por `insert_array_ctx()`; verificado em `var` para garantir que so arrays podem ser indexados |
| `KIND_FUNC` | Simbolo e uma funcao. | Inserido por `insert_function_ctx()`; verificado em `call` para garantir que so funcoes podem ser chamadas |

### 3.2 Enumeracoes da AST

#### `NodeKind` - Categorias de nos

```c
typedef enum { STMTK, EXPK, VARK } NodeKind;
```

| Valor | Descricao |
|---|---|
| `STMTK` | No de comando (statement): declaracoes de tipo, `if`, `while`, `return`, compound |
| `EXPK` | No de expressao: operacao, constante, identificador, atribuicao, chamada, vetor |
| `VARK` | No de variavel/identificador: usado para nomes de variaveis, funcoes e arrays com metadados detalhados |

#### `StmtKind` - Subtipos de statement

```c
typedef enum { INTEGERK, VOIDK, IFK, WHILEK, RETURNK, COMPK } StmtKind;
```

| Valor | Descricao | Filhos (child[]) |
|---|---|---|
| `INTEGERK` | Declaracao com tipo `int` (variavel, array ou funcao) | `child[0]`: no `VARK` com o identificador declarado |
| `VOIDK` | Declaracao com tipo `void` (funcao) | `child[0]`: no `VARK` com o identificador declarado |
| `IFK` | Comando `if` (com ou sem `else`) | `child[0]`: condicao, `child[1]`: bloco then, `child[2]`: bloco else (ou NULL) |
| `WHILEK` | Comando `while` | `child[0]`: condicao, `child[1]`: corpo do loop |
| `RETURNK` | Comando `return` | `child[0]`: expressao de retorno (ou NULL para `return;`) |
| `COMPK` | Bloco composto `{ ... }` | `child[0]`: declaracoes locais (lista encadeada por `sibling`), `child[1]`: lista de statements (encadeada por `sibling`) |

#### `ExpKind` - Subtipos de expressao

```c
typedef enum { OPK, CONSTK, IDK, ASSIGNK, CALLK, VECTORK } ExpKind;
```

| Valor | Descricao | Campos relevantes |
|---|---|---|
| `OPK` | Operacao binaria (aritmetica ou relacional) | `op`: codigo do operador, `child[0]`: operando esquerdo, `child[1]`: operando direito |
| `CONSTK` | Constante numerica inteira | `kind.var.attr.val`: valor inteiro |
| `IDK` | Identificador simples | `kind.var.attr.name`: nome do identificador |
| `ASSIGNK` | Atribuicao `var = expressao` | `child[0]`: lado esquerdo (variavel), `child[1]`: lado direito (expressao) |
| `CALLK` | Chamada de funcao `func(args)` | `kind.var.attr.name`: nome da funcao, `child[0]`: lista de argumentos (encadeados por `sibling`) |
| `VECTORK` | Acesso a vetor `id[expr]` | `kind.var.attr.name`: nome do array, `child[0]`: expressao de indice (NULL para referencia ao array inteiro) |

#### `VarAccessK` - Modo de acesso a variavel

```c
typedef enum { DECLK, ACCESSK } VarAccessK;
```

| Valor | Descricao |
|---|---|
| `DECLK` | O no representa a **declaracao** de uma variavel, array ou funcao. Aparece quando o identificador esta sendo definido pela primeira vez. |
| `ACCESSK` | O no representa o **uso/acesso** a uma variavel, array ou funcao ja declarada. Aparece em expressoes e comandos que referenciam o identificador. |

Esta distincao e fundamental para a geracao do GraphViz (cores diferentes para declaracao vs. acesso) e para a geracao de codigo intermediario (declaracao gera instrucoes como `array`, acesso gera leitura de valor).

### 3.3 Estrutura `Identifier`

```c
typedef struct Identifier {
    TipoSimbolo varKind;   /* categoria: VAR, ARRAY ou FUNC */
    VarAccessK acesso;     /* modo: DECLK ou ACCESSK */
    union {
        int val;           /* valor numerico (para CONSTK) */
        char *name;        /* nome do identificador (para IDK, CALLK, VECTORK) */
    } attr;
} Identifier;
```

| Campo | Tipo | Descricao |
|---|---|---|
| `varKind` | `TipoSimbolo` | Indica se este identificador refere-se a uma variavel simples (`KIND_VAR`), um array (`KIND_ARRAY`) ou uma funcao (`KIND_FUNC`). E preenchido durante a construcao da AST com base na informacao da tabela de simbolos. |
| `acesso` | `VarAccessK` | Distingue se o no e uma declaracao (`DECLK`) ou um uso (`ACCESSK`). Preenchido na acao semantica da regra que cria o no. |
| `attr.val` | `int` | Armazena valor inteiro. Usado por nos `CONSTK` (constantes numericas) e por nos de tamanho de array. |
| `attr.name` | `char *` | Armazena nome do identificador como string alocada dinamicamente com `strdup()`. Usado por nos `IDK`, `CALLK`, `VECTORK` e nos `VARK`. |

A `union attr` compartilha memoria entre `val` e `name`, pois um no nunca precisa dos dois ao mesmo tempo: ou e uma constante (usa `val`) ou e um identificador (usa `name`).

### 3.4 Estrutura `TreeNode` - No da AST

```c
#define MAXCHILDREN 3

typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN];  /* ate 3 filhos */
    struct treeNode *sibling;             /* proximo irmao */
    int lineno;                           /* linha no codigo-fonte */
    NodeKind nodekind;                    /* categoria do no: STMTK, EXPK ou VARK */
    union {
        StmtKind stmt;                    /* subtipo de statement */
        ExpKind exp;                      /* subtipo de expressao */
        struct Identifier var;            /* dados de identificador/variavel */
    } kind;
    int op;                               /* token do operador (para OPK) */
    TipoVar type;                         /* tipo de dado do no */
} TreeNode;
```

#### Descricao detalhada de cada campo

| Campo | Tipo | Descricao detalhada |
|---|---|---|
| `child[0..2]` | `TreeNode*[3]` | Array de ate 3 ponteiros para nos filhos. O significado de cada filho depende do `nodekind` e subtipo do no. Para `IFK`: child[0]=condicao, child[1]=then, child[2]=else. Para `WHILEK`: child[0]=condicao, child[1]=corpo. Para `COMPK`: child[0]=declaracoes, child[1]=statements. Para `OPK`: child[0]=operando esquerdo, child[1]=operando direito. Para `ASSIGNK`: child[0]=variavel destino, child[1]=expressao valor. Para `CALLK`: child[0]=lista de argumentos. Para nos `VARK` de funcao: child[0]=parametros, child[1]=corpo. Para nos `VARK` de array: child[0]=indice (ou tamanho na declaracao). Filhos nao usados sao NULL. |
| `sibling` | `TreeNode*` | Ponteiro para o proximo no irmao, formando uma lista encadeada horizontal. Usado para encadear multiplas declaracoes, multiplos statements em um bloco, multiplos parametros de funcao, e multiplos argumentos de chamada. A construcao desta lista e feita percorrendo ate o fim dos siblings existentes e anexando o novo no. |
| `lineno` | `int` | Numero da linha no codigo-fonte onde este no foi criado, obtido de `yylineno`. Usado em mensagens de erro semantico para indicar a localizacao do problema ao usuario. |
| `nodekind` | `NodeKind` | Discriminador principal do no. Determina qual membro da union `kind` esta ativo: `STMTK` -> `kind.stmt`, `EXPK` -> `kind.exp`, `VARK` -> `kind.var`. E o primeiro campo verificado ao processar qualquer no. |
| `kind.stmt` | `StmtKind` | Ativo quando `nodekind == STMTK`. Indica o tipo especifico de comando: `INTEGERK`, `VOIDK`, `IFK`, `WHILEK`, `RETURNK` ou `COMPK`. |
| `kind.exp` | `ExpKind` | Ativo quando `nodekind == EXPK`. Indica o tipo especifico de expressao: `OPK`, `CONSTK`, `IDK`, `ASSIGNK`, `CALLK` ou `VECTORK`. |
| `kind.var` | `Identifier` | Ativo quando `nodekind == VARK`. Contem `varKind` (categoria), `acesso` (declaracao vs. uso) e `attr` (nome ou valor). Tambem e usado por nos `EXPK` para armazenar `attr.val` (em `CONSTK`) e `attr.name` (em `CALLK`, `IDK`, `VECTORK`). |
| `op` | `int` | Codigo do token do operador. Significativo apenas para nos `OPK`. Armazena constantes como `PLUS`, `MINUS`, `TIMES`, `DIVIDE`, `LT`, `LE`, `GT`, `GE`, `EQ`, `NE`. E 0 para todos os outros tipos de no. |
| `type` | `TipoVar` | Tipo de dado resultante deste no. Para expressoes, e o tipo do resultado da avaliacao. Para declaracoes, e o tipo declarado. Para nos de erro, e `TYPE_ERROR`. Inicializado como `TYPE_VOID` para statements e `TYPE_INT` para expressoes. Atualizado durante a analise semantica. |

### 3.5 Estrutura `Simbolo` - Entrada da tabela de simbolos

```c
typedef struct Simbolo {
    char *nome;               /* nome do identificador */
    TipoVar tipo;             /* tipo de dado */
    TipoSimbolo kind;         /* categoria: VAR, ARRAY ou FUNC */
    int tamanho;              /* tamanho do array (para arrays) */
    int num_params;           /* numero de parametros (para funcoes) */
    TipoVar *param_types;    /* tipos dos parametros */
    int linha;                /* linha de declaracao */
    int is_param;             /* 1 se este simbolo e um parametro */
    struct Escopo *def_scope; /* escopo do corpo (para funcoes) */
    struct Simbolo *prox;     /* proximo simbolo na lista */
} Simbolo;
```

| Campo | Tipo | Descricao detalhada |
|---|---|---|
| `nome` | `char *` | Nome do identificador, alocado com `strdup()`. Usado como chave de busca nas funcoes `lookup`. E comparado com `strcmp()` durante a resolucao de nomes. Liberado em `free_all_scopes_ctx()`. |
| `tipo` | `TipoVar` | Tipo de dado do simbolo. `TYPE_INT` para variaveis inteiras e funcoes que retornam int. `TYPE_VOID` para funcoes void. `TYPE_INT_ARRAY` para arrays. `TYPE_ERROR` nao e normalmente inserido. |
| `kind` | `TipoSimbolo` | Categoria do simbolo. Usada para distinguir variaveis (`KIND_VAR`), arrays (`KIND_ARRAY`) e funcoes (`KIND_FUNC`). Verificada em regras semanticas: so arrays podem ser indexados, so funcoes podem ser chamadas. |
| `tamanho` | `int` | Tamanho declarado do array (ex: `int x[10]` -> tamanho=10). Inicializado como 0 para nao-arrays. Preenchido por `insert_array_ctx()`. Exibido na tabela de simbolos. |
| `num_params` | `int` | Numero de parametros da funcao. Mantido por compatibilidade mas nao preenchido diretamente durante a insercao. O numero real e calculado em tempo de exibicao contando simbolos com `is_param=1` no `def_scope`. |
| `param_types` | `TipoVar *` | Array de tipos dos parametros. Alocado dinamicamente, mas **nao e preenchido** na implementacao atual. Mantido como campo reservado para futura validacao de tipos de argumentos em chamadas de funcao. Liberado em `free_all_scopes_ctx()`. |
| `linha` | `int` | Numero da linha no codigo-fonte onde o simbolo foi declarado. Obtido de `yylineno` no momento da insercao. Exibido na tabela de simbolos. |
| `is_param` | `int` | Flag booleana (0 ou 1). Marcada como 1 para simbolos que sao parametros de funcao. Preenchida na regra `param` apos insercao do simbolo. Usada em `ExibirTabelaSimbolos_ctx()` para contar parametros de funcoes percorrendo o `def_scope`. |
| `def_scope` | `Escopo *` | Ponteiro para o escopo do corpo da funcao. Preenchido apenas para simbolos `KIND_FUNC`, na regra `fun_declaration`, apos a criacao do escopo do corpo. Permite que `ExibirTabelaSimbolos_ctx()` acesse os parametros e locais da funcao para contagem. E NULL para variaveis e arrays. |
| `prox` | `Simbolo *` | Ponteiro para o proximo simbolo na lista encadeada do escopo. Novos simbolos sao inseridos no **inicio** da lista (`s->prox = escopo->simbolos; escopo->simbolos = s`), portanto a ordem e inversa a de declaracao. |

### 3.6 Estrutura `Escopo` - Escopo da tabela de simbolos

```c
typedef struct Escopo {
    Simbolo *simbolos;          /* lista de simbolos neste escopo */
    struct Escopo *pai;         /* escopo pai (encadeamento vertical) */
    struct Escopo *next_all;    /* proximo na lista global de escopos */
} Escopo;
```

| Campo | Tipo | Descricao detalhada |
|---|---|---|
| `simbolos` | `Simbolo *` | Cabeca da lista encadeada de simbolos declarados neste escopo. Novos simbolos sao inseridos no inicio (prepend), entao a iteracao percorre da declaracao mais recente para a mais antiga. Inicializado como NULL em `enter_scope_ctx()`. |
| `pai` | `Escopo *` | Ponteiro para o escopo envolvente (pai). Forma uma cadeia vertical de escopos, desde o mais interno ate o global. Usado por `lookup_symbol_ctx()` para buscar identificadores subindo na hierarquia de escopos. O escopo global tem `pai = NULL`. |
| `next_all` | `Escopo *` | Ponteiro para o proximo escopo na lista global de todos os escopos ja criados. Novos escopos sao inseridos no inicio (`novo->next_all = ctx->lista_escopos`), formando uma pilha. Usado por `ExibirTabelaSimbolos_ctx()` para iterar todos os escopos e exibir a tabela completa. Usado por `free_all_scopes_ctx()` para liberar toda a memoria. |

### 3.7 Estrutura `ParserContext` - Contexto global

```c
typedef struct ParserContext {
    struct treeNode *ast_root;       /* raiz da AST */
    struct Escopo *escopo_atual;     /* escopo ativo */
    struct Escopo *lista_escopos;    /* lista de todos os escopos */
    int has_errors;                  /* flag de erros */
} ParserContext;
```

| Campo | Tipo | Descricao detalhada |
|---|---|---|
| `ast_root` | `TreeNode *` | Ponteiro para a raiz da arvore sintatica abstrata. Preenchido na acao da regra `program` apos o parsing completo (`ctx->ast_root = $$`). Usado apos o parsing para gerar o GraphViz e o codigo intermediario. Inicializado como NULL. |
| `escopo_atual` | `Escopo *` | Ponteiro para o escopo corrente durante a analise. Atualizado por `enter_scope_ctx()` (aponta para o novo escopo) e `leave_scope_ctx()` (volta para o `pai`). Todas as insercoes e buscas de simbolos usam este ponteiro como ponto de partida. |
| `lista_escopos` | `Escopo *` | Cabeca da lista encadeada de todos os escopos ja criados (via `next_all`). Permite percorrer todos os escopos apos o parsing para exibicao da tabela e liberacao de memoria. |
| `has_errors` | `int` | Flag booleana. Inicializada como 0 (sem erros). Marcada como 1 por qualquer erro sintatico ou semantico. Consultada ao final: se `has_errors == 1`, a geracao de AST/DOT e codigo intermediario e suprimida. |

---

## 4. Analise Sintatica

### 4.1 Ferramenta e metodo

A analise sintatica e realizada pelo Bison, um gerador de parsers LALR(1). O Bison le a especificacao em `cminusSintSem.y` e gera o parser em C (`cminus.tab.c` e `cminus.tab.h`).

O parser utiliza o metodo **bottom-up** (ascendente): le tokens da esquerda para a direita, constroi a arvore de derivacao da folha para a raiz, realizando **reducoes** quando reconhece o lado direito de uma producao.

### 4.2 Configuracoes do parser

```
%expect 1
%error-verbose
%parse-param {ParserContext *ctx}
```

- **`%expect 1`**: informa ao Bison que exatamente 1 conflito shift/reduce e esperado. Este conflito surge da ambiguidade classica do *dangling else* (`if-then` vs `if-then-else`). O Bison resolve por padrao com *shift*, o que associa o `else` ao `if` mais interno (comportamento desejado).
- **`%error-verbose`**: habilita mensagens de erro detalhadas. Quando ocorre erro sintatico, o Bison inclui na mensagem quais tokens eram esperados e qual token foi encontrado.
- **`%parse-param`**: adiciona um parametro extra `ParserContext *ctx` a funcao `yyparse()`, permitindo que todas as acoes semanticas acessem o contexto global sem variaveis globais.

### 4.3 Declaracao de precedencia

```
%left PLUS MINUS
%left TIMES DIVIDE
%right ASSIGN
%left LT LE GT GE EQ NE
```

Essas diretivas resolvem ambiguidades de precedencia e associatividade:

| Diretiva | Tokens | Efeito |
|---|---|---|
| `%left PLUS MINUS` | `+`, `-` | Associatividade a esquerda; mesma precedencia entre si |
| `%left TIMES DIVIDE` | `*`, `/` | Associatividade a esquerda; precedencia maior que `+`, `-` |
| `%right ASSIGN` | `=` | Associatividade a direita (ex: `a = b = 1` avalia como `a = (b = 1)`) |
| `%left LT LE GT GE EQ NE` | `<`, `<=`, `>`, `>=`, `==`, `!=` | Associatividade a esquerda |

### 4.4 Gramatica implementada

A gramatica segue a especificacao da linguagem C-Minus. Cada regra gramatical possui acoes semanticas em C que constroem a AST e realizam verificacoes semanticas simultaneamente.

#### Regra 1: `program -> declaration_list`

Ponto de entrada da gramatica. A acao armazena a raiz da AST em `ctx->ast_root` e imprime mensagens de conclusao.

#### Regra 2: `declaration_list -> declaration_list declaration | declaration`

Constroi uma lista encadeada de declaracoes usando `sibling`. O padrao de encadeamento e:

```c
TreeNode *t = $1;
if (t != NULL) {
    while (t->sibling != NULL)
        t = t->sibling;
    t->sibling = $2;
    $$ = $1;
} else {
    $$ = $2;
}
```

Este padrao percorre ate o final da lista de siblings existente e anexa o novo no. E reutilizado em `param_list`, `local_declarations`, `statement_list` e `arg_list`.

#### Regra 4: `var_declaration`

Duas alternativas:

**Variavel simples** (`type_specifier ID SEMI`):
1. Insere o simbolo na tabela via `insert_symbol_ctx()`
2. Cria no `STMTK` com subtipo `INTEGERK` ou `VOIDK`
3. Cria no filho `VARK` com `IDK`, marcado como `DECLK` e `KIND_VAR`

**Array** (`type_specifier ID LBRACK NUM RBRACK SEMI`):
1. Verifica se o tipo nao e void
2. Insere na tabela via `insert_array_ctx()`
3. Cria no `STMTK` com subtipo `INTEGERK`/`VOIDK`
4. Cria no filho `VARK` com `VECTORK`, marcado como `DECLK` e `KIND_ARRAY`
5. Cria no neto `EXPK`/`CONSTK` com o tamanho do array

#### Regra 6: `fun_declaration`

Regra com acao intermediaria (mid-rule action):

```
type_specifier ID { /* acao intermediaria */ } LPAREN params RPAREN compound_stmt
```

A acao intermediaria:
1. Insere a funcao na tabela de simbolos no escopo atual
2. Abre um novo escopo para parametros e corpo
3. Liga o escopo ao simbolo da funcao via `def_scope`

A acao final:
1. Fecha o escopo da funcao
2. Cria o no da AST com a estrutura funcao

#### Regra 10: `compound_stmt -> { local_declarations statement_list }`

Cria um no `COMPK` com:
- `child[0]`: lista de declaracoes locais
- `child[1]`: lista de statements

#### Regra 13: `compound_stmt_with_scope`

Wrapper que cria e destroi um escopo ao redor de `compound_stmt` quando usado como statement isolado. Isso garante que blocos `{ }` usados diretamente como comandos tenham escopo proprio.

#### Regra 15: `selection_stmt` (if/if-else)

Cria no `IFK` com ate 3 filhos. Realiza verificacao semantica da condicao.

#### Regra 16: `iteration_stmt` (while)

Cria no `WHILEK` com 2 filhos. Realiza verificacao semantica da condicao.

#### Regra 18: `expression` (atribuicao)

Para `var ASSIGN expression`:
1. Verifica se o destino nao e array completo
2. Verifica compatibilidade de tipos
3. Cria no `ASSIGNK`

#### Regra 19: `var` (acesso a variavel)

Busca o simbolo na tabela, verifica existencia, e cria o no apropriado:
- `ID` simples: `IDK` ou `VECTORK` (se o simbolo for array)
- `ID[expr]`: `VECTORK` com verificacoes de array e tipo do indice

#### Regras 20-25: Expressoes aritmeticas e relacionais

Seguem o padrao:
1. Geram expressoes dos operandos recursivamente
2. Verificam tipos via `check_expression_type_ctx()`
3. Criam no `OPK` com o operador armazenado em `op`

#### Regra 27: `call` (chamada de funcao)

1. Busca a funcao na tabela de simbolos
2. Verifica que o simbolo existe e e `KIND_FUNC`
3. Cria no `CALLK` com nome da funcao e argumentos como filhos

### 4.5 Tratamento de erros sintaticos

A funcao `yyerror()` processa as mensagens do Bison e as traduz para portugues:

1. Extrai os tokens "unexpected" e "expecting" da mensagem do Bison
2. Traduz nomes de tokens para formas amigaveis usando `translate_token()` (ex: `SEMI` -> `';'`, `ID` -> `identificador`)
3. Trata casos especiais:
   - `$end` inesperado -> sugere delimitador nao fechado
   - `ELSE` inesperado -> sugere `else` sem `if`
   - Contexto de statement -> mensagem mais descritiva
4. Para multiplos tokens esperados, traduz cada um e os separa com "ou"

---

## 5. Arvore Sintatica Abstrata (AST)

### 5.1 Funcoes construtoras de nos

O compilador possui tres funcoes para criar nos da AST, cada uma inicializando campos de forma adequada ao tipo de no:

#### `newStmtNode(StmtKind kind)`

- Aloca um `TreeNode` com `malloc`
- Define `nodekind = STMTK` e `kind.stmt = kind`
- Inicializa `type = TYPE_VOID` (statements nao tem tipo por padrao)
- Inicializa `op = 0` e todos os `child[]` e `sibling` como NULL
- Define `lineno = yylineno`

#### `newExpNode(ExpKind kind)`

- Aloca um `TreeNode` com `malloc`
- Define `nodekind = EXPK` e `kind.exp = kind`
- Inicializa `type = TYPE_INT` (expressoes tem tipo inteiro por padrao)
- Inicializa `kind.var.attr.name = NULL` (previne ponteiro pendente)
- Define `lineno = yylineno`

#### `newVarNode(ExpKind kind)`

- Aloca um `TreeNode` com `malloc`
- Define `nodekind = VARK`
- Inicializa `kind.var.acesso = DECLK` e `kind.var.varKind = KIND_VAR`
- Inicializa `kind.var.attr.name = NULL`
- Define `type = TYPE_INT`
- Define `lineno = yylineno`

### 5.2 Estrutura hierarquica da AST

A AST tem uma estrutura mista de arvore e lista:

- **Relacoes pai-filho**: representadas pelo array `child[0..2]`, expressam subordinacao estrutural (ex: condicao pertence ao `if`, operandos pertencem a operacao).
- **Relacoes de sequencia**: representadas por `sibling`, expressam sequencia temporal ou enumeracao (ex: lista de statements em um bloco, lista de parametros, lista de argumentos).

Exemplo conceitual para `int x; x = 1 + 2;`:

```
[INTEGERK]                    (declaracao int)
  child[0]: [Var: x, DECLK]  (nome da variavel declarada)
  sibling --> [ASSIGNK]       (atribuicao)
                child[0]: [x, ACCESSK]  (variavel destino)
                child[1]: [OPK: +]      (operacao)
                             child[0]: [CONSTK: 1]
                             child[1]: [CONSTK: 2]
```

### 5.3 Padrao de construcao de listas por sibling

O padrao repetido em varias regras para construir listas encadeadas e:

```c
TreeNode *t = $1;           /* cabeca da lista existente */
if (t != NULL) {
    while (t->sibling != NULL)
        t = t->sibling;    /* percorre ate o ultimo */
    t->sibling = $2;        /* anexa novo no ao final */
    $$ = $1;                /* retorna a cabeca original */
} else {
    $$ = $2;                /* lista vazia, novo no e a cabeca */
}
```

Este padrao e O(n) por insercao, resultando em O(n^2) para construir uma lista de n elementos. Para os tamanhos tipicos de programas C-Minus, isso nao e um problema pratico.

### 5.4 Visualizacao GraphViz

O compilador gera um arquivo `ast.dot` com representacao visual da AST para o Graphviz. A funcao `printTreeDOT()` utiliza um contexto `DotContext` contendo:

- `node_counter`: contador global de nos para IDs unicos no DOT
- `fp`: ponteiro do arquivo de saida

A funcao `printTreeDOT_simplified()` percorre a AST recursivamente, aplicando simplificacoes visuais:
- Nos `INTEGERK`/`VOIDK` sao **pulados** (seus filhos sao conectados diretamente ao pai)
- Nos `COMPK` sao **pulados** (compound statements transparentes)
- Cada tipo de no recebe forma e cor distintas:

| Tipo de no | Forma | Cor |
|---|---|---|
| IF, WHILE | `diamond` | `orange` |
| RETURN | `box` | `pink` |
| Function | `ellipse` | `gold` |
| Array (decl) | `box` | `lightgreen` |
| Var (decl) | `box` | `lightgreen` |
| Var (acesso) | `box` | `lightyellow` |
| Operador | `circle` | `lavender` |
| Constante | `circle` | `lightpink` |
| Atribuicao | `circle` | `khaki` |
| Chamada | `box` | `peachpuff` |

---

## 6. Tabela de Simbolos e Gerenciamento de Escopos

### 6.1 Modelo de escopos

O compilador implementa escopos aninhados usando uma lista encadeada em pilha:

```
Escopo Global (pai = NULL)
    |
    +-- Escopo da funcao gcd (pai = Global)
    |
    +-- Escopo da funcao main (pai = Global)
```

A variavel `ctx->escopo_atual` sempre aponta para o escopo mais interno ativo. A cadeia `pai` permite subir na hierarquia de escopos para resolver nomes.

### 6.2 Funcoes de gerenciamento de escopo

#### `enter_scope_ctx(ParserContext *ctx)`

```c
void enter_scope_ctx(ParserContext *ctx) {
    Escopo *novo = (Escopo*) malloc(sizeof(Escopo));
    novo->simbolos = NULL;
    novo->pai = ctx->escopo_atual;
    novo->next_all = ctx->lista_escopos;
    ctx->lista_escopos = novo;
    ctx->escopo_atual = novo;
}
```

1. Aloca novo escopo
2. Liga ao escopo atual como pai
3. Insere no inicio da lista global (`next_all`)
4. Atualiza `escopo_atual` para o novo escopo

Chamada em:
- `main()`: cria escopo global
- `fun_declaration`: cria escopo da funcao (para parametros e corpo)
- `compound_stmt_with_scope`: cria escopo de bloco

#### `leave_scope_ctx(ParserContext *ctx)`

```c
void leave_scope_ctx(ParserContext *ctx) {
    if (!ctx->escopo_atual) return;
    ctx->escopo_atual = ctx->escopo_atual->pai;
}
```

Simplesmente sobe para o escopo pai. O escopo abandonado **nao** e liberado imediatamente -- permanece na lista global para exibicao posterior e e liberado ao final.

Chamada em:
- `fun_declaration` (apos processar corpo)
- `compound_stmt_with_scope` (apos processar bloco)

### 6.3 Funcoes de busca

#### `lookup_symbol_current_ctx()` - Busca local

Percorre apenas a lista de simbolos do escopo atual. Usada para detectar redeclaracao: antes de inserir um novo simbolo, verifica se ja existe no mesmo escopo.

#### `lookup_symbol_ctx()` - Busca hierarquica

Percorre a cadeia de escopos desde o atual ate o global:

```c
Escopo *e = ctx->escopo_atual;
while (e) {
    /* busca no escopo e */
    e = e->pai;  /* sobe para o pai */
}
```

Implementa a regra de visibilidade: um identificador e visivel se foi declarado no escopo atual ou em qualquer escopo ancestral. A busca para no primeiro casamento encontrado, implementando *shadowing* natural (variavel local com mesmo nome de global prevalece).

### 6.4 Funcoes de insercao

#### `insert_symbol_ctx()` - Insercao de variavel simples

1. Verifica se ha escopo ativo
2. Verifica redeclaracao no escopo atual
3. Aloca `Simbolo`, preenche todos os campos
4. Insere no inicio da lista do escopo (prepend)
5. Inicializa `is_param = 0` e `def_scope = NULL`

#### `insert_array_ctx()` - Insercao de array

Similar a `insert_symbol_ctx()`, mas:
- Define `tipo = TYPE_INT_ARRAY`
- Define `kind = KIND_ARRAY`
- Define `tamanho` com o valor passado como argumento

#### `insert_function_ctx()` - Insercao de funcao

Similar a `insert_symbol_ctx()`, mas:
- Define `kind = KIND_FUNC`
- `tipo` recebe o tipo de retorno (int ou void)
- `def_scope` sera preenchido posteriormente pela regra `fun_declaration`

### 6.5 Exibicao da tabela

A funcao `ExibirTabelaSimbolos_ctx()` exibe a tabela formatada:

1. Conta todos os escopos na lista global
2. Aloca array de ponteiros para os escopos
3. Percorre do escopo mais antigo (ultimo na lista) ao mais recente, numerando-os a partir de 0
4. Para cada simbolo, exibe: nome, tipo, categoria, numero do escopo, tamanho (arrays), numero de parametros (funcoes) e linha
5. O numero de parametros de funcoes e calculado **dinamicamente** contando simbolos com `is_param=1` no `def_scope`

### 6.6 Liberacao de memoria

`free_all_scopes_ctx()` percorre a lista global de escopos via `next_all`, liberando cada simbolo e cada escopo. Para cada simbolo, libera `nome` e `param_types` (se existir).

---

## 7. Analise Semantica

### 7.1 Estrategia de implementacao

A analise semantica e **integrada ao parser** (syntax-directed translation). As verificacoes ocorrem dentro das acoes semanticas das regras gramaticais, durante as reducoes do Bison. Isso significa que a analise semantica acontece simultaneamente com a construcao da AST.

Nao existe uma fase separada de travessia da AST para checagem de tipos. As vantagens desta abordagem sao:
- Implementacao mais simples (unico passo)
- Acesso direto ao contexto do parser (`yylineno`, `yytext`)
- Tabela de simbolos disponivel no momento da verificacao

### 7.2 Funcao central de verificacao de tipos

```c
TipoVar check_expression_type_ctx(ParserContext *ctx, const char *op,
                                   TipoVar t1, TipoVar t2, int linha)
```

Esta funcao e chamada em todas as operacoes binarias (aritmeticas e relacionais):

1. Se algum operando e `TYPE_ERROR`: retorna `TYPE_ERROR` silenciosamente (propagacao de erro sem cascata)
2. Se algum operando e `TYPE_VOID`: emite erro "operacao X com tipo void"
3. Se ambos nao sao `TYPE_INT`: emite erro "operacao X requer operandos inteiros"
4. Se ambos sao `TYPE_INT`: retorna `TYPE_INT`

O parametro `op` e uma string descritiva (`"relacional"`, `"aditivo"`, `"multiplicativo"`) usada na mensagem de erro.

### 7.3 Catalogo completo de verificacoes semanticas

#### 7.3.1 Declaracoes

| Verificacao | Local no codigo | Mensagem de erro |
|---|---|---|
| Redeclaracao no mesmo escopo | `insert_symbol_ctx()`, `insert_array_ctx()`, `insert_function_ctx()` | `identificador '<nome>' ja declarado neste escopo` |
| Array com tipo void | `var_declaration` (segunda alternativa) | `array '<nome>' nao pode ser void` |
| Parametro simples void | `param` (primeira alternativa) | `parametro '<nome>' nao pode ser void` |
| Parametro array void | `param` (segunda alternativa) | `parametro array '<nome>' nao pode ser void` |

#### 7.3.2 Uso de variaveis

| Verificacao | Local no codigo | Mensagem de erro |
|---|---|---|
| Variavel nao declarada | `var` (ambas alternativas) | `variavel '<nome>' nao declarada` |
| Indexacao de nao-array | `var` (segunda alternativa) | `'<nome>' nao e um array` |
| Indice nao inteiro | `var` (segunda alternativa) | `indice de array deve ser inteiro` |

#### 7.3.3 Atribuicoes

| Verificacao | Local no codigo | Mensagem de erro |
|---|---|---|
| Atribuicao a array completo | `expression` (primeira alternativa) | `nao e possivel atribuir a array completo` |
| Tipos incompativeis | `expression` (primeira alternativa) | `tipos incompativeis na atribuicao` |

#### 7.3.4 Expressoes e operacoes

| Verificacao | Local no codigo | Mensagem de erro |
|---|---|---|
| Operando void em operacao | `check_expression_type_ctx()` | `operacao <tipo> com tipo void` |
| Operando nao inteiro | `check_expression_type_ctx()` | `operacao <tipo> requer operandos inteiros` |

#### 7.3.5 Comandos de controle

| Verificacao | Local no codigo | Mensagem de erro |
|---|---|---|
| Condicao do if nao inteira | `selection_stmt` | `condicao do IF deve ser inteira` |
| Condicao do while nao inteira | `iteration_stmt` | `condicao do WHILE deve ser inteira` |

#### 7.3.6 Chamadas de funcao

| Verificacao | Local no codigo | Mensagem de erro |
|---|---|---|
| Funcao nao declarada | `call` | `funcao '<nome>' nao declarada` |
| Identificador nao e funcao | `call` | `'<nome>' nao e uma funcao` |

### 7.4 Funcoes built-in

Antes do parsing, a `main()` insere no escopo global:
- `input`: funcao que retorna `TYPE_INT`, 0 parametros
- `output`: funcao que retorna `TYPE_VOID`, 0 parametros

Isso permite que o usuario chame essas funcoes sem declara-las explicitamente.

### 7.5 Propagacao de erros

O mecanismo de `TYPE_ERROR` evita cascata de mensagens: se uma subexpressao ja tem tipo de erro, operacoes que a utilizam simplesmente propagam o erro sem gerar nova mensagem. Alem disso, `ctx->has_errors` e marcado uma unica vez e nunca resetado, garantindo que qualquer erro detectado impeca a geracao de codigo.

### 7.6 Limitacoes da analise semantica

As seguintes verificacoes **nao** estao implementadas:
- Verificacao da quantidade de argumentos em chamadas de funcao
- Verificacao dos tipos dos argumentos passados
- Verificacao de compatibilidade de tipo em `return`
- Exigencia da existencia da funcao `main`
- Proibicao de declaracao de variavel simples `void`

---

## 8. Geracao de Codigo Intermediario

### 8.1 Visao geral

A geracao de codigo intermediario e implementada em `code_generator.c` e produz **codigo de tres enderecos** (Three-Address Code, TAC). Este codigo e impresso na saida padrao (`stdout`).

A geracao e ativada apenas se `ctx->has_errors == 0`, ou seja, se nao houve erros sintaticos ou semanticos.

### 8.2 Variaveis globais do gerador

```c
static int contadorTemporarios = 0;
static int contadorLabels = 0;
```

| Variavel | Tipo | Descricao |
|---|---|---|
| `contadorTemporarios` | `int` | Contador global para nomes de variaveis temporarias. Incrementado a cada chamada de `novoTemporario()`. Gera nomes `t0`, `t1`, `t2`, ... |
| `contadorLabels` | `int` | Contador global para nomes de labels. Incrementado a cada chamada de `novoLabel()`. Gera nomes `L0`, `L1`, `L2`, ... |

### 8.3 Funcoes auxiliares

#### `novoTemporario()`

Aloca string de ate 10 caracteres e gera nome `tN` onde N e o contador atual. Retorna ponteiro para a string alocada.

#### `novoLabel()`

Aloca string de ate 10 caracteres e gera nome `LN` onde N e o contador atual. Retorna ponteiro para a string alocada.

#### `gerarOffsetBytes(char* indice)`

Gera uma instrucao de multiplicacao por 4 para converter indice de array em offset de bytes:

```
tN = indice * 4
```

Isso assume que cada elemento do array ocupa 4 bytes (tamanho de `int`). Retorna o nome do temporario contendo o offset.

### 8.4 Arquitetura do gerador

O gerador utiliza tres funcoes mutuamente recursivas:

```
codeGen()
    |
    +-> percorrerArvore()     -- percorre siblings no nivel superior
            |
            +-> gerarComando()         -- gera codigo para statements
            |       |
            |       +-> gerarExpressao()       -- gera codigo para expressoes
            |       +-> gerarComandoExpressao() -- gera codigo para expressoes-comando
            |
            +-> gerarComandoExpressao() -- gera codigo para expressoes no nivel superior
```

### 8.5 Funcao `codeGen(TreeNode* arvoreSintatica)`

Ponto de entrada. Imprime cabecalho, chama `percorrerArvore()`, imprime rodape.

### 8.6 Funcao `percorrerArvore(TreeNode* no)`

Percorre a lista de nos no nivel superior (ligados por `sibling`). Para cada no:
- Se `STMTK`: chama `gerarComando()`
- Se `EXPK`: chama `gerarComandoExpressao()`

### 8.7 Funcao `gerarExpressao(TreeNode* no)` - Detalhamento

Esta funcao gera codigo para expressoes e **retorna o nome** do temporario ou variavel onde o resultado esta armazenado.

#### Caso `CONSTK` (constante numerica)

```
tN = valor
```

Aloca um temporario e atribui o valor constante. Retorna o nome do temporario.

#### Caso `IDK` (identificador simples)

Retorna diretamente `no->kind.var.attr.name` sem gerar nenhuma instrucao. A variavel ja existe no contexto do programa.

#### Caso `OPK` (operacao binaria)

1. Gera recursivamente codigo para o operando esquerdo -> `esquerda`
2. Gera recursivamente codigo para o operando direito -> `direita`
3. Aloca novo temporario
4. Traduz o token do operador para string (`+`, `-`, `*`, `/`, `<`, `<=`, `>`, `>=`, `==`, `!=`)
5. Emite: `tN = esquerda op direita`
6. Retorna o nome do temporario

#### Caso `CALLK` (chamada de funcao como expressao)

1. Percorre a lista de argumentos (por `sibling`)
2. Para cada argumento, gera expressao e emite: `param tempArg`
3. Conta o numero de argumentos
4. Aloca temporario para o retorno
5. Emite: `tN = call nomeFuncao, numArgs`
6. Retorna o nome do temporario

#### Caso `VARK` com `KIND_ARRAY` e indice

1. Gera expressao para o indice
2. Calcula offset em bytes via `gerarOffsetBytes()`
3. Aloca temporario
4. Emite: `tN = nomeArray[offset]`
5. Retorna o nome do temporario

#### Caso `VARK` sem indice (variavel simples)

Retorna diretamente `no->kind.var.attr.name`.

### 8.8 Funcao `gerarComando(TreeNode* no)` - Detalhamento

Gera codigo para nos do tipo `STMTK`.

#### Caso `IFK` (comando if / if-else)

**if sem else:**
```
<codigo da condicao -> teste>
if_false teste goto Lfalso
<codigo do bloco then>
Lfalso:
```

**if com else:**
```
<codigo da condicao -> teste>
if_false teste goto Lfalso
<codigo do bloco then>
goto Lfim
Lfalso:
<codigo do bloco else>
Lfim:
```

Estrategia: gera dois labels (falso e fim). Se ha else, o bloco then termina com um `goto` para pular o bloco else.

#### Caso `WHILEK` (comando while)

```
Linicio:
<codigo da condicao -> teste>
if_false teste goto Lfim
<codigo do corpo>
goto Linicio
Lfim:
```

Estrategia: gera label de inicio e fim. O corpo e executado enquanto a condicao for verdadeira. Ao fim do corpo, retorna ao inicio para reavaliar a condicao.

#### Caso `RETURNK` (comando return)

**Com expressao:**
```
<codigo da expressao -> valor>
return valor
```

**Sem expressao:**
```
return
```

#### Caso `COMPK` (bloco composto)

Percorre sequencialmente:
1. `child[0]` (declaracoes locais): para cada declaracao, chama `gerarComando()`
2. `child[1]` (statements): para cada statement, chama `gerarComando()` ou `gerarComandoExpressao()` conforme o `nodekind`

#### Caso `INTEGERK` / `VOIDK` (declaracoes)

Examina o `child[0]` para determinar o tipo de declaracao:

**Declaracao de funcao (`KIND_FUNC`):**
```
func nomeFuncao:
param nomeParam1
param nomeParam2
<codigo do corpo>
endfunc
```

**Declaracao de array (`KIND_ARRAY` com `DECLK`):**
```
array nomeArray[tamanho]
```

**Declaracao de variavel simples:** nao gera codigo (a variavel existe implicitamente).

### 8.9 Funcao `gerarComandoExpressao(TreeNode* no)` - Detalhamento

Gera codigo para expressoes que aparecem como statements (sem valor de retorno utilizado).

#### Caso `ASSIGNK` (atribuicao)

**Para variavel simples:**
```
<codigo da expressao -> valor>
nomeVar = valor
```

**Para acesso a array:**
```
<codigo do indice -> indice>
<codigo da expressao -> valor>
nomeArray[indice] = valor
```

#### Caso `CALLK` (chamada de funcao como statement)

```
param arg1
param arg2
call nomeFuncao, numArgs
```

Diferenca em relacao ao `CALLK` como expressao: nao aloca temporario para o retorno, pois o resultado e descartado.

### 8.10 Estrategia geral da geracao de codigo

1. **Travessia top-down da AST**: o gerador percorre a arvore de cima para baixo, processando declaracoes e statements na ordem em que aparecem.

2. **Avaliacao bottom-up de expressoes**: dentro de `gerarExpressao()`, os operandos sao avaliados recursivamente antes do operador, garantindo que os temporarios dos operandos estejam prontos.

3. **Nomeacao de temporarios**: temporarios sao nomeados sequencialmente (`t0`, `t1`, ...) sem reutilizacao. Isso simplifica a implementacao mas pode gerar muitos temporarios.

4. **Nomeacao de labels**: labels sao nomeados sequencialmente (`L0`, `L1`, ...`) e usados para desvios condicionais e incondicionais.

5. **Acesso a arrays**: indices sao multiplicados por 4 para converter em offset de bytes antes do acesso.

6. **Passagem de parametros**: argumentos sao avaliados e passados com `param` antes da instrucao `call`.

7. **Funcoes**: delimitadas por `func nome:` e `endfunc`, com parametros listados no inicio.

---

## 9. Fluxo Completo de Execucao

A funcao `main()` em `cminusSintSem.y` orquestra todo o processo:

```
1. Abre arquivo de entrada (ou stdin)
2. Cria ParserContext via parser_context_create()
3. Cria escopo global via enter_scope_ctx()
4. Insere funcoes built-in (input, output)
5. Executa yyparse(ctx) -- analise lexica + sintatica + semantica
   - Flex tokeniza a entrada
   - Bison reduz producoes, executando acoes semanticas
   - AST e construida incrementalmente
   - Tabela de simbolos e atualizada durante as reducoes
   - Erros sao acumulados via has_errors
6. Exibe tabela de simbolos via ExibirTabelaSimbolos_ctx()
7. Se nao ha erros:
   a. Gera arquivo GraphViz (ast.dot) via printTreeDOT()
   b. Gera codigo intermediario via codeGen()
8. Libera memoria:
   a. free_all_scopes_ctx() -- escopos e simbolos
   b. parser_context_destroy() -- contexto
   c. fclose(yyin) -- arquivo de entrada
9. Retorna 0 (sucesso) ou 1 (erros)
```

### Condicao para geracao de saida

A geracao do arquivo DOT e do codigo intermediario e **condicional**:

```c
if (!ctx->has_errors && ctx->ast_root) {
    printTreeDOT(ctx->ast_root, "ast.dot");
    codeGen(ctx->ast_root);
}
```

Ambas as condicoes devem ser verdadeiras:
- Nenhum erro sintatico ou semantico foi detectado
- A AST foi construida (a raiz nao e NULL)

---

## Apendice A: Mapeamento de Regras Gramaticais para Nos da AST

| Regra gramatical | NodeKind | Subtipo | Filhos |
|---|---|---|---|
| `var_declaration (simples)` | `STMTK` | `INTEGERK`/`VOIDK` | child[0]: VARK/IDK |
| `var_declaration (array)` | `STMTK` | `INTEGERK`/`VOIDK` | child[0]: VARK/VECTORK -> child[0]: EXPK/CONSTK |
| `fun_declaration` | `STMTK` | `INTEGERK`/`VOIDK` | child[0]: VARK/IDK(FUNC) -> child[0]: params, child[1]: corpo |
| `param (simples)` | `STMTK` | `INTEGERK`/`VOIDK` | child[0]: VARK/IDK |
| `param (array)` | `STMTK` | `INTEGERK`/`VOIDK` | child[0]: VARK/VECTORK |
| `compound_stmt` | `STMTK` | `COMPK` | child[0]: decls, child[1]: stmts |
| `selection_stmt (if)` | `STMTK` | `IFK` | child[0]: cond, child[1]: then, child[2]: else/NULL |
| `iteration_stmt` | `STMTK` | `WHILEK` | child[0]: cond, child[1]: corpo |
| `return_stmt` | `STMTK` | `RETURNK` | child[0]: expr/NULL |
| `expression (assign)` | `EXPK` | `ASSIGNK` | child[0]: var, child[1]: expr |
| `simple_expression (relop)` | `EXPK` | `OPK` | child[0]: esquerdo, child[1]: direito |
| `additive_expression (addop)` | `EXPK` | `OPK` | child[0]: esquerdo, child[1]: direito |
| `term (mulop)` | `EXPK` | `OPK` | child[0]: esquerdo, child[1]: direito |
| `factor (NUM)` | `EXPK` | `CONSTK` | (folha) |
| `call` | `EXPK` | `CALLK` | child[0]: argumentos |
| `var (simples)` | `VARK` | IDK/VECTORK | (folha) |
| `var (indexado)` | `VARK` | `VECTORK` | child[0]: indice |

## Apendice B: Ciclo de vida de um simbolo

```
1. [Declaracao encontrada pelo parser]
       |
2. [Verificacao de redeclaracao no escopo atual]
       | (se ja existe -> ERRO, para)
       v
3. [Alocacao e preenchimento do Simbolo]
       |
4. [Insercao no inicio da lista do escopo atual]
       |
5. [Marcacao especial (is_param, def_scope) se aplicavel]
       |
6. [Consultas via lookup durante expressoes e comandos]
       |
7. [Exibicao na tabela apos parsing]
       |
8. [Liberacao em free_all_scopes_ctx()]
```
