# 🚀 Rocket Flight Controller + Real-Time 3D Telemetry Dashboard

A real-time rocket flight telemetry system built using ESP32 microcontrollers and a custom web-based ground station featuring a live 3D motion model, altitude tracking, and flight analytics.

This project combines embedded systems, wireless communication, and real-time visualization to replicate core concepts used in aerospace flight computers and mission control systems.

---

## 🌌 Overview

The onboard ESP32 collects motion and altitude data from sensors during flight and streams it in real time to a ground station.

A custom web dashboard transforms this data into a live interactive experience:
- 3D model that rotates based on real sensor orientation
- Real-time altitude tracking
- Automatic apogee detection
- Live flight motion visualization

---

## 🛰 System Architecture

Onboard Flight Controller (ESP32)  
→ Sensor Data (IMU + Altitude)  
→ Wireless Telemetry Stream  
→ Web Ground Station  
→ 3D Visualization + Flight Analytics  

---

## 🧠 Features

### 📡 Live Telemetry
Continuous streaming of sensor data from the ESP32 during flight.

### 🧭 3D Orientation Model
Real-time visualization of pitch, roll, and yaw using sensor input.

### 📈 Altitude Tracking
Displays live altitude changes during ascent and descent.

### 🏔 Apogee Detection
Automatically records maximum altitude reached during flight.

### 🌐 Web Dashboard
Browser-based mission control interface for real-time monitoring.

---

## 📊 Data Format

ax, ay, az, gx, gy, gz, altitude, timestamp

- ax, ay, az → acceleration  
- gx, gy, gz → angular velocity  
- altitude → height measurement  
- timestamp → time reference  

---

## 🔬 Purpose

This project demonstrates real aerospace principles:
- Flight computer telemetry systems
- Attitude and motion tracking
- Real-time sensor data visualization
- Ground station monitoring systems

It turns raw sensor data into a live understanding of flight behavior.

---

## 🧰 Hardware

- ESP32 Development Board  
- IMU Sensor (gyro + accelerometer)  
- Altitude sensor  
- Battery system  
- Wireless communication (Wi-Fi / telemetry)

---

## 🚀 Project Status

Fully functional core telemetry system and 3D visualization dashboard.

---

## 📜 License

Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)

Commercial use is not permitted. Use is allowed for educational and personal purposes only.

---

## ⚠️ Disclaimer

For educational and experimental use only. Not certified for real aerospace or safety-critical applications.
