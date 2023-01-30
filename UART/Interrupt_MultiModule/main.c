#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

volatile char recv_character;
volatile uint8_t recv_byte;
//char send_str[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"\r\n";
char send_str[]="AT\r\n";
//char send_str[] = "AT+CWMODE=1\r\n";
char recv_str[];
volatile uint8_t len;
volatile uint8_t i = 0;
volatile uint8_t j = 0;

void uart1_init(void)
{
  // configure the first UART module for transmitting data initially
  UBRR1H = 0;  
  UBRR1L = 8; // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1)|(1 << UDRIE1);  // enable transmitter and UDRE interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void uart2_init(void) 
{
  // configure the second UART module for receiving data
  UBRR2H = 0;  
  UBRR2L = 8;              // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << RXEN2)| (1 << RXCIE2);
  UCSR2A = 0x00;
  UCSR2C = 0x06;  
}

void uart3_init(void) 
{
	 // configure the third UART module for transmitting the received data
  
  UBRR3H = 0;  
  UBRR3L = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR3B |=(1 << RXEN3);
  UCSR3A = 0x00;
  UCSR3C = 0x06;  
}


ISR(USART1_UDRE_vect)
{
  if (i < len) 
  { 
  	UDR1 = send_str[i++];
  }
}

ISR(USART2_RX_vect) 
{
  recv_byte = UDR2;
  recv_str[j++] = recv_byte; 
  UDR3 = recv_byte;  
}


int main(void) 
{
	
  uart1_init();
  uart2_init();
  uart3_init();
  len=strlen(send_str);
  sei();  // enable global interrupts
  while(1);
  return 0;
}

