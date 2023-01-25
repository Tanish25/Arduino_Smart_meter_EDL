#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#define BAUDRATE  115200L
//uint8_t recv_byte;
volatile char recv_str[100];
volatile char recv_character;
//char transmit_ch=' ';

//char send_str[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"\r\n";
char send_str[]="AT\r\n";
//char send_str[]="AT+CWMODE=1\r\n";
volatile uint8_t len;
volatile uint8_t i=0;



void uart0_init(void)
{
		/* set UART0 control and status registers (UCSR0x) */
		//UCSR0A is at its default values;
		UBRR0H = 0; //Baud rate registers
		//UBRR0L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UBRR0L = 8;
		UCSR0A = 0x00;
		UCSR0B = (1 << RXEN0)|(1<<RXCIE0);//Transmitter and receiver being enabled
		UCSR0C = 0x06;//Transmission size = 8 bit; asynchronous transmission

}

void uart2_init(void)
{
		UBRR2H = 0; //Baud rate registers
		//UBRR1L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UBRR2L = 8; 
		//UBRR0L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UCSR2A = 0x00;
		UCSR2B = (1 << TXEN2)|(1<<TXCIE2)|(1<<UDRIE2);//Enabling Interupts
		UCSR2C = 0x06;//Transmission size = 8 bit; asynchronous transmission
}

ISR(USART0_RX_vect)
{
	recv_character = UDR0;
	UDR2 = recv_character;
}

int main(void)
{	
		//DDRC = 0xFF;//PORTC for output
		//DDRA = 0xFF;//PORTA for output
		//DDRL = 0xFF;//PORTL for output
  //len=strlen(send_str);
	uart2_init();
	uart0_init();
	sei();
	while(1);
  return 0;
}
//PORT A= 22-29; PORT C = 37-30; PORTL= 42-49
//TX0: 1
//RX1: 19

 //UDRE Flag is to be reset manually whereas TXC flag gets cleared automatically in case of interrupt transmission