#include <stdio.h>
#include <string.h>
#include <avr/io.h>

#define DEFAULT_BUFFER_SIZE 160
#define API_KEY "JGMS30H5A71UCKZ1"
#define SSID "AndroidAP"
#define PASSWORD "tan12345"

// Declare variables as static or local whenever possible
static volatile uint8_t index;
static char RESPONSE_BUFFER[DEFAULT_BUFFER_SIZE];
static volatile int16_t Counter = 0, pointer = 0;
volatile uint8_t rx_complete = 0;

//remove this
volatile uint16_t Volts = 250;
volatile uint16_t curr = 250;
volatile uint16_t poww=0;



void uart1_init(void) {
  // Set baud rate
  UBRR1 = 8;

  // Enable transmitter and receiver, and enable receive and transmit complete interrupts
  UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1);

  // Set asynchronous mode, no parity, 1 stop bit, 8 data bits
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void uart_send_string(char* str) {
  while (*str != '\0') {
    // Wait for the transmit buffer to be empty
    while (!(UCSR1A & (1 << UDRE1)))
      ;

    // Send the next character
    UDR1 = *str;

    // Move to the next character in the string
    str++;
  }
}

void wifi_connect(void) {
  char cmd[64];

  // Construct Wi-Fi connection command with SSID and password
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWORD);

  // Send AT command
  uart_send_string("AT\r\n");
  _delay_ms(1000);
  // uart_send_string(RESPONSE_BUFFER);

  // // Set Wi-Fi mode to station mode
  uart_send_string("AT+CWMODE=1\r\n");
  _delay_ms(1000);


  // // Connect to Wi-Fi network with SSID and password
  uart_send_string(cmd);
  _delay_ms(10000);

  uart_send_string("AT+CIPMUX=1\r\n");
  _delay_ms(1000);
}

void Send_to_thingspeak(uint16_t Voltage, uint16_t Current, uint16_t Power) {
  char Get_string[128];
  char len_string[128];
  uint8_t len;

  
  sprintf(Get_string, "GET https://api.thingspeak.com/update?api_key=%s&field1=%d&field2=%d&field3=%u\r\n", API_KEY, Voltage, Current, Power);
  len = strlen(Get_string);


  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(1000);

  uart_send_string(Get_string);
  _delay_ms(1000);

  uart_send_string("AT+CIPCLOSE\r\n");
  _delay_ms(15000);
}


int main(void) {
  // Initialize UART
  uart1_init();

  // Connect to Wi-Fi
  wifi_connect();

  poww= Volts*curr;

  Send_to_thingspeak(Volts, curr, poww);


  return 0;
}