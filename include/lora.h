#ifndef LORA_H
#define LORA_H

#include "rn2xx3.h"

// Define Arduino pin numbers for RN2xx3 module communication
#define ARDUINO_TO_RN_TX 11
#define ARDUINO_TO_RN_RX 10

// Define the baud rate for communication with the RN2xx3 module
#define RN_BAUDRATE 9600

// Define the Application EUI and Application Key for LoRa communication
#define APPEUI "6081F929A37D7466"
#define APPKEY "F645CEA709ABC21D1E9763C78CCE53BF"

// Declare an external instance of the rn2xx3 class
extern rn2xx3 myLora;

// Function prototypes for LoRa initialization and radio configuration
void lora_init(void);
void initialize_radio(void);

#endif
