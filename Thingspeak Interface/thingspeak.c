#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>



void uart1_init(void) {
  // configure the first UART module for receiving data
  UBRR1 = 8;                                                                             // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << UDRIE1) | (1 << RXEN1) | (1 << RXCIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00; 
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void wifi_init(void){

}

void Send_Buffer(char ptr[]){
  volatile uint8_t current_char = 0; 
  char buffer[100]=ptr;
  char data[];
  if (buffer[current_char] != '\0')
    data = buffer[current_char++]; // send the current character and increment the index
  void uart1_init();
  sei();
}

ISR(USART1_UDRE_vect) {
  UDR1 = data; // send the current character and increment the index
}

int main(void) {

    while (1) {
        // do other tasks here
    }

    return 0;
}