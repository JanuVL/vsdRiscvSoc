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

# 07 - UART Bare-Metal Boot on QEMU

## 🎯 Objective
To demonstrate how to boot a bare-metal RISC-V ELF file using QEMU and display UART output.

## 📁 Files

`hello2.c`
```bash
// hello2.c
#define UART0 0x10000000

void uart_putc(char c) {
    *(volatile char *)UART0 = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void main() {
    uart_puts("Welcome to RISC-V UART Output!\n");
    int y = 99;
    uart_puts("Current value of y: ");

    // Convert y to ASCII
    if (y >= 10) {
        uart_putc((y / 10) + '0');
    }
    uart_putc((y % 10) + '0');
    uart_putc('\n');
}
```
`start.s`
```bash
.section .text.start
.global _start
_start:
    la sp, _stack_top      # Initialize stack pointer
    jal main               # Call main
    j .                    # Infinite loop after main returns
.section .bss
.align 4
.space 1024                # Reserve 1KB for stack
_stack_top:
```
`linker.ld`
```bash
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x80000000, LENGTH = 16M
    RAM (rw)   : ORIGIN = 0x81000000, LENGTH = 16M
}

SECTIONS
{
    . = 0x80000000;

    .text : {
        *(.text.start)
        *(.text)
        *(.text.*)
    } > FLASH

    .rodata : ALIGN(4) {
        *(.rodata)
        *(.rodata.*)
    } > FLASH

    .data : ALIGN(4) {
        *(.data)
        *(.data.*)
    } > RAM AT > FLASH

    .bss : ALIGN(4) {
        *(.bss)
        *(.bss.*)
    } > RAM

    _end = .;
}
```
## Steps
✅ Compile:
```bash
riscv-none-elf-gcc -g -march=rv32im -mabi=ilp32 -nostdlib -T linker.ld -o hello2.elf hello2.c start.s
```
✅ Verify ELF
```bash
riscv-none-elf-readelf -h hello2.elf
riscv-none-elf-readelf -l hello2.elf
```
✅ Run with QEMU
```bash
qemu-system-riscv32 -nographic -machine virt -bios none -kernel hello2.elf
```
✅ Expected UART Output
```bash
Welcome to RISC-V UART Output!
Current value of y: 99
```
## Output
![image](https://github.com/user-attachments/assets/4b78638d-ecba-419d-b64d-6c9b925351c8)
![image](https://github.com/user-attachments/assets/cbb998a4-8d89-4dd8-9d8e-ec0ed7222e4e)

# 08 - RISC-V Bare-Metal Compiler Optimization Analysis (-O0 vs. -O2)

## 🎯 Objective
Compare assembly output with -O0 and -O2 to understand how GCC optimizations like dead-code elimination, register allocation, and inlining improve performance.

## Steps

✅ Building the Program with -O0 (No Compiler Optimization)

➤ Compile to ELF:
```bash
riscv-none-elf-gcc -g -O0 -march=rv32im -mabi=ilp32 -nostdlib \
-T linker.ld -o hello2_O0.elf hello2.c start.s
```
➤ Generate Assembly Output:
```bash
riscv-none-elf-gcc -g -O0 -march=rv32im -mabi=ilp32 -nostdlib \
-S -o hello2_O0.s hello2.c
```
✅ Building with -O2 (Level 2 Optimization)

➤ Compile to Optimized ELF:
```bash
riscv-none-elf-gcc -g -O2 -march=rv32im -mabi=ilp32 -nostdlib \
-T linker.ld -o hello2_O2.elf hello2.c start.s
```
➤ Produce Optimized Assembly:
```bash
riscv-none-elf-gcc -g -O2 -march=rv32im -mabi=ilp32 -nostdlib \
-S -o hello2_O2.s hello2.c
```
✅ ELF Header Inspection
```bash
riscv-none-elf-readelf -h hello2_O0.elf
riscv-none-elf-readelf -h hello2_O2.elf
```
✅ Execute in QEMU Emulator

➤ Run the Unoptimized Version:
```bash
qemu-system-riscv32 -nographic -machine virt -bios none \
-kernel ~/hello2_O0.elf
```
➤ Run the Optimized Version:
```bash
qemu-system-riscv32 -nographic -machine virt -bios none \
-kernel ~/hello2_O2.elf
```
📤 Expected Serial Output (UART Console):
```bash
Welcome to RISC-V UART Output!
Current value of y: 99
```
## Output

![image](https://github.com/user-attachments/assets/f48942fb-4789-477f-be89-42ea3281c70f)
![image](https://github.com/user-attachments/assets/9c7e695d-fc9c-4952-a325-a0dc54232adb)

## 🔍 Key Differences Between `-O0` and `-O2`

| Feature             | `-O0` (No Optimization)                          | `-O2` (Optimized)                                   | Explanation                                                                 |
|---------------------|--------------------------------------------------|-----------------------------------------------------|-----------------------------------------------------------------------------|
| **Stack Usage**     | Large stack frame (~48 bytes)                   | Minimal stack frame (~16 bytes)                     | `-O2` avoids unnecessary stack allocations and uses registers effectively.  |
| **Variable Storage**| Variables stored on stack                       | Variables held in registers                         | Reduces memory access latency and improves speed.                          |
| **Instruction Count**| More instructions, repeated loads/stores       | Fewer, cleaner instructions                         | Optimizer eliminates redundancy and merges operations.                     |
| **Function Calls**  | Regular calls to helper functions               | Small functions like `uart_putc` may be inlined     | Reduces overhead of function calling.                                      |
| **Loop Handling**   | Uses explicit memory access for counters        | Keeps loop counters in registers                    | Enhances loop performance and minimizes RAM traffic.                       |
| **Debug Info**      | Full debug-friendly layout                      | Less debug information available                    | `-O2` sacrifices debuggability for runtime efficiency.                     |
| **Dead Code**       | Retained, even if unused                        | Removed if it doesn't affect output                 | Compiler prunes code paths that have no effect.                           |

🧠 Key Concepts Explained:
🧹 Dead-Code Elimination:
Removes code that does not impact final output. For example, unused variables or unreachable branches may be stripped out.

🗂 Register Allocation:
Instead of using memory (RAM/stack), frequently accessed variables are stored in CPU registers, which significantly improves execution speed.

🔁 Function Inlining:
Small, frequently-used functions (like uart_putc) are expanded inline where they are called. This removes the overhead of a function call, improving performance.

# 09 - RISC-V Inline Assembly: Reading the Cycle Counter

## 🎯 Objective
To demonstrate how to read the RISC-V CPU cycle counter (`CSR 0xC00`) using inline assembly in C and to explain the GCC inline assembly syntax and constraints.

## 📁 Files

 `cycle_counter.c`
 ```bash
 // cycle_counter.c
#include <stdint.h>

// Function to read the RISC-V cycle counter
static inline uint32_t rdcycle(void) {
    uint32_t c;
    asm volatile("csrr %0, cycle" : "=r"(c));
    return c;
}
```
 `hello2.c`
 ```bash
// hello2.c
#define UART0 0x10000000
typedef unsigned int uint32_t;

// UART output single character
void uart_putc(char c) {
    *(volatile char *)UART0 = c;
}

// UART output string
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Print a number (unsigned) using UART
void print_number(uint32_t num) {
    char buf[10];
    int i = 0;

    if (num == 0) {
        uart_putc('0');
        return;
    }

    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i--) {
        uart_putc(buf[i]);
    }
}

// Read cycle counter CSR (0xC00)
static inline uint32_t rdcycle(void) {
    uint32_t c;
    asm volatile("csrr %0, cycle" : "=r"(c));
    return c;
}

void main() {
    uart_puts("Welcome to RISC-V UART Output!\n");

    int y = 99;
    uart_puts("Current value of y: ");

    // Print y as ASCII
    if (y >= 10) {
        uart_putc((y / 10) + '0');
    }
    uart_putc((y % 10) + '0');
    uart_putc('\n');

    // Read starting cycle count
    uint32_t start = rdcycle();

    // Dummy loop for delay
    for (volatile int i = 0; i < 10000; i++);

    // Read ending cycle count
    uint32_t end = rdcycle();

    uart_puts("Cycle difference: ");
    print_number(end - start);
    uart_putc('\n');
}
```
## Steps

✅Compile
```bash
riscv-none-elf-gcc -g -O0 -march=rv32im_zicsr -mabi=ilp32 -nostdlib -T linker.ld -o hello2.elf hello2.c start.s
```
✅Running the Program on QEMU
```bash
qemu-system-riscv32 -nographic -machine virt -bios none -kernel cycle_counter.elf
```
Expected output:
Cycle difference: <some number>

## Output
![image](https://github.com/user-attachments/assets/73a73f59-54b6-4081-8b66-a986f055420f)

## Explanation of Inline Assembly Constraints

| Part                  | Description                                                                                               |
|-----------------------|-----------------------------------------------------------------------------------------------------------|
| `asm volatile`        | GCC keyword for embedding assembly; `volatile` ensures the instruction is not optimized away.            |
| `"csrr %0, cycle"`    | Assembly code that reads the cycle counter CSR into operand `%0`.                                         |
| `: "=r"(c)`           | Output constraint: `=` means write-only; `r` means assign a general-purpose register; `c` stores the result. |
| *(empty input operands)* | No input operands are required because the instruction reads a CSR directly.                              |
| *(empty clobber list)* | No additional registers are modified beyond the output register, so this list is empty.                    |

# 10 - Memory-Mapped I/O Demo — GPIO Toggle 

## 🎯 Objective
To demonstrate how to toggle a GPIO pin using memory-mapped I/O in a bare-metal RISC-V environment and explain how the volatile keyword ensures correct behavior by preventing compiler optimizations.

## 📁 Files

`gpio_toggle.c`
```bash
#define UART0     0x10000000
#define GPIO_ADDR 0x10012000  // GPIO output base address (QEMU virt)

// UART print one character
void uart_putc(char c) {
    *(volatile char *)UART0 = c;
}

// UART print string
void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// GPIO toggle function
void gpio_toggle() {
    volatile unsigned int *gpio = (volatile unsigned int *)GPIO_ADDR;
    *gpio |= (1 << 0);   // Set bit 0 (turn GPIO on)
    *gpio &= ~(1 << 0);  // Clear bit 0 (turn GPIO off)
}

// Entry point
void main() {
    uart_puts("gpio toggled\n");
    gpio_toggle();

    // Optional debug output to confirm code is still running
    while (1) {
        uart_putc('.');                    // Print dot repeatedly
        for (volatile int i = 0; i < 100000; i++); // Crude delay
    }
}
```
## Steps

✅Compile:
```bash
riscv-none-elf-gcc -g -O2 -march=rv32im -mabi=ilp32 -nostdlib -T linker.ld -o gpio_toggle.elf gpio_toggle.c start.s
```
✅Verify ELF:
```bash
riscv-none-elf-readelf -h gpio_toggle.elf
```
✅Run in QEMU:
```bash
qemu-system-riscv32 -nographic -machine virt -bios none -kernel ~/gpio_toggle.elf
```
## How volatile Ensures Correct Behavior

The volatile keyword tells the compiler that the value of a variable may change at any time (e.g., due to hardware or another thread), so it must not optimize away reads or writes to that variable. This is crucial for hardware registers like GPIO, as the compiler might otherwise remove or reorder accesses, leading to incorrect or unexpected behavior.

## Output
![image](https://github.com/user-attachments/assets/a5a881bf-53aa-42e3-8af2-84f26b44168f)

# 11 - RISC-V Minimal Linker Script Project

## 🎯 Objective

This project demonstrates how to manually control memory layout in a bare-metal RISC-V environment using a **custom linker script**, a startup file (`start.s`), and a simple C program (`hello2.c`) that prints UART output via QEMU.

## 📁 Files

`linker4.ld`
```bash
/* Minimal linker script for hello2.elf */

ENTRY(_start)

MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
  SRAM  (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS {
  /* Place .text at 0x00000000 in FLASH */
  .text 0x0 : {
    *(.text*)
    *(.rodata*)
  } > FLASH

  /* Place .data at 0x10000000 in SRAM */
  .data 0x10000000 : {
    _data_start = .;
    *(.data*)
    _data_end = .;
  } > SRAM

  /* Place .bss immediately after .data in SRAM */
  .bss : {
    _bss_start = .;
    *(.bss*)
    *(COMMON)
    _bss_end = .;
  } > SRAM

  /* Set the top of the stack to end of SRAM */
  _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
}
```
## Steps
✅ Compile:
riscv-none-elf-gcc -g -march=rv32im -mabi=ilp32 -nostdlib -T linker4.ld -o hello2.elf hello2.c start.s
✅ Verify ELF:
riscv-none-elf-readelf -h hello2.elf
riscv-none-elf-readelf -l hello2.elf
✅ Run with QEMU:
qemu-system-riscv32 -nographic -machine virt -bios none -kernel ~/hello2.elf
 ## Output
 ![image](https://github.com/user-attachments/assets/066c9f30-290f-4cf3-97af-2ddcdef5a21d)
 ![image](https://github.com/user-attachments/assets/6d65c0cd-6716-426c-97fb-4ca80e8e7f78)
 
 why Flash and SRAM addresses differ:

 | Memory Type | Typical Use         | Volatile? | Example Address | Key Properties                      |
|-------------|--------------------|-----------|-----------------|-------------------------------------|
| Flash       | Code, constants    | No        | 0x00000000      | Non-volatile, read-only at runtime  |
| SRAM        | Data, stack, heap  | Yes       | 0x10000000      | Volatile, fast read/write           |

# 12 - Start-up Code & crt0

❓ What is crt0.S?
crt0.S is the start-up assembly code for bare-metal systems. It is the first code executed after a system reset—before main() is called.

🔧 Responsibilities of crt0.S in RISC-V Bare-Metal:
🪜 Set up the stack pointer

🧹 Zero out the .bss section (uninitialized globals)

📥 Copy .data from Flash to SRAM (if necessary)

🔁 Call main()

🔒 Enter infinite loop to prevent return from main()

📌 Where to Find or Write One?
You can:

Write your own using ### `.section .text`, ### `lui`, ### `addi`, ### `la`, ### `etc`.

Use examples from:

newlib/libgloss

Board vendor SDKs (e.g., SiFive, Kendryte)

Open-source RTOSes like FreeRTOS (in portable/GCC/RISC-V/)

✅ Your start.s Here Acts as crt0

In this project, the start.s file acts as a minimal crt0:
```bash
_start:
  lui sp, %hi(_stack_top)
  addi sp, sp, %lo(_stack_top)
  call main
1: j 1b
```

# 13 - RISC-V Timer Interrupt UART Demo

## 🎯 Objective
To demonstrate a simple bare-metal RISC-V program that uses the machine timer interrupt to periodically output a message via UART on a QEMU virt platform.

## 📁 Files

`timer_interrupt.c`
```bash
#define UART0 0x10000000       // UART transmit register (QEMU virt)
#define MTIME 0x0200bff8       // Machine timer register (QEMU virt CLINT)
#define MTIMECMP 0x02004000    // Machine timer compare register

typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

void uart_putc(char c) {
    *(volatile char *)UART0 = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Timer interrupt handler
void timer_handler(void) {
    uart_puts("MTIP\n");

    volatile uint64_t *mtime = (volatile uint64_t *)MTIME;
    volatile uint64_t *mtimecmp = (volatile uint64_t *)MTIMECMP;
    *mtimecmp = *mtime + 1000000;  // Set next interrupt ~1s later
}

void enable_timer_interrupt(void) {
    volatile uint64_t *mtime = (volatile uint64_t *)MTIME;
    volatile uint64_t *mtimecmp = (volatile uint64_t *)MTIMECMP;
    *mtimecmp = *mtime + 1000000;

    // Enable MTIP (bit 7) in mie CSR
    asm volatile ("li t0, 0x80");
    asm volatile ("csrs mie, t0");

    // Enable global interrupts (MIE, bit 3) in mstatus CSR
    asm volatile ("csrs mstatus, 0x8");
}

void main() {
    uart_puts("Timer interrupt demo\n");
    enable_timer_interrupt();

    while (1) {
        uart_putc('.');
        for (volatile int i = 0; i < 100000; i++);
    }
}
```
`trap_handler.s`
```bash
.section .text
.global trap_handler
.align 4
trap_handler:
    # Save registers
    addi sp, sp, -64
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw t1, 8(sp)

    # Check mcause for MTIP (0x80000007)
    csrr t0, mcause
    li t1, 0x80000007
    bne t0, t1, skip

    # Call C handler
    jal timer_handler

skip:
    # Restore registers
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw t1, 8(sp)
    addi sp, sp, 64

    # Return
    mret
```
`start.s`
```bash
.section .text.start
.global _start
_start:
    # Initialize stack pointer
    la sp, _stack_top

    # Early UART output
    li t0, 0x10000005
    li t1, 0x20  # Bit 5
wait_uart:
    lb t2, 0(t0)
    and t2, t2, t1
    beq t2, zero, wait_uart
    li t0, 0x10000000
    li t1, 'S'   # Print 'S' to confirm entry
    sb t1, 0(t0)

    # Set trap vector
    la t0, trap_handler
    csrw mtvec, t0

    # Jump to main
    jal main
    j .

.section .bss
.align 4
.space 1024
_stack_top:
```
`linker.ld`
```bash
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x80000000, LENGTH = 16M
    RAM   (rw) : ORIGIN = 0x81000000, LENGTH = 16M
}

SECTIONS
{
    .text : {
        *(.text.start)
        *(.text)
        *(.text.*)
    } > FLASH

    .rodata : ALIGN(4) {
        *(.rodata)
        *(.rodata.*)
    } > FLASH

    .data : ALIGN(4) {
        *(.data)
        *(.data.*)
    } > RAM AT > FLASH

    .bss : ALIGN(4) {
        *(.bss)
        *(.bss.*)
    } > RAM

    _end = .;
}
```

## Steps

✅ Compile:
```bash
riscv-none-elf-gcc -g -O2 -march=rv32imc_zicsr -mabi=ilp32 -nostdlib -T linker.ld -o timer.elf timer_interrupt.c trap_handler.s start.s
```
✅ Verify ELF:
```bash
riscv-none-elf-readelf -h timer.elf
```
✅ Run in QEMU:
```bash
qemu-system-riscv32 -nographic -machine virt -bios none -kernel timer.elf
```
✅ Expected Output:
```bash
S A Timer enabled .MTIP .MTIP ... (MTIP every ~1s, dots continue)
```

## Output
![image](https://github.com/user-attachments/assets/00979455-d241-42b1-89ac-aae059062cd5)

# 13 - RV32IMAC vs RV32IMC – What’s the “A” Extension?

The **‘A’ extension** in `rv32imac` stands for **Atomic operations**. It adds a set of instructions that enable **atomic read-modify-write memory operations**, which are crucial for implementing safe concurrency in multi-threaded or multi-core systems.

#### Key Instructions Included:
- **`lr.w` (Load-Reserved Word):** Reads a word from memory and marks it for a possible subsequent conditional store.
- **`sc.w` (Store-Conditional Word):** Attempts to store a word to memory only if no other write has occurred since the corresponding `lr.w`.
- **Atomic Memory Operations (AMOs):** These perform combined read-modify-write operations atomically, such as:  
  - `amoadd.w` (atomic add)  
  - `amoxor.w` (atomic XOR)  
  - `amoor.w` (atomic OR)  
  - `amomin.w` / `amomax.w` (atomic min/max)  

#### Why Is the Atomic Extension Important?

- **Lock-free synchronization:** Enables building efficient lock-free data structures without disabling interrupts or using heavy locking mechanisms.
- **Operating system support:** Essential for implementing mutexes, spinlocks, and other kernel-level synchronization primitives.
- **Race condition prevention:** Guarantees atomicity of critical memory operations in multi-core or multi-threaded environments, avoiding inconsistent or corrupted shared data.


In comparison, the `rv32imc` ISA subset includes the base integer (`I`), multiplication (`M`), and compressed (`C`) extensions but **does not provide atomic instructions**, so it lacks built-in support for hardware-level synchronization.

The atomic instructions in `rv32imac` make it a more powerful ISA for systems requiring concurrency control.

# 15 – Atomic Test Program using `lr.w` / `sc.w` on RV32IMAC

## 🎯 Objective

To demonstrate the use of **atomic instructions (`lr.w` and `sc.w`)** on the **RV32IMAC** architecture by implementing a **spin-lock (mutex)** in C using **inline assembly**. This program simulates **two pseudo-threads** in `main()` accessing a shared counter with **mutual exclusion**.

## 📁 Files

`mutex1.c`
```bash
#define UART_TX 0x10000000    // UART transmit register (QEMU virt)
#define UART_READY 0x10000005 // UART status register (bit 5 = TX ready)

typedef unsigned int uint32_t;

// Shared counter and mutex
volatile uint32_t shared_counter = 0;
volatile uint32_t mutex = 0; // 0 = unlocked, 1 = locked

void uart_putc(char c) {
    volatile char* uart_tx = (volatile char*)UART_TX;
    volatile char* uart_ready = (volatile char*)UART_READY;
    while (!(*uart_ready & (1 << 5))); // Wait until TX ready
    *uart_tx = c;
}

void uart_puts(const char* s) {
    while (*s) {
        uart_putc(*s++);
    }
}

// Integer to UART decimal print
void uart_print_uint(uint32_t val) {
    char buf[10];
    int i = 0;
    if (val == 0) {
        uart_putc('0');
        return;
    }
    while (val > 0) {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }
    while (i--) {
        uart_putc(buf[i]);
    }
}

// Mutex lock using lr.w/sc.w
void mutex_lock(volatile uint32_t* lock) {
    uint32_t tmp;
    do {
        asm volatile ("lr.w %0, (%1)" : "=r"(tmp) : "r"(lock));
        if (tmp != 0) continue; // Locked, retry
        asm volatile ("sc.w %0, %2, (%1)" : "=r"(tmp) : "r"(lock), "r"(1));
    } while (tmp != 0);
}

// Mutex unlock
void mutex_unlock(volatile uint32_t* lock) {
    *lock = 0;
}

// Thread 1
void thread1(void) {
    mutex_lock(&mutex);
    uart_puts("T1: Enter critical section\n");
    shared_counter++;
    uart_puts("T1: Counter = ");
    uart_print_uint(shared_counter);
    uart_putc('\n');
    mutex_unlock(&mutex);
    uart_puts("T1: Exit critical section\n");
}

// Thread 2
void thread2(void) {
    mutex_lock(&mutex);
    uart_puts("T2: Enter critical section\n");
    shared_counter++;
    uart_puts("T2: Counter = ");
    uart_print_uint(shared_counter);
    uart_putc('\n');
    mutex_unlock(&mutex);
    uart_puts("T2: Exit critical section\n");
}

int main() {
    uart_putc('A'); // Start indicator
    uart_puts("\nStarting threads\n");

    thread1();
    thread2();
    thread1();
    thread2();

    uart_puts("Final Counter = ");
    uart_print_uint(shared_counter);
    uart_putc('\n');

    while (1) {
        uart_putc('.');
        for (volatile int i = 0; i < 100000; i++);
    }

    return 0;
}
```
## Steps

✅ Compile:
```bash
riscv-none-elf-gcc -g -O2 -march=rv32imac_zicsr -mabi=ilp32 -nostdlib -T linker.ld -o mutex1.elf mutex1.c start.s
```
✅ Verify ELF:
```bash
riscv-none-elf-readelf -h mutex1.elf
```
✅ Run in QEMU:
```bash
qemu-system-riscv32 -nographic -machine virt -bios none -kernel mutex1.elf
```

## Output
![image](https://github.com/user-attachments/assets/ed6ad734-7de8-487a-b153-cce2addfeee2)
![image](https://github.com/user-attachments/assets/92b194bb-5bd3-4447-b04e-e218814d8d06)

# 16 – Retargeting `printf` to UART in Bare-Metal RISC-V

## ✅ Objective

Enable the use of `printf()` without an operating system by retargeting the `_write` system call to a memory-mapped UART interface. This allows formatted text output via `printf` to be displayed on the UART console in QEMU or real hardware.

## 🔁 Output Chain Overview

- **`printf()`** → formats data to a buffer  
- **`fputc()`/`fputs()`** → used internally by `printf`  
- **`_write()`** → sends each character to UART
- 
## 📌 Background

In bare-metal systems, the standard C library (`newlib`) uses weak implementations of low-level I/O functions like `_write`, `_read`, and `_sbrk`. Since there is no OS, we must provide our own implementation of `_write()` to send output to UART.

## 🧾 Code Summary

### main.c

```c
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

// UART registers
volatile uint32_t *const UART_DR = (volatile uint32_t *)0x10000000;
volatile uint32_t *const UART_SR = (volatile uint32_t *)0x10000005;
#define UART_SR_TX_READY (1 << 5)

// Retargeted _write syscall
ssize_t _write(int file, const void *ptr, size_t len) {
    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        const char *buf = (const char *)ptr;
        for (size_t i = 0; i < len; ++i) {
            while (!(*UART_SR & UART_SR_TX_READY));
            *UART_DR = buf[i];
        }
        return len;
    }
    return -1;
}

int main() {
    printf("Welcome to RISC-V UART printf!\n");

    int y = 99;
    printf("Current value of y: %d\n", y);

    while (1) {
        printf(".");
        for (volatile int i = 0; i < 100000; i++);
    }
    return 0;
}
```
⚙️ Compilation
```bash
riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 \
  -T linker.ld -nostartfiles -o printf_uart.elf main.c
```
📎 Notes
No OS, RTOS, or runtime required.

Works with QEMU using -nographic -kernel printf_uart.elf.

## Output
![image](https://github.com/user-attachments/assets/0ad30e63-22f5-43bd-8356-d5060942a50c)







































































 





