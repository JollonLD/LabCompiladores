## Tabela de Quadruplas

| # | Nome | Operador | Arg1 | Arg2 | Arg3 | Formato TAC |
|---|---|---|---|---|---|---|
| Q1 | Atribuição de constante | `LOAD_CONST` | RD | CONST | _ | `tN = const` |
| Q2 | Atribuicao simples | `LOAD` | Destino | Fonte | _ | `var = src` |
| Q3 | Soma | `ADD` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a + b` |
| Q4 | Subtracao | `SUB` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a - b` |
| Q5 | Multiplicacao | `MUL` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a * b` |
| Q6 | Divisao | `DIV` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a / b` |
| Q7 | Menor que | `LT` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a < b` |
| Q8 | Menor ou igual | `LE` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a <= b` |
| Q9 | Maior que | `GT` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a > b` |
| Q10 | Maior ou igual | `GE` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a >= b` |
| Q11 | Igualdade | `EQ` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a == b` |
| Q12 | Diferenca | `NE` | operando1 (RD) | operando2 (RS) | operando3 (RT) | `tN = a != b` |
| Q13 | Desvio condicional | `IF_FALSE` | condicao | - | label | `if_false c goto L` |
| Q14 | Desvio incondicional | `GOTO` | - | - | label | `goto L` |
| Q15 | Definicao de label | `LABEL` | - | - | label | `L:` |
| Q16 | Offset de array | `MUL_OFFSET`| temporario | indice | 4 | `tN = idx * 4` |
| Q17 | Leitura de array | `ARRAY_LOAD`| temporario | array | offset | `tN = arr[off]` |
| Q18 | Escrita em array | `ARRAY_STORE`| array | valor | offset | `arr[off] = val` |
| Q19 | Declaracao de array | `ARRAY_DECL` | array | tamanho | - | `array arr[N]` |
| Q20 | Inicio de funcao | `FUNC_BEGIN` | nome | - | - | `func nome:` |
| Q21 | Fim de funcao | `FUNC_END` | - | - | - | `endfunc` |
| Q22 | Parametro formal | `PARAM_DECL` | nome | - | - | `param nome` |
| Q23 | Passagem de argumento | `PARAM_PASS` | valor | - | - | `param valor` |
| Q24 | Chamada com retorno | `CALL_EXPR` | funcao | num_args | temporario | `tN = call f, N` |
| Q25 | Chamada sem retorno | `CALL_STMT` | funcao | num_args | - | `call f, N` |
| Q26 | Retorno com valor | `RETURN_VAL` | valor | - | - | `return val` |
| Q27 | Retorno void | `RETURN_VOID` | - | - | - | `return` |