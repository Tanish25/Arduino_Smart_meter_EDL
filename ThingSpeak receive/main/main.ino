#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "usart.h"

#define DEFAULT_BUFFER_SIZE 160
#define API_KEY "JGMS30H5A71UCKZ1"
#define SSID "Pixel-5"
#define PASSWORD "cabhinav"
#define BUFF_SIZE   25
char rx_buffer;
volatile uint8_t rx_index = 0;


void relay_init(void){

    DDRC = 0x01; 
    PORTC = 0x01;
}

void relay_toggle(uint8_t status){

if(status==1)
PORTC = 0x01;
else if(status==0)
PORTC = 0x00;
}


void uart_send_string(char *str)
{
  while (*str != '\0')
  {
    // Wait for the transmit buffer to be empty
    while (!(UCSR1A & (1 << UDRE1)))
      ;

    // Send the next character
    UDR1 = *str;

    // Move to the next character in the string
    str++;
  }
}



char* Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  char response_buffer[50];
  sei();
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4/last.txt\r\n");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(100);

  uart_send_string(Get_string);
  _delay_ms(3000);
  cli();
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;


  uart_send_string("AT+CIPCLOSE=5\r\n");

  return response_buffer;
}



int main(void)
{

	uart0_init(BAUD_CALC(115200)); 
	uart1_init(BAUD_CALC(115200));
	
	stdout = &uart0_io; // attach uart stream to stdout & stdin
	stdin = &uart0_io; // uart0_in and uart0_out are only available if NO_USART_RX or NO_USART_TX is defined
	
	sei(); // enable interrupts
	char buffer;
	uart1_puts("AT\r\n"); 
  while(1){
	scanf("%d", rx_buffer);
  printf(rx_buffer); }
	while(1);
}
