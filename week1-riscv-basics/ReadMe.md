# 01 - Toolchain Setup

### 🎯 Objective
Set up the RISC-V GCC toolchain (rv32imac) on Ubuntu by unpacking the archive, updating the `PATH`, and verifying that `gcc`, `objdump`, and `gdb` binaries work correctly for RISC-V development.

## Steps

✅ Download the Toolchain

```bash
wget https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v14.2.0-3/xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz
```
✅ Unpack the Archive

```bash
tar -xvzf riscv-toolchain-rv32imac-x86_64-ubuntu.tar.gz
```
This creates a directory such as:
xpack-riscv-none-elf-gcc-14.2.0-3/

✅ Permanent PATH Setup

```bash
echo 'export PATH=/home/janani/xpack-riscv-none-elf-gcc-14.2.0-3/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

✅ Verify the Installation

```bash
riscv-none-elf-gcc --version
riscv-none-elf-objdump --version
riscv-none-elf-gdb --version
```
## Output
![image](https://github.com/user-attachments/assets/536ffe22-3f43-4aa2-b828-6dc6885a4adb)

# 02 - Bare-Metal "Hello, World!"

### 🎯 Objective
This example demonstrates compiling a minimal bare-metal "Hello, World!" program for RV32IMC using a memory-mapped UART output.


## 📁 Files

### `hello.c` – Minimal Bare-Metal UART Output

```bash
// hello.c
int main() {
    volatile char *uart = (char *)0x10000000; // Memory-mapped UART address
    const char *msg = "Hello, World!\n";

    while (*msg) {
        *uart = *msg++; // Write each character to UART
    }
    return 0;
}

