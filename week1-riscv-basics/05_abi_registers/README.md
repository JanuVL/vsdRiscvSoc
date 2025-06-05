#  05 – ABI & Registers
## ✅ Objective
Understand the 32 RISC-V general-purpose registers, their ABI names, and roles in the calling convention.

## Register Table

| Register | ABI Name | Role / Description                          |
|----------|----------|---------------------------------------------|
| x0       | zero     | Constant zero (always 0)                    |
| x1       | ra       | Return address                              |
| x2       | sp       | Stack pointer                               |
| x3       | gp       | Global pointer                              |
| x4       | tp       | Thread pointer                              |
| x5       | t0       | Temporary (caller-saved)                    |
| x6       | t1       | Temporary (caller-saved)                    |
| x7       | t2       | Temporary (caller-saved)                    |
| x8       | s0/fp    | Saved register / frame pointer (callee-saved) |
| x9       | s1       | Saved register (callee-saved)              |
| x10      | a0       | Argument 0 / Return value 0 (caller-saved)  |
| x11      | a1       | Argument 1 / Return value 1 (caller-saved)  |
| x12      | a2       | Argument 2 (caller-saved)                   |
| x13      | a3       | Argument 3 (caller-saved)                   |
| x14      | a4       | Argument 4 (caller-saved)                   |
| x15      | a5       | Argument 5 (caller-saved)                   |
| x16      | a6       | Argument 6 (caller-saved)                   |
| x17      | a7       | Argument 7 (caller-saved)                   |
| x18      | s2       | Saved register (callee-saved)              |
| x19      | s3       | Saved register (callee-saved)              |
| x20      | s4       | Saved register (callee-saved)              |
| x21      | s5       | Saved register (callee-saved)              |
| x22      | s6       | Saved register (callee-saved)              |
| x23      | s7       | Saved register (callee-saved)              |
| x24      | s8       | Saved register (callee-saved)              |
| x25      | s9       | Saved register (callee-saved)              |
| x26      | s10      | Saved register (callee-saved)              |
| x27      | s11      | Saved register (callee-saved)              |
| x28      | t3       | Temporary (caller-saved)                    |
| x29      | t4       | Temporary (caller-saved)                    |
| x30      | t5       | Temporary (caller-saved)                    |
| x31      | t6       | Temporary (caller-saved)                    |


## 🔁 RISC-V Calling Convention Summary

- **a0–a7 (x10–x17)**: Function arguments and return values (**caller-saved**)
- **s0–s11 (x8–x9, x18–x27)**: Saved registers (**callee-saved**)
- **t0–t6 (x5–x7, x28–x31)**: Temporaries (**caller-saved**)
- **ra (x1)**: Return address register
- **sp (x2)**: Stack pointer
- **fp (x8)**: Frame pointer (alias for s0)
- **gp (x3), tp (x4)**: Global/thread pointers
