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
// to detect a small box.
int smallBox = 6;

// Time to delay signal in milliseconds.
int signalDelay = 300;

// Time to delay until next cycle in milliseconds.
int cycleDelay = 1000;

// Create ultrasonic sensor object.
Ultrasonic ultrasonic(usTrigger,usEcho);

void setup() {
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
    
    // If a small box is detected, pass through
    // box sensor signal to PrintApply.
    else if (distance > smallBox) {
      digitalWrite(redLED,HIGH);
      digitalWrite(strokeOutput,HIGH);
      delay(1000);
      digitalWrite(strokeOutput,LOW);
      digitalWrite(redLED,LOW);
      }
    
    // Otherwise, assume large box.
    else {
      digitalWrite(yellowLED,HIGH);
      delay(signalDelay);
      digitalWrite(strokeOutput,HIGH);
      delay(cycleDelay);
      digitalWrite(strokeOutput,HIGH);
      digitalWrite(yellowLED,LOW);
      }
    }
  delay(25);
}
