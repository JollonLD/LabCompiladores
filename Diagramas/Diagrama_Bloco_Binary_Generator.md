# Diagrama de Blocos — `binary_generator.c`

Visão modular do gerador binário: blocos funcionais, formatos de instrução e dependências internas.

---

## 1. Contexto externo

```mermaid
flowchart LR
    ASM["assembly_generator.c"]
    BIN["binary_generator.c"]
    IN[("saida_asm.txt")]
    OUT[("saida_bin.txt")]

    ASM -->|"traduzirArquivoAssemblyParaBinario()"| BIN
    IN --> BIN
    BIN --> OUT
```

---

## 2. Blocos internos

```mermaid
flowchart TB
    subgraph API["Bloco de interface"]
        TAB["traduzirArquivoAssemblyParaBinario()"]
    end

    subgraph IO["Bloco de E/S de arquivo"]
        FOPEN["fopen / fclose"]
        FGETS["fgets (linha a linha)"]
        EB["escreverBinario()"]
    end

    subgraph PARSE["Bloco parser de linha"]
        CL["codificarLinha()"]
        LT["lerToken()"]
        LI["lerInteiro()"]
        PE["pularEspacos()"]
        PV["pularVirgula()"]
        PR["parseRegistrador()"]
        PM["paraMinusculas()"]
    end

    subgraph OPC["Bloco tabela de opcodes"]
        MAP["mapaOpcodes[]"]
        BO["buscarOpcode()"]
    end

    subgraph COD["Bloco codificadores"]
        CTR["codificarTipoR()"]
        CI14["codificarTipoI14()"]
        CI20["codificarTipoI20()"]
        CB["codificarBranch()"]
        CRU["codificarRegistradorUnico()"]
    end

    subgraph MASK["Bloco máscaras de imediato"]
        IM14["montarIm14()"]
        IM20["montarIm20()"]
        IM26["montarIm26()"]
    end

    subgraph EST["Estado de tradução"]
        PC["enderecoPc"]
        WORD["instrucao (32 bits)"]
        ERRO["buffer de erro"]
    end

    TAB --> IO
    TAB --> PARSE
    IO --> FGETS
    FGETS --> CL
    CL --> BO
    CL --> PARSE
    CL --> COD
    BO --> MAP
    BO --> PM

    CTR --> WORD
    CI14 --> IM14
    CI14 --> WORD
    CI20 --> IM20
    CI20 --> WORD
    CB --> IM14
    CB --> WORD
    CRU --> WORD

    CL -->|"opcode + im26"| CI20
    TAB --> EB
    EB --> OUT[("saida_bin.txt")]
    TAB --- PC
    CL --- ERRO
```

---

## 3. Fluxo de dados entre blocos

```mermaid
flowchart TD
    START["Abrir assembly e binário"]
    START --> PC0["PC = 0"]
    PC0 --> READ["Ler linha"]
    READ --> EOF{EOF?}
    EOF -->|sim| CLOSE["Fechar arquivos"]
    EOF -->|não| CL["codificarLinha()"]
    CL --> RES{Resultado}
    RES -->|vazia/comentário| READ
    RES -->|erro| ERR["stderr + abortar"]
    RES -->|ok| EB["escreverBinario()\n32 caracteres 0/1"]
    EB --> INC["PC++"]
    INC --> READ
    CLOSE --> OK["Retornar sucesso"]
```

---

## 4. Bloco `codificarLinha` — decisão por formato

```mermaid
flowchart TB
    CL["codificarLinha()"]
    CL --> IGN{Comentário\nou vazia?}
    IGN -->|sim| SKIP["Retornar 0 (pular)"]
    IGN -->|não| MNEM["buscarOpcode()"]

    MNEM --> FMT{Tipo?}

    FMT -->|nop / hlt| F0["opcode << 26"]
    FMT -->|j / jal| F26["opcode + im26"]
    FMT -->|jr / in / out / push / pop| FREG["codificarRegistradorUnico"]
    FMT -->|li / lw / sw / swr| F20["codificarTipoI20"]
    FMT -->|move| FMOV["opcode + rd + rs"]
    FMT -->|lwd / swd| F14M["codificarTipoI14"]
    FMT -->|beq…bne| FBR["codificarBranch\noffset = dest − PC − 1"]
    FMT -->|add / sub / mult / div…| FR["codificarTipoR"]
    FMT -->|not| FNOT["codificarTipoR (rt=0)"]
    FMT -->|addi / subi / multi…| F14I["codificarTipoI14"]

    F0 --> RET["Retornar palavra 32 bits"]
    F26 --> RET
    FREG --> RET
    F20 --> RET
    FMOV --> RET
    F14M --> RET
    FBR --> RET
    FR --> RET
    FNOT --> RET
    F14I --> RET
```

---

## 5. Formatos de palavra de instrução

```mermaid
flowchart LR
    subgraph R["Tipo R"]
        R1["opcode | rd | rs | rt"]
    end

    subgraph I14["Tipo I14"]
        I1["opcode | rd | rs | im14"]
    end

    subgraph I20["Tipo I20"]
        I2["opcode | campo | im20"]
    end

    subgraph I26["Tipo I26"]
        I3["opcode | im26"]
    end

    subgraph REG1["Registrador único"]
        U1["opcode | reg | …"]
    end

    CL2["codificarLinha()"] --> R
    CL2 --> I14
    CL2 --> I20
    CL2 --> I26
    CL2 --> REG1
```

---

## 6. Inventário de funções por bloco

| Bloco | Funções |
|---|---|
| Interface | `traduzirArquivoAssemblyParaBinario` |
| E/S | `escreverBinario` (+ `fopen`, `fgets`, `fclose` na interface) |
| Parser | `codificarLinha`, `lerToken`, `lerInteiro`, `pularEspacos`, `pularVirgula`, `parseRegistrador`, `paraMinusculas` |
| Opcodes | `buscarOpcode`, `mapaOpcodes[]` |
| Codificadores | `codificarTipoR`, `codificarTipoI14`, `codificarTipoI20`, `codificarBranch`, `codificarRegistradorUnico` |
| Máscaras | `montarIm14`, `montarIm20`, `montarIm26` |

| Constante | Uso |
|---|---|
| `IM14_MASK` | Limita imediato de 14 bits |
| `IM20_MASK` | Limita imediato de 20 bits |
| `IM26_MASK` | Limita endereço de salto |
| `OPCODE_NOP_PADDING` | Padding especial do `nop` |

| Mnemônicos suportados (grupos) | Codificador |
|---|---|
| `nop`, `hlt` | word simples |
| `j`, `jal` | I26 |
| `jr`, `in`, `out`, `push`, `pop` | registrador único |
| `li`, `lw`, `lwr`, `sw`, `swr` | I20 |
| `move` | híbrido (rd + rs) |
| `lwd`, `swd` | I14 |
| `beq`, `bgt`, `blt`, `bge`, `ble`, `bne` | branch (I14 + offset relativo) |
| `add`, `sub`, `mult`, `div`, `and`, `or`, `sr`, `sl` | R |
| `not` | R (rt = 0) |
| `addi`, `subi`, `multi`, `andi`, `ori` | I14 |
