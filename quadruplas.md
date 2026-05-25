# Quadruplas

| Quadrupla | Função |
|---|---|
| (NOP, ___, ___, ___) | Marca o início da geração. Emitida por `percorrerArvore` antes de processar nós. |
| (FUNC, \<tipo>, \<nome>, _) | Início de declaração de função; emitida por `gerarComando` ao processar `INTEGERK`/`VOIDK` com `KIND_FUNC`. Define `funcaoAtual` e reinicia caches. |
| (ARG, \<nome>, \<func>, _) | Declaração de parâmetro formal; emitida ao processar a lista de parâmetros da função. |
| (ALLOCAMEMVET, \<escopo>, \<nome>, \<tamanho>) | Aloca espaço para um vetor; emitida em declarações de arrays (escopo global ou função). |
| (ALLOCAMEMVAR, \<escopo>, \<nome>, ___) | Aloca espaço para variável simples; emitida em declarações (`INTEGERK`/`VOIDK`) no escopo atual. |
| (LOADVAR, \<func>, \<variavel>, \<temp>) | Carrega o valor de uma variável em um temporário; emitida por `carregarOuReusarTemp` quando um `ID` precisa ser usado. |
| (LOADCONST, \<temp>, \<valor>, ___) | Coloca uma constante em temporário; emitida por `carregarOuReusarConst` na primeira vez que aparece a constante. |
| (LOADVET, \<func>, \<enderecoTemp>, \<temp>) | Lê elemento de vetor: carrega de memória no endereço (enderecoTemp) para temporário; emitida quando se acessa `a[index]`. |
| (STOREVAR, \<temp>, \<variavel>, \<func>) | Armazena valor de temporário em variável (atribuição); emitida por `gerarComandoExpressao` no caso `ASSIGNK` para `var = expr`. |
| (STOREVET, \<temp>, \<addrTemp>, \<func>) | Armazena valor de temporário em posição de vetor; usada para `a[index] = expr` (emitida por ramos de ASSIGN que tratam arrays). |
| (ADD, \<op1>, \<op2>, \<dest>) | Operação soma / cálculo de endereço; emitida por `gerarExpressao` para `+` e por `assignVetor`/acesso a vetores para somar base+offset. |
| (SUB, \<op1>, \<op2>, \<dest>) | Operação subtração; emitida por `gerarExpressao` para `-`. |
| (MULT, \<op1>, \<op2>, \<dest>) | Operação multiplicação; emitida por `gerarExpressao` para `*`. |
| (DIV, \<op1>, \<op2>, \<dest>) | Operação divisão; emitida por `gerarExpressao` para `/`. |
| (PARAM, \<temp>, ___, ___) | Passagem de argumento antes de chamada; emitida por `gerarExpressao`/`gerarComandoExpressao` ao processar `CALLK`. |
| (CALL, \<nome_func>, \<nArgs>, ___) | Chamada de função com número de argumentos; emitida após os `PARAM`s. Após `CALL` o gerador chama `resetarCachesReuso()`. Quando `CALL` é usada em expressão, o gerador retorna o token especial `$rf`. |
| (RETURN, \<temp> ou ___, ___, ___) | Retorno de função: com temporário de valor ou vazio; emitida por `gerarComando` ao processar `RETURNK`. |
| (LABEL, \<Lx>, ___, ___) | Define um rótulo/etiqueta no código intermediário; criado por `novoLabel()` e emitido por `gerarComando` para IF/WHILE. |
| (JUMP, \<Lx>, ___, ___) | Salto incondicional para label; emitido por `gerarComando` (ex.: pular THEN para FIM quando há ELSE) ou para loops. |
| (BGE, \<esq>, \<dir>, \<Lx>) | Salto condicional: branch se `esq >= dir` → Vai para `Lx`; emitido por `gerarCondicao` quando a condição é `LT` (inverte relação). |
| (BGT, \<esq>, \<dir>, \<Lx>) | Branch se `esq > dir`; usado por `gerarCondicao` para `LE` inverso. |
| (BLE, \<esq>, \<dir>, \<Lx>) | Branch se `esq \<= dir`; usado por `gerarCondicao` para `GT` inverso. |
| (BLT, \<esq>, \<dir>, \<Lx>) | Branch se `esq \< dir`; usado por `gerarCondicao` para `GE` inverso. |
| (BNE, \<esq>, \<dir>, \<Lx>) | Branch se `esq != dir`; usado por `gerarCondicao` para `EQ` inverso (ex.: `if (v == 0)` emite `BNE` para ramo falso). |
| (BEQ, \<esq>, \<dir>, \<Lx>) | Branch se `esq == dir`; usado por `gerarCondicao` para `NE` inverso. |
| (ENDFUNC, \<func>, ___, ___) | Marca o fim da função; emitida ao terminar de gerar o corpo da função em `gerarComando`. Restaura `funcaoAtual`. |
| (HALT, ___, ___, ___) | Marca fim do programa/intermediário; emitida por `percorrerArvore` ao final. |

