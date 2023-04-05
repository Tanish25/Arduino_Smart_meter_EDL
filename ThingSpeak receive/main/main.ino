#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ThingSpeak.h>

#define DEFAULT_BUFFER_SIZE 160
#define API_KEY "JGMS30H5A71UCKZ1"
#define SSID "Pixel-5"
#define PASSWORD "cabhinav"

volatile char rx_buffer[50];
volatile uint8_t rx_index = 0;

ISR(USART1_RX_vect)
{
  char received_byte = UDR1;
  rx_buffer[rx_index++] = received_byte;
}

void uart1_init()
{
  UBRR1 = 8;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
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

void wifi_connect(void)
{
  char cmd[64];

  // Construct Wi-Fi connection command with SSID and password
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWORD);

  char response_buffer[50];
// Send AT to ESP
sendAT:
  uart_send_string("AT\r\n");
  _delay_ms(1000);
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;

  // Set Wi-Fi mode to station mode
  if (strstr(response_buffer, "OK"))
  {
  sendCWMODE:
    uart_send_string("AT+CWMODE=1\r\n");
  }
  else
    goto sendAT;

  _delay_ms(500);
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;

  // Connect to Wi-Fi network with SSID and password
  if (strstr(response_buffer, "OK"))
  {
  sendCIPMUX:
    uart_send_string(cmd);
  }
  else
    goto sendCWMODE;

  _delay_ms(10000);
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;

  // Connect to Wi-Fi network with SSID and password
  if (strstr(response_buffer, "WIFI"))
  {
    uart_send_string("AT+CIPMUX=1\r\n");
  }
  else
    goto sendCIPMUX;

}

void Send_to_thingspeak( uint16_t Voltage, uint16_t Current, uint16_t Power )
{
  char Get_string[128];
  char len_string[128];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/update?api_key=%s&field1=%d&field2=%d&field3=%d\r\n",API_KEY,Voltage,Current,Power);
  len=strlen(Get_string);
  
  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);
  
  sprintf(len_string,"AT+CIPSEND=4,%d\r\n",len);
  uart_send_string(len_string);
  _delay_ms(1000);

  uart_send_string(Get_string);  
  _delay_ms(1000);

  uart_send_string("AT+CIPCLOSE=5\r\n");  
  _delay_ms(4000);
  _delay_ms(4000);
  _delay_ms(4000);
  _delay_ms(2500);

}


void Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4.json?results=1");
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

int main()
{

  sei();
  uart1_init();

  // Connect to Wi-Fi
  wifi_connect();
  Send_to_thingspeak(5,55,55);

  cli();
  return 0;

  // char response_buffer[50];
  //   uart_send_string("AT\r\n");
  //   _delay_ms(500);
  //   strcpy(response_buffer, rx_buffer);
  //   rx_index = 0;

  //   if(strstr(response_buffer,"OK"))
  //   {
  //       uart_send_string("Done_out");
  //   }
  //   else
  //       uart_send_string("not received_out");

  // memset(received_string, 0, sizeof(received_string));  buuffer clear
}
