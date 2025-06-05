# 03 – C to Assembly
## ✅ Objective
Generate the assembly (.s) version of the `hello.c` file and understand the function prologue and epilogue.
## Command

```bash
riscv32-unknown-elf-gcc -S -O0 hello.c
cat hello.s
``` 

## Output
![image](https://github.com/user-attachments/assets/c566491b-89f6-4948-80a5-78ff4db0681f)

##Explanation
Example prologue:

```bash
addi sp, sp, -16
sw   ra, 12(sp)
``` 

Example epilogue:

```bash
lw   ra, 12(sp)
addi sp, sp, 16
ret
``` 
These setup and clean the stack frame for main()

