# 1 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
# 2 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 3 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 4 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 5 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 6 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 7 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2
# 8 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 2






volatile char rx_buffer[50];
volatile uint8_t rx_index = 0;


# 17 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
extern "C" void __vector_36 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_36 (void)

# 18 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
{
  char received_byte = 
# 19 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                      (*(volatile uint8_t *)(0XCE))
# 19 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                          ;
  rx_buffer[rx_index++] = received_byte;
}

void uart1_init()
{
  
# 25 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 (*(volatile uint16_t *)(0xCC)) 
# 25 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
       = 8;
  
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 (*(volatile uint8_t *)(0XC9)) 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
        = (1 << 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                4
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                     ) | (1 << 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                               3
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                                    ) | (1 << 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                                              7
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                                                    );
  
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 (*(volatile uint8_t *)(0xCA)) 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
        = (1 << 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                2
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                      ) | (1 << 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                                1
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                                      );
}

void relay_init(void){

    
# 32 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)((0x07) + 0x20)) 
# 32 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
        = 0x01;
    
# 33 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)((0x08) + 0x20)) 
# 33 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
         = 0x01;
}

void relay_toggle(uint8_t status){

if(status==1)

# 39 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
(*(volatile uint8_t *)((0x08) + 0x20)) 
# 39 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
     = 0x01;
else if(status==0)

# 41 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
(*(volatile uint8_t *)((0x08) + 0x20)) 
# 41 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
     = 0x00;
}


void uart_send_string(char *str)
{
  while (*str != '\0')
  {
    // Wait for the transmit buffer to be empty
    while (!(
# 50 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
            (*(volatile uint8_t *)(0xC8)) 
# 50 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                   & (1 << 
# 50 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                           5
# 50 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                                )))
      ;

    // Send the next character
    
# 54 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)(0XCE)) 
# 54 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
        = *str;

    // Move to the next character in the string
    str++;
  }
}

void wifi_connect(void)
{
  char cmd[64];

  // Construct Wi-Fi connection command with SSID and password
  sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", "Pixel-5", "cabhinav");

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
  if (strstr(response_buffer, "WIFI"))
  {
    uart_send_string("AT+CIPMUX=1\r\n");
  }
  else
    goto sendCIPMUX;

}




char* Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  char response_buffer[50];
  
# 120 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 120 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
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
  
# 134 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("cli" ::: "memory")
# 134 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;


  uart_send_string("AT+CIPCLOSE=5\r\n");

  return response_buffer;
}



int main()
{

  
# 149 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 149 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  uart1_init();
  relay_init();

  // Connect to Wi-Fi
  wifi_connect();
  
# 155 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("cli" ::: "memory")
# 155 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  while(1){
  if (Read_from_thingspeak())
    relay_toggle(1);
  else if (Read_from_thingspeak() == '0')
    relay_toggle(0);};
  
# 161 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("cli" ::: "memory")
# 161 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  return 0;

}
