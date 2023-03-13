#include <stdio.h>
#include <string.h>
#include <avr/io.h>

#define RESPONSE_BUFFER_SIZE 256
volatile char response[RESPONSE_BUFFER_SIZE];
volatile uint8_t index;

char* ssid = "Pixel-5";
char* password = "cabhinav";

void uart1_init(void) {
  // configure the first UART module for receiving data
  UBRR1 = 8;                                                              // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1);  // enable receiver and receive complete interrupt
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

void uart2_init(void) {
  // configure the first UART module for receiving data
  UBRR2 = 8;                                                              // for 115200 baud rate with F_CPU = 16000000
  UCSR2B |= (1 << TXEN2) | (1 << RXEN2) | (1 << RXCIE2) | (1 << TXCIE2);  // enable receiver and receive complete interrupt
  UCSR2A = 0x00;
  UCSR2C = 0x06;  //Transmission size = 8 bit; asynchronous transmission
}

// Function to send a string over USART
void uart_send_string(char* str) {
  while (*str != '\0') {
    while (!(UCSR1A & (1 << UDRE1)))
      ;           // wait for buffer to be empty
    UDR1 = *str;  // send character
    str++;
  }
}

ISR(USART1_RX_vect) {
  response[index] = (uint8_t)UDR1;
  UDR2 = response[index];
  index++;    // read the received data
}



// Function to send an AT command to ESP8266 and wait for response
void send_at_command(char* command) {
  memset(response, 0, RESPONSE_BUFFER_SIZE);
  index=0;
  uart_send_string(command);
  // process the response as needed
}

// Function to connect to Wi-Fi network using ESP8266
void wifi_connect(char* ssid, char* password) {
  char cmd[64];
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);  // construct command string
  const char* commands[] = {
    "AT\r\n",
    "AT+CWMODE=1\r\n",
    cmd,
  };
  for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    send_at_command((char*)commands[i]);
    _delay_ms(2000);
    // check response and retry if needed
    if (!(response[0] == 'A' & response[1] == 'T')) {
      i--;
    }
  }



  // send command to connect to Wi-Fi network
}


void connect_to_thingspeak(void) {
}

int main() {

  uart1_init();
  uart2_init();
  wifi_connect(ssid, password);

  while (1) {
    // Do other stuff here
  }

  return 0;
}
