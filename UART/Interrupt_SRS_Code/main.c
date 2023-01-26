#include <avr/io.h>
#include <avr/interrupt.h>


volatile char recv_character;
char string[] = "AT+CWMODE=1\r"; // the string to be sent
uint8_t current_char = 0;        // the current character index

void uart1_init(void) {
  // configure the first UART module for receiving data
  UBRR1 = 8;                                                                             // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1) | (1 << UDRIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void uart2_init(void) 
{
   // configure the second UART module for transmitting the received data
  UBRR2 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << TXEN2)|(1 << UDRIE2);
  UCSR2A = 0x00;
  UCSR2C = 0x06;  
}

ISR(USART1_UDRE_vect) {
    // check if there are more characters to send
    if (string[current_char] != '\0') {
        UDR1 = string[current_char++]; // send the current character and increment the index
    } 
}

ISR(USART1_RX_vect) {
  recv_character = UDR1;     // read the received data
  UCSR2B |= (1 << UDRIE2);  // enable the data register empty interrupt for the second UART module
}

ISR(USART2_UDRE_vect) {
  UDR2 = recv_character;      // send the received data through the second UART module
  UCSR2B &= ~(1 << UDRIE2);  // disable the data register empty interrupt for the second UART module
}

int main(void) {
  
    uart1_init();
    uart2_init();
    sei(); // enable global interrupts

    while (1) {
        // do other tasks here
    }

    return 0;
}