// Minimal _start (no C runtime)
void _start() {
    main();
    while (1); // Trap CPU after main returns
}
```
### `linker.ld` – Minimal Linker Script

```bash
/* linker.ld - Minimal linker script for RISC-V RV32IMC */
ENTRY(_start)
SECTIONS {
    . = 0x80000000; /* Code starts here */
    .text : {
        *(.text*)
    }
    .data : {
        *(.data*)
    }
    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
```
## Steps
✅ Compile the ELF Binary

```bash
riscv-none-elf-gcc hello.c -o hello.elf \
  -march=rv32imc -mabi=ilp32 \
  -nostartfiles -nostdlib -T linker.ld
```
✅ Verify the Output
Disassembly:
```bash
riscv-none-elf-objdump -d hello.elf
```
You should see _start and the main code logic.

ELF Entry Point:
```bash
riscv-none-elf-readelf -h hello.elf | grep Entry
```
## Output
![image](https://github.com/user-attachments/assets/1ba5fa38-50f0-40f9-8eca-677ad6bbeb97)
![image](https://github.com/user-attachments/assets/bb7fa8fe-315b-484d-b08b-772044ec6791)
![image](https://github.com/user-attachments/assets/3ed7f57a-9a4d-48a2-8aba-59077ccf28aa)

# 03 - Assembly Generation and Prologue/Epilogue Analysis

### 🎯 Objective

To compile a simple C program into RISC-V assembly (`.s`), understand the structure of the generated function, and analyze the **prologue** and **epilogue** of the `main()` function based on the RISC-V calling convention.

## Steps
✅ Generate Assembly (.s) File
```bash
riscv-none-elf-gcc hello.c -S -o hello.s \
  -march=rv32imc -mabi=ilp32 \
  -nostartfiles -nostdlib -T linker.ld
```
 ✅ Inspect Generated Assembly (hello.s)
 ```bash
 main:
    addi    sp,sp,-32
    sw      ra,28(sp)
    sw      s0,24(sp)
    addi    s0,sp,32
    ...
    lw      ra,28(sp)
    lw      s0,24(sp)
    addi    sp,sp,32
    ret
```
✅ Prologue and Epilogue Analysis

🔍Prologue – Function Setup
```bash
addi    sp,sp,-32        # Allocate 32 bytes on stack
sw      ra,28(sp)        # Save return address
sw      s0,24(sp)        # Save old frame pointer
addi    s0,sp,32         # Establish new frame pointer
```

Purpose:

Prepares a stack frame for the function

Saves the calling context (RA, FP)

🔍Epilogue – Function Cleanup
```bash
lw      ra,28(sp)        # Restore return address
lw      s0,24(sp)        # Restore old frame pointer
addi    sp,sp,32         # Deallocate stack
ret                     # Return to caller
```

Purpose:

Cleans up the stack frame

Restores the previous execution context

## Output
![image](https://github.com/user-attachments/assets/dec40748-2ff1-44ea-9bc2-c1c3fdd79a3e)

# 04 - ELF to Raw Hex Conversion and Disassembly Guide

### 🎯 Objective

To convert an ELF executable file into raw binary or hex formats and how to disassemble it using `objdump`. It also describes the meaning of each part of the disassembly output.

## Steps

✅ Converting ELF to Raw Binary or HEX Format

 1.Create a Raw Binary File (.bin)
   ```bash
   riscv-none-elf-objcopy -O binary hello.elf hello.bin
   ```
2.Generate an Intel HEX File (.hex)
  ```bash
  riscv-none-elf-objcopy -O ihex hello.elf hello.hex
  ```
✅ Disassembling the ELF File

```bash
riscv-none-elf-objdump -d hello.elf
riscv-none-elf-objdump -S hello.elf
```

✅ Understanding the Disassembly Output

```bash
80000000 <main>:
80000000: 1101       addi   sp, sp, -32
80000002: ce06       sw     ra, 28(sp)
80000004: cc22       sw     s0, 24(sp)
```
Column	- Description
80000000: -	The memory address of the instruction
1101 - The machine code (hexadecimal opcode)
addi sp, sp, -32 - The assembly instruction (human-readable)

✅ Viewing the Raw Binary Contents

```bash
hexdump -C hello.bin
```

## Output
![image](https://github.com/user-attachments/assets/d0c0985d-9e81-4c8b-a36d-c6c34ff28cca)
![image](https://github.com/user-attachments/assets/a967694c-5150-40b6-9fd2-c44338e624e3)

# 05 - RISC-V RV32I Register ABI Cheat-Sheet

### 🎯 Objective

To provide a quick reference for all 32 integer registers in the RISC-V RV32I architecture. Each register is listed with its ABI (Application Binary Interface) name and its typical role in calling conventions.


## 🧠 Register Mapping Table

| Register | ABI Name | Description                     | Calling Convention Role      |
|----------|----------|----------------------------------|------------------------------|
| x0       | zero     | Hardwired zero                   | Constant value (0)           |
| x1       | ra       | Return address                   | Caller-saved                 |
| x2       | sp       | Stack pointer                    | Callee-saved                 |
| x3       | gp       | Global pointer                   | Static/global data access    |
| x4       | tp       | Thread pointer                   | Thread-local storage         |
| x5       | t0       | Temporary                        | Caller-saved                 |
| x6       | t1       | Temporary                        | Caller-saved                 |
| x7       | t2       | Temporary                        | Caller-saved                 |
| x8       | s0/fp    | Saved register / Frame pointer   | Callee-saved                 |
| x9       | s1       | Saved register                   | Callee-saved                 |
| x10      | a0       | Function argument / return value | Caller-saved (arg 0 / ret 0) |
| x11      | a1       | Function argument / return value | Caller-saved (arg 1 / ret 1) |
| x12      | a2       | Function argument                | Caller-saved (arg 2)         |
| x13      | a3       | Function argument                | Caller-saved (arg 3)         |
| x14      | a4       | Function argument                | Caller-saved (arg 4)         |
| x15      | a5       | Function argument                | Caller-saved (arg 5)         |
| x16      | a6       | Function argument                | Caller-saved (arg 6)         |
| x17      | a7       | Function argument                | Caller-saved (arg 7)         |
| x18      | s2       | Saved register                   | Callee-saved                 |
| x19      | s3       | Saved register                   | Callee-saved                 |
| x20      | s4       | Saved register                   | Callee-saved                 |
| x21      | s5       | Saved register                   | Callee-saved                 |
| x22      | s6       | Saved register                   | Callee-saved                 |
| x23      | s7       | Saved register                   | Callee-saved                 |
| x24      | s8       | Saved register                   | Callee-saved                 |
| x25      | s9       | Saved register                   | Callee-saved                 |
| x26      | s10      | Saved register                   | Callee-saved                 |
| x27      | s11      | Saved register                   | Callee-saved                 |
| x28      | t3       | Temporary                        | Caller-saved                 |
| x29      | t4       | Temporary                        | Caller-saved                 |
| x30      | t5       | Temporary                        | Caller-saved                 |
| x31      | t6       | Temporary                        | Caller-saved                 |


## 📌 Calling Convention Summary

- **Arguments / Return Values**:  
  `a0–a7` are used to pass arguments to functions and return values from them. These are **caller-saved**.

- **Saved Registers**:  
  `s0–s11` are used to preserve values across function calls. These are **callee-saved**.

- **Temporaries**:  
  `t0–t6` are temporary registers and **not preserved** across calls. These are **caller-saved**.

- **Special Registers**:
  - `zero`: Always reads as 0.
  - `ra`: Stores return address from function calls.
  - `sp`: Points to the current stack frame.
  - `gp`, `tp`: Used for global and thread-local data.


This cheat-sheet helps understand how function calls interact with registers in RISC-V, especially during assembly-level programming or debugging.

# 06 - Debugging RISC-V ELF with GDB

## 🎯 Objective

 To use `riscv32-unknown-elf-gdb` to load and debug a RISC-V ELF file by setting a breakpoint at `main`, stepping through code, and inspecting registers.

## 📁 Files

🔷hello1.c
```bash
int main() {
    volatile int x = 42;
    x = x + 1;
    return x;
}
```

🔷linker.ld
```bash
OUTPUT_ARCH(riscv)
ENTRY(main)
SECTIONS
{
    . = 0x80000000;
    .text : {
        *(.text.start)
        *(.text)
    }
    .data : ALIGN(4) {
        *(.data)
    }
    .bss : ALIGN(4) {
        *(.bss)
    }
    _end = .;
}
 ```
## Steps
✅ Compile the Program
```bash
riscv-none-elf-gcc -march=rv32im -mabi=ilp32 -nostdlib -T linker.ld -o hello1.elf hello1.c
```
✅ Check the ELF Header
```bash
riscv-none-elf-readelf -h hello1.elf
```
✅ Launch GDB
```bash
riscv-none-elf-gdb hello1.elf
```
Once in the GDB prompt:
```bash
(gdb) set breakpoint auto-hw off   # Avoid hardware breakpoint fallback
(gdb) target sim                  # Use the built-in simulator
(gdb) load                        # Load ELF into memory
```
✅ Set Breakpoint and Run
```bash
(gdb) info breakpoints
(gdb) delete
(gdb) break main
(gdb) info breakpoints
```
✅ Step Through and Inspect
```bash
(gdb) step         # Step to next line (C-level)
(gdb) stepi        # Step by one instruction (assembly)
(gdb) info registers
(gdb) print x
(gdb) continue
(gdb) quit
```
## Output
![image](https://github.com/user-attachments/assets/339ed8c6-19a3-4325-aed8-ec6d6317bb97)
![image](https://github.com/user-attachments/assets/2407c624-dcbb-49a0-b386-f2afbaa48431)
![image](https://github.com/user-attachments/assets/979ebc47-6166-41ff-b8c5-85396ed0f7d0)


























 





