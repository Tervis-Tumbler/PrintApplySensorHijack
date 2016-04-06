#include <SPI.h>

// Intercepted signals.
int boxSensorPin = 4;
int randomStrokeDelayOutputPin = 5;

// LED status pins
int redLED = 7;
int yellowLED = 6;

// Accelerometer cable select
int spiCableSelect = 10;

long accelerometerCurveArea = 0;
int accelerometerSampleDelay = 5;
int applicatorHomePositionSensor = 2;
int cycleDelay = 2000;
int randomStrokeDelay;
int registerAddress;
long minVelocityToCalculate = 1000;
long maxVelocityToCalculate = 37500;
int maxRandomStrokeDelayTime = 155; // +15 to compensate for overpacked large boxes
int minRandomStrokeDelayTime = 60; // +5 to compensate for overpacked small boxes

SPISettings settingsA(400000,MSBFIRST,SPI_MODE3);

void initiateAccelerometer (){

  SPI.begin();
  pinMode(spiCableSelect,OUTPUT);
  digitalWrite(spiCableSelect,HIGH);

  SPI.beginTransaction(settingsA);
  digitalWrite(spiCableSelect,LOW);

//  PowerCtl 
  registerAddress = 0x2D;
  registerAddress = registerAddress | 0x40;
  SPI.transfer(registerAddress);
  SPI.transfer(0x08);

  digitalWrite(spiCableSelect,HIGH);
  delay(1);
  digitalWrite(spiCableSelect,LOW);

//  Data Format
  registerAddress = 0x31;
  registerAddress = registerAddress | 0x40;
  SPI.transfer(registerAddress);
  SPI.transfer(0x0B);

  digitalWrite(spiCableSelect,HIGH);
  delay(1);
  digitalWrite(spiCableSelect,LOW);

//  DataRate
  registerAddress = 0x2C;
  registerAddress = registerAddress | 0x40;
  SPI.transfer(registerAddress);
  SPI.transfer(0x0D);
  
  digitalWrite(spiCableSelect,HIGH);
  delay(1);
}

int getZ(){
  digitalWrite(spiCableSelect,LOW);

//  Get Z data
  registerAddress = 0x36;
  registerAddress = registerAddress | 0x40; // Multiple bytes
  registerAddress = registerAddress | 0x80; // Read operation
  int a = SPI.transfer(registerAddress);
  int b = SPI.transfer(0x00);
  int c = SPI.transfer(0x00);
  digitalWrite(spiCableSelect,HIGH);

//  Combine bytes
  int result;
  result  = ((c << 8) | b) + 7;
  if ( result > 240 && result < 260 ) {
    return 250;
  }
  else {
  return result;    
  }
}

int calculateRandomStrokeDelay(long input) {
  if (input < minVelocityToCalculate){
    return maxRandomStrokeDelayTime;
  }
  else if (input > minVelocityToCalculate && input < maxVelocityToCalculate){
    return map(input,minVelocityToCalculate,maxVelocityToCalculate,maxRandomStrokeDelayTime,minRandomStrokeDelayTime);
  }
  else {
    return minRandomStrokeDelayTime;
  }
}

void printStatus(int randomStrokeDelay, long accelerometerCurveArea){
  Serial.print("Random Stroke Delay: ");
  Serial.print(randomStrokeDelay);
  Serial.println(" ms");
  Serial.print("Velocity: ");
  Serial.println(accelerometerCurveArea);
  Serial.println("");
}

bool isApplicatorMoving () {
  if (digitalRead(applicatorHomePositionSensor)){
    digitalWrite(yellowLED,HIGH);
    return true;
  }
  else {
    return false;
  }
}

bool boxDetected() {
  if (digitalRead(boxSensorPin)) {
    return false;
  }
  else {
    return true;
  }
}

void sendRandomStrokeDelaySignal () {
  digitalWrite(redLED,HIGH);
  delay(randomStrokeDelay);
  digitalWrite(randomStrokeDelayOutputPin,HIGH);
  delay(cycleDelay);
  digitalWrite(yellowLED,LOW);
  digitalWrite(redLED,LOW);
  digitalWrite(randomStrokeDelayOutputPin,LOW);   
}

void printSettings() {
  Serial.println("");
  Serial.print("Minimum Velocity: ");
  Serial.println(minVelocityToCalculate);
  Serial.print("Maximum Velocity: ");
  Serial.println(maxVelocityToCalculate);
  Serial.print("Minimum Delay Time: ");
  Serial.println(minRandomStrokeDelayTime);  
  Serial.print("Maximum Delay Time: ");
  Serial.println(maxRandomStrokeDelayTime);
  Serial.println("");
  
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(applicatorHomePositionSensor,INPUT_PULLUP);
  pinMode(boxSensorPin,INPUT_PULLUP);
  pinMode(randomStrokeDelayOutputPin,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  initiateAccelerometer();
  printSettings();
  Serial.println("Ready!");
}

void loop() {
  if (isApplicatorMoving()) {
    Serial.println("Collecting accelerometer data...");
    while (!boxDetected()){
      delay(accelerometerSampleDelay);  
      accelerometerCurveArea += (((getZ()*-1)+250) * accelerometerSampleDelay);
    }
    
    Serial.println("Box detected!");
    randomStrokeDelay = calculateRandomStrokeDelay(accelerometerCurveArea);
    printStatus(randomStrokeDelay,accelerometerCurveArea);
    sendRandomStrokeDelaySignal();
    accelerometerCurveArea = 0;
    Serial.println("Waiting...");
  }

  else{
    delay(1);
  }

}
