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
