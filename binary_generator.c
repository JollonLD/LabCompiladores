#include "binary_generator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IM14_MASK 0x3FFFu
#define IM20_MASK 0xFFFFFu
#define IM26_MASK 0x3FFFFFFu

typedef struct {
    const char* nome;
    unsigned int opcode;
} MapaOpcode;

static const MapaOpcode mapaOpcodes[] = {
    {"add",   0x00}, {"sub",   0x01}, {"mult",  0x02}, {"div",   0x03},
    {"AND",   0x04}, {"OR",    0x05}, {"NOT",   0x06},
    {"addi",  0x07}, {"subi",  0x08}, {"multi", 0x09},
    {"ANDI",  0x0A}, {"ORI",   0x0B}, {"sr",    0x0C}, {"sl",    0x0D},
    {"bge",   0x0E}, {"beq",   0x0F}, {"bgt",   0x10}, {"blt",   0x11},
    {"ble",   0x12}, {"bne",   0x24},
    {"move",  0x13}, {"li",    0x14}, {"lw",    0x15}, {"sw",    0x16},
    {"lwr",   0x17}, {"swr",   0x18}, {"lwd",   0x19}, {"swd",   0x1A},
    {"j",     0x1B}, {"jr",    0x1C}, {"jal",   0x1D},
    {"push",  0x1E}, {"pop",   0x1F}, {"in",    0x20}, {"out",   0x21},
    {"nop",   0x22}, {"hlt",   0x23},
    {"PUSH",  0x1E}, {"POP",   0x1F}, {"IN",    0x20}, {"OUT",   0x21},
    {"NOP",   0x22}, {"HLT",   0x23},
    {NULL, 0}
};

static unsigned int montarIm14(int valor) {
  unsigned int im;

  if (valor < 0)
    valor = (1 << 14) + valor;
  im = (unsigned int)valor & IM14_MASK;
  return im;
}

static unsigned int montarIm20(int valor) {
  return (unsigned int)valor & IM20_MASK;
}

static unsigned int montarIm26(int valor) {
  return (unsigned int)valor & IM26_MASK;
}

