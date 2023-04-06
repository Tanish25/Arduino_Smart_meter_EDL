// Include necessary libraries
#include <TinyGPSPlus.h>
#include <TinyGPS++.h>
#include <Servo.h>

// Create a Servo object
Servo myservo;

// Define trigger and echo pins for ultrasonic sensor
const int triggerPin = 11;
const int echoPin = 10;

// Declare variables for measuring distance, height, and volume
float pulse_width, distance, height, volume;

// Create a TinyGPS++ object
TinyGPSPlus gps;

void setup() {
  // Start serial communication with computer
  Serial.begin(9600);

  // Start software serial for GPS module
  Serial1.begin(9600);

  // Start software serial for GSM module
  Serial2.begin(9600);

  // Send AT command to check if the GSM module is working
  Serial2.println("AT");
  delay(1000);

  // Set trigger pin as output and echo pin as input for ultrasonic sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Send AT commands to configure GSM module
  Serial2.println("AT+CMGF=1");  // Set SMS text mode
  delay(1000);
  Serial2.println("AT+CNMI=2,2,0,0,0");  // Set module to automatically receive SMS
  delay(1000);

  // Print "setup" message to serial monitor
  Serial.println("setup");
}

void loop() {
  // Check if there is an incoming SMS message
  if (Serial2.available()) {
    // Send AT commands to configure GSM module for receiving SMS
    Serial2.println("AT+CMGF=1");  // Set SMS text mode
    delay(1000);
    Serial2.println("AT+CNMI=2,2,0,0,0");  // Set module to automatically receive SMS
    pinMode(2, OUTPUT);
    delay(1000);
  }

  // If there is an incoming SMS message
  if (Serial2.available()) {
    delay(500);
    String message = "";
    while (Serial2.available()) {
      message += (char)Serial2.read();
    }
    // Print the message to serial monitor
    Serial.println("SMS Received: " + message);

    // If the message contains "Lock", activate the servo motor to lock the container
    if (message.indexOf("Lock") > 0) {
      // myservo.write(90); // rotate the servo motor to 180 degree
      Serial.println("open");
      digitalWrite(2, HIGH);

      // Send a confirmation SMS message to the sender
      Serial2.println("AT+CMGS=\"+919140886252\"\r");  // Replace x with mobile number
      delay(1000);
      Serial2.println("Closed");  // The SMS text you want to send
      delay(100);
      Serial2.println((char)26);  // ASCII code of CTRL+Z for saying the end of sms to  the module
      delay(1000);
    }

    if (message.indexOf("Unlock") > 0) {
      myservo.write(0);  // rotate the servo motor to 0 degree
      Serial.println("close");
      digitalWrite(2, LOW);

      Serial2.println("AT+CMGS=\"+919398060764\"\r");  // Replace x with mobile number
      delay(1000);
      Serial2.println("Opened");  // The SMS text you want to send
      delay(100);
      Serial2.println((char)26);  // ASCII code of CTRL+Z for saying the end of sms to  the module
      delay(1000);
    }
    Serial1.begin(9600);
    // If the message contains "Gps", get GPS location and send it back as an SMS message
    if (message.indexOf("Gps") > 0) {
      Serial.println("connected");
      delay(500);
      Serial.println(Serial1.available());
      // This sketch displays information every time a new sentence is correctly encoded.
      while (Serial1.available() > 0) {
        delay(500);
        if (gps.encode(Serial1.read())) {
          delay(500);
          displayInfo();
        }
      }
         //   // If 5000 milliseconds pass and there are no characters coming in
    //   // over the software serial port, show a "No GPS detected" error
      if (millis() > 100000 && gps.charsProcessed() < 10)
      {
        Serial.println("No GPS detected");
        while(true);
      }

    }

    // If 5000 milliseconds pass and there are no
  }


}
void displayInfo() {
  delay(5000);
  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  } else {
    Serial.println("Location: Not Available");
  }

  delay(1000);
}
