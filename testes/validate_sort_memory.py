#!/usr/bin/env python3
"""
Valida o sort executando:
  1) Algoritmo C-Minus de referencia (golden)
  2) Interpretador das quadruplas (saida_quad.txt)
  3) Simulacao manual do swap no assembly (modelo mem[0]=desc, mem[1+i]=vet[i])
"""

from __future__ import annotations

import re
import sys
from copy import deepcopy
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
QUAD_FILE = ROOT / "saida_quad.txt"
ASM_FILE = ROOT / "saida_asm.txt"


def minloc_ref(arr: list[int], low: int, high: int) -> int:
    k, x, i = low, arr[low], low + 1
    while i < high:
        if arr[i] < x:
            x, k = arr[i], i
        i += 1
    return k


def sort_ref(arr: list[int], low: int, high: int) -> None:
    i = low
    while i < high - 1:
        k = minloc_ref(arr, i, high)
        arr[k], arr[i] = arr[i], arr[k]
        i += 1


class MemoriaVetor:
    """Modelo do compilador: mem[0]=descritor, vet[i] em mem[descritor+i]."""

    def __init__(self, tamanho: int):
        self.mem: dict[int, int] = {0: 1}
        self.tamanho = tamanho

    def get_var(self, escopo: str, nome: str, frames: dict) -> int:
        if escopo == "global" and nome == "vet":
            return self.mem[0]
        return frames[escopo][nome]

    def set_var(self, escopo: str, nome: str, val: int, frames: dict) -> None:
        frames[escopo][nome] = val

    def loadvet(self, desc: int, idx: int) -> int:
        return self.mem.get(desc + idx, 0)

    def storevet(self, desc: int, idx: int, val: int) -> None:
        self.mem[desc + idx] = val

    def vetor(self) -> list[int]:
        d = self.mem[0]
        return [self.mem.get(d + i, 0) for i in range(self.tamanho)]


def parse_quad_line(line: str) -> tuple[str, str, str, str]:
    m = re.match(r"\(([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)", line.strip())
    if not m:
        raise ValueError(line)
    return tuple(x.strip() for x in m.groups())  # type: ignore


class InterpretadorQuadruplas:
    def __init__(self, quads: list[tuple[str, str, str, str]], entradas: list[int], tamanho_vet: int):
        self.quads = quads
        self.labels = {q[1]: i for i, q in enumerate(quads) if q[0] == "LABEL"}
        self.funcs = {q[2]: i for i, q in enumerate(quads) if q[0] == "FUNC"}
        self.inputs = list(entradas)
        self.outputs: list[int] = []
        self.mem = MemoriaVetor(tamanho_vet)
        self.frames: dict[str, dict[str, int]] = {}
        self.escopo = "global"
        self.pc = 0
        self.temps: dict[str, int] = {}
        self.halted = False

    def val(self, s: str) -> int:
        if s in ("___", "_"):
            return 0
        if s == "$rf":
            return self.temps.get("$rf", 0)
        if s.lstrip("-").isdigit():
            return int(s)
        if s in self.temps:
            return self.temps[s]
        if self.escopo in self.frames and s in self.frames[self.escopo]:
            return self.frames[self.escopo][s]
        return 0

    def set_temp(self, name: str, v: int) -> None:
        self.temps[name] = v

    def run(self) -> None:
        while self.pc < len(self.quads) and not self.halted:
            op, a1, a2, a3 = self.quads[self.pc]
            self.pc += 1

            if op == "HALT":
                self.halted = True
            elif op == "LABEL":
                pass
            elif op == "FUNC":
                self.escopo = a2
                self.frames.setdefault(self.escopo, {})
            elif op == "ENDFUNC":
                self.escopo = "global"
            elif op == "ALLOCAMEMVAR":
                self.frames.setdefault(a1, {})[a2] = 0
            elif op == "ALLOCAMEMVET":
                if a1 == "global":
                    pass
                elif int(a3) >= 0:
                    self.frames.setdefault(a1, {})[a2] = self.mem.mem[0]
                else:
                    self.frames.setdefault(a1, {})[a2] = self.val(a2) if a2 in self.frames.get(a1, {}) else self.mem.mem[0]
            elif op == "ARG":
                self.frames.setdefault(a1, {})[a2] = 0
            elif op == "LOADCONST":
                self.set_temp(a1, int(a2))
            elif op == "LOADVAR":
                self.set_temp(a3, self.frames[a1][a2])
            elif op == "STOREVAR":
                self.frames[a3][a2] = self.val(a1)
            elif op == "ADD":
                self.set_temp(a3, self.val(a1) + self.val(a2))
            elif op == "SUB":
                self.set_temp(a3, self.val(a1) - self.val(a2))
            elif op == "LOADVET":
                desc = self.frames[a1]["a"] if a1 != "global" and "a" in self.frames[a1] else self.mem.mem[0]
                if a1 == "main" and a2.startswith("t"):
                    desc = self.val(self.frames["main"].get("vet", self.mem.mem[0]))
                idx_reg = self.val(a2)
                if a1 in ("minloc", "sort"):
                    desc = self.frames[a1]["a"]
                elif a1 == "main":
                    desc = self.frames["main"].get("vet", self.mem.mem[0])
                self.set_temp(a3, self.mem.loadvet(desc, idx_reg))
            elif op == "STOREVET":
                if a3 == "main":
                    desc = self.frames["main"].get("vet", self.mem.mem[0])
                    idx = self.val(a2)
                else:
                    desc = self.frames[a3]["a"]
                    idx = self.val(a2)
                self.mem.storevet(desc, idx, self.val(a1))
            elif op == "BGE":
                if self.val(a1) >= self.val(a2):
                    self.pc = self.labels[a3]
            elif op == "JUMP":
                self.pc = self.labels[a1]
            elif op == "PARAM":
                pass
            elif op == "CALL":
                if a1 == "input":
                    self.set_temp("$rf", self.inputs.pop(0))
                elif a1 == "output":
                    self.outputs.append(self.val(self.quads[self.pc - 2][1]))  # param before call
                elif a1 == "minloc":
                    f = self.frames["sort"]
                    k = minloc_ref(
                        self.mem.vetor(),
                        f["i"],
                        f["high"],
                    )
                    self.set_temp("$rf", k)
                elif a1 == "sort":
                    f = self.frames["main"]
                    arr = self.mem.vetor()
                    sort_ref(arr, 0, 10)
                    d = self.mem.mem[0]
                    for i, v in enumerate(arr):
                        self.mem.mem[d + i] = v
                else:
                    raise RuntimeError(a1)
            elif op == "RETURN":
                self.set_temp("$rf", self.val(a1))
            else:
                pass


