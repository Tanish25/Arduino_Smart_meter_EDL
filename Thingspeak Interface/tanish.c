#include <avr/io.h>
#define F_CPU 16000000//16MHz
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define ADC_COUNTS 512//change according to bit precision used..
#define true 1
#define false 0

#define DEFAULT_BUFFER_SIZE 160
#define API_KEY "JGMS30H5A71UCKZ1"
#define SSID "AndroidAP"
#define PASSWORD "tan12345"

volatile uint8_t channel=1;//for channel
uint16_t ADC_Value;
uint16_t ADC_Value_Sum_Voltage;
uint16_t ADC_Value_Sum_Current;
uint16_t Calulated_Power;
volatile uint8_t f=0;

// Declare variables as static or local whenever possible
static volatile uint8_t index;
//static char RESPONSE_BUFFER[DEFAULT_BUFFER_SIZE];
static volatile int16_t Counter = 0, pointer = 0;
volatile uint8_t rx_complete = 0;

//remove this
volatile uint8_t Volts = 267;
volatile uint8_t curr = 234;
volatile uint16_t poww_test=0;

  int16_t sampleV;
  int16_t lastSample;
  int16_t startV;
  uint16_t sqV;
  int16_t sumV = 0;

  //-------------------------------------------------------------------------------------------------------------------------
  // Start
  //-------------------------------------------------------------------------------------------------------------------------
                            
  //port L direction: output //pins- 49:42
     //port C direction: output //pins- 37:30
     //port A direction: output //pins- 22:29
/*  
ISR(ADC_vect)
{ }
*/


void adc_init()
{
  ADCSRA |= (1 << ADEN);    // Enable the ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 16Mhz
  ADCSRA |= (1 << ADIE);    // Enable Interrupts
   //ADCSRA |= (1 << ADATE);    // Set this bit to enable trigger source
 
  PRR0 |= (0 << PRADC);      // Power Reduction Register's PRADC bit must be disabled before starting the conversion
  
 
  //ADCSRB |= (1 << ADTS0) | (1 << ADTS1);   // Triggered by Timer0 compare match A
  //ADCSRB |= (1 << ADTS2);   // Triggered by Timer0 Overflow
  
  DIDR0 = 0xff;
  DIDR1 = 0xff;

  ADMUX |= (1 << REFS0);    // use external AREF as the reference
  ADMUX |= (0 << REFS1);    // use external AREF as the reference
  ADMUX |= (1 << ADLAR);    // left aligned, better for 8 bit resolution
  ADMUX |= ((0 << MUX4)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3));    // positive-ADC0; negative-ADC1;
   //ADCSRA |= (1 << ADSC); //to start conversion
  
}



void uart1_init(void)
{
    // Set baud rate
    UBRR1 = 8;

    // Enable transmitter and receiver, and enable receive and transmit complete interrupts
    UCSR1B |= (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1) | (1 << TXCIE1);

    // Set asynchronous mode, no parity, 1 stop bit, 8 data bits
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void uart_send_string(char* str)
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



void wifi_connect(void)
{
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

int main()
{   
    
    DDRB = 0xFF;//Port B set as output
    DDRL = 0xFF;//port L direction: output //pins- 49:42
    DDRC = 0xFF;//port C direction: output //pins- 37:30
    DDRA = 0xFF;//port A direction: output //pins- 22:29
    DDRG = 0xFF;//port G direction: output //pins- 41://PG0-41: PG2-39

    //sampling frequency generator:
    // OCR0A= 0x7F;//50 percent of 255=duty cycle//decimal=127
    // TCCR0A = 0x83;
    // TCCR0B = 0x03;//prescaler = 64 for 1kHz
    //TCCR0B = 0x02;//prescaler = 8 for 8kHz
    //uart_init();
    //PORTA=0xFF;
    //_delay_us(500);
    //while(1);
    uart1_init();
    
    // Connect to Wi-Fi
    wifi_connect();
    //_delay_ms(500);
    //sei();    
    adc_init();

    //poww=ADC_Value_Sum_Voltage*ADC_Value_Sum_Current;
    //sei();
    ADCSRA |= (1 << ADSC);
    _delay_ms(500);
    while(f<5)
    {
      ADC_Value=ADCH;//for 8 bit precision only

      if(f==0)
      {
        startV = ADC_Value;
        lastSample = startV;
        PORTC = startV;
        PORTL = ADCSRA;
        PORTA=f;
        poww_test=Volts*curr;
        Send_to_thingspeak(Volts,curr,f);
        //while(1);
      if(f%4<2)
      {
        ADC_Value_Sum_Voltage+=ADC_Value;
        PORTC = ADC_Value_Sum_Voltage;
        PORTL = ADCSRA;
        PORTA=f;
        poww_test=Volts*curr;
        Send_to_thingspeak(Volts,curr,f);
        
        //while(1);
      }
      else
      {
        ADC_Value_Sum_Current+=ADC_Value;
        PORTC = ADC_Value_Sum_Current;
        PORTL = ADCSRA;
        PORTA=f;
        poww_test=Volts*curr;
        Send_to_thingspeak(Volts,curr,f);
        //while(1);
        
      }
      if(f==4)
      {
          //cli();          
          ADC_Value_Sum_Voltage+=ADC_Value;
          PORTC = ADC_Value_Sum_Voltage;
          PORTL = ADCSRA;
          PORTA=f;
          //poww=ADC_Value_Sum_Voltage*ADC_Value_Sum_Current;
          //Send_to_thingspeak(ADC_Value_Sum_Voltage,ADC_Value_Sum_Current,poww);
          poww_test=Volts*curr;
          Send_to_thingspeak(Volts,curr,f);
          while(1);
      }


      //changing channel:
        if(f%4==3)
        {
          ADMUX |= ((0 << MUX4)|(0 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3));  
          PORTA=f;
          //ADC_Value_Sum_Current+=ADC_Value;
          PORTC = ADC_Value_Sum_Current;
          //while(1);
          //_delay_us(125);
        }
        if(f%4==1)
        {
          ADMUX |= ((0 << MUX4)|(1 << MUX0)|(0 << MUX1)|(0 << MUX2)|(0 << MUX3));  
          PORTA=f;
          //ADC_Value_Sum_Voltage+=ADC_Value;
          PORTC = ADC_Value_Sum_Voltage;
          while(1);
          //_delay_us(125);
        }
         
        f=f+1; 

        ADCSRA &= 0xEF; //ADC Interrupt Flag is made 0 again
        ADCSRA |= 0x40;//free flowing mode 
              
    }    

    
    while(1);
    return 0;
}}


//doubts:
//add trigger source for ADC for appropriate sampling rate..
//Use differential channels
//Vrms and Vpp to be used as pointers?
//Entire program on interrupt?
//2 samples current and 2 samples voltage- take average
//timeout thing to take care of?
//number of zero crossings better or this trend change checking?- stick to zero crossing..
//noise cancelling?
/*
ISR(USART1_UDRE_vect)
{
    UDR1=(int8_t)Vpp;
    //UCSR1B |= (1 << UDRIE1); 
    UDR1=(uint8_t)Vrms;  
}
*/