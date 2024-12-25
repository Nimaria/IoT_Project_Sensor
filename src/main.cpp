#include <Arduino.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include "main.h"
#include "ADXL345.h"
#include "lora.h"
#include "sampling.h"

/*
    2B - accelerometer read value
    4B - microphone read value
*/

// Define an array to hold the data to be transmitted
uint8_t data_buffer[6];

// Variable to store accelerometer reading
int read_value;

// Variable to store microphone reading
unsigned long sample;

void setup() {
    // Initialize serial communication at 9600 baud with 8N1 configuration
    Serial.begin(9600, SERIAL_8N1);

    // Initialize LoRa communication
    lora_init();

    // Initialize the sampling subsystem
    sampling_init();

    // Initialize the accelerometer
    adxl_init();
}

// Function to restart the device (currently empty)
static void restart_device(void)
{
    // You can add custom actions here before restarting the device
    // For example, you may want to close any open connections or perform cleanup

    // Perform a soft restart using the watchdog timer
    cli();  // Disable interrupts
    wdt_enable(WDTO_15MS);  // Enable the watchdog timer with a 15ms timeout
    while (1);  // Wait for the watchdog timer to reset the device
}

void loop() {
    // Read accelerometer data and store it in 'read_value'
    read_accelerometer(&read_value);

    // Measure loudness using the microphone and store the result in 'sample'
    sample = measureLoudness();

    // Print accelerometer and microphone values to the serial monitor for debugging
    Serial.print("accelerometer value: ");
    Serial.println(read_value);
    Serial.print("mic value: ");
    Serial.println(sample);

    // Pack the data into the 'data_buffer' for transmission
    data_buffer[0] = (uint8_t)(read_value & 0x00FF);  // Lower byte of 'read_value'
    data_buffer[1] = (uint8_t)((read_value & 0xFF00) >> 8);  // Upper byte of 'read_value'
    data_buffer[2] = (uint8_t)(sample & 0x000000FF);  // First byte of 'sample'
    data_buffer[3] = (uint8_t)((sample & 0x0000FF00) >> 8);  // Second byte of 'sample'
    data_buffer[4] = (uint8_t)((sample & 0x00FF0000) >> 16);  // Third byte of 'sample'
    data_buffer[5] = (uint8_t)((sample & 0xFF000000) >> 24);  // Fourth byte of 'sample'

    // Transmit the data using LoRa communication
    myLora.txBytes(data_buffer, sizeof(data_buffer));

    // Delay before the next transmission (controlled by 'TRANSMISION_DELAY_S' constant)
    delay(TRANSMISION_DELAY_S * 1e3);

    // Check if there is any received data via LoRa
    if (myLora.getRx().length() != 0)
        restart_device(); // If data received, restart the device (you can add actions here)
}
