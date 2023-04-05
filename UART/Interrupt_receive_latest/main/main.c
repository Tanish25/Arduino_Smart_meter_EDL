#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define F_CPU 16000000UL
#define BAUD 115200
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

volatile char rx_buffer[50];
volatile uint8_t rx_index = 0;

ISR(USART1_RX_vect)
{
    char received_byte = UDR1;
    rx_buffer[rx_index++] = received_byte;
}

void init_uart()
{
    UBRR1 = 8;
    UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
}

void send_command(char* command)
{
    while(*command)
    {
        while(!(UCSR1A & (1<<UDRE1)));
        UDR1 = *command++;
    }
    while(!(UCSR1A & (1<<TXC1)));
}

int main()
{
    sei();
    init_uart();
    char response_buffer[50];
    send_command("AT\r\n");
    _delay_ms(500);
    strcpy(response_buffer, rx_buffer);
    rx_index = 0;

    if(strstr(response_buffer,"OK"))
    {
        send_command("Done");
    }
    else
        send_command("not received");
    return 0;
}
