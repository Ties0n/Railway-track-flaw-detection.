#include <SoftwareSerial.h>
#include <TinyGPS++.h> // TinyGPS++ Library for GPS
// Pin Definitions
#define RX_GPS 12
#define TX_GPS 13
#define RX_GSM 4
#define TX_GSM 5
#define TRIG_PIN 3
#define ECHO_PIN 2
#define IR_LEFT A0
#define IR_RIGHT A1
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENA 10
#define ENB 11
// Thresholds
#define IR_THRESH HIGH
#define OBSTACLE_THRESH 20
// GPS & GSM Serial Objects
SoftwareSerial gpsSerial(RX_GPS, TX_GPS);
SoftwareSerial gsmSerial(RX_GSM, TX_GSM);
TinyGPSPlus gps;
// Global Variables
float latitude = 0.0;
float longitude = 0.0;
void setup() {
Serial.begin(9600);
gpsSerial.begin(9600);
gsmSerial.begin(115200);
pinMode(IR_LEFT, INPUT);
pinMode(IR_RIGHT, INPUT);
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(ENA, OUTPUT);
pinMode(ENB, OUTPUT);
Serial.println(F("System Initialized"));
// Initialize GSM
gsmSerial.println(F("AT"));
delay(1000);
if (gsmSerial.available()) {
Serial.println(F("GSM Ready"));
} else {
Serial.println(F("GSM Error!"));
}
gsmSerial.println(F("AT+CMGF=1"));
delay(1000);
// Initialize GPS
Serial.println(F("Initializing GPS..."));
delay(2000);
}
void loop() {
while (gpsSerial.available() > 0) {
gps.encode(gpsSerial.read());
if (gps.location.isUpdated()) {
latitude = gps.location.lat();
longitude = gps.location.lng();
Serial.print("Latitude: ");
Serial.print(latitude, 6);
Serial.print(" Longitude: ");
Serial.println(longitude, 6);
}
}
// IR Sensor Readings
int irLeftVal = digitalRead(IR_LEFT);
int irRightVal = digitalRead(IR_RIGHT);
Serial.print("IR Left: "); Serial.println(irLeftVal);
Serial.print("IR Right: "); Serial.println(irRightVal);
// Ultrasonic Distance
long distance = getUltrasonicDistance();
Serial.print("Ultrasonic Distance: ");
Serial.print(distance);
Serial.println(" cm");
// Decision Making
if (irLeftVal < IR_THRESH && irRightVal < IR_THRESH && distance >= OBSTACLE_THRESH)
{
moveForward();
Serial.println(F("Moving Forward"));
} else {
stopMotors();
if (distance < OBSTACLE_THRESH) {
Serial.println(F("Obstacle detected! Sending SMS..."));
sendSMS("Obstacle detected!");
} else if (irLeftVal >= IR_THRESH && irRightVal >= IR_THRESH) {
Serial.println(F("Crack on both sides! Sending SMS..."));
sendSMS("Crack detected on both sides!");
} else if (irLeftVal >= IR_THRESH) {
Serial.println(F("Crack on left! Sending SMS..."));
sendSMS("Crack detected on left!");
} else if (irRightVal >= IR_THRESH) {
Serial.println(F("Crack on right! Sending SMS..."));
sendSMS("Crack detected on right!");
}
}
delay(1000);
}
void sendSMS(String msg) {
Serial.println("Sending SMS...");
gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
delay(1000);
gsmSerial.println("AT+CMGS=\"+918688388722\""); // Replace with your number
delay(1000);
gsmSerial.print(msg);
gsmSerial.print("\nLocation: Lat ");
gsmSerial.print(latitude, 6);
gsmSerial.print(", Lon ");
gsmSerial.print(longitude, 6);
gsmSerial.print("\nGoogle Maps: https://www.google.com/maps?q=");
gsmSerial.print(latitude, 6);
gsmSerial.print(",");
gsmSerial.print(longitude, 6);
gsmSerial.write(26); // Send SMS (CTRL+Z)
delay(3000);
Serial.println("SMS Sent!");
}
void moveForward() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
analogWrite(ENA, 60);
analogWrite(ENB, 60);
}
void stopMotors() {
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
digitalWrite(IN1, HIGH);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, HIGH);
analogWrite(ENA, 0);
analogWrite(ENB, 0);
}
long getUltrasonicDistance() {
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);
long duration = pulseIn(ECHO_PIN, HIGH, 30000);
if (duration == 0) return 250; // No echo received, assume far obstacle
return duration * 0.034 / 2;
}
