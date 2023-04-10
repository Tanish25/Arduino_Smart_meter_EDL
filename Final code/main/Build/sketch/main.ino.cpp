#include <Arduino.h>
#line 1 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
#include <avr/io.h>
#define F_CPU 16000000 // 16MHz
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Thingspeak Credentials
#define API_KEY "JGMS30H5A71UCKZ1"

///////////////////////////////////////////////////////
/**/      #define SSID "FD-06"                   //
/**/      #define PASSWORD "Rahul1312"                //
/**/      //const uint32_t Energy_limit_Wh = 100;        //
///////////////////////////////////////////////////////

// receiver variables
volatile uint8_t rx_index = 0;

// ADC Variables
volatile uint16_t ADC_Value;
volatile uint16_t ADC_Value_Sum_Voltage;
volatile uint16_t ADC_Value_Sum_Current;
volatile uint16_t ADC_Value_Avg_Voltage;
volatile uint16_t ADC_Value_Avg_Current;
volatile uint16_t Overall_Sum_Voltage = 0;
volatile uint16_t Overall_Sum_Current = 0;
volatile uint32_t Overall_Sum_Power = 0;

uint32_t Power[100];
uint16_t Voltage[100];
uint16_t Current[100];
volatile uint8_t pow_count = 0;
volatile uint8_t v_count = 0;
volatile uint8_t i_count = 0;

// Energy Paramters
volatile uint32_t Energy = 0;
volatile uint32_t Energyi = 0;
const uint32_t Energy_limit_Wh = 2;
uint32_t Energy_max = Energy_limit_Wh*16000;    //after conversion

volatile uint16_t max_val_voltage = 0;
volatile uint16_t max_val_current = 0;
volatile uint32_t max_val_power = 0;

// threshold values for comparing
const uint8_t threshold_voltage = 100;
const uint8_t threshold_current = 100;
volatile uint8_t relay_status;

//--------------------------------------------------------------------------
// Initialize ADC
//--------------------------------------------------------------------------
#line 56 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void adc_init();
#line 80 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void uart1_init();
#line 90 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void relay_init(void);
#line 129 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void uart_send_string(char *str);
#line 148 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void wifi_connect(void);
#line 175 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void Send_to_thingspeak(uint16_t Voltaget, uint16_t Currentt, uint16_t Powert, uint16_t relay_value, uint32_t Energyt, uint16_t Vpk, uint16_t Ipk, uint32_t Pwrpk);
#line 201 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
char Read_from_thingspeak(void);
#line 230 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void pwm_setup();
#line 256 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void ADC_read(void);
#line 403 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
int main();
#line 56 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Final code\\main\\main.ino"
void adc_init()
{
  ADCSRA |= (1 << ADEN);                                // Enable the ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz/128= 125 Khz
  ADCSRA |= (1 << ADIE);                                // Enable Interrupts

  PRR0 |= (0 << PRADC); // Power Reduction Register's PRADC bit must be disabled before starting the conversion

  DIDR0 = 0xff;
  DIDR1 = 0xff;

  ADMUX |= (1 << REFS0); // use external AREF as the reference
  ADMUX |= (0 << REFS1); // use external AREF as the reference
  ADMUX |= (1 << ADLAR); // left aligned, better for 8 bit resolution

  // Set the ADC auto trigger source to Timer/Counter1 Compare Match A
  ADCSRA |= (1 << ADATE);
  ADCSRB &= ~(1 << ADTS2) & ~(1 << ADTS1) & ~(1 << ADTS0);
  ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
}

