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
int mediumBoxDelay = 70;

// Time to delay small box in milliseconds.
int smallBoxDelay = 50;

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
    
    Serial.print("Distance: ");
    Serial.print(distance);

    // Do nothing if at rest.
    if (distance < largeBox) {}
    
    // If a small box is detected, delay signal according
    // to smallBoxDelay.
    else if (distance > smallBox) {
      randomStrokeDelay(smallBoxDelay,redLED);
      Serial.println("   Small");
      }
      
    // If a medium box is detected, delay signal according
    // to mediumBoxDelay.
    else if (distance > mediumBox) {
      randomStrokeDelay(mediumBoxDelay,yellowLED);
      Serial.println("   Med");
    }
    
    // Otherwise, assume large box.
    else {
      randomStrokeDelay(largeBoxDelay,yellowLED);
      Serial.println("   Large");
      }
    Serial.println("");
    }
  delay(1);
}
