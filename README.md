# 🚆 IoT-Based Railway Track Flaw Detection System

**Authors:** B. Nikhil Sai Venkat et al.
**Degree Project (B.Tech — ECE)** — Gayatri Vidya Parishad College (2024–25)

---

## Project overview (executive summary)
An autonomous IoT-based robotic vehicle that detects cracks and obstacles on railway tracks in real-time using IR sensors, ultrasonic sensors, GPS, and GSM communication. The system logs precise coordinates and sends instant alerts (SMS/Email/Google Maps link) to maintenance teams. This repository contains code, schematics, and the full project report.

---

## What’s in this repo
- `/src/arduino` — Arduino sketch (track_detection.ino) extracted & cleaned from the report
- `/assets` — Placeholder folder for photos, diagrams, and screenshots (add your images)
- `/docs/Project_Report.pdf` — Full project report (copy of your PDF)
- `README.md` (this file) — Hybrid-style documentation
- `LICENSE` — MIT license
- `demo/` — sample/demo assets (add testing photos or video)

---

## Key features (technical highlights)
- Real-time crack detection using **IR sensors** (reflectance-based)
- Obstacle detection using **HC-SR04 ultrasonic sensor**
- Location tagging using **NEO-6M GPS**
- Alerts sent via **GSM module (A7670C / AT command flow)** and optional Termux-based email/SMS
- Motor control via **L298N** motor driver IC
- Lightweight, low-cost, and designed for field testing / rapid prototyping

---

## System architecture (brief)
1. Sensors (IR, Ultrasonic) → Arduino reads values
2. Arduino integrates GPS (TinyGPS++) to get lat/long
3. On detection, Arduino triggers GSM SMS with message + Google Maps link

_For the full block diagrams, figures, tests and explanations, see `docs/Project_Report.pdf`._

---

## Quick setup (hardware)
**Required components**
- Arduino Uno (or compatible)
- 2× IR sensors (for left/right crack detection)
- 1× HC-SR04 ultrasonic sensor
- NEO-6M GPS module
- GSM module (A7670C or SIM800/Sim900 alternative)
- L298N motor driver + DC motors
- Chassis / battery / jumper wires

_Wiring hints:_
- IR sensors → A0, A1 (analog or digital depending on module)
- HC-SR04 → TRIG 3, ECHO 2
- GPS → SoftwareSerial pins (e.g., RX 12, TX 13)
- GSM → SoftwareSerial pins (e.g., RX 4, TX 5)
- Motor driver → IN1..IN4 (pins 6..9), ENA/ENB PWM pins 10/11

---

## Arduino sketch (src/arduino/track_detection.ino)
Below is the main Arduino program. Copy into the Arduino IDE, install `TinyGPS++` and upload.