static int buscarOpcode(const char* mnemonico, unsigned int* opcode) {
  size_t i;

  for (i = 0; mapaOpcodes[i].nome != NULL; i++) {
    if (strcmp(mapaOpcodes[i].nome, mnemonico) == 0) {
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

  return 0;
}

static void pularEspacos(char** p) {
  while (**p != '\0' && isspace((unsigned char)**p))
    (*p)++;
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

static int codificarLinha(const char* linha, int enderecoPc, unsigned int* instrucao, char* erro, size_t tamanhoErro) {
  char buffer[256];
  char mnemonico[32];
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

  if (strcmp(mnemonico, "nop") == 0 || strcmp(mnemonico, "NOP") == 0 ||
      strcmp(mnemonico, "hlt") == 0 || strcmp(mnemonico, "HLT") == 0) {
    *instrucao = opcode << 26;
    return 1;
  }

  if (strcmp(mnemonico, "j") == 0 || strcmp(mnemonico, "jal") == 0) {
    if (!lerInteiro(&p, &im)) {
      snprintf(erro, tamanhoErro, "endereco ausente em %s", linha);
      return -1;
    }
    *instrucao = (opcode << 26) | montarIm26(im);
    return 1;
  }

  if (strcmp(mnemonico, "jr") == 0 || strcmp(mnemonico, "in") == 0 ||
      strcmp(mnemonico, "out") == 0 || strcmp(mnemonico, "push") == 0 ||
      strcmp(mnemonico, "pop") == 0 || strcmp(mnemonico, "PUSH") == 0 ||
      strcmp(mnemonico, "POP") == 0 || strcmp(mnemonico, "IN") == 0 ||
      strcmp(mnemonico, "OUT") == 0) {
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rs)) {
      snprintf(erro, tamanhoErro, "registrador ausente em %s", linha);
      return -1;
    }
    *instrucao = (opcode << 26) | (rs << 20);
    return 1;
  }

  if (strcmp(mnemonico, "li") == 0) {
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rd)) {
      snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerInteiro(&p, &im)) {
      snprintf(erro, tamanhoErro, "imediato ausente em %s", linha);
      return -1;
    }
    *instrucao = (opcode << 26) | (rd << 20) | montarIm20(im);
    return 1;
  }

  if (strcmp(mnemonico, "move") == 0) {
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rd)) {
      snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rs)) {
      snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
      return -1;
    }
    *instrucao = (opcode << 26) | (rd << 20) | (rs << 14);
    return 1;
  }

  if (strcmp(mnemonico, "lwd") == 0 || strcmp(mnemonico, "swd") == 0) {
  char reg1[32];
  char reg2[32];

    if (!lerToken(&p, reg1, sizeof(reg1))) {
      snprintf(erro, tamanhoErro, "operandos ausentes em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerToken(&p, reg2, sizeof(reg2))) {
      snprintf(erro, tamanhoErro, "segundo operando ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerInteiro(&p, &im)) {
      snprintf(erro, tamanhoErro, "imediato ausente em %s", linha);
      return -1;
    }

    if (strcmp(mnemonico, "lwd") == 0) {
      if (!parseRegistrador(reg1, &rd) || !parseRegistrador(reg2, &rs)) {
        snprintf(erro, tamanhoErro, "registrador invalido em %s", linha);
        return -1;
      }
      *instrucao = (opcode << 26) | (rd << 20) | (rs << 14) | montarIm14(im);
    } else {
      if (!parseRegistrador(reg1, &rs) || !parseRegistrador(reg2, &rt)) {
        snprintf(erro, tamanhoErro, "registrador invalido em %s", linha);
        return -1;
      }
      *instrucao = (opcode << 26) | (rs << 20) | (rt << 14) | montarIm14(im);
    }
    return 1;
  }

  if (strcmp(mnemonico, "beq") == 0 || strcmp(mnemonico, "bgt") == 0 ||
      strcmp(mnemonico, "blt") == 0 || strcmp(mnemonico, "bge") == 0 ||
      strcmp(mnemonico, "ble") == 0 || strcmp(mnemonico, "bne") == 0) {
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rs)) {
      snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rt)) {
      snprintf(erro, tamanhoErro, "comparando ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerInteiro(&p, &im)) {
      snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
      return -1;
    }
    im -= enderecoPc;
    *instrucao = (opcode << 26) | (rs << 20) | (rt << 14) | montarIm14(im);
    return 1;
  }

  if (strcmp(mnemonico, "add") == 0 || strcmp(mnemonico, "sub") == 0 ||
      strcmp(mnemonico, "mult") == 0 || strcmp(mnemonico, "div") == 0 ||
      strcmp(mnemonico, "AND") == 0 || strcmp(mnemonico, "OR") == 0) {
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rd)) {
      snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rs)) {
      snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rt)) {
      snprintf(erro, tamanhoErro, "operando ausente em %s", linha);
      return -1;
    }
    *instrucao = (opcode << 26) | (rd << 20) | (rs << 14) | (rt << 8);
    return 1;
  }

  if (strcmp(mnemonico, "addi") == 0 || strcmp(mnemonico, "subi") == 0 ||
      strcmp(mnemonico, "multi") == 0 || strcmp(mnemonico, "ANDI") == 0 ||
      strcmp(mnemonico, "ORI") == 0) {
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rd)) {
      snprintf(erro, tamanhoErro, "destino ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerToken(&p, buffer, sizeof(buffer)) || !parseRegistrador(buffer, &rs)) {
      snprintf(erro, tamanhoErro, "origem ausente em %s", linha);
      return -1;
    }
    if (p[0] == ',')
      p++;
    if (!lerInteiro(&p, &im)) {
      snprintf(erro, tamanhoErro, "imediato ausente em %s", linha);
      return -1;
    }
    *instrucao = (opcode << 26) | (rd << 20) | (rs << 14) | montarIm14(im);
    return 1;
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
