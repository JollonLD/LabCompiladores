#include "binary_generator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IM14_MASK 0x3FFFu
#define IM20_MASK 0xFFFFFu
#define IM26_MASK 0x3FFFFFFu
#define OPCODE_NOP_PADDING 0x3Fu

typedef struct {
    const char* nome;
    unsigned int opcode;
} MapaOpcode;

static const MapaOpcode mapaOpcodes[] = {
    {"add",   0x00}, {"sub",   0x01}, {"mult",  0x02}, {"div",   0x03},
    {"and",   0x04}, {"or",    0x05}, {"not",   0x06},
    {"addi",  0x07}, {"subi",  0x08}, {"multi", 0x09},
    {"andi",  0x0A}, {"ori",   0x0B}, {"sr",    0x0C}, {"sl",    0x0D},
    {"bge",   0x0E}, {"beq",   0x0F}, {"bgt",   0x10}, {"blt",   0x11},
    {"ble",   0x12}, {"move",  0x13}, {"li",    0x14}, {"lw",    0x15},
    {"sw",    0x16}, {"lwr",   0x17}, {"swr",   0x18}, {"lwd",   0x19},
    {"swd",   0x1A}, {"j",     0x1B}, {"jr",    0x1C}, {"jal",   0x1D},
    {"push",  0x1E}, {"pop",   0x1F}, {"in",    0x20}, {"out",   0x21},
    {"hlt",   0x23}, {"bne",   0x24},
    {NULL, 0}
};

static void paraMinusculas(char* dest, const char* origem, size_t tamanho) {
    size_t i;

    for (i = 0; origem[i] != '\0' && i + 1 < tamanho; i++)
        dest[i] = (char)tolower((unsigned char)origem[i]);
    dest[i] = '\0';
}

static unsigned int montarIm14(unsigned int valor) {
    return valor & IM14_MASK;
}

static unsigned int montarIm20(unsigned int valor) {
    return valor & IM20_MASK;
}

static unsigned int montarIm26(unsigned int valor) {
    return valor & IM26_MASK;
}

static int buscarOpcode(const char* mnemonico, unsigned int* opcode) {
    char nome[32];
    size_t i;

    paraMinusculas(nome, mnemonico, sizeof(nome));

    if (strcmp(nome, "nop") == 0) {
        *opcode = OPCODE_NOP_PADDING;
        return 1;
    }

    for (i = 0; mapaOpcodes[i].nome != NULL; i++) {
        if (strcmp(mapaOpcodes[i].nome, nome) == 0) {
            *opcode = mapaOpcodes[i].opcode;
            return 1;
        }
    }
    return 0;
}

static int parseRegistrador(const char* token, unsigned int* reg) {
    char limpo[32];
    size_t i;
    int numero;
    const char* inicio;

    if (token == NULL || *token == '\0')
        return 0;

    i = 0;
    while (token[i] != '\0' && token[i] != ',')
        i++;
    if (i >= sizeof(limpo))
        return 0;
    memcpy(limpo, token, i);
    limpo[i] = '\0';

    if (strcmp(limpo, "$zero") == 0) { *reg = 0; return 1; }
    if (strcmp(limpo, "$ra") == 0)   { *reg = 1; return 1; }
    if (strcmp(limpo, "$rf") == 0)   { *reg = 2; return 1; }
    if (strcmp(limpo, "$rp") == 0)   { *reg = 3; return 1; }
    if (strcmp(limpo, "$sp") == 0)   { *reg = 3; return 1; }
    if (strcmp(limpo, "$fp") == 0)   { *reg = 30; return 1; }
    if (strcmp(limpo, "$ro") == 0)   { *reg = 31; return 1; }

    if (limpo[0] == '$' && limpo[1] == 'r') {
        numero = atoi(limpo + 2);
        if (numero < 0 || numero > 63)
            return 0;
        *reg = (unsigned int)numero;
        return 1;
    }

    if (limpo[0] == '$' && isdigit((unsigned char)limpo[1])) {
        numero = atoi(limpo + 1);
        if (numero < 0 || numero > 63)
            return 0;
        *reg = (unsigned int)numero;
        return 1;
    }

    inicio = limpo;
    if (inicio[0] == 'r' || inicio[0] == 'R')
        inicio++;
    if (*inicio != '\0' && isdigit((unsigned char)*inicio)) {
        numero = atoi(inicio);
        if (numero < 0 || numero > 63)
            return 0;
        *reg = (unsigned int)numero;
        return 1;
    }

    return 0;
}

static void pularEspacos(char** p) {
    while (**p != '\0' && isspace((unsigned char)**p))
        (*p)++;
}

static void pularVirgula(char** p) {
    pularEspacos(p);
    if (**p == ',') {
        (*p)++;
        pularEspacos(p);
    }
}

