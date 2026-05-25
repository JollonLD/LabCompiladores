# Quadruplas SORT

| C- | Quadruplas |
|---|---|
|  | (NOP, ___, ___, ___) |
| int vet[10]; | (ALLOCAMEMVET, global, vet, 10) |
| int jonas; | (ALLOCAMEMVAR, global, jonas, ___) |
| int minloc ( int a[], int low, int high ) { | (FUNC, int, minloc, _)  <br> (ARG, a, minloc, _)  <br> (ARG, low, minloc, _)  <br> (ARG, high, minloc, _) |
| int i; int x; int k; | (ALLOCAMEMVAR, minloc, i, ___)  <br> (ALLOCAMEMVAR, minloc, x, ___)  <br> (ALLOCAMEMVAR, minloc, k, ___) |
| k = low; | (LOADVAR, minloc, low, t0)  <br> (STOREVAR, t0, k, minloc) |
| x = a[low]; | (LOADVAR, minloc, a, t1)  <br> (ADD, t0, t1, t2)  <br> (LOADVET, minloc, t2, t3)  <br> (STOREVAR, t3, x, minloc) |
| vet[2] = x; | (LOADVAR, minloc, x, t4)  <br> (LOADCONST, t5, 2, ___)  <br> (LOADVAR, minloc, vet, t6)  <br> (ADD, t5, t6, t7)  <br> (STOREVET, t4, t7, minloc) |
| i = low + 1; | (LOADCONST, t8, 1, ___)  <br> (ADD, t0, t8, t9)  <br> (STOREVAR, t9, i, minloc) |
| jonas = 0; | (LOADCONST, t10, 0, ___)  <br> (STOREVAR, t10, jonas, minloc) |
| while (i < high) { | (LABEL, L0, ___, ___)  <br> (LOADVAR, minloc, i, t11)  <br> (LOADVAR, minloc, high, t12)  <br> (BGE, t11, t12, L1) |
| if (a[i] < x) { | (ADD, t11, t1, t13)  <br> (BGE, t13, t4, L2) |
| x = a[i]; | (ADD, t11, t1, t14)  <br> (LOADVET, minloc, t14, t15)  <br> (STOREVAR, t15, x, minloc) |
| k = i; | (STOREVAR, t11, k, minloc) |
| } else { jonas = jonas + 1; } | (JUMP, L3, ___, ___)  <br> (LABEL, L2, ___, ___)  <br> (LOADVAR, minloc, jonas, t16)  <br> (ADD, t16, t8, t17)  <br> (STOREVAR, t17, jonas, minloc)  <br> (LABEL, L3, ___, ___) |
| i = i + 1; | (ADD, t11, t8, t18)  <br> (STOREVAR, t18, i, minloc)  <br> (JUMP, L0, ___, ___) |
| } // fim while | (LABEL, L1, ___, ___) |
| return k; | (LOADVAR, minloc, k, t19)  <br> (RETURN, t19, ___, ___) |
| } // fim minloc | (ENDFUNC, minloc, ___, ___) |
| void sort( int a[], int low, int high) { | (FUNC, void, sort, _)  <br> (ARG, a, sort, _)  <br> (ARG, low, sort, _)  <br> (ARG, high, sort, _) |
| int i; int k; | (ALLOCAMEMVAR, sort, i, ___)  <br> (ALLOCAMEMVAR, sort, k, ___) |
| i = low; | (LOADVAR, sort, low, t20)  <br> (STOREVAR, t20, i, sort) |
| while (i < high-1) { | (LABEL, L4, ___, ___)  <br> (LOADVAR, sort, i, t21)  <br> (LOADCONST, t22, 1, ___)  <br> (LOADVAR, sort, high, t23)  <br> (SUB, t23, t22, t24)  <br> (BGE, t21, t24, L5) |
| int t; | (ALLOCAMEMVAR, sort, t, ___) |
| k = minloc(a,i,high); | (LOADVAR, sort, a, t25)  <br> (PARAM, t25, ___, ___)  <br> (PARAM, t21, ___, ___)  <br> (PARAM, t23, ___, ___)  <br> (CALL, minloc, 3, ___)  <br> (STOREVAR, $rf, k, sort) |
| t = a[k]; | (LOADVAR, sort, k, t26)  <br> (LOADVAR, sort, a, t27)  <br> (ADD, t26, t27, t28)  <br> (LOADVET, sort, t28, t29)  <br> (STOREVAR, t29, t, sort) |
| a[k] = a[i]; | (LOADVAR, sort, i, t30)  <br> (ADD, t30, t27, t31)  <br> (ADD, t26, t27, t32)  <br> (STOREVET, t31, t32, sort) |
| a[i] = t; | (ADD, t30, t27, t33)  <br> (STOREVET, t, t33, sort) |
| i = i + 1; | (LOADCONST, t34, 1, ___)  <br> (ADD, t30, t34, t35)  <br> (STOREVAR, t35, i, sort)  <br> (JUMP, L4, ___, ___) |
| } // fim while | (LABEL, L5, ___, ___) |
| } // fim sort | (ENDFUNC, sort, ___, ___) |
| void main(void) { | (FUNC, void, main, _) |
| int i; | (ALLOCAMEMVAR, main, i, ___) |
| i = 0; | (LOADCONST, t36, 0, ___)  <br> (STOREVAR, t36, i, main) |
| while (i < 10) { | (LABEL, L6, ___, ___)  <br> (LOADVAR, main, i, t37)  <br> (LOADCONST, t38, 10, ___)  <br> (BGE, t37, t38, L7) |
| vet[i] = input(); | (CALL, input, 0, ___)  <br> (LOADVAR, main, i, t39)  <br> (LOADVAR, main, vet, t40)  <br> (ADD, t39, t40, t41)  <br> (STOREVET, $rf, t41, main) |
| i = i + 1; | (LOADCONST, t42, 1, ___)  <br> (ADD, t39, t42, t43)  <br> (STOREVAR, t43, i, main)  <br> (JUMP, L6, ___, ___) |
| } // fim while | (LABEL, L7, ___, ___) |
| sort(vet,0,10); | (PARAM, t40, ___, ___)  <br> (LOADCONST, t44, 0, ___)  <br> (PARAM, t44, ___, ___)  <br> (LOADCONST, t45, 10, ___)  <br> (PARAM, t45, ___, ___)  <br> (CALL, sort, 3, ___) |
| i = 0; | (LOADCONST, t46, 0, ___)  <br> (STOREVAR, t46, i, main) |
| while (i < 10) { | (LABEL, L8, ___, ___)  <br> (LOADVAR, main, i, t47)  <br> (LOADCONST, t48, 10, ___)  <br> (BGE, t47, t48, L9) |
| output(vet[i]); | (LOADVAR, main, vet, t49)  <br> (ADD, t47, t49, t50)  <br> (PARAM, t50, ___, ___)  <br> (CALL, output, 1, ___) |
| i = i + 1; | (LOADCONST, t51, 1, ___)  <br> (LOADVAR, main, i, t52)  <br> (ADD, t52, t51, t53)  <br> (STOREVAR, t53, i, main)  <br> (JUMP, L8, ___, ___) |
| } // fim while | (LABEL, L9, ___, ___) |
| } // fim main | (ENDFUNC, main, ___, ___)
|| (HALT, ___, ___, ___) |
