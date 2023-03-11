#include <avr/io.h>
#include <avr/interrupt.h>

// Define constants
#define F_CPU 16000000UL // AVR clock frequency
#define BAUD 115200      // UART baud rate
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

// Define variables
volatile uint8_t tx_buffer[64];
volatile uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;

// Initialize UART
void uart_init(void)
{
  UBRR0H = (uint8_t)(UBRR_VALUE>>8);
  UBRR0L = (uint8_t)(UBRR_VALUE);
  UCSR0B = (1<<TXEN0) | (1<<TXCIE0); // Enable transmitter and TX complete interrupt
  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8-bit data format, no parity, 1 stop bit
}

// Transmit a byte over UART
void uart_transmit(uint8_t data)
{
  uint8_t tmp_head = (tx_buffer_head + 1) & 0x3F; // Buffer size is 64, so mask with 0x3F to wrap around
  while (tmp_head == tx_buffer_tail); // Wait for buffer space to become available
  tx_buffer[tmp_head] = data; // Add data to buffer
  tx_buffer_head = tmp_head; // Update buffer head pointer
  UCSR0B |= (1<<UDRIE0); // Enable data register empty interrupt
}

// Interrupt service routine for UART data register empty
ISR(USART_UDRE_vect)
{
  if (tx_buffer_head != tx_buffer_tail) // If there is data in the buffer
  {
    tx_buffer_tail = (tx_buffer_tail + 1) & 0x3F; // Update buffer tail pointer
    UDR0 = tx_buffer[tx_buffer_tail]; // Load data into transmit buffer
  }
  else // If there is no data in the buffer
  {
    UCSR0B &= ~(1<<UDRIE0); // Disable data register empty interrupt
  }
}

int main(void)
{
  sei(); // Enable interrupts

  uart_init(); // Initialize UART

  // Send AT command
  uart_transmit('A');
  uart_transmit('T');
  uart_transmit('\r');
  uart_transmit('\n');

  while (1)
  {
    // Main program loop
  }
}
