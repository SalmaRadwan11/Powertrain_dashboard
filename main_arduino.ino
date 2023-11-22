#include <LiquidCrystal.h>
const int rs = 13;
const int en = 12;
const int d4 = 11;
const int d5 = 10;
const int d6 = 9;
const int d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int switchPin = 2;
int switchState;
int lockState;

int buzzPin = 3;
int buzzButtPin = A0;
int buzzButtState;

int rightLightPin = 4;
int leftLightPin = 5;
int frontLightPin = 6;
int rightLightButtPin = A1;
int leftLightButtPin = A2;
int frontLightButtPin = A3;
int rightLightButtState;
int leftLightButtState;
int frontLightButtState;

int massegeCode;
int speedCode = 101020;         //for the communication
int distanceCode = 101021;
int voltageCode = 101022;

float Speed;
int distance;
float batteryVoltage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(50);
  delay(100);

  lcd.begin(16,2);
    
  pinMode(switchPin, INPUT);

  pinMode(buzzPin, OUTPUT);
  pinMode(buzzButtPin, INPUT_PULLUP);

  pinMode(rightLightPin,  OUTPUT);
  pinMode(leftLightPin,  OUTPUT);
  pinMode(frontLightPin,  OUTPUT);
  pinMode(rightLightButtPin, INPUT_PULLUP);
  pinMode(leftLightButtPin, INPUT_PULLUP);
  pinMode(frontLightButtPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(switchPin);
  if (switchState == HIGH)  {
    //the switch isn't pressed
    lockState = 0;
  }
  if (switchState == LOW)  {
    //the switch is pressed
    lockState = 1;
  }

  buzzButtState = digitalRead(buzzButtPin);
  rightLightButtState = digitalRead(rightLightButtPin);
  leftLightButtState = digitalRead(leftLightButtPin);
  frontLightButtState = digitalRead(frontLightButtPin);
  
  if (buzzButtState == 1)  {
    digitalWrite(buzzPin, HIGH);
  }
  else {
    digitalWrite(buzzPin, LOW); 
  }
  if (rightLightButtState == 1)  {
    digitalWrite(rightLightPin, HIGH);
  }
  else {
    digitalWrite(rightLightPin, LOW); 
  }
  if (leftLightButtState == 1)  {
    digitalWrite(leftLightPin, HIGH);
  }
  else {
    digitalWrite(leftLightPin, LOW); 
  }
  if (frontLightButtState == 1)  {
    digitalWrite(frontLightPin, HIGH);
  }
  else {
    digitalWrite(frontLightPin, LOW); 
  }
   
  if (Serial.available())  {
    massegeCode = Serial.parseInt();
    delay(1);
    if (massegeCode == speedCode)  {
      if (Serial.available())  {
        Speed = Serial.parseFloat();
      }
    }
    else if (massegeCode == distanceCode)  {
      if (Serial.available())  {
        distance = Serial.parseInt();
      } 
    }
    else if (massegeCode == voltageCode)  {
      if (Serial.available())  {
        batteryVoltage = Serial.parseFloat();
      } 
    }
  }

  lcd.setCursor(0,0);
  lcd.print("lock:");
  lcd.print(lockState);
  lcd.setCursor(6,0);
  lcd.print(",");
  lcd.print("dist:");
  lcd.print(distance);
  lcd.setCursor(0,1);
  lcd.print("VEL:");
  lcd.print(Speed);
  lcd.setCursor(9,1);
  lcd.print(",BV:");
  lcd.print(batteryVoltage);
}
