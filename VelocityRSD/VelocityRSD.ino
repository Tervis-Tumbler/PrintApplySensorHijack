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
//bool velocityRead = false;
long minVelocity = 6000;
long maxVelocity = 45000;
int cycleDelay = 1000;
int randomStrokeDelay;
int registerAddress;

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
//  SPI.endTransaction();
}

int getZ(){
//    SPI.beginTransaction(settingsA);
  digitalWrite(spiCableSelect,LOW);

//  Get Z data
  registerAddress = 0x36;
  registerAddress = registerAddress | 0x40; // Multiple bytes
  registerAddress = registerAddress | 0x80; // Read operation
  int a = SPI.transfer(registerAddress);
  int b = SPI.transfer(0x00);
  int c = SPI.transfer(0x00);
//  SPI.endTransaction();
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
  if (input < minVelocity){
    return 150;
  }
  else if (input > minVelocity && input < maxVelocity){
    return map(input,minVelocity,maxVelocity,150,50);
  }
  else {
    return 50;
  }
}

//void beginVelocityRead () {
//  velocityRead = true;
//  digitalWrite(yellowLED,HIGH);
//}

void printStatus(int randomStrokeDelay){
  Serial.print("Random Stroke Delay: ");
  Serial.print(randomStrokeDelay);
  Serial.println(" ms");
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
  delay(randomStrokeDelay);
  digitalWrite(randomStrokeDelayOutputPin,HIGH);
  digitalWrite(redLED,HIGH);
  delay(cycleDelay);
  digitalWrite(yellowLED,LOW);
  digitalWrite(redLED,LOW);
  digitalWrite(randomStrokeDelayOutputPin,LOW);   
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(applicatorHomePositionSensor,INPUT_PULLUP);
  pinMode(boxSensorPin,INPUT_PULLUP);
  pinMode(randomStrokeDelayOutputPin,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  initiateAccelerometer();
  delay(1000);
//  attachInterrupt(digitalPinToInterrupt(applicatorHomePositionSensor),beginVelocityRead,FALLING);
  Serial.println("Ready!");
}

void loop() {
  if (isApplicatorMoving()) {

    
    Serial.println("Collecting acclerometer data...");
    while (!boxDetected()){
      delay(accelerometerSampleDelay);  
      accelerometerCurveArea += (((getZ()*-1)+250) * accelerometerSampleDelay);
//      Serial.println(accelerometerCurveArea);
    }
    
    Serial.println("Box detected!");
    randomStrokeDelay = calculateRandomStrokeDelay(accelerometerCurveArea);
    accelerometerCurveArea = 0;
    printStatus(randomStrokeDelay);
    sendRandomStrokeDelaySignal();
    Serial.println("Waiting...");
  }

  else{
    delay(1);
  }

}
