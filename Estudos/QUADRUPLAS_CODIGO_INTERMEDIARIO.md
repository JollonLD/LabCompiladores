# Quadruplas do Codigo Intermediario

Este documento define o tipo, o formato e a tabela completa de todas as quadruplas previstas no codigo intermediario gerado pelo compilador C-Minus. A analise foi feita com base no arquivo `code_generator.c`.

---

## 1. Formato Geral

O codigo intermediario gerado segue o modelo de **codigo de tres enderecos** (Three-Address Code - TAC). Cada instrucao pode ser mapeada para uma **quadrupla** no formato:

```
(operador, argumento1, argumento2, resultado)
```

Onde:
- **operador**: a operacao a ser realizada
- **argumento1** (arg1): primeiro operando (pode ser temporario, variavel, constante, label ou vazio)
- **argumento2** (arg2): segundo operando (pode ser temporario, variavel, constante, numero ou vazio)
- **resultado**: onde o resultado e armazenado (pode ser temporario, variavel, label ou vazio)

Convencoes de nomenclatura usadas:
- `tN`: variaveis temporarias geradas automaticamente (`t0`, `t1`, `t2`, ...)
- `LN`: labels gerados automaticamente (`L0`, `L1`, `L2`, ...)
- `var`: nome de uma variavel do programa fonte
- `arr`: nome de um array do programa fonte
- `func`: nome de uma funcao do programa fonte
- `const`: valor inteiro literal
- `-`: campo nao utilizado (vazio)

---

## 2. Classificacao das Quadruplas

As quadruplas sao organizadas nas seguintes categorias:

1. Carga e atribuicao
2. Operacoes aritmeticas
3. Operacoes relacionais
4. Controle de fluxo
5. Arrays
6. Funcoes e procedimentos

---

## 3. Tabela Completa de Quadruplas

### 3.1 Carga e Atribuicao

| # | Quadrupla | Formato TAC | Descricao | Exemplo |
|---|---|---|---|---|
| Q1 | `(LOAD_CONST, const, -, tN)` | `tN = const` | Carrega um valor inteiro constante em um temporario | `t0 = 42` |
| Q2 | `(ASSIGN, src, -, var)` | `var = src` | Atribui o valor de um temporario ou variavel a uma variavel simples | `x = t0` |

**Q1 - Carga de constante:**
- Gerada em `gerarExpressao()`, caso `CONSTK`
- `arg1` e o valor inteiro literal extraido de `no->kind.var.attr.val`
- `resultado` e um novo temporario gerado por `novoTemporario()`
- Toda constante numerica no programa fonte resulta nesta quadrupla

**Q2 - Atribuicao simples:**
- Gerada em `gerarComandoExpressao()`, caso `ASSIGNK`, ramo de variavel simples
- `arg1` e o resultado de `gerarExpressao()` aplicada ao lado direito da atribuicao
- `resultado` e o nome da variavel destino (`no->child[0]->kind.var.attr.name`)

### 3.2 Operacoes Aritmeticas

| # | Quadrupla | Formato TAC | Descricao | Exemplo |
|---|---|---|---|---|
| Q3 | `(ADD, arg1, arg2, tN)` | `tN = arg1 + arg2` | Soma dois operandos | `t2 = t0 + t1` |
| Q4 | `(SUB, arg1, arg2, tN)` | `tN = arg1 - arg2` | Subtrai arg2 de arg1 | `t2 = x - t1` |
| Q5 | `(MUL, arg1, arg2, tN)` | `tN = arg1 * arg2` | Multiplica dois operandos | `t2 = t0 * t1` |
| Q6 | `(DIV, arg1, arg2, tN)` | `tN = arg1 / arg2` | Divide arg1 por arg2 | `t2 = t0 / t1` |

