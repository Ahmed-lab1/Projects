#include <LiquidCrystal.h>
#include <Servo.h>
#include <AFMotor.h>

#define RT0 10000   // Ω
#define B 3977      // K
#define Echo A0
#define Trig A1
#define motor 10
#define Speed 0
#define spoint 103
#define VCC 5    //Supply voltage
#define RR 10000  //R=10KΩ


char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;
float RT, VR, ln, TX, T0, VRT;
int Contrast = 60;

//AF_DCMotor M3(3);  // check which M pins used in robotic car.
//AF_DCMotor M4(4);

Servo servo;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
LiquidCrystal lcd(24, 25, 52, 53, 48, 51);


void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
 // M3.run(FORWARD);
 // M4.run(FORWARD);
}
void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
 // M3.run(BACKWARD);
 // M4.run(BACKWARD);
}
void right() {
  M1.run(BACKWARD);
  //M2.run(BACKWARD);
  M2.run(FORWARD);
  //M4.run(FORWARD);
}
void left() {
  M1.run(FORWARD);
 // M2.run(FORWARD);
  M2.run(BACKWARD);
  //M4.run(BACKWARD);
}
void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  //M3.run(RELEASE);
 // M4.run(RELEASE);
}
void setup() {
  analogWrite(43, Contrast);
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  T0 = 25 + 273.15;
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(motor);
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  
//  M3.setSpeed(Speed);
 // M4.setSpeed(Speed);
}
void loop() {
  int reading  = analogRead(A14);
  M2.setSpeed(map(reading, 0, 1023, 0, 255)); 
  M1.setSpeed(map(reading, 0, 1023, 0, 255));
  VRT = analogRead(A9);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / RR);               //Resistance of RT
 
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
 
  TX = TX - 278.15;  
  lcd.setCursor(0, 1);
  lcd.print("Temp = ");
  lcd.print(TX); 
    
  //lcd.print("F");
  
  delay(500);            
  lcd.clear();
  Obstacle();
  Bluetooth();

 
}


void Bluetooth(){
    while(Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
   if (value == 'F') {
    forward();
  } else if (value == 'B') {
    backward();
  } else if (value == 'L') {
    left();
  } else if (value == 'R') {
    right();
  } else if (value == 'S') {
    Stop();
  }
    }
  
  }
void Obstacle() {
  distance = ultrasonic();
  if (distance <= 12) {
    Stop();
    backward();
    delay(100);
    Stop();
    L = leftsee();
    servo.write(spoint);
    delay(800);
    R = rightsee();
    servo.write(spoint);
    if (L < R) {
      right();
      delay(500);
      Stop();
      delay(200);
    } else if (L > R) {
      left();
      delay(500);
      Stop();
      delay(200);
    }
  } else {
    forward();
  }

}

int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2; //time convert distance
  return cm;
}


int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}

int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}
