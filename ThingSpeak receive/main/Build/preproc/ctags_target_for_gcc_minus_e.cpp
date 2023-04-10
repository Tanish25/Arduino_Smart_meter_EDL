# 1 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
# 2 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 3 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 4 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 5 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 6 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 7 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 8 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2






char rx_buffer;
volatile uint8_t rx_index = 0;


void relay_init(void){

    
# 20 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)((0x07) + 0x20)) 
# 20 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
        = 0x01;
    
# 21 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)((0x08) + 0x20)) 
# 21 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
         = 0x01;
}

void relay_toggle(uint8_t status){

if(status==1)

# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
(*(volatile uint8_t *)((0x08) + 0x20)) 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
     = 0x01;
else if(status==0)

# 29 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
(*(volatile uint8_t *)((0x08) + 0x20)) 
# 29 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
     = 0x00;
}


void uart_send_string(char *str)
{
  while (*str != '\0')
  {
    // Wait for the transmit buffer to be empty
    while (!(
# 38 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
            (*(volatile uint8_t *)(0xC8)) 
# 38 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                   & (1 << 
# 38 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                           5
# 38 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                                )))
      ;

    // Send the next character
    
# 42 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)(0XCE)) 
# 42 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
        = *str;

    // Move to the next character in the string
    str++;
  }
}



char* Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  char response_buffer[50];
  
# 56 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 56 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4/last.txt\r\n");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(100);

  uart_send_string(Get_string);
  _delay_ms(3000);
  
# 70 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("cli" ::: "memory")
# 70 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;


  uart_send_string("AT+CIPCLOSE=5\r\n");

  return response_buffer;
}



int main(void)
{

 uart0_init(((16000000L +(115200)*8UL) / (16UL*(115200))-1UL) /* macro calculating precise UBRR value*/);
 uart1_init(((16000000L +(115200)*8UL) / (16UL*(115200))-1UL) /* macro calculating precise UBRR value*/);

 
# 88 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
(__iob[1]) 
# 88 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
       = &uart0_io; // attach uart stream to stdout & stdin
 
# 89 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
(__iob[0]) 
# 89 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      = &uart0_io; // uart0_in and uart0_out are only available if NO_USART_RX or NO_USART_TX is defined

 
# 91 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
__asm__ __volatile__ ("sei" ::: "memory")
# 91 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
     ; // enable interrupts
 char buffer;
 uart1_putstr((const char*)("AT\r\n"))/* macro to avoid const char* conversion restrictions*/;
  while(1){
 scanf("%d", rx_buffer);
  printf(rx_buffer); }
 while(1);
}