**Q3 a Q6 - Operacoes aritmeticas binarias:**
- Geradas em `gerarExpressao()`, caso `OPK`
- `arg1` e o resultado de `gerarExpressao(no->child[0])` (operando esquerdo)
- `arg2` e o resultado de `gerarExpressao(no->child[1])` (operando direito)
- `resultado` e um novo temporario
- O operador e determinado por `no->op`: `PLUS`->`+`, `MINUS`->`-`, `TIMES`->`*`, `DIVIDE`->`/`
- `arg1` e `arg2` podem ser temporarios (`tN`), variaveis do programa ou nomes retornados por expressoes anteriores

### 3.3 Operacoes Relacionais

| # | Quadrupla | Formato TAC | Descricao | Exemplo |
|---|---|---|---|---|
| Q7 | `(LT, arg1, arg2, tN)` | `tN = arg1 < arg2` | Menor que | `t2 = x < t1` |
| Q8 | `(LE, arg1, arg2, tN)` | `tN = arg1 <= arg2` | Menor ou igual | `t2 = t0 <= y` |
| Q9 | `(GT, arg1, arg2, tN)` | `tN = arg1 > arg2` | Maior que | `t2 = t0 > t1` |
| Q10 | `(GE, arg1, arg2, tN)` | `tN = arg1 >= arg2` | Maior ou igual | `t2 = x >= y` |
| Q11 | `(EQ, arg1, arg2, tN)` | `tN = arg1 == arg2` | Igualdade | `t2 = t0 == t1` |
| Q12 | `(NE, arg1, arg2, tN)` | `tN = arg1 != arg2` | Diferenca | `t2 = x != t1` |

**Q7 a Q12 - Operacoes relacionais:**
- Geradas no mesmo trecho de `gerarExpressao()` caso `OPK`, porem com operadores relacionais
- Seguem exatamente a mesma mecanica das operacoes aritmeticas
- O resultado e um inteiro: 1 (verdadeiro) ou 0 (falso) semanticamente
- O valor resultante e armazenado em um temporario e tipicamente usado como argumento de `if_false`

### 3.4 Controle de Fluxo

| # | Quadrupla | Formato TAC | Descricao | Exemplo |
|---|---|---|---|---|
| Q13 | `(IF_FALSE, cond, -, label)` | `if_false cond goto label` | Desvio condicional: salta para label se cond for falso (zero) | `if_false t2 goto L0` |
| Q14 | `(GOTO, -, -, label)` | `goto label` | Desvio incondicional para label | `goto L1` |
| Q15 | `(LABEL, -, -, label)` | `label:` | Definicao de um label (ponto de destino de desvios) | `L0:` |

**Q13 - Desvio condicional (if_false):**
- Gerada em `gerarComando()`, casos `IFK` e `WHILEK`
- `arg1` (cond) e o resultado de `gerarExpressao()` aplicada a condicao
- `resultado` (label) e um label gerado por `novoLabel()`
- Semantica: se `cond` for 0 (falso), o controle salta para `label`; caso contrario, continua na proxima instrucao
- Para `IFK`: salta para o bloco else ou para apos o if
- Para `WHILEK`: salta para apos o loop

**Q14 - Desvio incondicional (goto):**
- Gerada em dois contextos:
  - `IFK` com else: ao final do bloco then, para pular o bloco else (`goto Lfim`)
  - `WHILEK`: ao final do corpo, para voltar ao inicio do loop (`goto Linicio`)
- `resultado` e o label de destino

**Q15 - Label:**
- Gerada imediatamente antes do codigo de destino de um desvio
- `resultado` e o nome do label seguido de `:`
- Usada em `IFK` (label falso e label fim) e `WHILEK` (label inicio e label fim)

### 3.5 Operacoes com Arrays

