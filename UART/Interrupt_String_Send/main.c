
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

void uart1_init(void) {
  // configure the first UART module for receiving data
  UBRR1 = 8;                                                                             // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1) | (1 << UDRIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void uart2_init(void) {
  // configure the second UART module for transmitting data
  UBRR2 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << TXEN2);  // enable transmitter
}

ISR(USART1_UDRE_vect)
 {
  //lenr=len_calc(send_str);

  if (i < len) 
  	{ 
  	UDR1 = send_str[i++];
  	UCSR1B |= (1 << UDRIE1); 
   }
}

ISR(USART1_RX_vect) {
  recv_character = UDR1;    // read the received data
  UCSR2B |= (1 << UDRIE2);  // enable the data register empty interrupt for the second UART module
}

ISR(USART2_UDRE_vect) {
  UDR2 = recv_character;     // send the received data through the second UART module
  UCSR2B &= ~(1 << UDRIE2);  // disable the data register empty interrupt for the second UART module
}

int main(void) {

  uart1_init();
  uart2_init();
  len=strlen(send_str);
  sei();  // enable global interrupts

  while (1);
  return 0;
}


/*


#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#define BAUDRATE  115200L
uint8_t recv_byte;
char recv_str[]="";
char recv_character=' ';
char transmit_ch=' ';

//char send_str[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"\r\n";
char send_str[]="AT\r\n";
//char send_str[]="AT+CWMODE=1\r\n";
volatile uint8_t len;
volatile uint8_t i=0;



void uart0_init(void)
{
		

		
		
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

//(for transmitting part):
ISR(USART0_UDRE_vect)
{
	//lenr=len_calc(send_str);
	
  	if(i<len)
  		{UDR0 = send_str[i++];}
  	

  		
  //Isnt it polling?
}

ISR(USART2_UDRE_vect)
{
  UDR2 = recv_character;
}


ISR(USART0_RX_vect)
{
	//i=0;
	recv_character = UDR0;
	//UDR2 = recv_byte;
	//See if there's a flashing issue if TX and RX are connected
	//_delay_us(182);
	//recv_character = (char) (recv_byte);
	//strncat(recv_str, &recv_character, 1);
}

int main(void)
{
	
    len=strlen(send_str);
	uart2_init();
	uart0_init();
	

	sei();
	while(1);
    return 0;
}
//PORT A= 22-29; PORT C = 37-30; PORTL= 42-49
//TX0: 1
//RX1: 19

//Interrupt method(for receiving part):

//check if global interrupts are enabled

 //UDRE Flag is to be reset manually whereas TXC flag gets cleared automatically in case of interrupt transmission


*/