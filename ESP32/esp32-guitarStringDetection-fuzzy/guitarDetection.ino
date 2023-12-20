#include "arduinoFFT.h"
#include <Stepper.h>

// Define the number of steps per revolution
const int stepsPerRevolution = 256;

// Define the motor control pins
const int motorPin1 = 12;
const int motorPin2 = 13;
const int motorPin3 = 14;
const int motorPin4 = 27;

// Initialize the stepper library on the motor control pins
Stepper myStepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);

#define SAMPLES 128
#define SAMPLING_FREQUENCY 2048

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  Serial.begin(115200);
  samplingPeriod = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
  myStepper.setSpeed(32);  // RPM
}

void loop() {
  for (int i = 0; i < SAMPLES; i++) {
    microSeconds = micros();

    vReal[i] = analogRead(34);
    vImag[i] = 0;

    while (micros() < (microSeconds + samplingPeriod)) {
      delay(1);
    }
  }
  // Perform FFT on samples
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  // Find peak frequency and print peak
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  if (peak > 570 && peak < 680) {
    checkeString(peak);
  } else if (peak > 465 && peak < 511) {
    checkBString(peak);
  } else if (peak > 800 && peak < 810) {
    checkGString(peak);
  } else if (peak > 275 && peak < 305) {
    checkDString(peak);
  } else if (peak > 410 && peak < 455) {
    checkAString(peak);
  } else if (peak > 310 && peak < 340) {
    checkEString(peak);
  } else {
    Serial.print("No Detection: ");
    Serial.println(peak);
  }
  delay(100);
}

void checkeString(double peak) {
  while (peak > 570 && peak < 680) {
    for (int waitSecond = 0; waitSecond < 3; waitSecond++) {
      for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();

        vReal[i] = analogRead(34);
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod)) {
          delay(1);
        }
      }
      // Perform FFT on samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      if (peak > 570 && peak < 685) {
        Serial.print("e String detected, Frequency: ");
        Serial.print(peak);
        Serial.println(" Hz || Tune to 679 Hz");
        if (peak < 676) {
          Serial.println("=== Tune UP === ");
          myStepper.step(-stepsPerRevolution);
          delay(500);
        } else if (peak > 680) {
          Serial.println("=== Tune DOWN === ");
          myStepper.step(stepsPerRevolution);
          delay(500);
        } else if (peak > 676 && peak < 680) {
          Serial.println("=== Perfect ====");
        }
      } else {
        break;
      }
      delay(500);
    }
  }
}

void checkBString(double peak) {
  while (peak > 465 && peak < 511) {
    for (int waitSecond = 0; waitSecond < 3; waitSecond++) {
      for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();

        vReal[i] = analogRead(34);
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod)) {
          delay(1);
        }
      }
      // Perform FFT on samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      if (peak > 465 && peak < 511) {
        Serial.print("B String detected, Frequency: ");
        Serial.print(peak);
        Serial.println(" Hz || Tune to 508 Hz");
        if (peak < 505) {
          Serial.println("=== Tune UP === ");
          myStepper.step(-stepsPerRevolution);
          delay(500);
        } else if (peak > 509) {
          Serial.println("=== Tune DOWN === ");
          myStepper.step(stepsPerRevolution);
          delay(500);
        } else if (peak > 505 && peak < 509) {
          Serial.println("=== Perfect ====");
        }
      } else {
        break;
      }
      delay(500);
    }
  }
}

void checkGString(double peak) {
  while (peak > 800 && peak < 810) {
    for (int waitSecond = 0; waitSecond < 3; waitSecond++) {
      for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();

        vReal[i] = analogRead(34);
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod)) {
          delay(1);
        }
      }
      // Perform FFT on samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      if (peak > 800 && peak < 810) {
        Serial.print("G String detected, Frequency: ");
        Serial.print(peak);
        Serial.println(" Hz || Tune to 807 Hz");
        if (peak < 805) {
          Serial.println("=== Tune UP === ");
          myStepper.step(-stepsPerRevolution);
          delay(500);
        } else if (peak > 808) {
          Serial.println("=== Tune DOWN === ");
          myStepper.step(stepsPerRevolution);
          delay(500);
        } else if (peak > 805 && peak < 808) {
          Serial.println("=== Perfect ====");
        }
      } else {
        break;
      }
      delay(500);
    }
  }
}

void checkDString(double peak) {
  while (peak > 275 && peak < 305) {
    for (int waitSecond = 0; waitSecond < 3; waitSecond++) {
      for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();

        vReal[i] = analogRead(34);
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod)) {
          delay(1);
        }
      }
      // Perform FFT on samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      if (peak > 275 && peak < 305) {
        Serial.print("D String detected, Frequency: ");
        Serial.print(peak);
        Serial.println(" Hz || Tune to 303 Hz");
        if (peak < 301) {
          Serial.println("=== Tune UP === ");
          myStepper.step(-stepsPerRevolution);
          delay(500);
        } else if (peak > 304) {
          Serial.println("=== Tune DOWN === ");
          myStepper.step(stepsPerRevolution);
          delay(500);
        } else if (peak > 301 && peak < 304) {
          Serial.println("=== Perfect ====");
        }
      } else {
        break;
      }
      delay(500);
    }
  }
}

void checkAString(double peak) {
  while (peak > 410 && peak < 455) {
    for (int waitSecond = 0; waitSecond < 3; waitSecond++) {
      for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();

        vReal[i] = analogRead(34);
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod)) {
          delay(1);
        }
      }
      // Perform FFT on samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      if (peak > 410 && peak < 455) {
        Serial.print("A String detected, Frequency: ");
        Serial.print(peak);
        Serial.println(" Hz || Tune to 451 Hz");
        if (peak < 448) {
          Serial.println("=== Tune UP === ");
          myStepper.step(-stepsPerRevolution);
          delay(500);
        } else if (peak > 452) {
          Serial.println("=== Tune DOWN === ");
          myStepper.step(stepsPerRevolution);
          delay(500);
        } else if (peak > 448 && peak < 452) {
          Serial.println("=== Perfect ====");
        }
      } else {
        break;
      }
      delay(500);
    }
  }
}

void checkEString(double peak) {
  while (peak > 310 && peak < 340) {
    for (int waitSecond = 0; waitSecond < 3; waitSecond++) {
      for (int i = 0; i < SAMPLES; i++) {
        microSeconds = micros();

        vReal[i] = analogRead(34);
        vImag[i] = 0;

        while (micros() < (microSeconds + samplingPeriod)) {
          delay(1);
        }
      }
      // Perform FFT on samples
      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      if (peak > 310 && peak < 340) {
        Serial.print("E String detected, Frequency: ");
        Serial.print(peak);
        Serial.println(" Hz || Tune to 338 Hz");
        if (peak < 336) {
          Serial.println("=== Tune UP === ");
          myStepper.step(-stepsPerRevolution);
          delay(500);
        } else if (peak > 340) {
          Serial.println("=== Tune DOWN === ");
          myStepper.step(stepsPerRevolution);
          delay(500);
        } else if (peak > 336 && peak < 340) {
          Serial.println("=== Perfect ====");
        }
      } else {
        break;
      }
      delay(500);
    }
  }
}