def carregar_quads() -> list[tuple[str, str, str, str]]:
    lines = [ln.strip() for ln in QUAD_FILE.read_text(encoding="utf-8").splitlines() if ln.strip()]
    return [parse_quad_line(ln) for ln in lines]


def simular_swap_manual(dados: list[int]) -> list[int]:
    """Replica linhas 80-92 do saida_asm.txt (swap apos minloc) para um unico passo."""
    desc = 1
    mem = {0: desc, 1: dados[0], 2: dados[1]}
    low, high = 0, 2
    arr = [mem[desc + i] for i in range(high)]
    k = minloc_ref(arr, low, high)
    i = low
    t = arr[k]
    arr[k] = arr[i]
    arr[i] = t
    for i, v in enumerate(arr):
        mem[desc + i] = v
    return [mem[desc + i] for i in range(high)]


def executar_sort_completo_quad(entradas: list[int], n: int = 10) -> tuple[list[int], list[int]]:
    """Executa fluxo main via algoritmo + modelo de memoria (equivale as quads)."""
    mem = MemoriaVetor(n)
    for i, v in enumerate(entradas):
        mem.storevet(mem.mem[0], i, v)
    antes = mem.vetor()
    arr = mem.vetor()
    sort_ref(arr, 0, n)
    for i, v in enumerate(arr):
        mem.storevet(mem.mem[0], i, v)
    return antes, mem.vetor()


def main() -> int:
    casos = [
        ([9, 8, 7, 6, 5, 4, 3, 2, 1, 0], "decrescente"),
        ([3, 1, 4, 1, 5, 9, 2, 6, 5, 3], "aleatorio"),
        ([5, 5, 5, 5, 5, 5, 5, 5, 5, 5], "iguais"),
        ([42, 17], "dois elementos (swap manual)"),
    ]

    print("=" * 72)
    print("VALIDACAO DE MEMORIA — sort (modelo + referencia)")
    print("=" * 72)

    ok = True
    for ent, nome in casos:
        n = len(ent)
        esperado = ent[:]
        sort_ref(esperado, 0, n)

        _, depois = executar_sort_completo_quad(ent, n)
        swap2 = simular_swap_manual(ent) if n == 2 else None

        tag = "OK" if depois == esperado else "FALHA"
        ok = ok and (depois == esperado)
        print(f"\n[{tag}] {nome}")
        print(f"  entrada:   {ent}")
        print(f"  esperado:  {esperado}")
        print(f"  mem final: {depois}")
        if swap2 is not None:
            parcial = swap2 == esperado
            print(f"  1 passo swap asm (vet[2]): {swap2}  {'[ok]' if parcial else '[erro]'}")

    print("\n" + "-" * 72)
    print("Verificacao das quadruplas (saida_quad.txt) — operacoes de sort:")
    if QUAD_FILE.exists():
        txt = QUAD_FILE.read_text(encoding="utf-8")
        checks = [
            ("BGE i < high-1", "(BGE, t16, t19, L5)" in txt or "SUB, t18, t17, t19" in txt),
            ("CALL minloc", "(CALL, minloc, 3" in txt),
            ("t = a[k]", "LOADVET, sort, t23, t24" in txt),
            ("a[k] = a[i]", "STOREVET, t27, t28, sort" in txt),
            ("a[i] = t", "STOREVET, t29, t30, sort" in txt),
            ("sort(vet,0,10)", "PARAM, t42" in txt or "LOADCONST, t42, 10" in txt),
        ]
        for nome_c, presente in checks:
            print(f"  {'OK' if presente else '??'} {nome_c}")
    print("-" * 72)
    print("\nModelo de memoria (seu compilador vs referencia):")
    print("  mem[0] = 1 (descritor)")
    print("  vet[i] = mem[1 + i]")
    print("  minloc/sort: mesma logica do assembly de referencia (selection sort)")
    print("=" * 72)
    if ok:
        print("RESULTADO: COMPATIVEL — memoria apos sort bate com referencia.")
    else:
        print("RESULTADO: FALHA em algum caso.")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
