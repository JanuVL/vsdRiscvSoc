# 04 – Disassembly & HEX Dump
## ✅ Objective
Disassemble the compiled `.elf` and extract its Intel HEX format for loading into memory models/emulators.

## Steps
🔹 1. Disassemble ELF (show human-readable assembly)
```bash
riscv32-unknown-elf-objdump -d hello.elf > hello.dump
``` 
🔹 2. Convert ELF to Intel HEX format
```bash
riscv32-unknown-elf-objcopy -O ihex hello.elf hello.hex
```
Columns in Disassembly
```bash
0: 1141     addi sp,sp,-16
```

Column -	Meaning
0: -	Offset address
1141 -	Opcode (hex)
addi -	Instruction mnemonic