| # | Quadrupla | Formato TAC | Descricao | Exemplo |
|---|---|---|---|---|
| Q16 | `(MUL_OFFSET, indice, 4, tN)` | `tN = indice * 4` | Calcula offset em bytes a partir do indice | `t1 = t0 * 4` |
| Q17 | `(ARRAY_LOAD, arr, offset, tN)` | `tN = arr[offset]` | Le o valor de um elemento do array | `t3 = v1[t1]` |
| Q18 | `(ARRAY_STORE, src, offset, arr)` | `arr[offset] = src` | Escreve um valor em um elemento do array | `v1[t1] = t2` |
| Q19 | `(ARRAY_DECL, arr, size, -)` | `array arr[size]` | Declara um array com tamanho especifico | `array v1[10]` |

**Q16 - Calculo de offset:**
- Gerada em `gerarOffsetBytes()`
- Converte indice logico (posicao) em offset fisico (bytes)
- Sempre multiplica por 4, assumindo inteiros de 4 bytes
- `arg1` e o temporario ou variavel contendo o indice
- `arg2` e a constante 4 (fixa)
- `resultado` e um novo temporario com o offset

**Q17 - Leitura de array:**
- Gerada em `gerarExpressao()`, caso `VARK` com `KIND_ARRAY` e indice nao-NULL
- `arg1` e o nome do array
- `arg2` e o offset em bytes (resultado de Q16)
- `resultado` e um novo temporario com o valor lido

**Q18 - Escrita em array:**
- Gerada em `gerarComandoExpressao()`, caso `ASSIGNK` com destino array
- `arg1` (src) e o valor a ser escrito (resultado de `gerarExpressao()` do lado direito)
- `arg2` (offset) e o resultado de `gerarExpressao()` do indice
- `resultado` e o nome do array
- Nota: nesta implementacao, o offset para escrita nao passa por `gerarOffsetBytes()` explicitamente na escrita; o indice gerado por `gerarExpressao()` do child[0] e usado diretamente

**Q19 - Declaracao de array:**
- Gerada em `gerarComando()`, caso `INTEGERK`/`VOIDK` para arrays
- `arg1` e o nome do array
- `arg2` e o tamanho (numero de elementos)
- Informa ao ambiente de execucao que deve alocar espaco para o array

### 3.6 Funcoes e Procedimentos

| # | Quadrupla | Formato TAC | Descricao | Exemplo |
|---|---|---|---|---|
| Q20 | `(FUNC_BEGIN, func, -, -)` | `func nome:` | Marca o inicio de uma funcao | `func gcd:` |
| Q21 | `(FUNC_END, -, -, -)` | `endfunc` | Marca o fim de uma funcao | `endfunc` |
| Q22 | `(PARAM_DECL, nome, -, -)` | `param nome` (em declaracao) | Declara um parametro formal da funcao | `param u` |
| Q23 | `(PARAM_PASS, valor, -, -)` | `param valor` (em chamada) | Passa um argumento para a proxima chamada de funcao | `param t0` |
| Q24 | `(CALL_EXPR, func, nargs, tN)` | `tN = call func, nargs` | Chama funcao e armazena retorno em temporario | `t3 = call gcd, 2` |
| Q25 | `(CALL_STMT, func, nargs, -)` | `call func, nargs` | Chama funcao descartando o retorno (statement) | `call output, 1` |
| Q26 | `(RETURN_VAL, valor, -, -)` | `return valor` | Retorna de funcao com um valor | `return t0` |
| Q27 | `(RETURN_VOID, -, -, -)` | `return` | Retorna de funcao sem valor (void) | `return` |

**Q20 - Inicio de funcao:**
- Gerada em `gerarComando()`, caso `INTEGERK`/`VOIDK` para declaracao de funcao
- `arg1` e o nome da funcao (`noIdentificador->kind.var.attr.name`)
- Marca o ponto de entrada da funcao no codigo intermediario

**Q21 - Fim de funcao:**
- Gerada imediatamente apos processar o corpo da funcao
- Marca o limite do escopo da funcao no codigo intermediario
- Funciona como par com Q20

