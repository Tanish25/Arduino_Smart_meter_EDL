#include <stdio.h>
#include <string.h>
#include <avr/io.h>

#define RESPONSE_BUFFER_SIZE 256

char ssid[] = "starship";
char password[] = "cabhinav";

void uart_init(void) {
  // configure the UART module for receiving data
  UBRR1 = 8; // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1); // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06; // Transmission size = 8 bit; asynchronous transmission
}

// Function to send a string over USART
void uart_send_string(const char* str) {
  for (const char* c = str; *c != '\0'; c++) {
    while (!(UCSR1A & (1 << UDRE1)))
      ; // wait for buffer to be empty
    UDR1 = *c; // send character
  }
}

// Function to read a response from ESP8266
void uart_read_response(char* response) {
  uint8_t index = 0;
  char received_char;
  do {
    while (!(UCSR1A & (1 << RXC1)))
      ; // wait for data to be received
    received_char = UDR1; // read received data
    response[index++] = received_char;
  } while (received_char != '\n' && index < RESPONSE_BUFFER_SIZE - 1); // assume response ends with \n
  response[index] = '\0'; // add null terminator
}

// Function to send an AT command to ESP8266 and wait for response
void send_at_command(char* command, char* response) {
  uart_send_string(command);
  uart_read_response(response);
  // process the response as needed
}

// Function to connect to Wi-Fi network using ESP8266
void wifi_connect(char* ssid, char* password) {
  char response[RESPONSE_BUFFER_SIZE];
  char cmd[64];
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);  // construct command string
  const char* commands[] = {
    cmd,
    "AT+CWMODE=1\r\n",
    // add more AT commands as needed
  };
  for (size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
    send_at_command((char*)commands[i], response);
    _delay_ms(4000);
    // check response and retry if needed
    if(!(response[0]=='A'))
      i--;


  }
  // send command to connect to Wi-Fi network
}

int main() {
  uart_init();
  wifi_connect(ssid, password);
  while (1) {
    // Do other stuff here
  }
  return 0;
}