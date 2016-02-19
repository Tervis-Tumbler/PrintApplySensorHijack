#include <SPI.h>

// Intercepted signals.
int sensorSwitch = 4;
int strokeOutput = 5;

// LED status pins
int redLED = 7;
int yellowLED = 6;

// Accelerometer cable select
int CS = 10;

long area = 0;
int tDelay = 5;
int applyStart = 2;
bool velocityRead = false;
long minVelocity = 6000;
long maxVelocity = 45000;
int cycleDelay = 500;
int RSD;
int add;

SPISettings settingsA(400000,MSBFIRST,SPI_MODE3);

void adxlInit (){

  SPI.begin();
  pinMode(CS,OUTPUT);
  digitalWrite(CS,HIGH);

  SPI.beginTransaction(settingsA);
  digitalWrite(CS,LOW);

//  PowerCtl 
  add = 0x2D;
  add = add | 0x40;
  SPI.transfer(add);
  SPI.transfer(0x08);

  digitalWrite(CS,HIGH);
  delay(1);
  digitalWrite(CS,LOW);

//  Data Format
  add = 0x31;
  add = add | 0x40;
  SPI.transfer(add);
  SPI.transfer(0x0B);

  digitalWrite(CS,HIGH);
  delay(1);
  digitalWrite(CS,LOW);

//  DataRate
  add = 0x2C;
  add = add | 0x40;
  SPI.transfer(add);
  SPI.transfer(0x0D);
  
  digitalWrite(CS,HIGH);
  delay(1);
//  SPI.endTransaction();
}

int getZ(){
//    SPI.beginTransaction(settingsA);
  digitalWrite(CS,LOW);

//  Get Z data
  add = 0x36;
  add = add | 0x40; // Multiple bytes
  add = add | 0x80; // Read operation
  int a = SPI.transfer(add);
  int b = SPI.transfer(0x00);
  int c = SPI.transfer(0x00);
//  SPI.endTransaction();
  digitalWrite(CS,HIGH);

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

int calcRSD(long input) {
  if (input < minVelocity){
    return 150;
  }
  else if (input > minVelocity && input < maxVelocity){
    int temp = map(input,minVelocity,maxVelocity,150,50);
    return temp;
  }
  else {
    return 50;
  }
}

void beginVelocityRead () {
  velocityRead = true;
//  digitalWrite(yellowLED,HIGH);
}

void printStatus(int RSD){
  Serial.println("");
  Serial.print("RSD: ");
  Serial.print(RSD);
  Serial.println(" ms");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(applyStart,INPUT);
  pinMode(sensorSwitch,INPUT_PULLUP);
  pinMode(strokeOutput,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(yellowLED,OUTPUT);
  adxlInit();
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(applyStart),beginVelocityRead,HIGH);
  Serial.println("Ready!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(applyStart));
  if (velocityRead) {
    while (digitalRead(sensorSwitch)){
      Serial.println("Collecting");
      delay(tDelay);  
      area += (((getZ()*-1)+250) * tDelay);
      Serial.println(area);
      digitalWrite(yellowLED,HIGH);
    }
    Serial.println("Output");
    velocityRead = false;
    RSD = calcRSD(area);
    area = 0;
    printStatus(RSD);
    delay(RSD);
    digitalWrite(strokeOutput,HIGH);
    digitalWrite(redLED,HIGH);
    delay(cycleDelay);
    digitalWrite(yellowLED,LOW);
    digitalWrite(redLED,LOW);
    digitalWrite(strokeOutput,LOW);    
  }
  else{
//  digitalWrite(yellowLED,HIGH);
  delay(1);
//  digitalWrite(yellowLED,LOW);
  }
//  delay(1);
}
