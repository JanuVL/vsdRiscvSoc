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
