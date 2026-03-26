# Tabela de quádruplas (alinhada ao `gerador_intermediario_base.c`)

Esta tabela define o conjunto de operações do **código intermediário em quádruplas** usado como referência para o compilador C- **e** espelha o comportamento do arquivo `gerador_intermediario_base.c` (vetor `OpString[]`, ordem dos operandos `(end1, end2, end3)`).

**Formato textual sugerido (CSV):** `op,arg1,arg2,arg3` — use `___` ou campo vazio quando o operando não se aplica.

**Convenções:** temporários `t0…tN`, labels `l0…lN`, retorno de chamada lógico `$rf`, **escopo** = nome da função (ou globais) para identificar variáveis na memória.

---

## Tabela principal

| # | Operação | Arg1 | Arg2 | Arg3 | Semântica (TAC) |
|---|----------|------|------|------|-------------------|
| 1 | `add` | esquerda | direita | destino `tN` | `tN = arg1 + arg2` |
| 2 | `sub` | esquerda | direita | destino `tN` | `tN = arg1 - arg2` |
| 3 | `mult` | esquerda | direita | destino `tN` | `tN = arg1 * arg2` |
| 4 | `divisao` | esquerda | direita | destino `tN` | `tN = arg1 / arg2` |
| 5 | `slt` | esquerda | direita | destino `tN` | `tN = arg1 < arg2` |
| 6 | `sgt` | esquerda | direita | destino `tN` | `tN = arg1 > arg2` |
| 7 | `slet` | esquerda | direita | destino `tN` | `tN = arg1 <= arg2` |
| 8 | `sget` | esquerda | direita | destino `tN` | `tN = arg1 >= arg2` |
| 9 | `set` | esquerda | direita | destino `tN` | `tN = arg1 == arg2` |
| 10 | `sdt` | esquerda | direita | destino `tN` | `tN = arg1 != arg2` |
| 11 | `ifFalso` | condição (`tN` ou const) | label destino | _ | `if_false cond goto label` |
| 12 | `jump` | label destino | _ | _ | `goto label` |
| 13 | `label_op` | nome do label | _ | _ | `label:` |
| 14 | `funInicio` | nome da função | _ | _ | início de função |
| 15 | `funFim` | nome da função | _ | _ | fim de função |
| 16 | `param` | valor do argumento | _ | _ | prepara argumento para `call` |
| 17 | `call` | nome da função | nº de argumentos (inteiro) | _ | chama função; resultado em `$rf` |
| 18 | `move` | valor retornado | `$rf` | _ | `return` com expressão |
| 19 | `allocaMemVar` | escopo | nome da variável | _ | aloca variável escalar (local/global conforme escopo) |
| 20 | `allocaMemVet` | escopo | nome do vetor | tamanho (inteiro) | aloca vetor (`-1` se tamanho ausente no AST) |
| 21 | `loadVar` | escopo | nome da variável / vetor | destino `tN` | carrega escalar **ou** endereço base do vetor em `tN` |
| 22 | `loadVet` | endereço (`tN`) | destino `tN` (valor lido) | _ | lê palavra na memória: `*addr → dest` |
| 23 | `storeVar` | valor | nome da variável | escopo | grava escalar: `var = valor` |
| 24 | `storeVet` | valor | endereço final (`tN`) | _ | grava em vetor: `*addr = valor` |
| 25 | `empilha` | índice do registrador (0…25) | _ | _ | salva temporário ativo antes de `call` |
| 26 | `desempilha` | índice do registrador | _ | _ | restaura temporário após `call` |

---

## O que foi unificado ou removido em relação à versão antiga da tabela

