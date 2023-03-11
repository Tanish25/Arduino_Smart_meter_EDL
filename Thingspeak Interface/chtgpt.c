#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

char message[] = "Hello, world!\n";
volatile uint8_t buffer_index = 0;

// Initialize UART
void USART_Init(unsigned int ubrr) {
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    // Enable receiver and transmitter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
    // Enable transmit complete interrupt
    UCSR0B |= (1<<TXCIE0);
}

// Transmit data through UART
void USART_Transmit(unsigned char data) {
    // Put data into buffer, sends the data
    UDR0 = data;
}

// Transmit string through UART using buffer
void USART_Transmit_Buffer(char *data) {
    buffer_index = 0;
    uint8_t data_length = strlen(data);
    memcpy(message, data, data_length);
    // Send first byte
    USART_Transmit(message[buffer_index++]);
}

int main(void) {
    USART_Init(MYUBRR);
    sei(); // Enable global interrupts
    while (1) {
        // Do nothing
    }
    return 0;
}

ISR(USART_TX_vect) {
    if (buffer_index < strlen(message)) {
        USART_Transmit(message[buffer_index++]);
    }
}