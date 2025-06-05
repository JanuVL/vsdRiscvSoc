# 02 – Hello RISC-V
## ✅ Objective
Write and compile a minimal RISC-V C program that prints “Hello, RISC-V!”.
## Steps 
🔹 2.1 Create a file: nano hello.c

🔹 2.2 Add minimal Hello C code:
#include <stdio.h>
int main() {
    printf("Hello, RISC-V!\\n");
    return 0;
} 

🔹 2.3 Compile it: riscv32-unknown-elf-gcc -o hello.elf hello.c

🔹 2.4 Use ELF Format: file hello.elf
## Output
![image](https://github.com/user-attachments/assets/cb422f67-c48a-4f93-bb36-9db0a8b4d94f)
![image](https://github.com/user-attachments/assets/7f2dbaab-aa17-4e74-b40a-e0e04fa332cd)





