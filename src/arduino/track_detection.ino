#include <SoftwareSerial.h>
#include <TinyGPS++.h>

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

SoftwareSerial gpsSerial(RX_GPS, TX_GPS);
SoftwareSerial gsmSerial(RX_GSM, TX_GSM);
TinyGPSPlus gps;

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
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  Serial.println(F("System Initialized"));

  gsmSerial.println(F("AT"));
  delay(1000);
  gsmSerial.println(F("AT+CMGF=1")); // Text mode
  delay(1000);
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }
  }

  int irLeftVal = digitalRead(IR_LEFT);
  int irRightVal = digitalRead(IR_RIGHT);
  long distance = getUltrasonicDistance();

  if (irLeftVal < IR_THRESH && irRightVal < IR_THRESH && distance >= OBSTACLE_THRESH) {
    moveForward();
  } else {
    stopMotors();
    if (distance < OBSTACLE_THRESH) {
      sendSMS("Obstacle detected!");
    } else if (irLeftVal >= IR_THRESH && irRightVal >= IR_THRESH) {
      sendSMS("Crack detected on both sides!");
    } else if (irLeftVal >= IR_THRESH) {
      sendSMS("Crack detected on left!");
    } else if (irRightVal >= IR_THRESH) {
      sendSMS("Crack detected on right!");
    }
  }

  delay(1000);
}

void sendSMS(String msg) {
  gsmSerial.println("AT+CMGF=1");
  delay(500);
  gsmSerial.println("AT+CMGS="+918688388722""); // replace with target number
  delay(500);
  gsmSerial.print(msg);
  gsmSerial.print("\nLocation: Lat ");
  gsmSerial.print(latitude, 6);
  gsmSerial.print(", Lon ");
  gsmSerial.print(longitude, 6);
  gsmSerial.print("\nGoogle Maps: https://www.google.com/maps?q=");
  gsmSerial.print(latitude, 6);
  gsmSerial.print(",");
  gsmSerial.print(longitude, 6);
  gsmSerial.write(26); // CTRL+Z
  delay(3000);
}

void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 60); analogWrite(ENB, 60);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

long getUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 250;
  return duration * 0.034 / 2;
}
