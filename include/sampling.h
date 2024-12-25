#ifndef SAMPLING_H
#define SAMPLING_H

// Define the sound sensor pin
#define soundSensorPin A0

// Define the sensitivity threshold for sound detection
#define soundSensitivity 10

// Define the duration for loudness measurement (in milliseconds)
#define sampleTime 100

// Define the number of sensitivity samples used for calibration
#define sensitivitySamples 20

// Function prototype to initialize the sampling subsystem
void sampling_init(void);

// Function prototype to measure loudness
unsigned long measureLoudness(void);

#endif
