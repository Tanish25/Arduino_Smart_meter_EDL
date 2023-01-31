#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

volatile char recv_character;
//char transmit_ch = ' ';
bool flag=false;
//char send_str[]="AT+CWJAP=\"AndroidAP\",\"tan12345\"\r\n";
char send_str[] = "AT+CWMODE\r\n";
char recv_str[100];
//char send_str[]="AT+CWMODE=1\r\n";
volatile uint8_t len;
volatile uint8_t i = 0;
volatile uint8_t j;

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
 
    // configure the second UART module for receiving data
  UBRR2 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << RXEN2)| (1 << RXCIE2);
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


//*void send_strings(char,uint8_t,uint8_t)
{
   if (i < len) 
    { 
    UDR1 = send_str[i++];
    UCSR1B |= (1 << UDRIE1); 
   }
}*/
ISR(USART1_UDRE_vect)
 {
  //lenr=len_calc(send_str);

  if (i < len) 
  	{ 
  	UDR1 = send_str[i++];
  	UCSR1B |= (1 << UDRIE1); 
   }
}

ISR(USART2_RX_vect) {
  cli(); 
  recv_character = UDR2;    // read the received data
  UCSR3B |= (1 << UDRIE3);  // enable the data register empty interrupt for the second UART module
  if(UDRIE2==0)
    flag=true;
  //recv_str[j++]=recv_character;
  sei();
  UDR3=recv_character;
  
}

ISR(USART3_UDRE_vect)
 {


  /*cli(); 
  //lenrc=len_calc(recv_character);
    
  
   sei();*/
   return 0;
}


int main(void) {

  uart1_init();
  uart2_init();
  uart3_init();
  len=strlen(send_str);
  sei();  // enable global interrupts
   if(flag){
        UCSR3B |= (1 << UDRIE3); 
        flag = false;
    }
  while (1);
  return 0;
}
