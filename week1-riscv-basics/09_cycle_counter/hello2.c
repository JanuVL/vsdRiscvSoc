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
