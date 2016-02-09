#include <Ultrasonic.h>

// Intercepted signals.
int sensorSwitch = 12;
int strokeOutput = 13;

// Ultrasonic sensor pins.
int usVCC = 4;
int usGND = 7;
int usTrigger = 5;
int usEcho = 6;

// LED status pins
int redLED = 11;
int yellowLED = 10;

// Time to delay until next cycle in milliseconds.
int cycleDelay = 1000;

// Time between US pulses in milliseconds
float timeSample = .060;

// Time out for detecting echo in microseconds
int timeOut = 30000;

// Initialize initial velocity variable
float velocityInit;

// Velocity of sound constant in meters per second
float velocitySound = 340;

// Acceleration of application reverse, m/s^2
// Vary this amount. If smashing, reduce. If not
// touching, increase.
float a = 4.6;

float timePulse1;
float timePulse2;

Ultrasonic ultrasonic(usTrigger,usEcho,timeOut);

void setup(){
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

void loop(){
  if (!digitalRead(sensorSwitch)){
    timePulse1 = float(ultrasonic.Timing())/1000000;
    Serial.print("Time Pulse 1: ");
    Serial.println(timePulse1,10);
    delay(timeSample);
    timePulse2 = float(ultrasonic.Timing())/1000000;
    Serial.print("Time Pulse 2: ");
    Serial.println(timePulse2,10);
    float diff = timePulse2 - timePulse1;
    Serial.print("Difference: ");
    if (diff > 0) {
      digitalWrite(yellowLED,HIGH);
    }
    else if (diff < 0) {
      digitalWrite(redLED,HIGH);
    }
    Serial.println(diff,10);
    // Insane math to calculate velocity with two ultrasonic pulses
    velocityInit = (velocitySound*(timePulse2-timePulse1))/(2*timeSample);
    Serial.print("Velocity: ");
    Serial.println(velocityInit,10);
    Serial.println("");
    delay(20);
    digitalWrite(strokeOutput,HIGH);
    delay(10);
    digitalWrite(strokeOutput,LOW);
    delay(cycleDelay-500);
    digitalWrite(redLED,LOW);
    digitalWrite(yellowLED,LOW);
  }
  delay(1);
}

