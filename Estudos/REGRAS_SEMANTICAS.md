# Regras Semanticas do Compilador C-Minus

Este documento consolida as regras semanticas realmente implementadas no compilador, com base principalmente em `cminusSintSem.y`. O foco aqui e o comportamento do codigo, nao apenas o que aparece na documentacao.

## Visao Geral

A analise semantica deste compilador:

- acontece integrada ao parser Bison;
- e executada durante a reducao de producoes da gramatica;
- usa `ParserContext` para manter o estado da compilacao;
- consulta e atualiza tabela de simbolos e escopos;
- marca erros pela flag `ctx->has_errors`;
- imprime mensagens imediatamente com `fprintf(stderr, ...)`.

Nao existe, neste projeto, uma fase semantica separada que percorre toda a AST depois do parsing. As verificacoes semanticas ocorrem nas acoes das regras gramaticais e em funcoes auxiliares chamadas por essas acoes.

## Estruturas Semanticas Utilizadas

### `ParserContext`

O contexto da compilacao armazena:

- `ast_root`: raiz da AST;
- `escopo_atual`: escopo ativo no momento da analise;
- `lista_escopos`: lista encadeada de todos os escopos criados;
- `has_errors`: indica se algum erro semantico ou sintatico foi detectado.

### `Simbolo`

Cada simbolo armazena:

- nome do identificador;
- tipo (`int`, `void`, `int[]` ou erro);
- categoria (`VAR`, `ARRAY`, `FUNC`);
- tamanho, no caso de arrays;
- linha de declaracao;
- se o simbolo e parametro;
- escopo associado, usado especialmente para funcoes.

### `Escopo`

Cada escopo possui:

- lista de simbolos declarados nele;
- ponteiro para o escopo pai;
- ponteiro para a lista global de escopos criados.

## Infraestrutura Semantica

Antes de listar as regras, e importante entender a infraestrutura que as sustenta.

### 1. Criacao e troca de escopos

As funcoes `enter_scope_ctx()` e `leave_scope_ctx()` controlam os escopos da linguagem.

Regras implementadas:

- existe um escopo global criado na `main`;
- ao iniciar a declaracao de uma funcao, um novo escopo e aberto para seus parametros e corpo;
- ao entrar em um bloco composto usado como comando (`compound_stmt_with_scope`), um novo escopo e criado;
- ao sair desses contextos, o compilador retorna ao escopo pai.

### 2. Busca de identificadores

O compilador usa duas estrategias de busca:

- `lookup_symbol_current_ctx()`: busca apenas no escopo atual;
- `lookup_symbol_ctx()`: busca no escopo atual e sobe para os escopos pais.

Isso viabiliza:

- deteccao de redeclaracao no mesmo escopo;
- uso de variaveis locais e globais com regras de visibilidade;
- uso de parametros dentro do corpo da funcao.

### 3. Insercao de simbolos

As funcoes `insert_symbol_ctx()`, `insert_array_ctx()` e `insert_function_ctx()` inserem simbolos na tabela.

Todas elas compartilham a mesma regra de base:

- um identificador nao pode ser declarado mais de uma vez no mesmo escopo.

Quando isso acontece, o compilador emite:

`ERRO SEMANTICO: identificador '<nome>' ja declarado neste escopo`

## Regras Semanticas Implementadas

As regras abaixo sao as verificacoes semanticas realmente presentes no codigo.

### 1. Redeclaracao de identificadores no mesmo escopo

#### Onde ocorre

- `insert_symbol_ctx()`
- `insert_array_ctx()`
- `insert_function_ctx()`

#### Regra

Um identificador nao pode ser declarado novamente no mesmo escopo, independentemente de ser:

- variavel;
- array;
- funcao.

#### Efeito

- emite erro semantico;
- marca `ctx->has_errors = 1`;
- nao reinsere o simbolo.

#### Observacao

A verificacao e local ao escopo atual. Sombras entre escopos diferentes sao permitidas.

### 2. Declaracao de array com tipo `void`

#### Onde ocorre

Na regra `var_declaration` para declaracoes do tipo `type_specifier ID [ NUM ] ;`.

#### Regra

Arrays nao podem ser declarados com tipo `void`.

#### Efeito

- emite `ERRO SEMANTICO: array '<nome>' nao pode ser void`;
- marca erro;
- nao executa a insercao do array pela rotina normal.

### 3. Parametro simples nao pode ser `void`

#### Onde ocorre

Na regra `param` para `type_specifier ID`.

#### Regra

Um parametro simples nao pode ter tipo `void`.

#### Efeito

- emite `ERRO SEMANTICO: parametro '<nome>' nao pode ser void`;
- marca erro;
- o simbolo nao e inserido pela rotina normal de parametro valido.

### 4. Parametro array nao pode ser `void`

#### Onde ocorre

Na regra `param` para `type_specifier ID [ ]`.

#### Regra

Um parametro array nao pode ser `void`.

#### Efeito

- emite `ERRO SEMANTICO: parametro array '<nome>' nao pode ser void`;
- marca erro;
- o parametro nao e inserido pela rotina normal de parametro valido.

