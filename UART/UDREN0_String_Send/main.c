#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define BAUDRATE  115200L
uint8_t recv_byte;
char recv_str[];
char recv_character;
		


void uart_init(void)
{
		

		
		/* set UART0 control and status registers (UCSR0x) */
		//UCSR0A is at its default values;
		UBRR0H = 0; //Baud rate registers
		//UBRR0L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UBRR0L = 8;
		UCSR0A = 0x00;
		UCSR0B = (1 << TXEN0)|(1<<RXEN0);//Transmitter is being enabled
		UCSR0C = 0x06;//Transmission size = 8 bit; asynchronous transmission

		/* set UART1 control and status registers (UCSR1x) */
		//UCSR0A is at its default values;
		

		UBRR2H = 0; //Baud rate registers
		//UBRR1L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UBRR2L = 8; 
		//UBRR0L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UCSR2A = 0x00;
		UCSR2B = (1 << TXEN2);//Receiver is being enabled
		UCSR2C = 0x06;//Transmission size = 8 bit; asynchronous transmission
}

void uart_send(char transmit_ch)
{

	//PORTL = 0x00;
	//UCSR0A &= 0xBF;	
    UDR0 = transmit_ch;
    //while((UCSR0A & 0x20) ^ 0x20);
    //PORTL=0xff;

    while(!(UCSR0A & (1<<UDRE0)));
    //_delay_us(10);

}

void uart_recv(void)
{
	while(1)
	{
		while(!(UCSR0A & (1<<RXC0))); //wait until character is received
		recv_byte = UDR0;
		PORTL=0XFF;
		//_delay_us(182);
		UDR2 = recv_byte;
		while(!(UCSR2A & (1<<UDRE2)));
	//recv_character = (char) (recv_byte);
	//strncat(recv_str, &recv_character, 1);
	}

}

int main(void)
{

	uart_init();
	//char send_str[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"";
	//char send_str[]="AT\r\n";
	char send_str[]="AT+CWMODE=1\r\n";
	uint8_t len=strlen(send_str);
	uint8_t i=0;

		
	while(i<len)
	{
		uart_send(send_str[i++]);
	}
	i=0;
	uart_recv();

    return 0;
}
//PORT A= 22-29; PORT C = 37-30; PORTL= 42-49
//TX0: 1
//RX1: 19
