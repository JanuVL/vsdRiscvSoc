# 01 – Toolchain Setup

## ✅ Objective
Download, extract, and configure the RISC-V GCC toolchain from VSD Labs. Verify that the compiler, debugger, and disassembler are working.

## Steps
🔹 1.1 Open Ubuntu 
🔹 1.2 Download the Toolchain in WSL Inside Ubuntu terminal: cd ~ wget https://vsd-labs.sgp1.cdn.digitaloceanspaces.com/vsd-labs/riscv-toolchain-rv32imac-x86_64-ubuntu.tar.gz
🔹 1.3 Extract the Toolchain: tar -xzf riscv-toolchain-rv32imac-x86_64-ubuntu.tar.gz 
🔹 1.4 Add Toolchain to PATH : echo 'export PATH=$HOME/riscv/bin:$PATH' >> ~/.bashrc 
                                source ~/.bashrc
🔹 1.5 Confirm Toolchain Installed: riscv32-unknown-elf-gcc --version 
                                    riscv32-unknown-elf-objdump --version 
                                    riscv32-unknown-elf-gdb --version 
## OUTPUT
  ![image](https://github.com/user-attachments/assets/a89ad280-4fa6-488a-8f50-fb05d658cef2)
  
                                    
