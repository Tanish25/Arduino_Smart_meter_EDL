#include <stdio.h>
#include <string.h>
#include <avr/io.h>


char* ssid = "Pixel-5";
char* password = "cabhinav";

void uart1_init(void) {
  // configure the first UART module for receiving data
  UBRR1 = 8;                                                              // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

// Function to send a string over USART
void sendString(char* str) {
  while (*str != '\0') {
    while (!(UCSR1A & (1 << UDRE1)))
      ;           // wait for buffer to be empty
    UDR1 = *str;  // send character
    str++;
  }
}

// Function to read a response from ESP8266
void readResponse(char* response) {
  uint8_t index = 0;
  char receivedChar;
  do {
    while (!(UCSR1A & (1 << RXC1)))
      ;                   // wait for data to be received
    receivedChar = UDR1;  // read received data
    response[index++] = receivedChar;
  } while (receivedChar != '\n' && index < 255);  // assume response ends with \n
  response[index] = '\0';                         // add null terminator
}

// Function to send an AT command to ESP8266 and wait for response
void sendATCommand(char* command) {
  char response[256];  // assuming max response size is 255 characters
  sendString(command);
 
  // process the response as needed
}

// Function to connect to Wi-Fi network using ESP8266
void connectToWifi(char* ssid, char* password) {
char *string[]={"AT+RST\r\n","AT+CWMODE=1\r\n"}
for(uint8_t i=0;i<8;i++)
{
  sendATCommand(string[i]);
  readResponse(response);
  if(!(response[0]=='A'&response[1]=='T'))
    i--;
}

}

int main() {

  uart1_init();
  connectToWifi(ssid, password);

  while (1) {
    // Do other stuff here
  }

  return 0;
}
