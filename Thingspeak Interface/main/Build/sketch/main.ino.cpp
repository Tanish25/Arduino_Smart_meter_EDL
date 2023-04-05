#include <Arduino.h>
#line 1 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
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

#line 23 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
void uart1_init();
#line 30 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
void relay_init(void);
#line 37 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
void relay_toggle(uint8_t status);
#line 45 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
uint8_t adc_read(void);
#line 79 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
void uart_send_string(char *str);
#line 95 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
void wifi_connect(void);
#line 145 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
void Send_to_thingspeak( uint16_t Voltage, uint16_t Current, uint16_t Power );
#line 172 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
char Read_from_thingspeak(void);
#line 224 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
float calc_rms_voltage();
#line 238 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
int main();
#line 17 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
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

void relay_init(void){

    DDRC = 0xFF; 
    PORTC = 0xFF;
    
}

void relay_toggle(uint8_t status){

if(status==1)
PORTC = 0x01;
else if(status==0)
PORTC = 0x00;
}

uint8_t adc_read(void)
{
// Select differential input mode
ADMUX |= (1 << MUX3) | (1 << MUX2) | (1 << MUX1); // Select ADC1 (A0) as positive input
ADCSRB |= (1 << MUX5); // Select ADC0 (A1) as negative input

// Set ADC reference voltage
ADMUX |= (1 << REFS1) | (1 << REFS0); // Use internal 5V as reference voltage

ADMUX |= (1 << ADLAR);    // left aligned, better for 8 bit resolution

// Set ADC clock frequency
ADCSRA |= (1 << ADPS2) | (1 << ADPS0); // Set prescaler to 32 for 5 kHz sampling frequency

// Enable ADC
ADCSRA |= (1 << ADEN);

// Start ADC conversion
ADCSRA |= (1 << ADSC);

// Wait for conversion to complete
while (ADCSRA & (1 << ADSC));

// Get ADC value
uint8_t adc_value = ADC;

// Calculate differential value
uint8_t differential_value = (uint8_t)adc_value - 512;

// Return differential value
return differential_value;
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

  _delay_ms(2000);
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;

  // Connect to Wi-Fi network with SSID and password
  while (strstr(response_buffer, "CONNECTED"))
  {
    uart_send_string("AT+CIPMUX=1\r\n");
     _delay_ms(2000);
  }

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


char Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  char response_buffer[50];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4/last.txt\r\n");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(1000);

  uart_send_string(Get_string);
  _delay_ms(3000);
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;


  uart_send_string("AT+CIPCLOSE=5\r\n");
  _delay_ms(1000);
  return response_buffer;
}

void wifi_connect(void)
{
    char cmd[64];

    // Construct Wi-Fi connection command with SSID and password
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWORD);

    // Send AT command
    uart_send_string("AT\r\n");
    _delay_ms(50);
    // uart_send_string(RESPONSE_BUFFER);

    // // Set Wi-Fi mode to station mode
     uart_send_string("AT+CWMODE=1\r\n");
     _delay_ms(50);


    // // Connect to Wi-Fi network with SSID and password
     uart_send_string(cmd);
     _delay_ms(8000);

     uart_send_string("AT+CIPMUX=1\r\n");
     _delay_ms(50);
}

float calc_rms_voltage()
{
    uint32_t sum = 0;
    float rms_voltage = 0;
    uint16_t adc_value;
    for (int i = 0; i < 256; i++) {
        adc_value = adc_read();
        sum += adc_value * adc_value;
        _delay_us(10);
    }
    rms_voltage = sqrt(sum / 256.0);
    return rms_voltage;
}

int main()
{

  sei();
  uart1_init();
  relay_init();

  // Connect to Wi-Fi
  wifi_connect();
  while(1){
  //Send_to_thingspeak(calc_rms_voltage(),0,0);
  if (Read_from_thingspeak()=='1')
    relay_toggle(1);
  else if (Read_from_thingspeak() == '0')
    relay_toggle(0);
    }
  return 0;

}