**Q22 - Declaracao de parametro:**
- Gerada dentro da secao de declaracao de funcao
- Percorre a lista de parametros (siblings) do no de funcao
- `arg1` e o nome do parametro formal
- Emitida apos Q20 e antes do codigo do corpo

**Q23 - Passagem de argumento:**
- Gerada em `gerarExpressao()` (caso `CALLK`) e em `gerarComandoExpressao()` (caso `CALLK`)
- Para cada argumento, `gerarExpressao()` e chamada primeiro, e o resultado e passado com `param`
- `arg1` e o temporario ou variavel contendo o valor do argumento
- Argumentos sao emitidos **na ordem** em que aparecem, da esquerda para a direita
- Devem ser emitidos imediatamente antes da instrucao `call` correspondente

**Q24 - Chamada de funcao como expressao:**
- Gerada em `gerarExpressao()`, caso `CALLK`
- Usada quando o retorno da funcao e utilizado (ex: `x = func(...)` ou `func1(func2(...))`)
- `arg1` e o nome da funcao
- `arg2` e o numero de argumentos passados
- `resultado` e um novo temporario que recebe o valor de retorno

**Q25 - Chamada de funcao como statement:**
- Gerada em `gerarComandoExpressao()`, caso `CALLK`
- Usada quando a funcao e chamada e o retorno e descartado (ex: `output(x);`)
- Mesmo formato que Q24, mas sem temporario de resultado
- `arg1` e o nome da funcao, `arg2` e o numero de argumentos

**Q26 - Return com valor:**
- Gerada em `gerarComando()`, caso `RETURNK` com `child[0] != NULL`
- `arg1` e o resultado de `gerarExpressao()` aplicada a expressao de retorno
- Encerra a execucao da funcao e retorna o valor ao chamador

**Q27 - Return void:**
- Gerada em `gerarComando()`, caso `RETURNK` com `child[0] == NULL`
- Encerra a execucao da funcao sem retornar valor
- Usada em funcoes `void`

---

## 4. Tabela Resumo de Todas as Quadruplas

| # | Nome | Operador | Arg1 | Arg2 | Resultado | Formato TAC |
|---|---|---|---|---|---|---|
| Q1 | Carga de constante | `LOAD_CONST` | constante | - | temporario | `tN = const` |
| Q2 | Atribuicao simples | `ASSIGN` | fonte | - | variavel | `var = src` |
| Q3 | Soma | `ADD` | operando1 | operando2 | temporario | `tN = a + b` |
| Q4 | Subtracao | `SUB` | operando1 | operando2 | temporario | `tN = a - b` |
| Q5 | Multiplicacao | `MUL` | operando1 | operando2 | temporario | `tN = a * b` |
| Q6 | Divisao | `DIV` | operando1 | operando2 | temporario | `tN = a / b` |
| Q7 | Menor que | `LT` | operando1 | operando2 | temporario | `tN = a < b` |
| Q8 | Menor ou igual | `LE` | operando1 | operando2 | temporario | `tN = a <= b` |
| Q9 | Maior que | `GT` | operando1 | operando2 | temporario | `tN = a > b` |
| Q10 | Maior ou igual | `GE` | operando1 | operando2 | temporario | `tN = a >= b` |
| Q11 | Igualdade | `EQ` | operando1 | operando2 | temporario | `tN = a == b` |
| Q12 | Diferenca | `NE` | operando1 | operando2 | temporario | `tN = a != b` |
| Q13 | Desvio condicional | `IF_FALSE` | condicao | - | label | `if_false c goto L` |
| Q14 | Desvio incondicional | `GOTO` | - | - | label | `goto L` |
| Q15 | Definicao de label | `LABEL` | - | - | label | `L:` |
| Q16 | Offset de array | `MUL_OFFSET` | indice | 4 | temporario | `tN = idx * 4` |
| Q17 | Leitura de array | `ARRAY_LOAD` | array | offset | temporario | `tN = arr[off]` |
| Q18 | Escrita em array | `ARRAY_STORE` | valor | offset | array | `arr[off] = val` |
| Q19 | Declaracao de array | `ARRAY_DECL` | array | tamanho | - | `array arr[N]` |
| Q20 | Inicio de funcao | `FUNC_BEGIN` | nome | - | - | `func nome:` |
| Q21 | Fim de funcao | `FUNC_END` | - | - | - | `endfunc` |
| Q22 | Parametro formal | `PARAM_DECL` | nome | - | - | `param nome` |
| Q23 | Passagem de argumento | `PARAM_PASS` | valor | - | - | `param valor` |
| Q24 | Chamada com retorno | `CALL_EXPR` | funcao | num_args | temporario | `tN = call f, N` |
| Q25 | Chamada sem retorno | `CALL_STMT` | funcao | num_args | - | `call f, N` |
| Q26 | Retorno com valor | `RETURN_VAL` | valor | - | - | `return val` |
| Q27 | Retorno void | `RETURN_VOID` | - | - | - | `return` |

