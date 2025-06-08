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
