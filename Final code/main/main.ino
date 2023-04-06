#include <avr/io.h>
#define F_CPU 16000000 // 16MHz
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define ADC_COUNTS 512 // change according to bit precision used..
#define true 1
#define false 0

#define DEFAULT_BUFFER_SIZE 160
#define API_KEY "JGMS30H5A71UCKZ1"
#define SSID "AndroidAP"
#define PASSWORD "tan12345"

    // receiving buffer variables
    volatile char rx_buffer[50];
volatile uint8_t rx_index = 0;

volatile uint8_t channel = 1; // for channel
volatile uint16_t ADC_Value;
volatile uint16_t ADC_Value_Sum_Voltage;
volatile uint16_t ADC_Value_Sum_Current;
volatile uint16_t ADC_Value_Avg_Voltage;
volatile uint16_t ADC_Value_Avg_Current;
volatile uint16_t Average_Voltage = 0;
volatile uint16_t Average_Current = 0;
volatile uint16_t Average_Power = 0;
volatile uint16_t Voltage_Vpk = 0;
volatile uint16_t Current_Vpk = 0;
volatile uint16_t Power_Vpk = 0;
volatile uint16_t Overall_Sum_Voltage = 0;
volatile uint16_t Overall_Sum_Current = 0;
volatile uint32_t Overall_Sum_Power = 0;

volatile uint16_t test_Voltage_sum = 0;
volatile uint16_t test_Current_sum = 0;
volatile uint16_t test_Power_sum = 0;

uint32_t poww[100];
uint16_t Voltage[100];
uint16_t Current[100];
volatile uint8_t pow_count = 0;
volatile uint8_t v_count = 0;
volatile uint8_t i_count = 0;
volatile uint32_t f = 0;

// Declare variables as static or local whenever possible
static volatile uint8_t index;
// static char RESPONSE_BUFFER[DEFAULT_BUFFER_SIZE];
static volatile int16_t Counter = 0, pointer = 0;
volatile uint8_t rx_complete = 0;

// remove this
// volatile uint8_t Volts = 267;
// volatile uint8_t curr = 234;
// volatile uint16_t poww_test = 0;


volatile uint16_t max_val_voltage = 0;
volatile uint16_t max_val_current = 0;
volatile uint32_t max_val_power = 0;

uint8_t threshold_voltage = 0;
uint8_t threshold_current = 0;

int16_t sampleV;
int16_t lastSample;
int16_t startV;
uint16_t sqV;
int16_t sumV = 0;

//-------------------------------------------------------------------------------------------------------------------------
// Start
//-------------------------------------------------------------------------------------------------------------------------

// port L direction: output //pins- 49:42
// port C direction: output //pins- 37:30
// port A direction: output //pins- 22:29
/*
  ISR(ADC_vect)
  { }
*/

// ISR(USART1_RX_vect)
// {
//   char received_byte = UDR1;
//   rx_buffer[rx_index++] = received_byte;
// }

void adc_init()
{
  ADCSRA |= (1 << ADEN);                                // Enable the ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz/128= 125 Khz
  ADCSRA |= (1 << ADIE);                                // Enable Interrupts
  // ADCSRA |= (1 << ADATE);    // Set this bit to enable trigger source

  PRR0 |= (0 << PRADC); // Power Reduction Register's PRADC bit must be disabled before starting the conversion

  // ADCSRB |= (1 << ADTS0) | (1 << ADTS1);   // Triggered by Timer0 compare match A
  // ADCSRB |= (1 << ADTS2);   // Triggered by Timer0 Overflow

  DIDR0 = 0xff;
  DIDR1 = 0xff;

  ADMUX |= (1 << REFS0); // use external AREF as the reference
  ADMUX |= (0 << REFS1); // use external AREF as the reference
  ADMUX |= (1 << ADLAR); // left aligned, better for 8 bit resolution
  // ADMUX |= ((0 << MUX4)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3));
  // ADCSRA |= (1 << ADSC); //to start conversion

  // Set the ADC auto trigger source to Timer/Counter1 Compare Match A
  ADCSRA |= (1 << ADATE);
  ADCSRB &= ~(1 << ADTS2) & ~(1 << ADTS1) & ~(1 << ADTS0);
  ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
}