| Antes | Agora |
|-------|--------|
| `LOAD_CONST` / `LOAD` genéricos | Literais entram como **operandos const** nas operações binárias; variáveis usam **`loadVar`** + **`storeVar`**. |
| `LT`…`NE` com nomes MIPS-like | Alinhado ao base: **`slt`…`sdt`** (mesma ordem de tokens do `verificaOp`). |
| `DIV` | **`divisao`** (nome do `OpString` no base). |
| `GOTO` / `LABEL` | **`jump`** / **`label_op`**. |
| `FUNC_BEGIN` / `FUNC_END` | **`funInicio`** / **`funFim`**. |
| `PARAM_DECL` separado | Formais podem ser tratados como **`allocaMemVar`** no escopo da função; o base não emite quad separada só de “declaração de parâmetro”. |
| `PARAM_PASS` | **`param`**. |
| `CALL_EXPR` e `CALL_STMT` | Uma só: **`call`**; expressão usa o resultado em **`$rf`**. |
| `RETURN_VAL` / `RETURN_VOID` | Com valor: **`move`**; sem valor: omitir ou quad extra `return_void` no *seu* gerador se precisar. |
| `MUL_OFFSET` / `ARRAY_*` genéricos | Endereço de elemento: **`loadVar`** (base) + **`add`** (base + índice) + **`loadVet`** / **`storeVet`**. Se o back-end exigir `×4`, faça isso com **`mult`** (const 4) ou na geração de assembly — não é quad separada obrigatória. |
| `ARRAY_DECL` | **`allocaMemVet`**. |

---

## Onde cada operação é usada na geração do código intermediário

Ordem aproximada conforme a árvore sintática e o `gerador_intermediario_base.c`:

| Operação | Momento / nó da AST |
|----------|---------------------|
| **`add` … `sdt`** | Em **`genExp`**, nó `OpK`: após gerar filhos esquerda/direita, resultado em temporário `tN`. Também **`add`** em atribuição/leitura de vetor: **endereço base + índice** → `tN` antes de `loadVet` / `storeVet`. |
| **`ifFalso`** | **`IfK`**: após gerar a condição; se falsa, pula o *then*. Em **`WhileK`**: após gerar a condição; se falsa, sai do laço. |
| **`jump`** | **`IfK`** com *else*: salto do fim do *then* para o fim do `if`. **`WhileK`**: salto do fim do corpo de volta ao label do início. |
| **`label_op`** | Marca destinos de **`ifFalso`** e **`jump`**, início de `while`, e junção do `if-else`. |
| **`funInicio`** / **`funFim`** | **`FuncaoK`**: envolve o processamento de parâmetros (`child[0]`) e corpo (`child[1]`); libera registradores ao fim. |
| **`allocaMemVar`** | **`VarK`**: declaração de variável escalar (local/global conforme `escopo`). |
| **`allocaMemVet`** | **`VetK`**: declaração de vetor; arg3 = tamanho. |
| **`loadVar`** | **`VarIdK`**: leitura de variável escalar. **`VetIdK`** e atribuição a vetor: primeiro carrega **base** do vetor em `tN`. |
| **`loadVet`** | **`VetIdK`**: após `loadVar` + `add` (base + índice), lê o valor para um temporário. |
| **`storeVar`** | **`AssignK`** com destino variável simples (`VarIdK`): valor da expressão → memória. |
| **`storeVet`** | **`AssignK`** com destino vetor (`VetIdK`): após calcular endereço em `tN`, grava o valor. |
| **`param`** | **`CallK`** (statement ou expressão): para cada argumento, gera expressão e emite `param` com o valor em `atual`. |
| **`empilha`** / **`desempilha`** | Antes de **`call`** e depois dela (exceto para funções especiais como `input`/`output` no base): preserva temporários em uso. |
| **`call`** | **`CallK`**: após os `param`s; arg2 = número de argumentos; resultado tratado como **`$rf`**. |
| **`move`** | **`ReturnK`** com expressão: move o valor da expressão para **`$rf`** (retorno da função). |

---

## Referência cruzada

Documento detalhado: **`Estudos/QUADRUPLAS_GERADOR_INTERMEDIARIO_BASE.md`**.
