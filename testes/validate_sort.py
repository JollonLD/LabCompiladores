#!/usr/bin/env python3
"""
Valida saida_asm.txt executando um simulador MIPS-like do compilador
e compara com o algoritmo C-Minus de referencia (minloc + sort).
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASM_FILE = ROOT / "saida_asm.txt"

REG_ALIASES = {
    "$zero": 0,
    "$ra": 1,
    "$rf": 2,
    "$rp": 3,
    "$sp": 3,
    "$fp": 30,
    "$ro": 31,
}


def reg_num(token: str) -> int:
    token = token.strip().rstrip(",")
    if token in REG_ALIASES:
        return REG_ALIASES[token]
    m = re.match(r"\$r(\d+)$", token, re.I)
    if m:
        return int(m.group(1))
    raise ValueError(f"registrador invalido: {token}")


def parse_line(line: str) -> list[str]:
    line = line.split("#", 1)[0].strip()
    if not line:
        return []
    parts = re.split(r"[,\s]+", line)
    return [p for p in parts if p]


def minloc_ref(arr: list[int], low: int, high: int) -> int:
    k = low
    x = arr[low]
    i = low + 1
    while i < high:
        if arr[i] < x:
            x = arr[i]
            k = i
        i += 1
    return k


def sort_ref(arr: list[int], low: int, high: int) -> None:
    i = low
    while i < high - 1:
        k = minloc_ref(arr, i, high)
        t = arr[k]
        arr[k] = arr[i]
        arr[i] = t
        i += 1


class SimuladorCompilador:
    """Simula o assembly gerado por assembly_generator.c (lwd/swd, saltos 0-based)."""

    # Pilha acima dos dados globais (descritor em mem[0], vet em mem[1..10])
    FP_INICIAL = 32

    def __init__(self, programa: list[str], entradas: list[int]):
        self.prog = programa
        self.mem: dict[int, int] = {}
        self.regs = [0] * 64
        self.pc = 0
        self.inputs = list(entradas)
        self.outputs: list[int] = []
        self.steps = 0
        self.max_steps = 5_000_000
        # Hardware mantem fp/sp fora da area do vetor global
        self.w(30, self.FP_INICIAL)
        self.w(3, self.FP_INICIAL)

    def r(self, i: int) -> int:
        return 0 if i == 0 else self.regs[i]

    def w(self, i: int, v: int) -> None:
        if i != 0:
            self.regs[i] = v & 0xFFFFFFFF

    def mem_get(self, addr: int) -> int:
        return self.mem.get(addr, 0)

    def mem_set(self, addr: int, val: int) -> None:
        self.mem[addr] = val & 0xFFFFFFFF

    def vetor_global(self, tamanho: int = 10) -> list[int]:
        """vet[i] em mem[descritor + i], descritor em mem[0]."""
        desc = self.mem_get(0)
        return [self.mem_get(desc + i) for i in range(tamanho)]

    def run(self) -> tuple[list[int], dict[int, int]]:
        while self.pc < len(self.prog) and self.steps < self.max_steps:
            self.steps += 1
            parts = parse_line(self.prog[self.pc])
            if not parts:
                self.pc += 1
                continue

            op = parts[0].lower()
            nxt = self.pc + 1

            if op == "nop":
                pass
            elif op == "hlt":
                break
            elif op == "j":
                nxt = int(parts[1])
            elif op == "jal":
                self.w(1, nxt)
                nxt = int(parts[1])
            elif op == "jr":
                nxt = self.r(reg_num(parts[1]))
            elif op == "li":
                self.w(reg_num(parts[1]), int(parts[2]))
            elif op == "move":
                self.w(reg_num(parts[1]), self.r(reg_num(parts[2])))
            elif op == "add":
                self.w(reg_num(parts[1]), self.r(reg_num(parts[2])) + self.r(reg_num(parts[3])))
            elif op == "sub":
                self.w(reg_num(parts[1]), self.r(reg_num(parts[2])) - self.r(reg_num(parts[3])))
            elif op == "addi":
                self.w(reg_num(parts[1]), self.r(reg_num(parts[2])) + int(parts[3]))
            elif op == "subi":
                self.w(reg_num(parts[1]), self.r(reg_num(parts[2])) - int(parts[3]))
            elif op == "bge":
                rs, rt, dest = reg_num(parts[1]), reg_num(parts[2]), int(parts[3])
                nxt = dest if self.r(rs) >= self.r(rt) else nxt
            elif op == "lwd":
                rd, base, off = reg_num(parts[1]), reg_num(parts[2]), int(parts[3])
                self.w(rd, self.mem_get(self.r(base) + off))
            elif op == "swd":
                rs, base, off = reg_num(parts[1]), reg_num(parts[2]), int(parts[3])
                self.mem_set(self.r(base) + off, self.r(rs))
            elif op == "in":
                if not self.inputs:
                    raise RuntimeError("entrada esgotada")
                self.w(reg_num(parts[1]), self.inputs.pop(0))
            elif op == "out":
                self.outputs.append(self.r(reg_num(parts[1])))
            else:
                raise RuntimeError(f"opcode nao suportado na linha {self.pc + 1}: {op}")

            self.pc = nxt

        if self.steps >= self.max_steps:
            raise RuntimeError(f"limite de passos ({self.max_steps}) excedido — possivel loop infinito")

        return self.outputs, dict(self.mem)


class SimuladorReferencia:
    """
    Simula o assembly de referencia (outro compilador) colado pelo usuario.
    Modelo de memoria: vetor global com descritor em mem[0]=1, dados em mem[1..10].
    Frame: mem[fp+off] para variaveis; r30=sp, r31=ra, r28=retorno/input.
  """

    def __init__(self, entradas: list[int]):
        self.mem: dict[int, int] = {0: 1}  # descritor vet global
        self.regs = [0] * 32
        self.r30 = 100  # sp inicial alto
        self.inputs = list(entradas)
        self.outputs: list[int] = []
        self.steps = 0
        self.max_steps = 5_000_000

    def fp_load(self, off: int) -> int:
        fp = self.regs[1]  # r1 usado como ponteiro de frame nas instrucoes lw r29 r1 N
        return self.mem.get(fp + off, 0)

    def fp_store(self, off: int, val: int) -> None:
        fp = self.regs[1]
        self.mem[fp + off] = val & 0xFFFFFFFF

    def vetor(self, tamanho: int = 10) -> list[int]:
        desc = self.mem.get(0, 1)
        return [self.mem.get(desc + i, 0) for i in range(tamanho)]

    # Implementacao direta da logica extraida do assembly de referencia
    def minloc(self, a_desc: int, low: int, high: int, fp: int) -> int:
        self.regs[1] = fp
        self.fp_store(7, low)   # k
        addr = a_desc + low
        x = self.mem.get(addr, 0)
        self.fp_store(6, x)
        self.fp_store(5, low + 1)  # i

        while self.fp_load(5) < high:
            i = self.fp_load(5)
            ai = self.mem.get(a_desc + i, 0)
            if ai < self.fp_load(6):
                self.fp_store(6, ai)
                self.fp_store(7, i)
            self.fp_store(5, i + 1)
        return self.fp_load(7)

    def sort(self, a_desc: int, low: int, high: int, fp: int) -> None:
        self.regs[1] = fp
        self.fp_store(5, low)  # i
        while self.fp_load(5) < high - 1:
            i = self.fp_load(5)
            k = self.minloc(a_desc, i, high, fp + 100)  # frame filho separado
            self.fp_store(6, k)
            t = self.mem.get(a_desc + k, 0)
            self.fp_store(7, t)
            ai = self.mem.get(a_desc + i, 0)
            self.mem[a_desc + k] = ai
            self.mem[a_desc + i] = t
            self.fp_store(5, i + 1)

    def run_algoritmo(self) -> tuple[list[int], list[int]]:
        """main: 10 inputs, sort(0,10), 10 outputs — mesma logica do assembly de referencia."""
        desc = self.mem[0]
        i = 0
        while i < 10:
            if not self.inputs:
                break
            self.mem[desc + i] = self.inputs.pop(0)
            i += 1

        antes = self.vetor()
        self.sort(desc, 0, 10, fp=200)

        saida = []
        for j in range(10):
            saida.append(self.mem.get(desc + j, 0))
        return saida, antes


def carregar_asm(caminho: Path) -> list[str]:
    return [ln.rstrip("\n") for ln in caminho.read_text(encoding="utf-8").splitlines() if ln.strip()]


def main() -> int:
    casos = [
        ([9, 8, 7, 6, 5, 4, 3, 2, 1, 0], "decrescente"),
        ([3, 1, 4, 1, 5, 9, 2, 6, 5, 3], "aleatorio"),
        ([5, 5, 5, 5, 5, 5, 5, 5, 5, 5], "iguais"),
        ([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], "ordenado"),
        ([10, 20, 30, 40, 50, 60, 70, 80, 90, 100], "crescente"),
    ]

    if not ASM_FILE.exists():
        print(f"ERRO: {ASM_FILE} nao encontrado. Execute: cminus.exe testes/test_sort.cm")
        return 1

    asm = carregar_asm(ASM_FILE)
    print("=" * 70)
    print("VALIDACAO DE MEMORIA — test_sort.cm (vet[10])")
    print("=" * 70)
    print(f"Assembly: {ASM_FILE} ({len(asm)} instrucoes)\n")

    todos_ok = True

    for entradas, nome in casos:
        esperado = entradas[:]
        sort_ref(esperado, 0, 10)

        sim = SimuladorCompilador(asm, entradas[:])
        saidas_asm, mem = sim.run()
        vet_mem = sim.vetor_global(10)

        ref = SimuladorReferencia(entradas[:])
        saidas_ref, vet_antes_ref = ref.run_algoritmo()

        ok_out = saidas_asm == esperado
        ok_mem = vet_mem == esperado
        ok_ref = saidas_ref == esperado
        ok = ok_out and ok_mem and ok_ref
        todos_ok = todos_ok and ok

        status = "OK" if ok else "FALHA"
        print(f"Caso: {nome}")
        print(f"  Entrada:     {entradas}")
        print(f"  Esperado:    {esperado}")
        print(f"  Saida asm:   {saidas_asm}  {'[ok]' if ok_out else '[ERRO]'}")
        print(f"  Mem vet asm: {vet_mem}  {'[ok]' if ok_mem else '[ERRO]'}")
        print(f"  Saida ref:   {saidas_ref}  {'[ok]' if ok_ref else '[ERRO]'}")
        print(f"  Passos asm:  {sim.steps}")
        print(f"  Resultado:   {status}\n")

    print("=" * 70)
    if todos_ok:
        print("RESULTADO FINAL: COMPATIVEL — assembly gerado ordena corretamente.")
        print("Memoria apos sort coincide com algoritmo de referencia e com saidas output.")
    else:
        print("RESULTADO FINAL: INCOMPATIVEL — verifique casos com FALHA acima.")
    print("=" * 70)
    return 0 if todos_ok else 1


if __name__ == "__main__":
    sys.exit(main())
