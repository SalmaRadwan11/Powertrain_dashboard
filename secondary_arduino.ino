int potPin = A0;
float potValue; 
float Speed;

int trigPin = 2;
int echoPin = 3;
float waveTravelTime;
int distance;

int voltageSensorPin = A1;      //voltage readings is based on the 0-25 voltage sensor module
float voltageSensorReading;
float arduinoReading;
float factor = 5.128;
float batteryVoltage;

int speedCode = 101020;         //for the communication
int distanceCode = 101021;
int voltageCode = 101022;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);

  pinMode(potPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  potValue = analogRead(potPin);
  Speed = potValue*0.432*3.14*1.66;      //in Km/h

  
  // Measure the distance using the Ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Calculate the distance in centimeters

//voltage sensor

  voltageSensorReading = analogRead(voltageSensorPin);
  arduinoReading = voltageSensorReading*(5./1023.);
  batteryVoltage = arduinoReading*factor;

  Serial.print(speedCode);
  Serial.print(Speed);
  Serial.print(distanceCode);
  Serial.print(distance);
  Serial.print(voltageCode);
  Serial.print(batteryVoltage);
}