**Total: 27 tipos de quadruplas**

---

## 5. Exemplo Completo

Dado o programa C-Minus:

```c
int gcd(int u, int v) {
    if (v == 0)
        return u;
    else
        return gcd(v, u - (u/v) * v);
}

void main(void) {
    int x; int y;
    x = input();
    y = input();
    output(gcd(x, y));
}
```

O codigo intermediario gerado (TAC) e:

```
func gcd:
param u
param v
t0 = 0
t1 = v == t0
if_false t1 goto L0
return u
goto L1
L0:
t2 = u / v
t3 = t2 * v
t4 = u - t3
param v
param t4
t5 = call gcd, 2
return t5
L1:
endfunc

func main:
t6 = call input, 0
x = t6
t7 = call input, 0
y = t7
param x
param y
t8 = call gcd, 2
param t8
call output, 1
endfunc
```

### Mapeamento para quadruplas:

| Linha TAC | Quadrupla | # |
|---|---|---|
| `func gcd:` | `(FUNC_BEGIN, gcd, -, -)` | Q20 |
| `param u` | `(PARAM_DECL, u, -, -)` | Q22 |
| `param v` | `(PARAM_DECL, v, -, -)` | Q22 |
| `t0 = 0` | `(LOAD_CONST, 0, -, t0)` | Q1 |
| `t1 = v == t0` | `(EQ, v, t0, t1)` | Q11 |
| `if_false t1 goto L0` | `(IF_FALSE, t1, -, L0)` | Q13 |
| `return u` | `(RETURN_VAL, u, -, -)` | Q26 |
| `goto L1` | `(GOTO, -, -, L1)` | Q14 |
| `L0:` | `(LABEL, -, -, L0)` | Q15 |
| `t2 = u / v` | `(DIV, u, v, t2)` | Q6 |
| `t3 = t2 * v` | `(MUL, t2, v, t3)` | Q5 |
| `t4 = u - t3` | `(SUB, u, t3, t4)` | Q4 |
| `param v` | `(PARAM_PASS, v, -, -)` | Q23 |
| `param t4` | `(PARAM_PASS, t4, -, -)` | Q23 |
| `t5 = call gcd, 2` | `(CALL_EXPR, gcd, 2, t5)` | Q24 |
| `return t5` | `(RETURN_VAL, t5, -, -)` | Q26 |
| `L1:` | `(LABEL, -, -, L1)` | Q15 |
| `endfunc` | `(FUNC_END, -, -, -)` | Q21 |
| `func main:` | `(FUNC_BEGIN, main, -, -)` | Q20 |
| `t6 = call input, 0` | `(CALL_EXPR, input, 0, t6)` | Q24 |
| `x = t6` | `(ASSIGN, t6, -, x)` | Q2 |
| `t7 = call input, 0` | `(CALL_EXPR, input, 0, t7)` | Q24 |
| `y = t7` | `(ASSIGN, t7, -, y)` | Q2 |
| `param x` | `(PARAM_PASS, x, -, -)` | Q23 |
| `param y` | `(PARAM_PASS, y, -, -)` | Q23 |
| `t8 = call gcd, 2` | `(CALL_EXPR, gcd, 2, t8)` | Q24 |
| `param t8` | `(PARAM_PASS, t8, -, -)` | Q23 |
| `call output, 1` | `(CALL_STMT, output, 1, -)` | Q25 |
| `endfunc` | `(FUNC_END, -, -, -)` | Q21 |

