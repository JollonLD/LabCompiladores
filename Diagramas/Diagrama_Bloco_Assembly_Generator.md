# Diagrama de Blocos — `assembly_generator.c`

Visão modular do gerador de assembly: blocos funcionais, estruturas de dados e dependências internas.

---

## 1. Contexto externo

```mermaid
flowchart LR
    CG["code_generator.c\n(lista de quádruplas)"]
    ASM["assembly_generator.c"]
    BIN["binary_generator.c"]
    OUT1[("saida_asm.txt")]
    OUT2[("saida_bin.txt")]

    CG -->|"traduzirQuadruplasParaAssembly()"| ASM
    ASM --> OUT1
    ASM -->|"traduzirArquivoAssemblyParaBinario()"| BIN
    BIN --> OUT2
```

---

## 2. Blocos internos

```mermaid
flowchart TB
    subgraph API["Bloco de interface"]
        TQA["traduzirQuadruplasParaAssembly()"]
    end

    subgraph PRE["Bloco de pré-processamento"]
        CIV["coletarInitsVetoresGlobais()"]
        MLPL["mapearLabelsParaLinhas()"]
        CIA["contarInstrucoesAssembly()"]
    end

    subgraph TRAD["Bloco de tradução"]
        TQ["traduzirQuadrupla()"]
        OQ["operadorQuadrupla()"]
        BSA["branchOuSaltoAsm()"]
        NIA["nomeInstrucaoAssembly()"]
    end

    subgraph LAB["Bloco de labels"]
        AL["adicionarLabel()"]
        BLL["buscarLinhaLabel()"]
        LL["liberarLabels()"]
    end

    subgraph REG["Bloco de registradores"]
        MPR["mapearParaRegistradorGeral()"]
        AIR["alocarIndiceRegistradorGeral()"]
        LMR["liberarMapaRegistradores()"]
    end

    subgraph ESC["Bloco de escopo e variáveis"]
        BEV["buscarEscopoVariavel()"]
        AEV["adicionarEscopoVariavel()"]
        IVNE["inserirVariavelNoEscopo()"]
        IVET["inserirVetorNoEscopo()"]
        BVNE["buscarVariavelNoEscopo()"]
        BPV["baseParaVariavel()"]
        LMEV["liberarMapaEscoposVariaveis()"]
    end

    subgraph VET["Bloco de vetores"]
        EIVG["emitirInitsVetoresGlobais()"]
        EIDVL["emitirInitDescritorVetorLocal()"]
        LIVG["liberarInitsVetoresGlobais()"]
    end

    subgraph SAIDA["Bloco de saída"]
        AP["asmPrint()"]
        ARQ[("arquivoSaidaAssembly")]
    end

    subgraph UTIL["Bloco utilitário"]
        EN["ehNumero()"]
        OV["operandoValido()"]
        DT["duplicarTexto()"]
        EFM["ehFuncaoMain()"]
        FPR["funcaoPossuiReturn()"]
        EEG["escopoEhGlobal()"]
        EQ["ehQuadrupla()"]
    end

    subgraph EST["Estruturas de estado"]
        S1["listaLabels"]
        S2["mapaRegistradores"]
        S3["listaEscoposVariaveis"]
        S4["listaInitsVetoresGlobais"]
        S5["escopoTraducaoAtual"]
        S6["listaQuadruplasAtual"]
    end

    TQA --> PRE
    TQA --> SAIDA
    TQA --> TRAD
    TQA --> REG
    TQA --> ESC
    TQA --> LAB
    TQA --> VET

    MLPL --> CIA
    MLPL --> LAB
    MLPL --> OQ

    TQ --> OQ
    TQ --> BSA
    TQ --> NIA
    TQ --> MPR
    TQ --> BVNE
    TQ --> BPV
    TQ --> BLL
    TQ --> ESC
    TQ --> VET
    TQ --> UTIL
    TQ --> AP

    MPR --> REG
    IVNE --> ESC
    IVET --> ESC
    EIVG --> AP
    EIDVL --> AP

    LAB --- S1
    REG --- S2
    ESC --- S3
    VET --- S4
    SAIDA --- ARQ
    UTIL --- S5
    UTIL --- S6
```

