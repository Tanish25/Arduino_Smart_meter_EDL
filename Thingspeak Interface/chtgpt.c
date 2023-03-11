#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL ((F_CPU/(16UL*BAUD))-1)

// Buffer variables
#define BUFFER_SIZE 64
volatile uint8_t buffer_head = 0;
volatile uint8_t buffer_tail = 0;
volatile uint8_t buffer[BUFFER_SIZE];

// UART initialization function
void uart_init(void) {
  UBRR0H = (uint8_t)(UBRR_VAL >> 8);
  UBRR0L = (uint8_t)UBRR_VAL;
  UCSR0B = (1 << TXEN0) | (1 << TXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// UART transmission interrupt service routine
ISR(USART_TX_vect) {
  if (buffer_head != buffer_tail) {
    UDR0 = buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
  }
}

// Buffer function to add data to the buffer
void uart_buffer(char data) {
  uint8_t next_head = (buffer_head + 1) % BUFFER_SIZE;
  if (next_head != buffer_tail) {
    buffer[buffer_head] = data;
    buffer_head = next_head;
  }
}

// Buffer function to add a string to the buffer
void uart_buffer_string(char* string) {
  uint8_t length = strlen(string);
  for (uint8_t i = 0; i < length; i++) {
    uart_buffer(string[i]);
  }
}

// Main function
int main(void) {
  uart_init();
  sei(); // Enable global interrupts

  while (1) {
    uart_buffer_string("Hello, World!\r\n");
  }

  return 0;
}
