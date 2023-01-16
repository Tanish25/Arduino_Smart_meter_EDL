#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define BAUDRATE  115200L
uint8_t recv_byte;
char recv_str[]="";
char recv_character=' ';
char transmit_ch=' ';

char send_str[]="AT";
//uint8_t len=strlen(send_str);
uint8_t i=0;
uint8_t len=2;


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
/*
void uart_send(char transmit_ch)
{
	
	
	    UDR0 = transmit_ch;
	    while(!(UCSR0A & (1<<TXC0)));
	    //_delay_us(10);
	    //regarding delay= mostly we observe it's not required
	

}
*/
/*
void uart_recv(void)
{
	while(1)
	{
	while(!(UCSR0A & (1<<RXC0))); //wait until character is received
	recv_byte = UDR0;
	//See if there's a flashing issue if TX and RX are connected
	//_delay_us(182);
	UDR2= recv_byte;// to transmit the received character using another UART module
	while(!(UCSR2A & (1<<TXC2)));
	recv_character = (char) (recv_byte);
	strncat(recv_str, &recv_character, 1);
	}

}
*/
/*
void uart_send_default(void)// this function is to send NL and CR characters for every string
{
	UDR0 = 0x0D;
	while(!(UCSR0A & (1<<TXC0)));
	_delay_us(182);//delay configured such that processing time is to be accounted for
	//this delay is equivalent to a stop bit(1)
	UDR0 = 0x0A;
	while(!(UCSR0A & (1<<TXC0)));
	
}
*/
//(for transmitting part):
ISR(USART0_UDRE_vect)
{
	while(i<len)
	{
  		UDR0 = send_str[i++];
	}
  //Isnt it polling?
}
ISR(USART2_UDRE_vect)
{
  UDR2 = recv_byte;
}

ISR(USART0_RXC_vect)
{
	recv_byte = UDR0;
	//See if there's a flashing issue if TX and RX are connected
	//_delay_us(182);
	recv_character = (char) (recv_byte);
	strncat(recv_str, &recv_character, 1);
}

int main(void)
{
	
	
		//DDRC = 0xFF;//PORTC for output
		//DDRA = 0xFF;//PORTA for output
		//DDRL = 0xFF;//PORTL for output

	DDRE=0x02; // SET PE1 / TX0 as output
	DDRD=0x00;// SET PD as input (we need PD2 = RX1)

	uart2_init();
	uart0_init();
	//UCSR1A &= 0x7F;

	
	
		//uart_send('U');
		//_delay_us(1000);

		//Wait for char on RX1
		
		//uart_send(recv_data);
		
	/*while(i<len)
	{
		//uart_send(send_str[i++]);//ISR
		transmit_ch=send_str[i++];
		//Using Tx flag to see which character next? Isnt it polling?
	}*/
	
	//uart_send_default();
	transmit_ch=0x0D;
	transmit_ch=0x0A;
	
	
	   
		//_delay_us(250);
	//uart_recv();
		//uart_send('U');

	__enable_interrupt();
	while(1);
    return 0;
}
//PORT A= 22-29; PORT C = 37-30; PORTL= 42-49
//TX0: 1
//RX1: 19

//Interrupt method(for receiving part):
//Check the interrupt flags(bits) involved and enable them accordingly
//Change ISR functions accordingly






//check if global interrupts are enabled

 //UDRE Flag is to be reset manually whereas TXC flag gets cleared automatically in case of interrupt transmission