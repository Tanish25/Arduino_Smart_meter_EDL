#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

volatile uint8_t current_char = 0;        // the current character index

void adc_init() {
  ADMUX = (1 << REFS0);  // Set reference voltage to VCC
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC and set prescaler to 128
}

void uart_init() {
    UBRR0 = 8;
    UCSR0B |= (1 << TXEN0);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

uint16_t read_adc(uint8_t channel) {
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Set ADC channel
  ADCSRA |= (1 << ADSC); // Start conversion
  while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
  return ADC;
}

void uart_puts(string) {
    while (/*string[current_char] != '\0' &*/ !(UCSR0A & (1 << UDRE0)));
    {
        UDR0 = string; // send the current character and increment the index
    }
}

int main(void) {
  char buffer[10];

  adc_init();
  uart_init();
  
  while(1) {
    uint16_t adc_value = read_adc(0); // Read ADC value from channel 0
    itoa(adc_value, buffer, 10); // Convert ADC value to string
    uart_puts(48+buffer);
    _delay_ms(1000); // Wait for 1 second
  }

  return 0;
}