static int lerToken(char** p, char* dest, size_t tamanho) {
    size_t i = 0;

    pularEspacos(p);
    while (**p != '\0' && !isspace((unsigned char)**p) && **p != ',') {
        if (i + 1 < tamanho)
            dest[i++] = **p;
        (*p)++;
    }
    dest[i] = '\0';
    return i > 0;
}

static int lerInteiro(char** p, int* valor) {
    char token[32];

    if (!lerToken(p, token, sizeof(token)))
        return 0;
    *valor = atoi(token);
    return 1;
}

static void escreverBinario(FILE* saida, unsigned int instrucao) {
    int bit;

    for (bit = 31; bit >= 0; bit--)
        fputc((instrucao >> bit) & 1u ? '1' : '0', saida);
    fputc('\n', saida);
}

static int codificarTipoR(unsigned int opcode, unsigned int rd, unsigned int rs, unsigned int rt,
                          unsigned int* instrucao) {
    *instrucao = (opcode << 26) | (rd << 20) | (rs << 14) | (rt << 8);
    return 1;
}

static int codificarTipoI14(unsigned int opcode, unsigned int rd, unsigned int rs, unsigned int imm14,
                            unsigned int* instrucao) {
    *instrucao = (opcode << 26) | (rd << 20) | (rs << 14) | montarIm14(imm14);
    return 1;
}

static int codificarTipoI20(unsigned int opcode, unsigned int campoAlto, unsigned int imm20,
                            unsigned int* instrucao) {
    *instrucao = (opcode << 26) | (campoAlto << 20) | montarIm20(imm20);
    return 1;
}

static int codificarBranch(unsigned int opcode, unsigned int rs, unsigned int rt, int alvo,
                           int enderecoPc, unsigned int* instrucao, char* erro, size_t tamanhoErro,
                           const char* linha) {
    int offset;
    unsigned int imm20;

    offset = alvo - (enderecoPc + 1);
    if (offset < 0 || offset > (int)IM14_MASK) {
        snprintf(erro, tamanhoErro, "deslocamento de branch invalido (%d) em %s", offset, linha);
        return -1;
    }

    imm20 = (rt << 14) | montarIm14((unsigned int)offset);
    *instrucao = (opcode << 26) | (rs << 20) | imm20;
    return 1;
}

static int codificarRegistradorUnico(unsigned int opcode, unsigned int reg, unsigned int* instrucao) {
    *instrucao = (opcode << 26) | (reg << 20);
    return 1;
}

