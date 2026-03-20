# Funcoes e Mecanismos da Analise Lexica

Este documento descreve em detalhes todos os mecanismos, funcoes, variaveis globais e regras da analise lexica do compilador C-Minus, implementada com Flex no arquivo `cminusLex.l`. Cada elemento e explicado com seu objetivo, parametros (quando aplicavel), logica interna e locais de uso.

---

## Sumario

1. [Visao Geral do Flex](#1-visao-geral-do-flex)
2. [Variaveis Globais do Scanner](#2-variaveis-globais-do-scanner)
3. [Opcoes de Configuracao](#3-opcoes-de-configuracao)
4. [A Funcao yylex](#4-a-funcao-yylex)
5. [Estados do Scanner](#5-estados-do-scanner)
6. [Regras de Reconhecimento de Espacos em Branco](#6-regras-de-reconhecimento-de-espacos-em-branco)
7. [Regras de Reconhecimento de Comentarios](#7-regras-de-reconhecimento-de-comentarios)
8. [Regras de Reconhecimento de Palavras Reservadas](#8-regras-de-reconhecimento-de-palavras-reservadas)
9. [Regra de Reconhecimento de Identificadores](#9-regra-de-reconhecimento-de-identificadores)
10. [Regra de Reconhecimento de Numeros](#10-regra-de-reconhecimento-de-numeros)
11. [Regras de Reconhecimento de Operadores Relacionais](#11-regras-de-reconhecimento-de-operadores-relacionais)
12. [Regras de Reconhecimento de Operadores e Pontuacao](#12-regras-de-reconhecimento-de-operadores-e-pontuacao)
13. [Regra de Tratamento de Erro Lexico](#13-regra-de-tratamento-de-erro-lexico)
14. [Mecanismo de Comunicacao Lexer-Parser](#14-mecanismo-de-comunicacao-lexer-parser)
15. [Processo Completo de Tokenizacao](#15-processo-completo-de-tokenizacao)

---

## 1. Visao Geral do Flex

O Flex (Fast Lexical Analyzer Generator) e a ferramenta usada para gerar o scanner (analisador lexico) do compilador. O arquivo de especificacao `cminusLex.l` e processado pelo Flex para produzir `lex.yy.c`, que contem a funcao `yylex()`.

O arquivo `.l` tem tres secoes separadas por `%%`:

```
Secao de definicoes
%%
Secao de regras (padroes e acoes)
%%
Secao de codigo auxiliar (vazia neste compilador)
```

O scanner opera como um **automato finito deterministico** (DFA): le caracteres da entrada sequencialmente, tenta casar com os padroes definidos e, quando um padrao e casado, executa a acao correspondente (geralmente retornar um token para o parser).

---

## 2. Variaveis Globais do Scanner

O Flex e o Bison compartilham varias variaveis globais que formam a interface entre a analise lexica e a analise sintatica.

### 2.1 `yyin`

**Tipo:** `FILE *`

**Definicao:** Variavel global do Flex, declarada como `extern` no Bison:
```c
extern FILE *yyin;
```

**Objetivo:** Ponteiro para o arquivo de entrada que o scanner le. O scanner le caracteres de `yyin` para realizar a analise lexica.

**Valor padrao:** `stdin` (entrada padrao). Se nenhum arquivo for aberto, o scanner le do terminal.

**Onde e configurada:** Na `main()` de `cminusSintSem.y`:
```c
if (argc > 1) {
    yyin = fopen(argv[1], "r");  // le do arquivo passado como argumento
} else {
    yyin = stdin;                 // le da entrada padrao
}
```

**Quem consome:** A funcao `yylex()` gerada pelo Flex le caracteres de `yyin` internamente usando `input()` e o buffer de entrada do Flex.

---

### 2.2 `yylineno`

**Tipo:** `int`

**Definicao:** Variavel global do Flex, habilitada pela opcao `%option yylineno` e declarada como `extern` no Bison:
```c
extern int yylineno;
```

**Objetivo:** Contador automatico de linhas. O Flex incrementa `yylineno` a cada caractere `\n` encontrado na entrada. Começa em 1.

**Quem atualiza:** O proprio Flex, automaticamente. O programador nao precisa (e nao deve) incrementar manualmente.

**Onde e usada (todos os contextos):**

1. **Em `yyerror()`:** Para informar a linha do erro sintatico:
   ```c
   fprintf(stderr, "ERRO SINTATICO: ... - LINHA: %d\n", yylineno);
   ```

2. **Nas acoes semanticas do Bison:** Para informar a linha de erros semanticos:
   ```c
   fprintf(stderr, "ERRO SEMANTICO: ... - LINHA: %d\n", yylineno);
   ```

3. **Nas funcoes de construcao da AST:** Para registrar a linha de cada no:
   ```c
   t->lineno = yylineno;  // em newStmtNode, newExpNode, newVarNode
   ```

4. **Nas acoes do scanner:** Para informar a linha de erros lexicos:
   ```c
   fprintf(stderr, "ERRO LEXICO: ... - LINHA: %d\n", yylineno);
   ```

---

### 2.3 `yytext`

**Tipo:** `char *`

**Definicao:** Variavel global do Flex, declarada como `extern` no Bison:
```c
extern char *yytext;
```

**Objetivo:** Ponteiro para a string do texto casado pelo padrao atual. Aponta para o buffer interno do Flex e contem exatamente os caracteres que casaram com o padrao.

**Tempo de vida:** `yytext` so e valido **durante a execucao da acao** do padrao que casou. Na proxima chamada a `yylex()`, o conteudo pode ser sobrescrito. Por isso, identificadores sao copiados com `strdup()`.

**Onde e usada:**

1. **Na regra de identificadores:** Para copiar o nome do identificador:
   ```c
   yylval.id = strdup(yytext);  // copia segura do texto
   ```

2. **Na regra de numeros:** Para converter o texto em inteiro:
   ```c
   yylval.ival = atoi(yytext);  // converte "42" para 42
   ```

3. **Na regra de erro lexico:** Para exibir o caractere nao reconhecido:
   ```c
   fprintf(stderr, "... caractere nao reconhecido: '%s' ...\n", yytext);
   ```

---

### 2.4 `yylval`

**Tipo:** `YYSTYPE` (union definida pelo Bison)

**Definicao:** Variavel global compartilhada entre Flex e Bison. Definida implicitamente pelo Bison com base na diretiva `%union`:

```c
%union {
    int ival;
    char *id;
    TipoVar tipo;
    struct { TipoVar tipo; int is_array; } var_info;
    TreeNode *node;
}
```

**Objetivo:** Transportar o **valor semantico** de um token do scanner para o parser. Quando o scanner reconhece um token que tem valor associado (como um numero ou identificador), ele preenche `yylval` antes de retornar o codigo do token.

**Como e usada no scanner:**

1. **Para identificadores (campo `id`):**
   ```c
   yylval.id = strdup(yytext);
   return ID;
   ```
   O parser acessa esse valor via `$1`, `$2`, etc. nas acoes semanticas, recebendo o tipo `char *`.

2. **Para numeros (campo `ival`):**
   ```c
   yylval.ival = atoi(yytext);
   return NUM;
   ```
   O parser acessa o valor inteiro via `$1`, `$2`, etc.

**Como o Bison sabe qual campo usar:** As diretivas `%token` definem a associacao:
```
%token<id> ID       --> yylval.id
%token<ival> NUM    --> yylval.ival
```

---

## 3. Opcoes de Configuracao

### 3.1 `%option noyywrap`

```
%option noyywrap
```

**O que faz:** Desabilita a chamada a funcao `yywrap()`.

**Contexto:** Quando o Flex chega ao fim de `yyin`, normalmente chama `yywrap()` para verificar se ha outro arquivo de entrada. Se `yywrap()` retorna 1, o scanner termina. Se retorna 0, o scanner continua lendo do (possivelmente novo) `yyin`.

**Efeito de `noyywrap`:** O Flex nao chama `yywrap()` e simplesmente termina quando o arquivo acaba. Isso elimina a necessidade de implementar `yywrap()` ou linkar com `-lfl`.

**Por que e usado:** Este compilador processa um unico arquivo de entrada, entao a funcionalidade de multiplos arquivos nao e necessaria.

---

### 3.2 `%option yylineno`

```
%option yylineno
```

**O que faz:** Habilita a contagem automatica de linhas pelo Flex.

**Efeito:** O Flex gera codigo que incrementa a variavel global `yylineno` toda vez que um caractere `\n` (newline) e encontrado no texto de entrada, inclusive dentro de padroes casados.

**Sem esta opcao:** `yylineno` nao existiria automaticamente, e o programador precisaria contabilizar linhas manualmente em cada regra que consome `\n`.

---

### 3.3 Definicao do scanner

```c
#define YY_DECL int yylex()
```

**O que faz:** Define a assinatura exata da funcao `yylex()` gerada pelo Flex.

**Efeito:** O Flex gera a funcao `yylex()` com a assinatura `int yylex()`, sem parametros e retornando `int`. O valor retornado e o codigo numerico do token reconhecido (ou 0 para fim de arquivo).

**Por que esta definicao e necessaria:** Permite controlar a assinatura exata da funcao para garantir compatibilidade com o parser gerado pelo Bison, que espera `int yylex(void)`.

---

### 3.4 Include do header do Bison

```c
#include "cminus.tab.h"
```

**O que faz:** Inclui o header gerado pelo Bison (`cminus.tab.h`), que contem:
- As constantes numericas dos tokens (`ELSE`, `IF`, `PLUS`, etc.)
- A definicao do tipo `YYSTYPE` (a union de valores semanticos)
- A declaracao de `yylval`
- As definicoes dos tipos usados no `%code requires` (como `TreeNode`, `TipoVar`, etc.)

**Por que e necessario:** O scanner precisa conhecer os codigos numericos dos tokens para retorna-los ao parser. Por exemplo, `return PLUS;` retorna o codigo numerico associado ao token `PLUS`.

---

## 4. A Funcao yylex

### 4.1 Visao geral

**Assinatura:**

```c
int yylex(void);
```

**Parametros:** Nenhum.

**Retorno:** `int` -- codigo numerico do token reconhecido. Retorna `0` para indicar fim de arquivo (EOF).

**Objetivo:** Ler caracteres de `yyin`, reconhecer o proximo token valido e retornar seu codigo numerico para o parser. Antes de retornar, pode preencher `yylval` com o valor semantico do token.

**Quem chama:** A funcao `yyparse()` gerada pelo Bison. Toda vez que o parser precisa do proximo token, ele chama `yylex()`. Declarada como `extern` no Bison:
```c
extern int yylex(void);
```

### 4.2 Funcionamento interno

O Flex gera `yylex()` como um loop infinito que:

1. Le caracteres de `yyin` e os armazena em um buffer interno.
2. Tenta casar o texto do buffer com todos os padroes definidos nas regras.
3. Usa duas regras de desempate:
   - **Casamento mais longo (maximal munch):** O padrao que casa o maior numero de caracteres vence. Ex: `>=` vence `>` quando o proximo caractere e `=`.
   - **Prioridade de declaracao:** Se dois padroes casam o mesmo comprimento, o que aparece primeiro no arquivo `.l` vence. Ex: `"if"` vence `[a-zA-Z][a-zA-Z0-9]*` porque aparece antes.
4. Executa a acao associada ao padrao vencedor.
5. Se a acao contem `return`, `yylex()` retorna ao parser com o codigo do token.
6. Se a acao **nao** contem `return` (como regras que ignoram espacos), volta ao passo 1 para continuar buscando.

---

## 5. Estados do Scanner

### 5.1 Estado `INITIAL`

**Tipo:** Estado padrao (start condition).

**Descricao:** E o estado normal de operacao do scanner. Todas as regras sem prefixo de estado pertencem a este estado. Quando o scanner esta em `INITIAL`, ele reconhece tokens normais da linguagem.

**Quando ativo:** Desde o inicio da analise lexica e sempre que nao estiver processando um comentario de bloco.

---

### 5.2 Estado `COMMENT`

**Declaracao:**

```
%x COMMENT
```

**Tipo:** Estado exclusivo (`%x`). Diferente de estados inclusivos (`%s`), quando o scanner esta em um estado exclusivo, **apenas** as regras prefixadas com esse estado sao ativas. Regras sem prefixo nao sao consideradas.

**Descricao:** Estado ativado ao encontrar `/*`. Enquanto neste estado, o scanner consome (ignora) todos os caracteres ate encontrar `*/`.

**Quando ativo:** Apos reconhecer o padrao `"/*"`.

**Por que exclusivo:** Um estado exclusivo garante que nenhum token normal (como `if`, numeros, operadores) seja acidentalmente reconhecido dentro de um comentario. Tudo e ignorado.

---

### 5.3 Funcao `BEGIN`

```c
BEGIN(COMMENT);    // muda para o estado COMMENT
BEGIN(INITIAL);    // volta para o estado normal
```

**O que e:** Macro do Flex que altera o estado atual do scanner.

**Parametro:** Nome do estado destino.

**Retorno:** Nenhum (macro, nao funcao).

**Efeito:** A partir da proxima tentativa de casamento de padroes, apenas as regras do novo estado serao consideradas.

**Onde e usada:**
- `BEGIN(COMMENT)` -- na regra `"/*"`, ao entrar em um comentario de bloco.
- `BEGIN(INITIAL)` -- na regra `<COMMENT>"*/"`, ao sair de um comentario de bloco.

---

## 6. Regras de Reconhecimento de Espacos em Branco

### 6.1 Espacos, tabs e retorno de carro

```
[ \t\r]+           ;
```

**Padrao:** `[ \t\r]+`
- `[ \t\r]`: classe de caracteres que casa espaco, tab (`\t`) e retorno de carro (`\r`).
- `+`: um ou mais caracteres consecutivos.

**Acao:** `;` (acao vazia -- nao faz nada).

**Objetivo:** Consumir e descartar sequencias de espacos em branco. O scanner volta a buscar o proximo padrao sem retornar ao parser.

**Nota:** `\r` (carriage return) e incluido para compatibilidade com quebras de linha Windows (`\r\n`).

---

### 6.2 Quebras de linha

```
\n                 ;
```

**Padrao:** `\n` -- um caractere de nova linha.

**Acao:** `;` (acao vazia).

**Objetivo:** Consumir quebras de linha. O Flex incrementa `yylineno` automaticamente (graças a `%option yylineno`) antes de executar a acao.

**Por que separado de `[ \t\r]+`:** Embora pudesse ser unificado como `[ \t\r\n]+`, mante-lo separado torna a intencao mais clara e facilita depuracao do contador de linhas.

---

## 7. Regras de Reconhecimento de Comentarios

### 7.1 Abertura de comentario de bloco

```
"/*"               { BEGIN(COMMENT); }
```

**Padrao:** `"/*"` -- a string literal `/*`.

**Acao:** `BEGIN(COMMENT);` -- muda o scanner para o estado exclusivo `COMMENT`.

**Objetivo:** Detectar o inicio de um comentario de bloco no estilo C e entrar no modo de ignorar conteudo.

**Efeito:** A partir deste ponto, apenas regras prefixadas com `<COMMENT>` serao ativas.

---

### 7.2 Fechamento de comentario de bloco

```
<COMMENT>"*/"      { BEGIN(INITIAL); }
```

**Padrao:** `"*/"` -- a string literal `*/`, ativa **apenas** no estado `COMMENT`.

**Acao:** `BEGIN(INITIAL);` -- volta ao estado normal.

**Objetivo:** Detectar o fim do comentario de bloco e retomar a analise lexica normal.

---

### 7.3 Conteudo dentro do comentario

```
<COMMENT>\n        ;
<COMMENT>.         ;
```

**Padrao `<COMMENT>\n`:** Quebra de linha dentro do comentario.
- **Acao:** Vazia. O Flex incrementa `yylineno` automaticamente.
- **Objetivo:** Manter a contagem de linhas correta mesmo dentro de comentarios.

**Padrao `<COMMENT>.`:** Qualquer caractere exceto `\n`, no estado `COMMENT`.
- **Acao:** Vazia. Consome e descarta o caractere.
- **Objetivo:** Ignorar todo o conteudo do comentario.

**Por que sao regras separadas:** O metacaractere `.` no Flex casa **qualquer caractere exceto `\n`**. Para consumir tambem quebras de linha, e necessaria a regra separada `<COMMENT>\n`.

---

### 7.4 Comentario nao fechado (EOF)

```
<COMMENT><<EOF>>   { fprintf(stderr, "ERRO LEXICO: 'comentario nao fechado' - LINHA: %d\n", yylineno); return 0; }
```

**Padrao:** `<<EOF>>` -- diretiva especial do Flex que casa com o fim do arquivo de entrada, ativa **apenas** no estado `COMMENT`.

**Acao:**
1. Imprime erro em `stderr` informando a linha.
2. `return 0;` -- retorna 0 ao parser, sinalizando fim de arquivo.

**Objetivo:** Detectar quando o arquivo termina com um comentario de bloco nao fechado (`/*` sem o correspondente `*/`) e informar o usuario.

**Por que `return 0`:** O codigo 0 indica fim de arquivo para o Bison. Isso faz com que o parser tente encerrar normalmente (e provavelmente gere um erro sintatico adicional se a gramatica nao foi completamente reduzida).

---

### 7.5 Comentarios de linha unica

```
"//".*             ;
```

**Padrao:** `"//".*`
- `"//"`: a string literal `//`.
- `.*`: qualquer sequencia de caracteres (exceto `\n`) ate o fim da linha.

**Acao:** Vazia -- consome e descarta.

**Objetivo:** Ignorar comentarios de linha unica no estilo C++.

**Nota:** O padrao nao consome o `\n` final (porque `.` nao casa `\n`). O `\n` sera consumido pela regra de quebra de linha, incrementando `yylineno`.

**Nota sobre a linguagem C-Minus:** Comentarios `//` nao fazem parte da especificacao original da linguagem C-Minus (que so define `/* */`). Sua inclusao e uma extensao do compilador.

---

## 8. Regras de Reconhecimento de Palavras Reservadas

```
"else"             { return ELSE; }
"if"               { return IF; }
"int"              { return INT; }
"return"           { return RETURN; }
"void"             { return VOID; }
"while"            { return WHILE; }
```

### 8.1 Mecanismo de funcionamento

Cada palavra reservada e definida como um padrao de string literal. O Flex tenta casar esses padroes **antes** da regra de identificadores porque aparecem primeiro no arquivo `.l`.

### 8.2 Regra de prioridade

Quando o scanner encontra a sequencia `if`, dois padroes podem casar:
1. `"if"` -- retorna `IF`
2. `[a-zA-Z][a-zA-Z0-9]*` -- retorna `ID`

Ambos casam 2 caracteres, entao ha empate de comprimento. A **regra de prioridade** do Flex resolve: o padrao que aparece **primeiro** no arquivo vence. Como `"if"` aparece antes da regra de identificadores, `IF` e retornado.

Se a entrada fosse `ifx`, o padrao `[a-zA-Z][a-zA-Z0-9]*` casaria 3 caracteres (`ifx`) contra 2 do `"if"`. Pelo **maximal munch**, a regra de identificadores venceria, retornando `ID` com `yylval.id = "ifx"`. Isso e o comportamento correto.

### 8.3 Detalhamento de cada palavra reservada

| Padrao | Token retornado | Uso na gramatica |
|---|---|---|
| `"else"` | `ELSE` | Parte do `if-else` na regra `selection_stmt` |
| `"if"` | `IF` | Inicio da regra `selection_stmt` |
| `"int"` | `INT` | Na regra `type_specifier`, indica tipo inteiro |
| `"return"` | `RETURN` | Inicio da regra `return_stmt` |
| `"void"` | `VOID` | Na regra `type_specifier`, indica tipo void; na regra `params`, indica sem parametros |
| `"while"` | `WHILE` | Inicio da regra `iteration_stmt` |

### 8.4 Valor semantico

Palavras reservadas **nao** preenchem `yylval`. O codigo do token por si so ja contem toda a informacao necessaria. O parser sabe que `IF` significa `if` sem precisar de dados adicionais.

---

## 9. Regra de Reconhecimento de Identificadores

```
[a-zA-Z][a-zA-Z0-9]* { 
    yylval.id = strdup(yytext); 
    return ID; 
}
```

### 9.1 Padrao

`[a-zA-Z][a-zA-Z0-9]*`:
- `[a-zA-Z]`: o primeiro caractere deve ser uma letra (maiuscula ou minuscula).
- `[a-zA-Z0-9]*`: seguido de zero ou mais letras ou digitos.

Exemplos de casamento: `x`, `gcd`, `main`, `v1`, `myVar`, `ABC123`.

Exemplos que **nao** casam: `_var` (comeca com `_`), `123abc` (comeca com digito), `my-var` (contem `-`).

### 9.2 Acao

```c
yylval.id = strdup(yytext);
return ID;
```

1. **`strdup(yytext)`:** Cria uma copia independente da string `yytext` na heap. Isso e **essencial** porque `yytext` aponta para o buffer interno do Flex, que sera sobrescrito na proxima chamada a `yylex()`. Sem `strdup`, o nome do identificador seria corrompido.

2. **`yylval.id = ...`:** Armazena o ponteiro da copia no campo `id` da union `yylval`. O parser acessara esse valor via `$N` na acao semantica correspondente.

3. **`return ID;`:** Retorna o codigo do token `ID` ao parser.

### 9.3 Associacao com o Bison

No Bison, a declaracao:
```
%token<id> ID
```
Associa o token `ID` ao campo `id` (tipo `char *`) da union. Isso significa que `$1`, `$2`, etc. para o token `ID` terao tipo `char *`.

### 9.4 Responsabilidade de memoria

A string alocada com `strdup()` e de responsabilidade do codigo que a recebe. Nas acoes semanticas do Bison, o nome e:
- Copiado novamente com `strdup()` para os nos da AST (`$$->child[0]->kind.var.attr.name = strdup($2)`).
- Liberado com `free($2)` apos a copia.

Isso garante que cada no da AST tenha sua propria copia independente do nome.

---

## 10. Regra de Reconhecimento de Numeros

```
[0-9]+             { 
    yylval.ival = atoi(yytext); 
    return NUM; 
}
```

### 10.1 Padrao

`[0-9]+`:
- `[0-9]`: digitos de 0 a 9.
- `+`: um ou mais digitos.

Exemplos de casamento: `0`, `42`, `100`, `999999`.

**Limitacao:** Reconhece apenas inteiros nao-negativos sem sinal. Numeros negativos como `-5` sao tratados como o operador `-` seguido do numero `5`.

### 10.2 Acao

```c
yylval.ival = atoi(yytext);
return NUM;
```

1. **`atoi(yytext)`:** Converte a string de digitos para um valor inteiro. `atoi` e simples mas nao detecta overflow. Para o escopo de C-Minus, isso e aceitavel.

2. **`yylval.ival = ...`:** Armazena o valor inteiro no campo `ival` da union.

3. **`return NUM;`:** Retorna o codigo do token `NUM`.

### 10.3 Associacao com o Bison

```
%token<ival> NUM
```

Associa `NUM` ao campo `ival` (tipo `int`). Nas acoes do Bison, `$N` para `NUM` tem tipo `int`.

### 10.4 Diferenca de `strdup` vs `atoi`

Para `ID`, e necessario `strdup()` porque strings tem tamanho variavel e precisam de alocacao dinamica. Para `NUM`, `atoi()` converte diretamente para `int`, um tipo de valor que cabe na union sem alocacao adicional.

---

## 11. Regras de Reconhecimento de Operadores Relacionais

```
"<="               { return LE; }
"<"                { return LT; }
">"                { return GT; }
">="               { return GE; }
"=="               { return EQ; }
"!="               { return NE; }
```

### 11.1 Principio do maximal munch

A ordem dessas regras no arquivo e importante para garantir o casamento correto, mas o Flex ja implementa **maximal munch** automaticamente. Quando a entrada contem `<=`:

1. `"<="` casa 2 caracteres.
2. `"<"` casa 1 caractere.
3. Flex escolhe `"<="` porque casa mais caracteres.

No entanto, a convencao e colocar padroes mais longos primeiro para clareza.

### 11.2 Detalhamento

| Padrao | Token | Significado | Usado em |
|---|---|---|---|
| `"<="` | `LE` | Menor ou igual | Regra `relop` do Bison |
| `"<"` | `LT` | Menor que | Regra `relop` do Bison |
| `">"` | `GT` | Maior que | Regra `relop` do Bison |
| `">="` | `GE` | Maior ou igual | Regra `relop` do Bison |
| `"=="` | `EQ` | Igualdade | Regra `relop` do Bison |
| `"!="` | `NE` | Diferenca | Regra `relop` do Bison |

### 11.3 Valor semantico

Estes tokens **nao** preenchem `yylval`. O codigo do token e suficiente. No Bison, a regra `relop` armazena o codigo do token em `$$`:
```
relop : LE { $$ = LE; }
      | LT { $$ = LT; }
      ...
```
Esse valor e depois armazenado no campo `op` do no da AST.

---

## 12. Regras de Reconhecimento de Operadores e Pontuacao

### 12.1 Operador de atribuicao

```
"="                { return ASSIGN; }
```

| Padrao | Token | Significado |
|---|---|---|
| `"="` | `ASSIGN` | Atribuicao |

**Nota sobre ambiguidade com `==`:** O maximal munch resolve automaticamente. Se a entrada e `==`, o Flex casa `"=="` (2 caracteres) em vez de `"="` (1 caractere) seguido de outro `"="`.

### 12.2 Delimitadores

```
";"                { return SEMI; }
","                { return COMMA; }
```

| Padrao | Token | Significado | Uso principal |
|---|---|---|---|
| `";"` | `SEMI` | Ponto e virgula | Termina declaracoes e expression statements |
| `","` | `COMMA` | Virgula | Separa parametros e argumentos |

### 12.3 Parenteses, colchetes e chaves

```
"("                { return LPAREN; }
")"                { return RPAREN; }
"["                { return LBRACK; }
"]"                { return RBRACK; }
"{"                { return LBRACE; }
"}"                { return RBRACE; }
```

| Padrao | Token | Significado | Uso principal |
|---|---|---|---|
| `"("` | `LPAREN` | Abre parentese | Chamadas de funcao, expressoes agrupadas, condicoes |
| `")"` | `RPAREN` | Fecha parentese | Idem |
| `"["` | `LBRACK` | Abre colchete | Declaracao e acesso a arrays |
| `"]"` | `RBRACK` | Fecha colchete | Idem |
| `"{"` | `LBRACE` | Abre chave | Inicio de bloco composto |
| `"}"` | `RBRACE` | Fecha chave | Fim de bloco composto |

### 12.4 Operadores aritmeticos

```
"+"                { return PLUS; }
"-"                { return MINUS; }
"*"                { return TIMES; }
"/"                { return DIVIDE; }
```

| Padrao | Token | Significado | Precedencia no Bison |
|---|---|---|---|
| `"+"` | `PLUS` | Adicao | `%left PLUS MINUS` (mais baixa entre aritmeticos) |
| `"-"` | `MINUS` | Subtracao | `%left PLUS MINUS` |
| `"*"` | `TIMES` | Multiplicacao | `%left TIMES DIVIDE` (mais alta entre aritmeticos) |
| `"/"` | `DIVIDE` | Divisao | `%left TIMES DIVIDE` |

**Nota sobre `"/"`:** Apesar de `/` tambem ser o inicio de comentarios (`//` e `/*`), nao ha ambiguidade porque o Flex usa maximal munch. Se a entrada e `//`, o padrao `"//".*` (comentario de linha) casa mais caracteres que `"/"` (operador). Se a entrada e `/*`, o padrao `"/*"` casa 2 caracteres contra 1 de `"/"`. Ambos os padroes de comentario vencem.

### 12.5 Nenhum valor semantico

Todos os tokens desta secao nao preenchem `yylval`. O codigo do token e suficiente. Para operadores aritmeticos, o Bison armazena o codigo nas regras `addop` e `mulop`:
```
addop : PLUS { $$ = PLUS; }
      | MINUS { $$ = MINUS; }
```

---

## 13. Regra de Tratamento de Erro Lexico

```
.                  {
    fprintf(stderr, "ERRO LEXICO:    caractere nao reconhecido: '%s' - LINHA: %d\n", yytext, yylineno);
}
```

### 13.1 Padrao

`.` -- o metacaractere `.` no Flex casa **qualquer caractere unico exceto `\n`**.

### 13.2 Por que e a ultima regra

Esta regra funciona como um **catch-all** (pegador geral). Ela e colocada como a **ultima regra** do arquivo para garantir que so seja ativada quando **nenhum outro padrao** casou. Isso e possivel porque:
- Todos os tokens validos da linguagem ja foram cobertos pelas regras anteriores.
- O `.` casa apenas 1 caractere, entao qualquer padrao mais longo tera prioridade pelo maximal munch.
- Para padroes de 1 caractere que empatam (ex: `@` casaria tanto com `.` quanto... nenhum padrao anterior), a regra de prioridade de declaracao escolheria a que aparece primeiro. Como nenhum padrao anterior casa `@`, o `.` o captura.

### 13.3 Acao

```c
fprintf(stderr, "ERRO LEXICO:    caractere nao reconhecido: '%s' - LINHA: %d\n", yytext, yylineno);
```

1. Imprime mensagem de erro em `stderr` com o caractere nao reconhecido e a linha.
2. **Nao retorna nenhum token** (`return` nao e chamado).

### 13.4 Comportamento apos o erro

Como nao ha `return`, o scanner **nao para**. O Flex consome o caractere invalido e continua tentando reconhecer tokens a partir do proximo caractere. Isso e um comportamento de **recuperacao de erro**: o compilador tenta continuar a analise mesmo apos encontrar um caractere invalido.

### 13.5 Impacto no parser

Como nenhum token e retornado ao parser para caracteres invalidos, o parser simplesmente nao ve esses caracteres. O proximo token valido sera entregue normalmente. Se o caractere invalido estiver em uma posicao sintaticamente importante, o parser detectara o problema como um erro sintatico.

### 13.6 Exemplos de caracteres capturados

Caracteres que nao pertencem a linguagem C-Minus e seriam capturados por esta regra:
- `@`, `#`, `$`, `%`, `^`, `&`, `~`, `` ` ``, `|`, `\`, `?`, `'`, `"`
- Caracteres acentuados: `á`, `ç`, `ñ`
- Qualquer outro caractere Unicode nao-ASCII (dependendo da configuracao)

---

## 14. Mecanismo de Comunicacao Lexer-Parser

### 14.1 Protocolo de chamada

O Bison chama `yylex()` sempre que precisa do proximo token. O fluxo e:

```
yyparse() <---> yylex()

1. yyparse() precisa de um token
2. Chama yylex()
3. yylex() le de yyin, casa um padrao
4. yylex() preenche yylval (se necessario)
5. yylex() retorna o codigo do token (ex: PLUS, ID, NUM)
6. yyparse() recebe o token e o valor semantico
7. yyparse() realiza shift ou reduce
8. Volta ao passo 1 quando precisa do proximo token
```

### 14.2 Codigos de retorno especiais

| Retorno | Significado |
|---|---|
| `> 0` (ex: `PLUS`, `ID`) | Token valido reconhecido |
| `0` | Fim de arquivo (EOF) |

### 14.3 Dados transportados

Para cada token, dois dados sao transportados:
1. **Codigo do token** (retorno de `yylex()`): identifica **qual** token e (PLUS, ID, NUM, SEMI, etc.)
2. **Valor semantico** (`yylval`): informacao adicional sobre o token:
   - Para `ID`: o nome do identificador (`char *`)
   - Para `NUM`: o valor inteiro (`int`)
   - Para todos os outros: nenhum valor adicional

---

## 15. Processo Completo de Tokenizacao

### 15.1 Exemplo passo a passo

Dado o trecho de entrada:

```c
int x;
x = 42;
```

O scanner produz a seguinte sequencia de tokens:

| Passo | Caracteres lidos | Padrao casado | Token retornado | `yylval` |
|---|---|---|---|---|
| 1 | `int` | `"int"` | `INT` | (nao preenchido) |
| 2 | ` ` | `[ \t\r]+` | (nenhum -- acao vazia, volta ao loop) | - |
| 3 | `x` | `[a-zA-Z][a-zA-Z0-9]*` | `ID` | `.id = "x"` |
| 4 | `;` | `";"` | `SEMI` | (nao preenchido) |
| 5 | `\n` | `\n` | (nenhum -- acao vazia) | - |
| 6 | `x` | `[a-zA-Z][a-zA-Z0-9]*` | `ID` | `.id = "x"` |
| 7 | ` ` | `[ \t\r]+` | (nenhum) | - |
| 8 | `=` | `"="` | `ASSIGN` | (nao preenchido) |
| 9 | ` ` | `[ \t\r]+` | (nenhum) | - |
| 10 | `42` | `[0-9]+` | `NUM` | `.ival = 42` |
| 11 | `;` | `";"` | `SEMI` | (nao preenchido) |
| 12 | `\n` | `\n` | (nenhum) | - |
| 13 | (EOF) | - | `0` | - |

Tokens entregues ao parser: `INT ID SEMI ID ASSIGN NUM SEMI` (7 tokens + EOF).

### 15.2 Exemplo com comentario

Dado:
```c
/* soma */ x + y
```

| Passo | Caracteres | Padrao | Acao |
|---|---|---|---|
| 1 | `/*` | `"/*"` | `BEGIN(COMMENT)` -- entra no estado COMMENT |
| 2 | ` soma ` | `<COMMENT>.` (repetido 6x) | Consome cada caractere, acao vazia |
| 3 | `*/` | `<COMMENT>"*/"` | `BEGIN(INITIAL)` -- volta ao normal |
| 4 | ` ` | `[ \t\r]+` | Acao vazia |
| 5 | `x` | `[a-zA-Z][a-zA-Z0-9]*` | Retorna `ID`, `yylval.id = "x"` |
| 6 | ` ` | `[ \t\r]+` | Acao vazia |
| 7 | `+` | `"+"` | Retorna `PLUS` |
| 8 | ` ` | `[ \t\r]+` | Acao vazia |
| 9 | `y` | `[a-zA-Z][a-zA-Z0-9]*` | Retorna `ID`, `yylval.id = "y"` |

Tokens entregues ao parser: `ID PLUS ID` (3 tokens).

---

## Apendice: Tabela Completa de Tokens

| Token | Valor semantico | Tipo em `yylval` | Padrao Flex | Categoria |
|---|---|---|---|---|
| `ELSE` | nenhum | - | `"else"` | Palavra reservada |
| `IF` | nenhum | - | `"if"` | Palavra reservada |
| `INT` | nenhum | - | `"int"` | Palavra reservada |
| `RETURN` | nenhum | - | `"return"` | Palavra reservada |
| `VOID` | nenhum | - | `"void"` | Palavra reservada |
| `WHILE` | nenhum | - | `"while"` | Palavra reservada |
| `ID` | nome (`char *`) | `.id` | `[a-zA-Z][a-zA-Z0-9]*` | Identificador |
| `NUM` | valor (`int`) | `.ival` | `[0-9]+` | Literal numerico |
| `LE` | nenhum | - | `"<="` | Operador relacional |
| `LT` | nenhum | - | `"<"` | Operador relacional |
| `GT` | nenhum | - | `">"` | Operador relacional |
| `GE` | nenhum | - | `">="` | Operador relacional |
| `EQ` | nenhum | - | `"=="` | Operador relacional |
| `NE` | nenhum | - | `"!="` | Operador relacional |
| `ASSIGN` | nenhum | - | `"="` | Operador |
| `SEMI` | nenhum | - | `";"` | Pontuacao |
| `COMMA` | nenhum | - | `","` | Pontuacao |
| `LPAREN` | nenhum | - | `"("` | Delimitador |
| `RPAREN` | nenhum | - | `")"` | Delimitador |
| `LBRACK` | nenhum | - | `"["` | Delimitador |
| `RBRACK` | nenhum | - | `"]"` | Delimitador |
| `LBRACE` | nenhum | - | `"{"` | Delimitador |
| `RBRACE` | nenhum | - | `"}"` | Delimitador |
| `PLUS` | nenhum | - | `"+"` | Operador aritmetico |
| `MINUS` | nenhum | - | `"-"` | Operador aritmetico |
| `TIMES` | nenhum | - | `"*"` | Operador aritmetico |
| `DIVIDE` | nenhum | - | `"/"` | Operador aritmetico |

**Total: 27 tokens distintos**
