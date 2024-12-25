#include "lora.h"
#include <rn2xx3.h>
#include <SoftwareSerial.h>

// Define a software serial interface for communication with RN2xx3 module
SoftwareSerial mySerial(ARDUINO_TO_RN_RX, ARDUINO_TO_RN_TX); // RX, TX

// Create an instance of the rn2xx3 class for LoRa communication
rn2xx3 myLora(mySerial);

// Function to initialize LoRa communication
void lora_init(void)
{
  // Begin serial communication with the RN2xx3 module
  mySerial.begin(RN_BAUDRATE);
  Serial.println("Startup");

  // Initialize the radio module
  initialize_radio();

  // Transmit a startup message to the LoRa network
  myLora.tx("TTN Mapper on TTN Enschede node");
}

// Function to initialize the radio module
void initialize_radio(void)
{
  // Reset the RN2xx3 module by controlling the reset pin
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);

  delay(100); // Wait for the RN2xx3's startup message
  mySerial.flush();

  // Autobaud the RN2xx3 module to 9600 baud. Default is 57600 baud.
  myLora.autobaud();

  // Check communication with the radio module
  String hweui = myLora.hweui();
  while (hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(1000);
    hweui = myLora.hweui();
  }

  // Print out the HWEUI for registration via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  // Configure keys and join the LoRa network
  Serial.println("Trying to join TTN");
  bool join_result = false;

  /*
   * OTAA: initOTAA(String AppEUI, String AppKey);
   * If you are using OTAA, paste the example code from the TTN console here:
   */

  join_result = myLora.initOTAA("xxx", "xxx");

  if (join_result)
    Serial.println("Successfully joined helium");
}