static int codificarLinha(const char* linha, int enderecoPc, unsigned int* instrucao, char* erro, size_t tamanhoErro) {
    char buffer[256];
    char mnemonico[32];
    char nomeOpcode[32];
    char token[32];
    char reg1[32];
    char reg2[32];
    char* p;
    unsigned int opcode;
    unsigned int rd;
    unsigned int rs;
    unsigned int rt;
    int im;
    size_t i;

    if (linha == NULL)
        return 0;

    i = 0;
    while (linha[i] != '\0' && isspace((unsigned char)linha[i]))
        i++;
    if (linha[i] == '\0' || linha[i] == '#')
        return 0;

    strncpy(buffer, linha, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    p = buffer;
    if (!lerToken(&p, mnemonico, sizeof(mnemonico))) {
        snprintf(erro, tamanhoErro, "mnemonico ausente: %s", linha);
        return -1;
    }

    if (!buscarOpcode(mnemonico, &opcode)) {
        snprintf(erro, tamanhoErro, "mnemonico desconhecido: %s", mnemonico);
        return -1;
    }

    paraMinusculas(nomeOpcode, mnemonico, sizeof(nomeOpcode));

    if (strcmp(nomeOpcode, "nop") == 0) {
        *instrucao = opcode << 26;
        return 1;
    }

    if (strcmp(nomeOpcode, "hlt") == 0) {
        *instrucao = opcode << 26;
        return 1;
    }

    if (strcmp(nomeOpcode, "j") == 0 || strcmp(nomeOpcode, "jal") == 0) {
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "endereco ausente em %s", linha);
            return -1;
        }
        *instrucao = (opcode << 26) | montarIm26((unsigned int)im);
        return 1;
    }

    if (strcmp(nomeOpcode, "jr") == 0 || strcmp(nomeOpcode, "in") == 0 ||
        strcmp(nomeOpcode, "out") == 0 || strcmp(nomeOpcode, "push") == 0 ||
        strcmp(nomeOpcode, "pop") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "registrador ausente em %s", linha);
            return -1;
        }
        return codificarRegistradorUnico(opcode, rs, instrucao);
    }

    if (strcmp(nomeOpcode, "li") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rd)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "imediato ausente em %s", linha);
            return -1;
        }
        return codificarTipoI20(opcode, rd, (unsigned int)im, instrucao);
    }

    if (strcmp(nomeOpcode, "lw") == 0 || strcmp(nomeOpcode, "lwr") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rd)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "endereco ausente em %s", linha);
            return -1;
        }
        return codificarTipoI20(opcode, rd, (unsigned int)im, instrucao);
    }

    if (strcmp(nomeOpcode, "sw") == 0 || strcmp(nomeOpcode, "swr") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "endereco ausente em %s", linha);
            return -1;
        }
        return codificarTipoI20(opcode, rs, (unsigned int)im, instrucao);
    }

    if (strcmp(nomeOpcode, "move") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rd)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
            return -1;
        }
        *instrucao = (opcode << 26) | (rd << 20) | (rs << 14);
        return 1;
    }

    if (strcmp(nomeOpcode, "lwd") == 0 || strcmp(nomeOpcode, "swd") == 0) {
        if (!lerToken(&p, reg1, sizeof(reg1))) {
            snprintf(erro, tamanhoErro, "operandos ausentes em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, reg2, sizeof(reg2))) {
            snprintf(erro, tamanhoErro, "segundo operando ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "imediato ausente em %s", linha);
            return -1;
        }

        if (strcmp(nomeOpcode, "lwd") == 0) {
            if (!parseRegistrador(reg1, &rd) || !parseRegistrador(reg2, &rs)) {
                snprintf(erro, tamanhoErro, "registrador invalido em %s", linha);
                return -1;
            }
            return codificarTipoI14(opcode, rd, rs, (unsigned int)im, instrucao);
        }

        if (!parseRegistrador(reg1, &rs) || !parseRegistrador(reg2, &rt)) {
            snprintf(erro, tamanhoErro, "registrador invalido em %s", linha);
            return -1;
        }
        return codificarTipoI14(opcode, rs, rt, (unsigned int)im, instrucao);
    }

    if (strcmp(nomeOpcode, "beq") == 0 || strcmp(nomeOpcode, "bgt") == 0 ||
        strcmp(nomeOpcode, "blt") == 0 || strcmp(nomeOpcode, "bge") == 0 ||
        strcmp(nomeOpcode, "ble") == 0 || strcmp(nomeOpcode, "bne") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rt)) {
            snprintf(erro, tamanhoErro, "comparando ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        return codificarBranch(opcode, rs, rt, im, enderecoPc, instrucao, erro, tamanhoErro, linha);
    }

    if (strcmp(nomeOpcode, "add") == 0 || strcmp(nomeOpcode, "sub") == 0 ||
        strcmp(nomeOpcode, "mult") == 0 || strcmp(nomeOpcode, "div") == 0 ||
        strcmp(nomeOpcode, "and") == 0 || strcmp(nomeOpcode, "or") == 0 ||
        strcmp(nomeOpcode, "sr") == 0 || strcmp(nomeOpcode, "sl") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rd)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rt)) {
            snprintf(erro, tamanhoErro, "operando ausente em %s", linha);
            return -1;
        }
        return codificarTipoR(opcode, rd, rs, rt, instrucao);
    }

    if (strcmp(nomeOpcode, "not") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rd)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
            return -1;
        }
        return codificarTipoR(opcode, rd, rs, 0, instrucao);
    }

    if (strcmp(nomeOpcode, "addi") == 0 || strcmp(nomeOpcode, "subi") == 0 ||
        strcmp(nomeOpcode, "multi") == 0 || strcmp(nomeOpcode, "andi") == 0 ||
        strcmp(nomeOpcode, "ori") == 0) {
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rd)) {
            snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerToken(&p, token, sizeof(token)) || !parseRegistrador(token, &rs)) {
            snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
            return -1;
        }
        pularVirgula(&p);
        if (!lerInteiro(&p, &im)) {
            snprintf(erro, tamanhoErro, "imediato ausente em %s", linha);
            return -1;
        }
        return codificarTipoI14(opcode, rd, rs, (unsigned int)im, instrucao);
    }

    snprintf(erro, tamanhoErro, "formato nao suportado: %s", linha);
    return -1;
}

int traduzirArquivoAssemblyParaBinario(const char* arquivoAssembly, const char* arquivoBinario) {
    FILE* entrada;
    FILE* saida;
    char linha[512];
    char erro[256];
    unsigned int instrucao;
    int enderecoPc = 0;
    int status;

    entrada = fopen(arquivoAssembly, "r");
    if (entrada == NULL)
        return 0;

    saida = fopen(arquivoBinario, "w");
    if (saida == NULL) {
        fclose(entrada);
        return 0;
    }

    status = 1;
    while (fgets(linha, sizeof(linha), entrada) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        switch (codificarLinha(linha, enderecoPc, &instrucao, erro, sizeof(erro))) {
            case 0:
                continue;
            case 1:
                escreverBinario(saida, instrucao);
                enderecoPc++;
                break;
            default:
                fprintf(stderr, "ERRO binario (PC=%d): %s\n", enderecoPc, erro);
                status = 0;
                break;
        }
    }

    fclose(entrada);
    fclose(saida);
    return status;
}
