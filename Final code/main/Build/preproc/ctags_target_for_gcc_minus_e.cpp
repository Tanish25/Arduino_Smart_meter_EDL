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

void uart_send_string(char *str)
{
  while (*str != '\0')
  {
    // Wait for the transmit buffer to be empty
    while (!(
# 35 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
            (*(volatile uint8_t *)(0xC8)) 
# 35 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                   & (1 << 
# 35 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
                           5
# 35 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
                                )))
      ;

    // Send the next character
    
# 39 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
   (*(volatile uint8_t *)(0XCE)) 
# 39 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
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

  _delay_ms(10000);
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

void Send_to_thingspeak( uint16_t Voltage, uint16_t Current, uint16_t Power )
{
  char Get_string[128];
  char len_string[128];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/update?api_key=%s&field1=%d&field2=%d&field3=%d\r\n","JGMS30H5A71UCKZ1",Voltage,Current,Power);
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


void Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4.json?results=1");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(1000);

  uart_send_string(Get_string);
  _delay_ms(1000);

  uart_send_string("AT+CIPCLOSE=5\r\n");
  _delay_ms(4000);
}

int main()
{

  
# 149 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 149 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  uart1_init();

  // Connect to Wi-Fi
  wifi_connect();
  Send_to_thingspeak(5,55,55);

  
# 156 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino" 3
 __asm__ __volatile__ ("cli" ::: "memory")
# 156 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\ThingSpeak receive\\main\\main.ino"
      ;
  return 0;

  // char response_buffer[50];
  //   uart_send_string("AT\r\n");
  //   _delay_ms(500);
  //   strcpy(response_buffer, rx_buffer);
  //   rx_index = 0;

  //   if(strstr(response_buffer,"OK"))
  //   {
  //       uart_send_string("Done_out");
  //   }
  //   else
  //       uart_send_string("not received_out");

  // memset(received_string, 0, sizeof(received_string));  buuffer clear
}
