/*
  Vehicle Accident Detection and Prevention System
  Extracted/reconstructed from the Arduino code screenshots in the project review PDF.

  IMPORTANT:
  - Replace PHONE_NUMBER with the intended emergency number before testing.
  - The PDF screenshots are split across columns and some lines are unclear.
  - Verify wiring, library versions, and the full sketch before uploading to hardware.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <math.h>

#define MQ3_A A0
#define RELAY 7
#define BUZZER 6
#define BUTTON 2
#define IMPACT_THRESHOLD 3
#define ALCOHOL_THRESHOLD 600

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3);
SoftwareSerial gsm(9, 8);

// The PDF contains a personal phone number. Add your own number locally.
String phoneNumber = "PHONE_NUMBER";

bool accidentDetected = false;
bool alertSent = false;
unsigned long accidentTime = 0;

void sendSMS(String message) {
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneNumber);
  gsm.println("\"");
  delay(1000);
  gsm.print(message);
  delay(500);
  gsm.write(26);
  delay(5000);
}

void makeCall() {
  gsm.print("ATD");
  gsm.print(phoneNumber);
  gsm.println(";");
  delay(20000);
  gsm.println("ATH");
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  gsm.begin(9600);

  pinMode(MQ3_A, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(RELAY, HIGH);
  digitalWrite(BUZZER, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Vehicle");
  lcd.setCursor(0, 1);
  lcd.print("System Start");
  delay(2000);
  lcd.clear();

  Serial.println("System Starting...");

  // GSM initialization
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1");
  delay(1000);

  if (!accel.begin()) {
    Serial.println("ADXL345 ERROR");
    lcd.setCursor(0, 0);
    lcd.print("ADXL Error!");
    while (1) {
      // Stop if the accelerometer is not detected.
    }
  }

  accel.setRange(ADXL345_RANGE_16_G);

  lcd.setCursor(0, 0);
  lcd.print("Sensor Warmup");
  lcd.setCursor(0, 1);
  lcd.print("Please Wait");
  delay(15000);
  lcd.clear();
}

void loop() {
  // GPS read
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Alcohol sensor
  int alcoholLevel = analogRead(MQ3_A);

  // Accelerometer / impact calculation
  sensors_event_t event;
  accel.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;
  float total = sqrt(x * x + y * y + z * z);
  float impact = abs(total - 9.8);

  // Serial output
  Serial.print("Alcohol: ");
  Serial.print(alcoholLevel);
  Serial.print(" Impact: ");
  Serial.println(impact);

  // LCD display
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.print(alcoholLevel);
  lcd.print(" I:");
  lcd.print(impact, 1);
  lcd.print(" ");

  // Alcohol detection
  if (alcoholLevel > ALCOHOL_THRESHOLD) {
    accidentDetected = false;
    alertSent = false;
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ENGINE LOCKED");
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    return;
  } else {
    if (!accidentDetected) {
      digitalWrite(RELAY, HIGH);
      digitalWrite(BUZZER, LOW);
    }
  }

  // Accident detection
  if (impact > IMPACT_THRESHOLD && !accidentDetected) {
    accidentDetected = true;
    alertSent = false;
    accidentTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCIDENT!");
    lcd.setCursor(0, 1);
    lcd.print("Engine Stopped");
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, HIGH);
    Serial.println("Accident Detected");
  }

  // Accident mode and manual cancellation
  if (accidentDetected) {
    digitalWrite(RELAY, LOW);
    digitalWrite(BUZZER, HIGH);

    if (digitalRead(BUTTON) == LOW) {
      Serial.println("Accident Cancelled");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alert Cancelled");
      digitalWrite(BUZZER, LOW);
      accidentDetected = false;
      alertSent = false;
      delay(2000);
      lcd.clear();
    }

    // Send alert after the 10-second cancellation window
    if ((millis() - accidentTime > 10000) && !alertSent) {
      Serial.println("ALERT SENT!");
      String message = "ACCIDENT ALERT!\n";

      if (gps.location.isValid()) {
        message += "Location:\nhttps://maps.google.com/?q=";
        message += String(gps.location.lat(), 6);
        message += ",";
        message += String(gps.location.lng(), 6);
      } else {
        message += "GPS not available";
      }

      sendSMS(message);
      makeCall();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sending Alert");
      lcd.setCursor(0, 1);
      lcd.print("Location Sent");
      alertSent = true;
      delay(3000);
      lcd.clear();
    }
  }

  // GPS display when no accident is active
  if (!accidentDetected) {
    if (gps.location.isValid()) {
      lcd.setCursor(0, 1);
      lcd.print("Lat:");
      lcd.print(gps.location.lat(), 2);
      delay(1500);

      lcd.setCursor(0, 1);
      lcd.print("Lng:");
      lcd.print(gps.location.lng(), 2);
      lcd.print(" ");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Waiting GPS...");
    }
  }

  delay(500);
}
