#include "Arduino.h"
#include "sampling.h"

// Initialize a variable to store the reference loudness value
int loudnessRef = 0;

// Function to initialize the sampling subsystem
void sampling_init(void) {
  // Set the sound sensor pin as an input
  pinMode(soundSensorPin, INPUT);
  
  // Initialize loudnessRef by averaging readings from the sound sensor over a period
  for (byte i = 0; i < sensitivitySamples; i++) {
    loudnessRef = loudnessRef + analogRead(soundSensorPin); // Read analog value from sound sensor
    delay(10); // Delay for stability
    Serial.println(loudnessRef); // Print the intermediate loudnessRef values (for debugging)
  }
  loudnessRef = loudnessRef / sensitivitySamples; // Calculate the average loudness reference value
}

// Function to measure loudness over a specific duration
unsigned long measureLoudness(void) {
  unsigned long loudnessMeas = 0;
  unsigned long millisLast = 0;
  unsigned long millisElapsed = 0;
  
  millisLast = millis(); // Record the current time in milliseconds
  
  // Measure loudness over a specified sampleTime duration
  while (millisElapsed < sampleTime) {
    int currentLoudness = abs(analogRead(soundSensorPin)); // Read current loudness value from the sound sensor
    
    // Check if the current loudness exceeds the reference loudness by a certain sensitivity value
    if (currentLoudness - loudnessRef > soundSensitivity) {
      loudnessMeas += currentLoudness - loudnessRef; // Accumulate the difference in loudness values
    }
    
    millisElapsed = millis() - millisLast; // Calculate elapsed time
  }
  return loudnessMeas; // Return the accumulated loudness measurement
}
