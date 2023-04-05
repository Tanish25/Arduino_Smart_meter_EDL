
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEFAULT_BUFFER_SIZE 160
#define API_KEY "JGMS30H5A71UCKZ1"
#define SSID "Pixel-5"
#define PASSWORD "cabhinav"


volatile char received_string[50];
volatile uint8_t string_index = 0;




ISR(USART1_RXC_vect)
{
    char received_char = UDR1;
    
    if (received_char == '\r') // End of string detected
    {
        received_string[string_index] = '\0'; // Add null terminator
        string_index = 0; // Reset string index
    }
    else // Add character to received string
    {
        received_string[string_index] = received_char;
        string_index++;
    }
}



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

void Read_from_thingspeak(void) {
  char Get_string[128];
  char len_string[128];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2079434/fields/1.json?api_key=OWD73ANWRQ40ORA8&results=2");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(1000);

  uart_send_string(Get_string);
  _delay_ms(1000);

  uart_send_string("AT+CIPCLOSE=5\r\n");
  _delay_ms(4000);
}



int main() {

  uart1_init();

  // Connect to Wi-Fi
  //wifi_connect();

  //memset(received_string, 0, sizeof(received_string));  buuffer clear
  uart_send_string("AT\r\n");
  while(1){
    if(strstr(received_string,"AT"))
    {
        uart_send_string("Done");
    }
    else
        break;
  }
  

  
  

    




}
