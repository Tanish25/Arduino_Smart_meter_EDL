# 1 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
# 2 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2
# 3 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2
# 4 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2
# 5 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2
# 6 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2
# 7 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2
# 8 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 2






volatile char rx_buffer[50];
volatile uint8_t rx_index = 0;


# 17 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
extern "C" void __vector_36 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_36 (void)

# 18 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
{
  char received_byte = 
# 19 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                      (*(volatile uint8_t *)(0XCE))
# 19 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                          ;
  rx_buffer[rx_index++] = received_byte;
}

void uart1_init()
{
  
# 25 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
 (*(volatile uint16_t *)(0xCC)) 
# 25 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
       = 8;
  
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
 (*(volatile uint8_t *)(0XC9)) 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
        = (1 << 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                4
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                     ) | (1 << 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                               3
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                    ) | (1 << 
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                                              7
# 26 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                                    );
  
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
 (*(volatile uint8_t *)(0xCA)) 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
        = (1 << 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                2
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                      ) | (1 << 
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                                1
# 27 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                      );
}

void relay_init(void){

    
# 32 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
   (*(volatile uint8_t *)((0x07) + 0x20)) 
# 32 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
        = 0xFF;
    
# 33 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
   (*(volatile uint8_t *)((0x08) + 0x20)) 
# 33 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
         = 0xFF;

}

void relay_toggle(uint8_t status){

if(status==1)

# 40 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)((0x08) + 0x20)) 
# 40 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
     = 0x01;
else if(status==0)

# 42 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)((0x08) + 0x20)) 
# 42 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
     = 0x00;
}

uint8_t adc_read(void)
{
// Select differential input mode

# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7C)) 
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
     |= (1 << 
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
              3
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                  ) | (1 << 
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                            2
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                ) | (1 << 
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                                          1
# 48 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                              ); // Select ADC1 (A0) as positive input

# 49 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7B)) 
# 49 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
      |= (1 << 
# 49 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
               3
# 49 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                   ); // Select ADC0 (A1) as negative input

// Set ADC reference voltage

# 52 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7C)) 
# 52 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
     |= (1 << 
# 52 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
              7
# 52 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                   ) | (1 << 
# 52 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                             6
# 52 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                  ); // Use internal 5V as reference voltage


# 54 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7C)) 
# 54 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
     |= (1 << 
# 54 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
              5
# 54 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                   ); // left aligned, better for 8 bit resolution

// Set ADC clock frequency

# 57 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7A)) 
# 57 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
      |= (1 << 
# 57 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
               2
# 57 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                    ) | (1 << 
# 57 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                              0
# 57 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                   ); // Set prescaler to 32 for 5 kHz sampling frequency

// Enable ADC

# 60 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7A)) 
# 60 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
      |= (1 << 
# 60 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
               7
# 60 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                   );

// Start ADC conversion

# 63 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
(*(volatile uint8_t *)(0x7A)) 
# 63 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
      |= (1 << 
# 63 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
               6
# 63 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                   );

// Wait for conversion to complete
while (
# 66 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
      (*(volatile uint8_t *)(0x7A)) 
# 66 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
             & (1 << 
# 66 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                     6
# 66 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                         ));

// Get ADC value
uint8_t adc_value = 
# 69 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                   (*(volatile uint16_t *)(0x78))
# 69 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                      ;

// Calculate differential value
uint8_t differential_value = (uint8_t)adc_value - 512;

// Return differential value
return differential_value;
}


void uart_send_string(char *str)
{
  while (*str != '\0')
  {
    // Wait for the transmit buffer to be empty
    while (!(
# 84 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
            (*(volatile uint8_t *)(0xC8)) 
# 84 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                   & (1 << 
# 84 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
                           5
# 84 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
                                )))
      ;

    // Send the next character
    
# 88 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
   (*(volatile uint8_t *)(0XCE)) 
# 88 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
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
  while (strstr(response_buffer, "CONNECTED"))
  {
    uart_send_string("AT+CIPMUX=1\r\n");
     _delay_ms(2000);
  }

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


char Read_from_thingspeak(void)
{
  char Get_string[128];
  char len_string[128];
  char response_buffer[50];
  uint8_t len;
  sprintf(Get_string, "GET https://api.thingspeak.com/channels/2053966/fields/4/last.txt\r\n");
  len = strlen(Get_string);

  uart_send_string("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80\r\n");
  _delay_ms(4000);

  sprintf(len_string, "AT+CIPSEND=4,%d\r\n", len);
  uart_send_string(len_string);
  _delay_ms(1000);

  uart_send_string(Get_string);
  _delay_ms(3000);
  strcpy(response_buffer, rx_buffer);
  rx_index = 0;


  uart_send_string("AT+CIPCLOSE=5\r\n");
  _delay_ms(1000);
  return response_buffer;
}

void wifi_connect(void)
{
    char cmd[64];

    // Construct Wi-Fi connection command with SSID and password
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", "Pixel-5", "cabhinav");

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

float calc_rms_voltage()
{
    uint32_t sum = 0;
    float rms_voltage = 0;
    uint16_t adc_value;
    for (int i = 0; i < 256; i++) {
        adc_value = adc_read();
        sum += adc_value * adc_value;
        _delay_us(10);
    }
    rms_voltage = sqrt(sum / 256.0);
    return rms_voltage;
}

int main()
{

  
# 241 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino" 3
 __asm__ __volatile__ ("sei" ::: "memory")
# 241 "C:\\Users\\abhin\\OneDrive - IIT Dharwad\\Documents\\GitHub\\Arduino_Smart_meter_EDL\\Thingspeak Interface\\main\\main.ino"
      ;
  uart1_init();
  relay_init();

  // Connect to Wi-Fi
  wifi_connect();
  while(1){
  //Send_to_thingspeak(calc_rms_voltage(),0,0);
  if (Read_from_thingspeak()=='1')
    relay_toggle(1);
  else if (Read_from_thingspeak() == '0')
    relay_toggle(0);
    }
  return 0;

}
