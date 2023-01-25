#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define BAUDRATE  115200L
//uint8_t recv_byte;
char recv_str[100];
char recv_character;
volatile uint8_t len;
volatile uint8_t i;
volatile uint8_t j;

void uart0_init(void)
{
		/* set UART0 control and status registers (UCSR0x) */
		//UCSR0A is at its default values;
		UBRR0H = 0; //Baud rate registers
		//UBRR0L = (uint8_t) (F_CPU / 16/ BAUDRATE)-1;
		UBRR0L = 8;
		UCSR0A = 0x00;
		UCSR0B = (1 << TXEN0)|(1 << RXEN0)|(1<<RXCIE0)|(1<<TXCIE0)|(1<<UDRIE0);//Transmitter and receiver being enabled
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

void string_handle(char recv_str[])
{
	len=strlen(recv_str);
	i=0;

	while(i<len)
	{
		uart_send(recv_str);
	}
	

	uart_recv();
}
void uart_send(char transmit_ch)
{	
    UDR0 = transmit_ch;
    while(!(UCSR0A & (1<<UDRE0))); 
}

void uart_recv(void)
{
	while(1)
	{
		while(!(UCSR0A & (1<<RXC0))); //wait until character is received
		recv_character = UDR0;
		PORTL=0XFF;
		recv_str[j++]=recv_character;
		//_delay_us(182);
		UDR2 = recv_character;
		while(!(UCSR2A & (1<<UDRE2)));
	}

}

int main(void)
{
	
	DDRL = 0xFF;//PORTL for output
	PORTL = 0x00;
	uart_init();

	char send_str1[]="AT\r\n";
	char send_str2[]="AT+RST\r\n";
	char send_str3[]="AT+CWMODE=1\r\n";
	char send_str4[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"\r\n";
/*	char send_str5[]="AT+CIPMUX=0"

    char send_str6[]="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80"

	char send_str7[]="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80"

	char send_str8[]="AT+CIPSEND=51"
	char send_str9[]="GET /update?api_key=XXXXXXXXXXXXXXXX&field1=255"
	char send_str10[]="AT+CIPCLOSE"*/
	string_handle(str1);
	/*
	volatile uint8_t len=strlen(send_str1);
	volatile uint8_t i=0;

	while(i<len)
	{
		uart_send(send_str1[i++]);
	}
	i=0;
	*/
	uart_recv();

    return 0;
}
//PORT A= 22-29; PORT C = 37-30; PORTL= 42-49
//TX0: 1
//RX1: 19
