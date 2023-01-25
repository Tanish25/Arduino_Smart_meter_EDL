#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

volatile char recv_character;
//char transmit_ch = ' ';

//char send_str[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"\r\n";
char send_str[] = "AT\r\n";
//char send_str[]="AT+CWMODE=1\r\n";
volatile uint8_t len;
volatile uint8_t i = 0;

void uart1_init(void)
 {
  // configure the first UART module for transmitting data initially
  UBRR1 = 8;                                                                             // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1)|(1 << UDRIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void uart2_init(void) 
{
  // configure the second UART module for transmitting the received data
  UBRR2 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << TXEN2);
   UCSR2A = 0x00;
  UCSR2C = 0x06;  
}

void uart3_init(void) 
{
  // configure the third UART module for receiving data
  UBRR3 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR3B |= (1 << RXEN3)| (1 << RXCIE3);
   UCSR3A = 0x00;
  UCSR3C = 0x06;  
}

ISR(USART1_UDRE_vect)
 {
  //lenr=len_calc(send_str);

  if (i < len) 
  	{ 
  	UDR1 = send_str[i++];
  	//UCSR1B |= (1 << UDRIE1); 
   }
}

ISR(USART3_RX_vect) {
  recv_character = UDR3;    // read the received data
  UCSR2B |= (1 << UDRIE2);  // enable the data register empty interrupt for the second UART module
}

ISR(USART2_UDRE_vect) {
  UDR2 = recv_character;     // send the received data through the second UART module
  UCSR2B &= ~(1 << UDRIE2);  // disable the data register empty interrupt for the second UART module
}

int main(void) {

  uart1_init();
  uart2_init();
  uart3_init();
  len=strlen(send_str);
  sei();  // enable global interrupts

  while (1);
  return 0;
}