void uart1_init()
{
  UBRR1 = 8;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void relay_init(void)
{

  DDRC = 0x01;
  PORTC = 0x01;
}

void relay_toggle(uint8_t status)
{

  if (status == 1)
    PORTC = 0x01;
  else if (status == 0)
    PORTC = 0x00;
}

void uart_send_string(char *str)
{
  while (*str != '\0')
  {
    // Wait for the transmit buffer to be empty
    while (!(UCSR1A & (1 << UDRE1)));

    // Send the next character
    UDR1 = *str;

    // Move to the next character in the string
    str++;
  }
}

// void wifi_connect(void)
// {
//   char cmd[64];

//   // Construct Wi-Fi connection command with SSID and password
//   sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWORD);

//   char response_buffer[50];
// // Send AT to ESP
// sendAT:
//   uart_send_string("AT\r\n");
//   _delay_ms(1000);
//   strcpy(response_buffer, rx_buffer);
//   rx_index = 0;

//   // Set Wi-Fi mode to station mode
//   if (strstr(response_buffer, "OK"))
//   {
//   sendCWMODE:
//     uart_send_string("AT+CWMODE=1\r\n");
//   }
//   //else
//     //goto sendAT;

//   _delay_ms(500);
//   strcpy(response_buffer, rx_buffer);
//   rx_index = 0;

//   // Connect to Wi-Fi network with SSID and password
//   if (strstr(response_buffer, "OK"))
//   {
//   sendCIPMUX:
//     uart_send_string(cmd);
//   }
//   //else
//     //goto sendCWMODE;

//   _delay_ms(2000);
//   strcpy(response_buffer, rx_buffer);
//   rx_index = 0;

//   // Connect to Wi-Fi network with SSID and password
//   if (strstr(response_buffer, "WIFI"))
//   {
//     uart_send_string("AT+CIPMUX=1\r\n");
//   }
//   //else
//     //goto sendCIPMUX;

// }

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

void Send_to_thingspeak(uint16_t Voltage, uint16_t Current, uint16_t Power, uint16_t Vpk, uint16_t Ipk, uint32_t Pwrpk)
{
  char Get_string[150];
  char len_string[150];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/update?api_key=%s&field1=%d&field2=%d&field3=%d&field5=%d&field6=%d&field7=%d\r\n", API_KEY, Voltage, Current, Power,Vpk,Ipk,Pwrpk);
  len = strlen(Get_string);

  //uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  //_delay_ms(3000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(50);

  uart_send_string(Get_string);
  _delay_ms(50);

  //uart_send_string("AT+CIPCLOSE=5\r\n");
  //_delay_ms(1000);
  // _delay_ms(4000);
  // _delay_ms(4000);
  // _delay_ms(4000);
  // _delay_ms(2500);
}

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
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;

  uart_send_string("AT+CIPCLOSE=5\r\n");

  return response_buffer;
}

// use below line to calculate size of array
// size_t num_values = sizeof(values) / sizeof(values[0]);

// float calc_avg(uint16_t values[], uint8_t num_values)
// {
//   float sum = 0.0f;
//   for (uint8_t i = 0; i < num_values; i++)
//   {
//     sum += (float)values[i];
//   }
//   float avg = sum / (float)num_values;
//   float average_data = avg / 255.0f * 5.0f;
//   return average_data;
// }

// uint8_t find_Peak(uint16_t arr[], uint8_t len)
// {
//   uint8_t max_val = arr[0];
//   for (uint8_t i = 1; i < len; i++)
//   {
//     if (arr[i] > max_val)
//     {
//       max_val = arr[i];
//     }
//   }
//   return max_val;
// }

// uint8_t threshold_detection(uint16_t ADC_live_value, uint8_t threshold)
// {
//   if (ADC_live_value > threshold)
//     relay_toggle(0);
//   return 0; // call relay function to open the relay
// }

