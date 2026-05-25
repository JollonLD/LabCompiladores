# Quadruplas GCD

| C- | Quadruplas |
|---|---|
|| (NOP, ___, ___, ___) |
| int gcd (int u, int v) { | (FUNC, int, gcd, _)  <br> (ARG, u, gcd, _)  <br> (ARG, v, gcd, _) |
| if (v == 0) | (LOADVAR, gcd, v, t0)  <br> (LOADCONST, t1, 0, ___)  <br> (BNE, t0, t1, L0) |
| return u; | (LOADVAR, gcd, u, t2)  <br> (RETURN, t2, ___, ___) <br> (JUMP, L1, ___, ___)|
| else return gcd(v, u - u/v*v); |  (LABEL, L0, ___, ___) <br> (PARAM, t0, ___, ___) <br> (DIV, t2, t0, t3) <br> (MULT, t3, t0, t4) <br> (SUB, t2, t4, t5) <br> (PARAM, t5, ___, ___) <br> (CALL, gcd, 2, ___) <br> (RETURN, $rf, ___, ___) <br> (LABEL, L1, ___, ___) |
| } | (ENDFUNC, gcd, ___, ___) |
| void main(void) { int x; int y; | (FUNC, void, main, _)  <br> (ALLOCAMEMVAR, main, x, ___)  <br> (ALLOCAMEMVAR, main, y, ___) |
| x = input(); | (CALL, input, 0, ___)  <br> (STOREVAR, $rf, x, main) |
| y = input(); | (CALL, input, 0, ___)  <br> (STOREVAR, $rf, y, main) |
| output(gcd(x,y)); | (LOADVAR, main, x, t6)  <br> (PARAM, t6, ___, ___)  <br> (LOADVAR, main, y, t7)  <br> (PARAM, t7, ___, ___)  <br> (CALL, gcd, 2, ___)  <br> (PARAM, $rf, ___, ___)  <br> (CALL, output, 1, ___) |
| } | (ENDFUNC, main, ___, ___) 
| | (HALT, ___, ___, ___) |