//--------------------------------------------------------------------------
// Initialize UART Module 1
//--------------------------------------------------------------------------
void uart1_init()
{
  UBRR1 = 8;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

//--------------------------------------------------------------------------
// Initialize pin 37 for relay signal
//--------------------------------------------------------------------------
void relay_init(void)
{
  DDRC = 0x01;
  PORTC = 0x01;
  relay_status = 1;
}

//--------------------------------------------------------------------------
// Toggle relay function
//--------------------------------------------------------------------------
// void relay_toggle(uint8_t status)
// {
//   if (status == 1)
//   {
//     PORTC = 0x01;
//     relay_status = 1;
//   }

//   else if (status == 0)
//   {
//     PORTC = 0x00;
//     relay_status = 0;
//   }
// }

//--------------------------------------------------------------------------
// Function to detect threshold crossings
//--------------------------------------------------------------------------
// void threshold_detection(uint16_t ADC_live_value, uint16_t threshold)
// {
//   if (ADC_live_value > threshold)
//   {
//     relay_toggle(0);
//   }
// }

//--------------------------------------------------------------------------
// Function to send strings on TX1
//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
// Initialize ther Wifi Connection
//--------------------------------------------------------------------------
void wifi_connect(void)
{
  char cmd[64];

  // Construct Wi-Fi connection command with SSID and password
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWORD);

  // Send AT command
  uart_send_string("AT\r\n");
  _delay_ms(50);

  // Set Wi-Fi mode to station mode
  uart_send_string("AT+CWMODE=1\r\n");
  _delay_ms(50);

  // Connect to Wi-Fi network with SSID and password
  uart_send_string(cmd);
  _delay_ms(8000);

  // close the connection
  uart_send_string("AT+CIPMUX=1\r\n");
  _delay_ms(50);
}

//--------------------------------------------------------------------------
// Function to send data to Thingspeak
//--------------------------------------------------------------------------
void Send_to_thingspeak(uint16_t Voltaget, uint16_t Currentt, uint16_t Powert, uint16_t relay_value, uint32_t Energyt, uint16_t Vpk, uint16_t Ipk, uint32_t Pwrpk)
{
  char Get_string[300];
  char len_string[300];
  uint8_t len;
  // relay_value=1;
  sprintf(Get_string, "GET https://api.thingspeak.com/update?api_key=%s&field1=%d&field2=%d&field3=%d&field4=%d&field5=%d&field6=%d&field7=%d&field8=%d\r\n", API_KEY, Voltaget, Currentt, Powert, relay_value, Energyt, Vpk, Ipk, Pwrpk);
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(3000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(50);

  uart_send_string(Get_string);
  _delay_ms(50);

  uart_send_string("AT+CIPCLOSE=5\r\n");
  _delay_ms(1000);
}

//--------------------------------------------------------------------------
// Function to read data from Thingspeak
//--------------------------------------------------------------------------
char Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  char response_buffer[50];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4/last.txt");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(100);

  uart_send_string(Get_string);
  _delay_ms(2000);
  // strcpy(response_buffer, rx_buffer);
  // rx_index = 0;

  uart_send_string("AT+CIPCLOSE=5\r\n");

  return response_buffer;
}

//--------------------------------------------------------------------------
// Function to setup PWM for ADC
//--------------------------------------------------------------------------
void pwm_setup()
{
  // Set Timer/Counter1 to Fast PWM mode

  OCR0A = 0x7F; // 50 percent of 255=duty cycle//decimal=127
  TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);

  // Set the prescaler to 64- 1KHz
  TCCR0B |= (1 << CS00) | (1 << CS01);
  TCCR1A |= (1 << WGM11) | (1 << WGM10);
  TCCR1B |= (1 << WGM13) | (1 << WGM12);

  // Set the prescaler to 8
  TCCR1B |= (1 << CS11);

  // Set the TOP value to 0x3FFF
  ICR1 = 0x3FFF;

  // Set the duty cycle to 50%
  OCR1A = 0x1FFF;
  // frequency:1KHz
}

