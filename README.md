# Vehicle Accident Detection and Prevention System

## 📌 Project Overview
The Vehicle Accident Detection and Prevention System is an Arduino-based embedded system designed to improve road safety by detecting potential accidents, preventing drunk driving, and sending emergency alerts with GPS location details.

## 🎯 Objectives
- Detect accidents automatically using an accelerometer.
- Detect alcohol using an MQ-3 sensor.
- Prevent vehicle operation when alcohol exceeds a predefined threshold.
- Send emergency alerts with GPS location using GSM.
- Reduce false alerts using a delay mechanism and manual cancellation.

## 🛠️ Hardware Components
- Arduino UNO
- MQ-3 Alcohol Sensor
- ADXL345 Accelerometer
- GPS Module
- GSM Module
- I2C LCD Display
- Relay Module
- Buzzer
- Push Button
- DC Motor
- Battery and Switch

## ⚙️ Working Principle
1. The Arduino reads the alcohol sensor values.
2. If the alcohol level exceeds the threshold, the relay locks the motor and the buzzer activates.
3. The accelerometer monitors sudden motion and potential accident conditions.
4. When an accident is detected, a 10-second delay begins.
5. The user can press the push button to cancel a false alert.
6. If the alert is not cancelled, the GPS module provides location coordinates.
7. The GSM module sends an emergency SMS containing the location.

## 💻 Technologies Used
- Arduino C++
- Embedded Systems
- Sensor Interfacing
- GPS and GSM Communication
- Microcontroller-Based System Design

## 🧪 Testing and Implementation
The prototype integrates alcohol detection, accident detection, relay-based motor control, buzzer alerts, and LCD status display. GPS location tracking and GSM communication are included for emergency notifications.

## ✅ Key Features
- Automatic accident detection
- Alcohol detection and motor locking
- GPS-based location tracking
- GSM-based emergency alerts
- Manual cancellation of false alerts
- LCD status display and audible warnings

## ⚠️ Limitations
- GPS accuracy may decrease indoors or in obstructed areas.
- GSM alerts depend on network availability.
- The MQ-3 sensor may be affected by environmental conditions.
- The system is a prototype and is not optimized for direct integration into a real vehicle.

## 🚀 Future Scope
- Driver fatigue and drowsiness detection
- Driver health monitoring
- Camera-based driver monitoring
- Mobile application integration
- Improved accident detection using sensor fusion

## 📂 Project Documents
- Review 2 – Project design and component details
- Review 3 – System integration, testing, results, and future scope

## 👩‍💻 Contributors
This project was developed as a team project by ECE and CSE students.

## 📄 Note
This project is an academic prototype developed for learning and demonstration purposes.