void pwm_setup()
{
  // Set Timer/Counter1 to Fast PWM mode

  OCR0A = 0x7F; // 50 percent of 255=duty cycle//decimal=127
  TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
  // Set the prescaler to 64- 1KHz
  TCCR0B |= (1 << CS00) | (1 << CS01);

  // TCCR0A = 0x83;
  // TCCR0B = 0x03;//prescaler = 64 for 1kHz
  // TCCR0B = 0x02;//prescaler = 8 for 8kHz

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

// void adc_read()
// {
//   while(1)
// {
//   uart_send_string(f); 
// Send_to_thingspeak(f,0,0,0,0,0); 
//  if (f % 4 < 2)
//   {
//     ADMUX &= ~(1 << MUX0); // Select ADC0 for conversion
//   }
//   else
//   {
//     ADMUX |= (1 << MUX0); // Select ADC1 for conversion
//   }

//   ADCSRA |= (1 << ADSC); // Start conversion
//   while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
//   if (f % 4 < 2)
//   {
//     // ADCH=0x00;
//     ADC_Value = 0;
//     // ADMUX |= ((0 << MUX4)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3));
//     // _delay_ms(500);
//     // ADCSRA |= (1 << ADSC);
//     // _delay_ms(500);
//     ADC_Value = ADCH;
//     ADC_Value_Sum_Voltage += ADC_Value;
//     PORTC = ADC_Value_Sum_Voltage;

//     PORTL = ADCSRA;
//     PORTA = f;
//     if (f % 4 == 1)
//     {
//       ADC_Value_Avg_Voltage = ADC_Value_Sum_Voltage >> 1;
//       ADC_Value_Sum_Voltage = 0;
//       Voltage[v_count] = ADC_Value_Avg_Voltage;
//       Overall_Sum_Voltage += Voltage[v_count]; // change here

//       max_val_voltage = Voltage[0];
//       if (Voltage[v_count] > max_val_voltage)
//       {
//         max_val_voltage = Voltage[v_count];   
//         //Send_to_thingspeak(max_val_voltage,v_count,1,f,0,0);
          
//       }
//       if (Voltage[v_count] > threshold_voltage)
//       {
//         relay_toggle(0);
//       }

//       // Send_to_thingspeak(Voltage[v_count],v_count,f);

//       // while(1);
//       v_count++;
//     }
//     // poww_test=Volts*curr;
//   }
//   else
//   {
//     ADC_Value = 0;
//     ADC_Value = ADCH;
//     ADC_Value_Sum_Current += ADC_Value;
//     PORTC = ADC_Value_Sum_Current;

//     PORTL = ADCSRA;
//     PORTA = f;
//     if (f % 4 == 3)
//     {
//       ADC_Value_Avg_Current = ADC_Value_Sum_Current >> 1;
//       ADC_Value_Sum_Current = 0;
//       Current[i_count] = ADC_Value_Avg_Current;
//       Overall_Sum_Current += Current[i_count]; // change here

//       max_val_current = Current[0];
//       if (Current[i_count] > max_val_current)
//       {
//         max_val_current = Current[i_count];
//         //Send_to_thingspeak(max_val_current,i_count,2,f,0,0);
//       }
//       if (Current[i_count] > threshold_current)
//       {
//         relay_toggle(0);
//       }

//       // Send_to_thingspeak(Current[i_count],i_count,f);
//       // while(1);
//       i_count++;
//       ADC_Value_Avg_Current=ADC_Value_Avg_Current;
//       ADC_Value_Avg_Voltage=ADC_Value_Avg_Voltage;
//       poww[pow_count] = ADC_Value_Avg_Current * ADC_Value_Avg_Voltage;
//       //Send_to_thingspeak(poww[pow_count],pow_count,ADC_Value_Avg_Voltage,ADC_Value_Avg_Current,0,0);

//       Overall_Sum_Power += poww[pow_count]; // change here

//       max_val_power = poww[0];
//       if (poww[pow_count] > max_val_power)
//       {
//         max_val_power = poww[pow_count];
//         //Send_to_thingspeak(max_val_power,pow_count,3,f,0,0);
//         //while(1);
//       }

//       pow_count++;
//       // Send_to_thingspeak(ADC_Value_Avg_Voltage,ADC_Value_Avg_Current,poww[pow_count]);
//       // while(1);
//     }
//   }

//   if (f == 400)
//   {
//     //Send_to_thingspeak(max_val_voltage, max_val_current, max_val_power,0,0,0);
//     // for(int i=0;i<3;i++)
//     // {
//     //   test_Current_sum+=Current[i];
//     //   test_Voltage_sum+=Voltage[i];
//     //   test_Power_sum+=poww[i];
//     // }

//     // Do conversion here maybe
//     //--------------------------------------------------------------

//     // Average_Voltage = calc_avg(Voltage, 100);
//     // Average_Current = calc_avg(Current, 100);
//     // Average_Power = calc_avg(poww, 100);
//     // Voltage_Vpk = find_Peak(Voltage, 100);
//     // Current_Vpk = find_Peak(Current, 100);
//     // Power_Vpk = find_Peak(poww, 100);
//     // Send_to_thingspeak(Average_Voltage, Average_Current, Average_Power,Voltage_Vpk,Current_Vpk,Power_Vpk);
//     //Send_to_thingspeak(Overall_Sum_Voltage / 100, Overall_Sum_Current / 100, Overall_Sum_Power / 100, max_val_voltage, max_val_current,max_val_power);

//     // reset the arrays to zero
//     for (int k = 0; k < 100; k++)
//     {
//       Current[k] = 0;
//       Voltage[k] = 0;
//       poww[k] = 0;
//     }

//      //while(1);
//   }
//   f++;
//   if (f%400==1 && f>1)
//   {
//     f = 0;
//     Send_to_thingspeak(0,0,0,0,0,0);
//   }

//   // ADCSRA &= 0xEF; //ADC Interrupt Flag is made 0 again
//   // ADCSRA |= 0x40;//free flowing mode
// }
// }

int main()
{

  DDRB = 0xFF; // Port B set as output
  DDRL = 0xFF; // port L direction: output //pins- 49:42
  DDRC = 0xFF; // port C direction: output //pins- 37:30
  DDRA = 0xFF; // port A direction: output //pins- 22:29
  DDRG = 0xFF; // port G direction: output //pins- 41://PG0-41: PG2-39

  pwm_setup();

  uart1_init();
//Send_to_thingspeak(0,0,0,0,0,0);
  // Connect to Wi-Fi
  wifi_connect();
  

  // sei();
  adc_init();

  while (1)
  {
    //adc_read();
    Send_to_thingspeak(f,0,0,0,0,0);
      //uart_send_string((char)f); 
//Send_to_thingspeak(f,0,0,0,0,0);  
 if (f % 4 < 2)
  {
    ADMUX &= ~(1 << MUX0); // Select ADC0 for conversion
  }
  else
  {
    ADMUX |= (1 << MUX0); // Select ADC1 for conversion
  }

  ADCSRA |= (1 << ADSC); // Start conversion
  while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
  if (f % 4 < 2)
  {
    // ADCH=0x00;
    ADC_Value = 0;
    // ADMUX |= ((0 << MUX4)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3));
    // _delay_ms(500);
    // ADCSRA |= (1 << ADSC);
    // _delay_ms(500);
    ADC_Value = ADCH;
    ADC_Value_Sum_Voltage += ADC_Value;
    PORTC = ADC_Value_Sum_Voltage;

    PORTL = ADCSRA;
    PORTA = f;
    if (f % 4 == 1)
    {
      ADC_Value_Avg_Voltage = ADC_Value_Sum_Voltage >> 1;
      ADC_Value_Sum_Voltage = 0;
      Voltage[v_count] = ADC_Value_Avg_Voltage;
      Overall_Sum_Voltage += Voltage[v_count]; // change here

      max_val_voltage = Voltage[0];
      if (Voltage[v_count] > max_val_voltage)
      {
        max_val_voltage = Voltage[v_count];   
        //Send_to_thingspeak(max_val_voltage,v_count,1,f,0,0);
          
      }
      if (Voltage[v_count] > threshold_voltage)
      {
        relay_toggle(0);
      }

      // Send_to_thingspeak(Voltage[v_count],v_count,f);

      // while(1);
      v_count++;
    }
    // poww_test=Volts*curr;
  }
  else
  {
    ADC_Value = 0;
    ADC_Value = ADCH;
    ADC_Value_Sum_Current += ADC_Value;
    PORTC = ADC_Value_Sum_Current;

    PORTL = ADCSRA;
    PORTA = f;
    if (f % 4 == 3)
    {
      ADC_Value_Avg_Current = ADC_Value_Sum_Current >> 1;
      ADC_Value_Sum_Current = 0;
      Current[i_count] = ADC_Value_Avg_Current;
      Overall_Sum_Current += Current[i_count]; // change here

      max_val_current = Current[0];
      if (Current[i_count] > max_val_current)
      {
        max_val_current = Current[i_count];
        //Send_to_thingspeak(max_val_current,i_count,2,f,0,0);
      }
      if (Current[i_count] > threshold_current)
      {
        relay_toggle(0);
      }

      // Send_to_thingspeak(Current[i_count],i_count,f);
      // while(1);
      i_count++;
      ADC_Value_Avg_Current=ADC_Value_Avg_Current;
      ADC_Value_Avg_Voltage=ADC_Value_Avg_Voltage;
      poww[pow_count] = ADC_Value_Avg_Current * ADC_Value_Avg_Voltage;
      //Send_to_thingspeak(poww[pow_count],pow_count,ADC_Value_Avg_Voltage,ADC_Value_Avg_Current,0,0);

      Overall_Sum_Power += poww[pow_count]; // change here

      max_val_power = poww[0];
      if (poww[pow_count] > max_val_power)
      {
        max_val_power = poww[pow_count];
        //Send_to_thingspeak(max_val_power,pow_count,3,f,0,0);
        //while(1);
      }

      pow_count++;
      // Send_to_thingspeak(ADC_Value_Avg_Voltage,ADC_Value_Avg_Current,poww[pow_count]);
      // while(1);
    }
  }

  if (f == 400)
  {
    //Send_to_thingspeak(max_val_voltage, max_val_current, max_val_power,0,0,0);
    // for(int i=0;i<3;i++)
    // {
    //   test_Current_sum+=Current[i];
    //   test_Voltage_sum+=Voltage[i];
    //   test_Power_sum+=poww[i];
    // }

    // Do conversion here maybe
    //--------------------------------------------------------------

    // Average_Voltage = calc_avg(Voltage, 100);
    // Average_Current = calc_avg(Current, 100);
    // Average_Power = calc_avg(poww, 100);
    // Voltage_Vpk = find_Peak(Voltage, 100);
    // Current_Vpk = find_Peak(Current, 100);
    // Power_Vpk = find_Peak(poww, 100);
    // Send_to_thingspeak(Average_Voltage, Average_Current, Average_Power,Voltage_Vpk,Current_Vpk,Power_Vpk);
    Send_to_thingspeak(Overall_Sum_Voltage / 100, Overall_Sum_Current / 100, Overall_Sum_Power / 100, max_val_voltage, max_val_current,max_val_power);

    // reset the arrays and values to zero
      Overall_Sum_Voltage=0;
      Overall_Sum_Current=0;
      Overall_Sum_Power=0;
    for (int k = 0; k < 100; k++)
    {
      
      Current[k] = 0;
      Voltage[k] = 0;
      poww[k] = 0;
    }

     //while(1);
  }
  f++;
  if (f == 401)
  {
    f = 0;
    Send_to_thingspeak(0,0,0,0,0,0);
  }

  // ADCSRA &= 0xEF; //ADC Interrupt Flag is made 0 again
  // ADCSRA |= 0x40;//free flowing mode
}


    // if (Read_from_thingspeak() == '1')
    //   relay_toggle(1);
    // else if (Read_from_thingspeak() == '0')
    //   relay_toggle(0);


  while (1);
  return 0;
}

// doubts:
// add trigger source for ADC for appropriate sampling rate..
// Use differential channels
// Vrms and Vpp to be used as pointers?
// Entire program on interrupt?
// 2 samples current and 2 samples voltage- take average
// timeout thing to take care of?
// number of zero crossings better or this trend change checking?- stick to zero crossing..
// noise cancelling?