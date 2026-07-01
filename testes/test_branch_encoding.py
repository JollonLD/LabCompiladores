#!/usr/bin/env python3
"""Regressao: encoding de branches (bge, beq, ...) conforme hardware."""

from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASM = ROOT / "saida_asm.txt"
BIN = ROOT / "saida_bin.txt"

BRANCH_OPS = {0x0E, 0x0F, 0x10, 0x11, 0x12, 0x24}


def decodificar_branch(word: int) -> dict:
    return {
        "opcode": word >> 26,
        "rs": (word >> 20) & 0x3F,
        "rt": (word >> 14) & 0x3F,
        "offset": word & 0x3FFF,
    }


def main() -> int:
    if not ASM.exists() or not BIN.exists():
        print("Execute: cminus.exe testes/test_sort.cm")
        return 1

    asm = [ln.strip() for ln in ASM.read_text(encoding="utf-8").splitlines() if ln.strip()]
    bins = [ln.strip() for ln in BIN.read_text(encoding="utf-8").splitlines() if ln.strip()]

    if len(asm) != len(bins):
        print(f"ERRO: asm ({len(asm)}) != bin ({len(bins)})")
        return 1

    erros = 0
    for pc, (linha, bits) in enumerate(zip(asm, bins)):
        if not linha.split()[0].lower().startswith(("bge", "beq", "bgt", "blt", "ble", "bne")):
            continue

        partes = linha.replace(",", " ").split()
        rs_esperado = int(partes[1].replace("$r", ""))
        rt_esperado = int(partes[2].replace("$r", ""))
        pc_dest = int(partes[3])
        offset_esperado = pc_dest - (pc + 1)

        w = int(bits, 2)
        d = decodificar_branch(w)
        dest_calc = pc + 1 + d["offset"]

        ok = (
            d["opcode"] in BRANCH_OPS
            and d["rs"] == rs_esperado
            and d["rt"] == rt_esperado
            and d["offset"] == offset_esperado
            and dest_calc == pc_dest
        )

        if not ok:
            erros += 1
            print(f"FALHA PC={pc}: {linha}")
            print(f"  esperado offset={offset_esperado} dest={pc_dest}")
            print(f"  obtido   offset={d['offset']} dest={dest_calc} word={bits}")

    # Word de referencia: bge $r21,$r24 dest PC 96 a partir de PC 52 -> offset 43
    pc52 = int(bins[52], 2)
    ref = (0x0E << 26) | (21 << 20) | (24 << 14) | 43
    if pc52 != ref:
        erros += 1
        d52 = decodificar_branch(pc52)
        print(f"FALHA regressao PC52: offset={d52['offset']} dest={52+1+d52['offset']}")

    if erros:
        print(f"\n{erros} branch(es) com encoding incorreto.")
        return 1

    print(f"OK: {len(bins)} instrucoes, branches verificados (PC52 offset=43 -> dest=96).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
