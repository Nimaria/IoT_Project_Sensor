//https://github.com/kosme/arduinoFFT/blob/master/README.md

/*
  In this example, the Arduino simulates the sampling of a sinusoidal 1000 Hz
  signal with an amplitude of 100, sampled at 5000 Hz. Samples are stored
  inside the vReal array. The samples are windowed according to Hamming
  function. The FFT is computed using the windowed samples. Then the magnitudes
  of each of the frequencies that compose the signal are calculated. Finally,
  the frequency with the highest peak is obtained, being that the main frequency
  present in the signal.
*/



#include "arduinoFFT.h"


//"The main component of the signal band when the vehicle passed was from 1 to 2 KHz" (for low to moderate speed), according https://sensors.myu-group.co.jp/sm_pdf/SM2434.pdf
#define SAMPLING_FREQUENCY 5000//might be a bit short, but 
/*The Arduino Pro Mini is typically based on the ATmega328P microcontroller, which has a maximum analog-to-digital
 conversion rate of about 10,000 to 15,000 samples per second. However, the effective sampling rate can be lower due to the processing 
 overhead and the specific settings used in your code.
*/


//to test our code
#define SIGNAL_FREQUENCY 1000
#define AMPLITUDE 100

#define FFT_N 256//must be a power of two
//that gives a frequency resolution of SAMPLING_Frequency/FFT_N !!





arduinoFFT FFT; // the object which will perform the FFT, initialized later

double vReal[FFT_N];
double vImag[FFT_N];

//no idea 
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03





void setup() {
  
//register acceloremeter pin as input

  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
  Serial.println("Frequency Resolution is:"+String(SAMPLING_FREQUENCY/FFT_N));
}

void loop() {
  // Collect and populate vReal with your data samples here.
  //Build Raw Data in this case
  double cycles=(((FFT_N-1) * SIGNAL_FREQUENCY) / SAMPLING_FREQUENCY); //Number of signal cycles that the sampling will read
  for (uint16_t i = 0; i < FFT_N; i++)
  {
    vReal[i] = int8_t((AMPLITUDE * (sin((i * (2*PI * cycles)) / FFT_N))) / 2.0);/* Build data with positive and negative values*/
    //vReal[i] = uint8_t((AMPLITUDE * (sin((i * (2*PI * cycles)) / FFT_N) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }

  FFT = arduinoFFT(vReal, vImag, FFT_N, SAMPLING_FREQUENCY); /* Create FFT object */

/* Print the results of the simulated sampling according to time */
  Serial.println("Data:");
  PrintVector(vReal, FFT_N, SCL_TIME);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
  Serial.println("Weighed data:");
  PrintVector(vReal, FFT_N, SCL_TIME);
  FFT.Compute(FFT_FORWARD); /* Compute FFT */
  Serial.println("Computed Real values:");
  PrintVector(vReal, FFT_N, SCL_INDEX);
  Serial.println("Computed Imaginary values:");
  PrintVector(vImag, FFT_N, SCL_INDEX);
  FFT.ComplexToMagnitude(); /* Compute magnitudes */
  Serial.println("Computed magnitudes:");
  PrintVector(vReal, (FFT_N >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak();
  Serial.println(x, 6);
  while(1); /* Run Once */
  // delay(2000); /* Repeat after delay */

}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{//Print the Data according the view desired: Normal, Time or Frequency (scaleType)
//Buffer Size is the number of samples 

  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / SAMPLING_FREQUENCY);
	break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * SAMPLING_FREQUENCY) / FFT_N);
	break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
