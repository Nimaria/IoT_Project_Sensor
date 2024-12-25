//see:
////https://github.com/kosme/arduinoFFT/blob/master/README.md


//You must add the arduino FFT library with the Library Manager
#include "arduinoFFT.h"

struct FFT_OUT{
    double* frequency;//frequency array
    double* magnitude;//magnitude Array
    double frequencyPeak;//frequency of the main component of the FFT
    double maxMagnitude;
};


FFT_OUT fromArrayToMagnitudeFFTComponent(int samplingFrequency, int samples, double data[]){


 double vImag[samples];
   for (uint16_t i = 0; i < samples; i++)
  {
    //Serial.println(data[i]);
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
   } 
   //FFT OPERATIONS----------------------------------------------------------------------------------------------
   arduinoFFT FFT=arduinoFFT(data,vImag,samples,samplingFrequency);/* Create FFT object */
   FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data: basic FFT parameters, we could eventually improve this to fit better our case */
    FFT.Compute(FFT_FORWARD); /* Compute FFT */
    FFT.ComplexToMagnitude(); /* Compute magnitudes */
   //------------------------------------------------------------------------------------------------------------- 




    //we build the array Frequency:-------------------------------------------------------------------------------
    double freqArray[samples>>1];//(samples>>1)=round(samples/2), linked to shannon Theorem
    double freqPeak=0;
    double max=0;

    /*DEBUG
    Serial.println("These are the Frequency axis produced");
    Serial.println(samplingFrequency);
    Serial.println(samples);
    Serial.println((samples>>1));
    Serial.println(9*samplingFrequency/samples);
    */
    for(int i=0;i<(samples>>1);i++){
      freqArray[i]=static_cast<double>(i)*samplingFrequency/samples;//to avoid integer overflow with division
      if(max<data[i]){
        max=data[i];
        freqPeak=freqArray[i];
      }
      
    }
    //-----------------------------------------------------------------------------------------------------------------

  FFT_OUT FunctionOut = {freqArray,data,freqPeak,max};
return FunctionOut;

}


void setup() {
 Serial.begin(115200);



}

void loop() {
//SAMPLES IS THE SIZE OF THE ARRAY GIVEN BY THE ACCELEROMETER WE PROCESS AT EACH UNIT OF TIME
const uint16_t samples = 254; //This value MUST ALWAYS be a power of 2


//test case:
//we generate an artificial sinus  of frequency signalFrequency--------------------------------------------------------------------------------
const double signalFrequency = 1000;
const double samplingFrequency = 10000;
const uint8_t amplitude = 100;
double vReal[samples];
double cycles = (((samples-1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
  for (uint16_t i = 0; i < samples; i++)
  {
    vReal[i] = int8_t((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);/* Build data with positive and negative values*/
    //vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
   } 
  
  //-----------------------------------------------------------------------------------------------------------------
  
  
  FFT_OUT out=fromArrayToMagnitudeFFTComponent(samplingFrequency,samples, vReal);
   //TO send to server:
    Serial.println("Frequency of High Magnitude");
    Serial.println(out.frequencyPeak);
    Serial.println("Max Magnitude");
    Serial.println(out.maxMagnitude);

    //To see FFT Arrays:
    Serial.println("");
    Serial.println("ARRAY BELOW");
    Serial.println("");
    for(int i=0;i<(samples>>1);i++){
    Serial.print(out.frequency[i]);
    Serial.print("Hz   ");
    Serial.println(out.magnitude[i]);
    }
  
  
  while(1);//we end there

}
