#include <stdio.h>
#include <string.h>
#include <avr/io.h>

#define DEFAULT_BUFFER_SIZE   160

char ssid[] = "starship";
char password[] = "cabhinav";
char RESPONSE_BUFFER[DEFAULT_BUFFER_SIZE];
volatile int16_t Counter = 0, pointer = 0;

void UART_init(void) {
  UBRR1 = 8;
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1);
  UCSR1A = 0x00;
  UCSR1C = 0x06; 
}

void USART_SendString(char* str) {
  pointer = 0;
  while (str[pointer] != '\0') {
    while (!(UCSR1A & (1 << UDRE1))); // wait for buffer to be empty
    UDR1 = str[pointer]; 
    pointer++;
  }
}

void ESP8266_Clear()
{
  memset(RESPONSE_BUFFER, 0, DEFAULT_BUFFER_SIZE);
  Counter = 0;
  pointer = 0;
}

int ESP8266_Begin()
{
  ESP8266_Clear();
  USART_SendString("AT\r\n");
  _delay_ms(1000);
  USART_SendString(RESPONSE_BUFFER);
  if (strstr(RESPONSE_BUFFER, "OK") != NULL) {
    return 1;
  } else {
    return 0;
  }
}

void ESP8266_WIFIMode()
	
  ESP8266_Clear();
  USART_SendString("AT+CWMODE=1\r\n");
}

int ESP8266_JoinAccessPoint()
{
  ESP8266_Clear();
  char cmd[64];
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
  USART_SendString(cmd);
  if(strstr(RESPONSE_BUFFER, "WIFI CONNECTED") != NULL) {
    return 1;
  } else {
    return 0;
  }
}

ISR (USART1_RX_vect)
{
  RESPONSE_BUFFER[Counter] = UDR1;
  Counter++;
  if (Counter == DEFAULT_BUFFER_SIZE) {
    Counter = 0; 
    pointer = 0;
  }
}

int main(void)
{
  UART_init();
  sei();
  if (ESP8266_Begin()) {
    ESP8266_WIFIMode();
    if (ESP8266_JoinAccessPoint()) {
      // Connected successfully
    } else {
      // Failed to connect to WiFi
    }
  } else {
    // Failed to communicate with ESP8266
  }
}
