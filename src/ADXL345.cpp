#include <SparkFun_ADXL345.h>         // Include the SparkFun ADXL345 Library
#include "ADXL345.h"

/*********** COMMUNICATION SELECTION ***********/
//ADXL345 adxl = ADXL345(10);           // USE FOR SPI COMMUNICATION, ADXL345(CS_PIN);
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION
static int lastX, lastY, lastZ;        // Static variables to hold the last acceleration values
int x, y, z;

/******************** SETUP ********************/
void adxl_init(void){  
  adxl.powerOn();                     // Power on the ADXL345
  adxl.setFullResBit(true);           // Use 13-bit resolution

  adxl.setRangeSetting(2);            // Set the measurement range to 2g
                                      // Accepted values are 2g, 4g, 8g, or 16g
                                      // Higher values = wider measurement range
                                      // Lower values = greater sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4-wire SPI mode when set to '0' or 3-wire SPI mode when set to '1'
                                      // Default: Set to '1'
                                      // SPI pins on the ATMega328: 11, 12, and 13 as referenced in the SPI Library 
  
  adxl.setActivityXYZ(1, 1, 1);       // Enable movement detection in all axes
  adxl.setActivityThreshold(10);      // Set activity threshold (62.5mg per increment)

  adxl.setInactivityXYZ(1, 0, 0);     // Enable inactivity detection in the X-axis
  adxl.setInactivityThreshold(75);    // Set inactivity threshold (62.5mg per increment)
  adxl.setTimeInactivity(10);         // Set the time required for inactivity detection

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Enable tap detection in the Z-axis
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);

  // You can attach an interrupt here if needed
}

/****************** MAIN CODE ******************/
/*     Accelerometer Readings and Interrupt    */
void read_accelerometer(int* maxDelta) {
  
  // Accelerometer Readings
  adxl.readAccel(&x, &y, &z); // Read the accelerometer values and store them in x, y, z

  // Calculate the deltas for each axis
  int deltaX = abs(x - lastX);
  int deltaY = abs(y - lastY);
  int deltaZ = abs(z - lastZ);

  // Define a threshold for detecting significant change
  int threshold = 10; // Replace this with your desired threshold value

  // Find the maximum delta that is above the threshold
  if(deltaX > threshold) *maxDelta = deltaX;
  if(deltaY > threshold && deltaY > *maxDelta) *maxDelta = deltaY;
  if(deltaZ > threshold && deltaZ > *maxDelta) *maxDelta = deltaZ;

  // Update the last acceleration values for the next loop iteration
  lastX = x;
  lastY = y;
  lastZ = z;
}