---

## 6. Tipos de Operandos nas Quadruplas

| Tipo de operando | Descricao | Exemplos |
|---|---|---|
| Temporario | Variavel gerada pelo compilador | `t0`, `t1`, `t2`, ... |
| Variavel | Nome de variavel do programa fonte | `x`, `y`, `u`, `v` |
| Constante | Valor inteiro literal | `0`, `1`, `42`, `4` |
| Label | Rotulo de desvio | `L0`, `L1`, `L2`, ... |
| Nome de funcao | Identificador de funcao | `gcd`, `main`, `input`, `output` |
| Numero de argumentos | Inteiro indicando quantidade de parametros | `0`, `1`, `2` |
| Vazio | Campo nao utilizado | `-` |

---

## 7. Relacao entre Construcoes da Linguagem e Quadruplas Geradas

| Construcao C-Minus | Quadruplas geradas |
|---|---|
| `int x;` | (nenhuma -- variavel existe implicitamente) |
| `int arr[10];` | Q19 |
| `int f(int a, int b) { ... }` | Q20, Q22, Q22, ..., Q21 |
| `x = expr;` | Quadruplas de `expr` + Q2 |
| `arr[i] = expr;` | Quadruplas de `i` + Quadruplas de `expr` + Q18 |
| `... arr[i] ...` | Quadruplas de `i` + Q16 + Q17 |
| `a + b` | Quadruplas de `a` + Quadruplas de `b` + Q3 |
| `a - b` | Quadruplas de `a` + Quadruplas de `b` + Q4 |
| `a * b` | Quadruplas de `a` + Quadruplas de `b` + Q5 |
| `a / b` | Quadruplas de `a` + Quadruplas de `b` + Q6 |
| `a < b` | Quadruplas de `a` + Quadruplas de `b` + Q7 |
| `a <= b` | Quadruplas de `a` + Quadruplas de `b` + Q8 |
| `a > b` | Quadruplas de `a` + Quadruplas de `b` + Q9 |
| `a >= b` | Quadruplas de `a` + Quadruplas de `b` + Q10 |
| `a == b` | Quadruplas de `a` + Quadruplas de `b` + Q11 |
| `a != b` | Quadruplas de `a` + Quadruplas de `b` + Q12 |
| `42` | Q1 |
| `x` (em expressao) | (nenhuma -- retorna nome) |
| `if (c) { T }` | Quadruplas de `c` + Q13 + Quadruplas de `T` + Q15 |
| `if (c) { T } else { E }` | Quadruplas de `c` + Q13 + Quadruplas de `T` + Q14 + Q15(Lfalso) + Quadruplas de `E` + Q15(Lfim) |
| `while (c) { B }` | Q15(Linicio) + Quadruplas de `c` + Q13 + Quadruplas de `B` + Q14 + Q15(Lfim) |
| `return expr;` | Quadruplas de `expr` + Q26 |
| `return;` | Q27 |
| `f(a1, a2, ..., aN)` (expr) | Quadruplas de `a1` + Q23 + ... + Quadruplas de `aN` + Q23 + Q24 |
| `f(a1, a2, ..., aN);` (stmt) | Quadruplas de `a1` + Q23 + ... + Quadruplas de `aN` + Q23 + Q25 |