//--------------------------------------------------------------------------
// Function to read ADC values from A0 and A1 and update to thingspeak
//--------------------------------------------------------------------------
void ADC_read(void)
{
  // set counter to zero
  volatile uint16_t f = 0;

  while (1)
  {
    if (f % 4 < 2)
    {
      ADMUX &= ~(1 << MUX0); // Select ADC0 for conversion
    }
    else
    {
      ADMUX |= (1 << MUX0); // Select ADC1 for conversion
    }

    ADCSRA |= (1 << ADSC); // Start conversion

    while (ADCSRA & (1 << ADSC))
      ; // Wait for conversion to complete

    if (f % 4 < 2)
    {
      ADC_Value = 0;
      ADC_Value = ADCH;
      ADC_Value_Sum_Voltage += ADC_Value;
      if (f % 4 == 1)
      {
        ADC_Value_Avg_Voltage = ADC_Value_Sum_Voltage >> 1;
        ADC_Value_Sum_Voltage = 0;
        Voltage[v_count] = ADC_Value_Avg_Voltage;
        Overall_Sum_Voltage += Voltage[v_count];

        max_val_voltage = Voltage[0];
        // Peak voltage detection
        if (Voltage[v_count] >= max_val_voltage)
        {
          max_val_voltage = Voltage[v_count];
        }

        // threshold crossing detection
        // threshold_detection(Voltage[v_count], threshold_voltage);
        // if (Voltage[v_count] > 100)
        // {
        //   PORTC = 0x00;
        //   while(1);
        // }

        v_count++;
      }
    }

    else
    {
      ADC_Value = 0;
      ADC_Value = ADCH;
      ADC_Value_Sum_Current += ADC_Value;

      if (f % 4 == 3)
      {
        ADC_Value_Avg_Current = ADC_Value_Sum_Current >> 1;
        ADC_Value_Sum_Current = 0;
        Current[i_count] = ADC_Value_Avg_Current;
        Overall_Sum_Current += Current[i_count];

        max_val_current = Current[0];
        // Peak current detection
        if (Current[i_count] > max_val_current)
        {
          max_val_current = Current[i_count];
        }

        // threshold crossing detection
        // threshold_detection(Current[i_count], threshold_current);
        // if (Current[i_count] > 100)
        // {
        //   PORTC = 0x00;
        //   while(1);
        // }

        i_count++;
        ADC_Value_Avg_Current = ADC_Value_Avg_Current;
        ADC_Value_Avg_Voltage = ADC_Value_Avg_Voltage;
        Power[pow_count] = (uint32_t)ADC_Value_Avg_Current * (uint32_t)ADC_Value_Avg_Voltage;   //typecast to 16
        // if (Power[pow_count] > 1000)
        // {
        //   Power[pow_count]=400;
        // }

        Overall_Sum_Power += Power[pow_count];

        max_val_power = Power[0];
        if (Power[pow_count] > max_val_power)
        {
          max_val_power = Power[pow_count];
        }

        pow_count++;
      }
    }

    if (f == 400)
    {
      // Energy calculation:
      if (Overall_Sum_Power / 100 > 1000)
        {
          Overall_Sum_Power =50000;
        }
      Energyi = (Overall_Sum_Power / 100)*4;
      Energy = Energy + Energyi;
      if (Energy > Energy_max)
      {
        PORTC = 0x00;
        relay_status = 0;
        _delay_ms(15000); // to make sure relay=0 is shown
        Send_to_thingspeak(Overall_Sum_Voltage / 100, Overall_Sum_Current / 100, Overall_Sum_Power / 100, 0, Energy, max_val_voltage, max_val_current, max_val_power);
        while (1)
          ;
      }
      else
      {
        Send_to_thingspeak(Overall_Sum_Voltage / 100, Overall_Sum_Current / 100, Overall_Sum_Power / 100, 1, Energy, max_val_voltage, max_val_current, max_val_power);
      }

      // reset the arrays and values to zero
      Overall_Sum_Voltage = 0;
      Overall_Sum_Current = 0;
      Overall_Sum_Power = 0;
      for (int k = 0; k < 100; k++)
      {

        Current[k] = 0;
        Voltage[k] = 0;
        Power[k] = 0;
      }
    }
    f++;
    if (f == 401)
    {
      f = 0;
    }
  }
}

//-------------------------------------------------------------------------------------------------------
// Main function
//-------------------------------------------------------------------------------------------------------
int main()
{
  // DDRC=0x01;
  // PORTC = 0x01;
  

  // initialize uart for sending commandsds to ESP
  uart1_init();

  //  Initialize the Wi-Fi connection
  wifi_connect();

  // initialize PWM for ADC trigger
  pwm_setup();

  //  initialize the relay signal pin(pin.no=37)
  relay_init();

  // initalize ADC to  read values from A0 and A1
  adc_init();

  // Reads values from ADC and sends to thingspeak
  ADC_read();

  while (1)
    ;
  return 0;
}

// port L direction: output //pins- 49:42
// port C direction: output //pins- 37:30
// port A direction: output //pins- 22:29
