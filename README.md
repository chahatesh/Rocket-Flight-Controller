# 🚀 ESP32 Rocket Telemetry & Flight Computer System

A real-time rocket telemetry system built using **ESP32-C3**, **MPU6050/MPU6500 IMU sensors**, and **ESP-NOW Long Range (LR)** communication.

This system streams flight data (acceleration, gyro, and g-force) from an onboard flight computer to a ground station with low latency and improved stability for small rocket platforms.

---

# 📡 System Overview

This project is split into two main components:

## 🛰️ Flight Computer (Sender - ESP32-C3)
- Reads IMU sensor data (MPU6050 / MPU6500 / MPU9250 compatible)
- Applies gyro calibration at startup
- Computes:
  - Acceleration (ax, ay, az in g)
  - Angular velocity (gx, gy, gz in °/s)
  - Total g-force magnitude
- Sends structured telemetry packets using ESP-NOW LR mode

## 📡 Ground Station (Receiver)
- Receives ESP-NOW telemetry packets
- Tracks packet loss using packet ID sequencing
- Detects signal loss / weak link conditions
- Outputs clean serial stream for logging or Python visualization

---

# 📦 Telemetry Packet Format (Binary Frame Structure)

The system uses a compact binary telemetry structure for efficiency:

```cpp
struct Telemetry {
  uint32_t packetID;

  float ax;
  float ay;
  float az;

  float gx;
  float gy;
  float gz;

  float gforce;
};
```

### Key Features:
- Fixed-size binary packet (low bandwidth)
- No string overhead (faster + more stable)
- Compatible with Python / serial dashboards
- Packet ID enables loss detection and ordering

---

# ⚙️ Communication System

## ESP-NOW Long Range (LR Mode)

The system uses ESP-NOW configured for Long Range operation:

- Fixed WiFi channel (Channel 1)
- ESP-NOW peer-to-peer communication
- LR PHY mode enabled
- Reduced PHY rate for stability (~250 kbps equivalent behavior)
- Max transmit power tuning for range

### Why ESP-NOW LR?
- Very low latency
- No router required
- High reliability at short-to-medium range
- Lightweight for embedded flight systems

---

# 🧠 IMU Sensor Configuration

Supported IMUs:
- MPU-6050 (6-axis)
- MPU-6500 (6-axis)
- MPU-9250 (9-axis, magnetometer unused)

## I2C Settings:
- Address: `0x68`
- Accelerometer range: ±2g
- Gyroscope range: ±250°/s
- Clock speed: 100–400 kHz (stability optimized)

---

# 📊 Flight Computation Logic

## g-force calculation:

```
g = sqrt(ax² + ay² + az²)
```

Used to detect:
- Launch acceleration
- Free fall conditions
- Flight stability

---

## Gyro Calibration

At startup, the system:
- Collects ~200–500 samples while stationary
- Computes average bias
- Subtracts offset during flight

This reduces drift and improves orientation stability.

---

# 📡 Packet Loss Detection

Packet loss is tracked using sequential packet IDs:

- If ID jumps → packets were lost
- Loss counter increments accordingly
- Helps evaluate RF stability during flight

Example:
```
ID: 1200 → 1201 → 1203
Loss detected: +1 packet
```

---

# ⚡ Known Issues & Limitations

This system is experimental and has known engineering constraints:

## 🔋 Power Instability (IMPORTANT)
- ESP32-C3 can brown out during RF transmission spikes
- Long Range mode increases power demand
- Requires stable 3.3V supply and capacitor buffering

## 📡 RF Sensitivity
- Performance depends heavily on:
  - antenna placement
  - battery noise
  - environmental interference

## 🧭 Gyro Drift
- MPU sensors require calibration every boot
- Small drift still exists over long duration flights

## 📉 Packet Loss
- Occasional packet loss is expected under:
  - high acceleration
  - weak power conditions
  - RF obstruction

---

# 🚀 Recommended Hardware Setup

## Power System:
- 1S LiPo battery OR stable USB power bank
- 3.3V regulator (≥500mA recommended)
- 220–470µF capacitor across ESP32 power rails

## Wiring Notes:
- Keep IMU wires short
- Keep antenna area clear of metal and battery
- Avoid long jumper wires for power lines

---

# 🧪 Status Output Example

```
AX:-0.03 AY:0.01 AZ:0.99 | GX:0.02 GY:-0.01 GZ:0.00 | G:1.00 | ID:1203 | LOSS:2 | LINK:OK
```

---

# 🚀 Future Improvements

Planned upgrades:
- Sensor fusion (Madgwick filter)
- Real-time attitude estimation (pitch/roll/yaw)
- Parachute deployment logic (apogee detection)
- Python ground station GUI dashboard
- LoRa fallback communication system

---

# 📜 License
Open for educational and experimental aerospace use.
Use responsibly for non-commercial rocketry and STEM learning.
