#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>


volatile char recv_character;
char string[] = "AT+CWMODE=1\r\n"; // the string to be sent
volatile uint8_t current_char = 0;        // the current character index
volatile uint8_t receive_char = 0; 
char receive_string[];
volatile uint8_t len; 

void uart1_init(void) {
  // configure the first UART module for receiving data
  UBRR1 = 8;                                                                             // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << UDRIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void uart2_init(void)
{
   // configure the second UART module for transmitting the received data
  UBRR2 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << RXEN2) | (1 << RXCIE2);
  UCSR2A = 0x00;
  UCSR2C = 0x06;
}

void uart3_init(void) 
{
   // configure the second UART module for transmitting the received data
  UBRR3 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR3B |= (1 << TXEN3)|(1 << UDRIE3);
  UCSR3A = 0x00;
  UCSR3C = 0x06;  
}

ISR(USART1_UDRE_vect) {
    // check if there are more characters to send
    //if(current_char<len)
    if (string[current_char] != '\0')
    {
        UDR1 = string[current_char++]; // send the current character and increment the index
    }
}

ISR(USART2_RX_vect) {
    while(UDR2 != '\0') {
        receive_string[receive_char++] = UDR2;
    }    // read the received data
  UCSR3B |= (1 << UDRIE3);  // enable the data register empty interrupt for the second UART module
}

ISR(USART3_UDRE_vect) {
  UDR3 = receive_string[receive_char];      // send the received data through the second UART module
  UCSR3B &= ~(1 << UDRIE3);  // disable the data register empty interrupt for the second UART module
}

int main(void) {

    uart1_init();
    uart2_init();
    uart3_init();
    len=strlen(string);
    sei(); // enable global interrupts

    while (1) {
        // do other tasks here
    }

    return 0;
}