### 5. Parametros validos sao registrados como parametros

#### Onde ocorre

Nas duas variantes da regra `param`.

#### Regra

Quando um parametro e valido:

- ele e inserido no escopo atual;
- seu campo `is_param` e marcado com `1`.

#### Objetivo

Esse metadado e usado depois, por exemplo, para exibir a quantidade de parametros de cada funcao na tabela de simbolos.

### 6. Funcoes sao declaradas no escopo externo antes da abertura do escopo interno

#### Onde ocorre

Na regra `fun_declaration`.

#### Regra

Ao reconhecer o inicio de uma funcao:

1. a funcao e inserida no escopo atual;
2. um novo escopo e criado para parametros e corpo;
3. o simbolo da funcao recebe referencia ao escopo do corpo via `def_scope`.

#### Consequencia semantica

Essa ordem garante que:

- a funcao exista no escopo onde foi declarada;
- seus parametros e locais pertençam ao escopo interno correto;
- o compilador possa contabilizar os parametros depois.

### 7. Blocos compostos criam escopo proprio quando usados como comando

#### Onde ocorre

Na regra `compound_stmt_with_scope`.

#### Regra

Quando um bloco `{ ... }` aparece como statement, ele abre um novo escopo antes de processar o bloco e fecha esse escopo ao final.

#### Consequencia semantica

Variaveis declaradas no bloco ficam restritas a ele e nao conflitam com escopos externos, exceto em caso de redeclaracao local.

### 8. Variavel usada deve estar declarada

#### Onde ocorre

Na regra `var` para o caso `ID`.

#### Regra

Todo identificador usado como variavel deve existir em algum escopo visivel.

#### Efeito se falhar

- emite `ERRO SEMANTICO: variavel '<nome>' nao declarada`;
- marca erro;
- o no gerado recebe tipo `TYPE_ERROR`.

### 9. Variavel indexada deve estar declarada

#### Onde ocorre

Na regra `var` para `ID [ expression ]`.

#### Regra

O identificador indexado deve existir na tabela de simbolos.

#### Efeito se falhar

- emite `ERRO SEMANTICO: variavel '<nome>' nao declarada`;
- marca erro.

### 10. Apenas arrays podem ser indexados

#### Onde ocorre

Na regra `var` para `ID [ expression ]`.

#### Regra

Se um identificador for usado com indice, ele precisa ter sido declarado como array.

#### Efeito se falhar

- emite `ERRO SEMANTICO: '<nome>' nao e um array`;
- marca erro.

### 11. Indice de array deve ser inteiro

#### Onde ocorre

Na regra `var` para `ID [ expression ]`.

#### Regra

A expressao usada como indice deve ter tipo `TYPE_INT`.

#### Efeito se falhar

- emite `ERRO SEMANTICO: indice de array deve ser inteiro`;
- marca erro.

### 12. Nao e permitido atribuir a um array completo

#### Onde ocorre

Na regra `expression` para `var ASSIGN expression`.

#### Regra

Se o lado esquerdo da atribuicao for um array sem indice, a atribuicao e invalida.

Exemplo conceitual invalido:

```c
arr = 10;
```

#### Efeito se falhar

- emite `ERRO SEMANTICO: nao e possivel atribuir a array completo`;
- marca erro.

### 13. Tipos devem ser compativeis em atribuicoes

#### Onde ocorre

Na regra `expression` para `var ASSIGN expression`.

#### Regra

O tipo do lado esquerdo e o tipo da expressao atribuida devem ser compativeis.

O codigo verifica incompatibilidade quando:

- `var_type != exp_type`
- e nenhum dos dois e `TYPE_ERROR`

#### Efeito se falhar

- emite `ERRO SEMANTICO: tipos incompativeis na atribuicao`;
- marca erro.

### 14. Operacoes relacionais exigem operandos inteiros

#### Onde ocorre

Na regra `simple_expression`, via `check_expression_type_ctx(ctx, "relacional", ...)`.

#### Regra

Em expressoes relacionais, ambos os operandos devem ser inteiros validos.

#### Efeito se falhar

Pode emitir:

- `ERRO SEMANTICO: operacao relacional com tipo void`
- `ERRO SEMANTICO: operacao relacional requer operandos inteiros`

O resultado da expressao passa a `TYPE_ERROR`.

### 15. Operacoes aditivas exigem operandos inteiros

#### Onde ocorre

Na regra `additive_expression`, via `check_expression_type_ctx(ctx, "aditivo", ...)`.

#### Regra

Operandos de `+` e `-` devem ser inteiros.

#### Efeito se falhar

Pode emitir:

- `ERRO SEMANTICO: operacao aditivo com tipo void`
- `ERRO SEMANTICO: operacao aditivo requer operandos inteiros`

O resultado fica como erro.

### 16. Operacoes multiplicativas exigem operandos inteiros

#### Onde ocorre

Na regra `term`, via `check_expression_type_ctx(ctx, "multiplicativo", ...)`.

#### Regra

Operandos de `*` e `/` devem ser inteiros.

