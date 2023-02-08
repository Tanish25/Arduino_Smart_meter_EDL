#include <avr/io.h>
#define F_CPU 16000000//16MHz
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define ADC_COUNTS 1023
#define true 1
#define false 0

int16_t Vpp;
uint16_t Vrms;
//shd it be global?- coz we are using in 2 functions
void adc_init()
{
    ADCSRA = 0x87;// ADC enabled and prescaler=128
    //ADMUX = 0x61;// Ref= Vcc// data is left-justified //ADC0 is source pin
    ADMUX = 0x41;// Ref= Vcc// data is right-justified //ADC0 is source pin
    ADCSRA |= 0x40; 
}

void uart_init()
{
  // configure the first UART module for transmitting data
  UBRR1 = 8;               // for 115200 baud rate with F_CPU = 16000000
  UCSR1B |= (1 << TXEN1)|(1 << TXCIE1)|(1 << UDRIE1);  // enable transmitter AND ITS Interrupts
  UCSR1A = 0x00;
  UCSR1C = 0x06;  //Transmission size = 8 bit; asynchronous transmission

}
void calcV(int ch, int8_t offsetV, uint8_t SupplyVoltage)//, int16_t* Vrms, int16_t* Vpp)//change double*
{
  uint8_t crossCount = 0;             //Used to measure number of times threshold is crossed.
  uint8_t numberOfSamples = 0;        //This is now incremented
  //uint8_t timeout = 200;              //mseconds 
  uint8_t crossings = 10;            // sampling 10 trace crossing 
  //convert all values to signed later..
  //Note- double has been converted to int16
  //check if these datatypes are really needed to be double or if 8 bits are enough?
  int16_t adjustedV;
  int16_t sampleV;
  int16_t startV;
  uint16_t sqV;
  int16_t sumV = 0;
  int16_t sumMin = 0, sumMax = 0, max = 0, min = 1023, Chng = 0, maxCount = 0, minCount = 0, lastSample, onLow = 0, onUp = 0, oldChng = 0, stayCount = 5;
  int16_t lastVCross;
  int16_t checkVCross=0;//initialized to remove warning
  //int16_t maxValue = 0;
  //int16_t minValue = 1024;
  //uint8_t i = 0;

  //change double to other datatypes

  //-------------------------------------------------------------------------------------------------------------------------
  // Start
  //-------------------------------------------------------------------------------------------------------------------------
  numberOfSamples = 0;
  crossCount = 0;
  adjustedV = 0;
  //double start = millis();//change this command//Returns the number of milliseconds passed since the Arduino board began running the current program.
  sumV = 0;                                       //Reset accumulator
  while(ADIF==0);//wait till conversion is done
  startV = (ADCH<<8)+(ADCL);//how to put data in 2 registers into integer?
  ADCSRA |= 0x40; //start conversion again
  lastSample = startV;

  //-------------------------------------------------------------------------------------------------------------------------
  // Measurement loop
  //------------------------------------------------------------------------------------------------------------------------- 
  while (crossCount < crossings)// && (millis() - start) < timeout)     //millis()-start makes sure it doesnt get stuck in the loop if there is an error.
  {
     numberOfSamples++;                       //Count number of times looped.
//-----------------------------------------------------------------------------
// Read in raw voltage samples
//-----------------------------------------------------------------------------
     while(ADIF==0);//wait till conversion is done
     sampleV = (ADCH<<8)+(ADCL);//how to put data in 2 registers into integer?
     ADCSRA |= 0x40; //start conversion again
    //---------------------------------------------------------------------------
    // Voltage calculation corrected by offset
    //-----------------------------------------------------------------------------
    adjustedV = sampleV / ADC_COUNTS * SupplyVoltage;//this formula will change if differential inputs used
    //Serial.print(adjustedV); Serial.print(" ");
    adjustedV -= offsetV;
    //Serial.println(adjustedV);

    //-----------------------------------------------------------------------------
    // RMS voltage
    //-----------------------------------------------------------------------------  
    sqV = adjustedV * adjustedV;
    sumV += sqV;
    //-----------------------------------------------------------------------------
    // Find the number of times the voltage has crossed the initial voltage
    // - every 2 crosses we will have sampled 1 wavelength 
    // - so this method allows us to sample an integer number of half wavelengths which increases accuracy
    //-----------------------------------------------------------------------------       
    lastVCross = checkVCross;
    if (sampleV > startV) 
      checkVCross = true;
    else 
      checkVCross = false;

    if (numberOfSamples == 1)
      lastVCross = checkVCross;

    if (lastVCross != checkVCross)
    {
      crossCount++;
    }

    //-----------------------------------------------------------------------------
    // Find max peak and min peak to measure peak to peak voltage
    //-----------------------------------------------------------------------------  
    if (lastSample < sampleV) 
    {               //trace is going up
      if (Chng < 0 && onUp < stayCount)       //If minimum was detected we are going to check there is not a false minimum
        oldChng = Chng;
      if (oldChng < 0 && onUp> stayCount) 
      {   //if we found a minimum and after stayCount samples the trace is going up then recorded the minimum and reset
        sumMin += min;
        min = 1023;
        minCount++;
        oldChng = 0;
        onUp = 0;
        onLow = 0;
      }
      lastSample = sampleV;
      Chng = 1;
      onUp++;
      if (max < sampleV)
        max = sampleV;                      // get max value
    }
    else if (lastSample > sampleV) 
    {        //trace is going down
      if (Chng > 0 && onLow < stayCount)    //If maximum was detected we are going to check there is not a false maximum
        oldChng = Chng;

      if (oldChng > 0 && onLow > stayCount) 
      {   //if we found a maximum and after stayCount samples the trace is going down then recorded the maximum and reset
        sumMax += max;
        max = 0;
        maxCount++;
        oldChng = 0;
        onLow = 0;
        onUp = 0;
      }
      lastSample = sampleV;
      Chng = -1;
      onLow++;
      if (min > sampleV)
        min = sampleV;                      //get min value
    }
    else
      lastSample = sampleV;

  }

  //-------------------------------------------------------------------------------------------------------------------------
  // Vrms and Vpp calculations
  //------------------------------------------------------------------------------------------------------------------------- 
  //Calculation of the root of the mean of the voltage squared (rms)

  //
  //Note=pointer variables were changed to normal variables 
  Vrms = (uint16_t)sqrt(sumV / numberOfSamples);
  if (minCount != 0 && maxCount != 0)
    Vpp = (int16_t)((sumMax / maxCount - sumMin / minCount) / ADC_COUNTS) * SupplyVoltage;
  else
    Vpp = 0;
  if (Vpp < 0)
    Vpp = 0;
  //return;
}
ISR(USART1_UDRE_vect)
{
    UDR1=(int8_t)Vpp;
    UCSR1B |= (1 << UDRIE1); 
    UDR1=(uint8_t)Vrms;  
}

int main()
{   
adc_init();
uart_init();
calcV(1, 2, 5);// &Vrms, &Vpp);//ch, offset, supply voltage
//are Vrms and Vpp to be sent as parameters?
UDR1=(int8_t)Vpp;
//configure how to send Vrms after some time?
UDR1=(int8_t)Vrms;
   return 0;
}


//doubts:
//any trigger source?
//shd data be calculated in arduino only?
//any other way apart from square-add?
//show vivado progress
//try entire code using interrupt
//explore differential channels
//Vrms and Vpp to be used as pointers?
//pointers and addresses doesnt work in embedded right?
//1 sample current and 1 sample voltage fine?
//timeout thing to take care of?
//number of zero crossings better or this trend change checking?

//recomm letters