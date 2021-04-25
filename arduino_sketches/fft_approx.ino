#include <arduinoFFT.h>

#include "arduinoFFT.h"
 
#define SAMPLES 256             //Must be a power of 2
#define SAMPLING_FREQUENCY 5000 //Hz, must be less than 10000 due to ADC
#define SOUND_PIN A8
 
/*
 * Samra Kasim
 * Computer Architecture EN605.611.8VL2
 * Arduino sound detection project with FastLED and sound sensor
 * April 25, 2021
 * The following code takes a sample of data from an analog SOUND_PIN and peforms FFT on the input. The algorithm code is from: 
 * https://www.norwegiancreations.com/2017/08/what-is-fft-and-how-can-you-implement-it-on-an-arduino/
 */
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup() {
  delay(3000);
  Serial.begin(115200);
 
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}
 
void loop() {
   
    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(SOUND_PIN);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD); /* Compute FFT */
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);  /* Computer magnitudes */
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/
//    Serial.println(peak);     //Print out what frequency is the most dominant.

    for(int i=0; i<(SAMPLES/2); i++)
    {
        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
         
        Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        Serial.print(" ");
        Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
    }

 
//    delay(1000);  //Repeat the process every second OR:
    while(1);       //Run code once
}