#### Efeito se falhar

Pode emitir:

- `ERRO SEMANTICO: operacao multiplicativo com tipo void`
- `ERRO SEMANTICO: operacao multiplicativo requer operandos inteiros`

### 17. Tipo `void` nao pode participar dessas operacoes

#### Onde ocorre

Na funcao `check_expression_type_ctx()`.

#### Regra

Se qualquer um dos operandos for `TYPE_VOID`, a operacao e invalida.

Essa regra vale para:

- operacoes relacionais;
- operacoes aditivas;
- operacoes multiplicativas.

#### Efeito se falhar

- emite erro indicando operacao com tipo `void`;
- marca erro;
- retorna `TYPE_ERROR`.

### 18. Propagacao de erro de tipo

#### Onde ocorre

Na funcao `check_expression_type_ctx()`.

#### Regra

Se algum operando ja estiver marcado como `TYPE_ERROR`, a funcao simplesmente retorna `TYPE_ERROR` sem gerar novo erro redundante.

#### Objetivo

Evitar cascata excessiva de mensagens a partir de um mesmo problema anterior.

### 19. Condicao de `if` deve ser inteira

#### Onde ocorre

Nas duas variantes da regra `selection_stmt`.

#### Regra

A expressao da condicao do `if` deve ter tipo `TYPE_INT`.

#### Efeito se falhar

- emite `ERRO SEMANTICO: condicao do IF deve ser inteira`;
- marca erro.

### 20. Condicao de `while` deve ser inteira

#### Onde ocorre

Na regra `iteration_stmt`.

#### Regra

A condicao do `while` deve ter tipo `TYPE_INT`.

#### Efeito se falhar

- emite `ERRO SEMANTICO: condicao do WHILE deve ser inteira`;
- marca erro.

### 21. Funcao chamada deve existir

#### Onde ocorre

Na regra `call`.

#### Regra

Todo identificador usado como chamada de funcao deve existir em algum escopo visivel.

#### Efeito se falhar

- emite `ERRO SEMANTICO: funcao '<nome>' nao declarada`;
- marca erro.

### 22. Identificador chamado deve ser realmente uma funcao

#### Onde ocorre

Na regra `call`.

#### Regra

Mesmo que o nome exista, ele precisa ter categoria `KIND_FUNC` para ser usado como chamada.

#### Efeito se falhar

- emite `ERRO SEMANTICO: '<nome>' nao e uma funcao`;
- marca erro.

### 23. O tipo da chamada e o tipo de retorno da funcao

#### Onde ocorre

Na regra `call`.

#### Regra

Quando uma chamada e valida, o no correspondente recebe como tipo o tipo de retorno da funcao consultada na tabela de simbolos.

#### Consequencia semantica

Esse tipo pode depois ser usado em:

- atribuicoes;
- expressoes;
- condicoes de controle.

### 24. Funcoes built-in sao predeclaradas no escopo global

#### Onde ocorre

Na `main`, antes do `yyparse(ctx)`.

#### Regras

O compilador insere automaticamente:

- `input` como funcao de retorno `int`;
- `output` como funcao de retorno `void`.

#### Consequencia semantica

Essas funcoes podem ser chamadas mesmo sem declaracao no codigo-fonte do usuario.

## Como os Erros Sao Tratados

Quando uma regra semantica falha, o comportamento padrao e:

1. emitir mensagem em `stderr`;
2. marcar `ctx->has_errors = 1`;
3. continuar a analise sempre que possivel.

Isso significa que o compilador tenta acumular erros semanticos em vez de parar no primeiro.

## Consequencias de `has_errors`

Ao final do parsing:

- a tabela de simbolos ainda e exibida;
- mas `ast.dot` e o codigo intermediario TAC so sao gerados se `ctx->has_errors == 0`.

## Regras que a Documentacao Cita, mas o Codigo Nao Implementa Completamente

Para evitar ambiguidade, vale destacar o que nao esta efetivamente implementado como regra semantica completa no codigo atual:

- verificacao da quantidade de argumentos em chamadas de funcao;
- verificacao dos tipos dos argumentos passados;
- verificacao de compatibilidade de tipo em `return`;
- exigencia explicita da existencia da funcao `main`;
- proibicao geral de variavel simples `void` na declaracao.

Esses pontos podem aparecer no PDF ou README como desejaveis, mas nao estao totalmente implementados como verificacoes semanticas efetivas em `cminusSintSem.y`.

## Resumo Final

As regras semanticas efetivamente implementadas no compilador cobrem:

- gerenciamento de escopos;
- construcao e consulta incremental da tabela de simbolos;
- deteccao de redeclaracao local;
- validacao de arrays;
- validacao de parametros;
- uso de variaveis declaradas;
- verificacao de tipos em expressoes e atribuicoes;
- validacao de condicoes de `if` e `while`;
- verificacao basica de chamadas de funcao;
- acumulacao de erros semanticos via `has_errors`.

Em resumo, a analise semantica do projeto e incremental, integrada ao parser e centrada em tabela de simbolos, escopos e tipos.