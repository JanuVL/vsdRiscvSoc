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




 





