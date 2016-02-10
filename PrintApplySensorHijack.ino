#include <Ultrasonic.h>

// Intercepted signals.
int sensorSwitch = 12;
int strokeOutput = 13;

// Ultrasonic sensor pins.
int usVCC = 4;
int usGND = 7;
int usTrigger = 5;
int usEcho = 6;

// Initialize distance variables.
int distance;

// LED status pins
int redLED = 11;
int yellowLED = 10;

// Minimum distance in inches from ultrasonic sensor
// to detect a large box.
int largeBox = 3;

// Minimum distance in inches from ultrasonic sensor
// to detect a medium box.
int mediumBox = 6;

// Minimum distance in inches from ultrasonic sensor
// to detect a small box.
int smallBox = 10;

// Time to delay large box in milliseconds.
int largeBoxDelay = 100;

// Time to delay medium box in milliseconds.
int mediumBoxDelay = 65;

// Time to delay small box in milliseconds.
int smallBoxDelay = 53;

// Time to delay until next cycle in milliseconds.
int cycleDelay = 1000;

// Create ultrasonic sensor object.
Ultrasonic ultrasonic(usTrigger,usEcho);

void randomStrokeDelay (int timeDelay, int led){
  digitalWrite(led,HIGH);
  delay(timeDelay);
  digitalWrite(strokeOutput,HIGH);
  delay(cycleDelay);
  digitalWrite(strokeOutput,LOW);
  digitalWrite(led,LOW);
  }

void consoleOutput (int d, int box) {
  // Used for debugging. Shows distance of applicator
  // from ultrasonic sensor when box sensor is triggered.
  Serial.print("Distance: ");
  Serial.print(d);
  switch (box) {
    case 1: // Small box
      Serial.println("   Small");
      break;
    case 2: // Medium box
      Serial.println("   Medium");
      break;
    case 3: // Large box
      Serial.println("   Large");
      break;
    Serial.println("");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(sensorSwitch,INPUT_PULLUP);
  pinMode(strokeOutput,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  pinMode(usVCC,OUTPUT);
  pinMode(usGND,OUTPUT);
  digitalWrite(usVCC,HIGH);
  digitalWrite(usGND,LOW);
}

void loop() {
  // Check to see if box sensor was triggered
  if (!digitalRead(sensorSwitch)){
    
    // Grab distance to applicator plate.
    distance = ultrasonic.Ranging(INC);

    // Do nothing if at rest.
    if (distance < largeBox) {}
    
    // If a small box is detected, delay signal according
    // to smallBoxDelay.
    else if (distance > smallBox) {
      randomStrokeDelay(smallBoxDelay,redLED);
      consoleOutput (distance, 1);
    }
      
    // If a medium box is detected, delay signal according
    // to mediumBoxDelay.
    else if (distance > mediumBox) {
      randomStrokeDelay(mediumBoxDelay,yellowLED);
      consoleOutput (distance, 2);
    }
    
    // Otherwise, assume large box.
    else {
      randomStrokeDelay(largeBoxDelay,yellowLED);
      consoleOutput (distance, 3);
    }
  }
  delay(1);
}