---

## 3. Fluxo de dados entre blocos

```mermaid
flowchart TD
    IN(["Entrada: quadList*"])
    IN --> INIT["Reset: registradores, escopos, escopo=global"]
    INIT --> P1["Pré-processamento\n(vetores globais + mapa de labels)"]
    P1 --> P2["Abrir saida_asm.txt"]
    P2 --> P3["Bootstrap: nop + inits globais + j main"]
    P3 --> LOOP["Para cada quádrupla"]
    LOOP --> TQ["traduzirQuadrupla\n(switch por operador)"]
    TQ --> OUT["Linhas de assembly"]
    OUT --> LOOP
    LOOP -->|fim| FIN["Fechar arquivo + liberar estado"]
    FIN --> BIN["Invocar binary_generator"]

    subgraph dados["Dados consultados na tradução"]
        D1["Mapa label → linha PC"]
        D2["Mapa símbolo → $rN"]
        D3["Escopo → variável → offset"]
    end

    P1 -.-> D1
    TQ -.-> D1
    TQ -.-> D2
    TQ -.-> D3
```

---

## 4. Bloco de tradução — casos de `traduzirQuadrupla`

```mermaid
flowchart LR
    TQ["traduzirQuadrupla()"]

    subgraph ARIT["Aritmética"]
        A1["ADD / SUB / MULT / DIV"]
    end

    subgraph MEM["Memória"]
        M1["LOADCONST / LOADVAR / STOREVAR"]
        M2["LOADVET / STOREVET"]
    end

    subgraph CTRL["Controle de fluxo"]
        C1["BEQ…BNE / JUMP / LABEL"]
    end

    subgraph PILHA["Pilha e alocação"]
        P1["ARG / ALLOCAMEMVAR / ALLOCAMEMVET"]
        P2["PARAM"]
    end

    subgraph FUN["Funções"]
        F1["FUNC / CALL / RETURN / ENDFUNC"]
    end

    subgraph SYS["Sistema"]
        S1["NOP / HALT"]
    end

    TQ --> ARIT
    TQ --> MEM
    TQ --> CTRL
    TQ --> PILHA
    TQ --> FUN
    TQ --> SYS
```

---

## 5. Inventário de funções por bloco

| Bloco | Funções |
|---|---|
| Interface | `traduzirQuadruplasParaAssembly` |
| Pré-processamento | `coletarInitsVetoresGlobais`, `mapearLabelsParaLinhas`, `contarInstrucoesAssembly` |
| Tradução | `traduzirQuadrupla`, `operadorQuadrupla`, `branchOuSaltoAsm`, `nomeInstrucaoAssembly` |
| Labels | `adicionarLabel`, `buscarLinhaLabel`, `liberarLabels` |
| Registradores | `mapearParaRegistradorGeral`, `alocarIndiceRegistradorGeral`, `registradorExclusivo`, `liberarMapaRegistradores` |
| Escopo / variáveis | `buscarEscopoVariavel`, `adicionarEscopoVariavel`, `inserirVariavelNoEscopo`, `inserirVetorNoEscopo`, `buscarVariavelNoEscopo`, `baseParaEscopo`, `baseParaVariavel`, `liberarMapaEscoposVariaveis` |
| Vetores | `emitirInitsVetoresGlobais`, `emitirInitDescritorVetorLocal`, `liberarInitsVetoresGlobais` |
| Saída | `asmPrint` |
| Utilitário | `ehNumero`, `operandoValido`, `duplicarTexto`, `ehFuncaoMain`, `funcaoPossuiReturn`, `escopoEhGlobal`, `ehQuadrupla`, `emitirComentarioMemoria` |

| Estrutura | Papel |
|---|---|
| `LabelLinha` | Nome de label → linha assembly |
| `MapaRegistrador` | Temporário/símbolo → `$rN` |
| `VariavelEscopo` | Variável ou vetor com deslocamento na pilha |
| `EscopoVariavel` | Função ou global com lista de variáveis |
| `VetorGlobalInit` | Fila de inits de descritores globais |
