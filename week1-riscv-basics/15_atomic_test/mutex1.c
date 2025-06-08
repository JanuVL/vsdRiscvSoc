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
