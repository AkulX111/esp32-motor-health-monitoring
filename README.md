# 🏭 RTOS-Based Predictive Maintenance System for Electric Motors using ESP32

> **Real-Time IoT Motor Health Monitoring with FreeRTOS, MQTT, and Blynk Dashboard**

<div align="center">

![Status](https://img.shields.io/badge/Status-Complete%20%26%20Validated-brightgreen?style=for-the-badge)
![Hardware](https://img.shields.io/badge/Hardware-ESP32%20WROOM--32-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-Embedded%20C-yellowgreen?style=for-the-badge)
![License](https://img.shields.io/badge/License-Academic%20Project-red?style=for-the-badge)
![Build](https://img.shields.io/badge/Build-Passing-success?style=for-the-badge)

**A production-ready IoT solution for predictive motor maintenance using FreeRTOS-based deterministic sensing**

[Documentation](#-documentation) • [Quick Start](#-quick-start-guide) • [Hardware](#-hardware-components) • [Contributing](#-contributing) • [License](#-license)

</div>

---

## 📑 Table of Contents

- [📋 Project Overview](#-project-overview)
- [🎯 Problem & Solution](#-problem--solution)
- [✨ Key Features](#-key-features)
- [🛠️ Hardware Components](#️-hardware-components)
- [💻 Software Architecture](#-software-architecture)
- [📁 Project Structure](#-project-structure)
- [🚀 Five Concurrent FreeRTOS Tasks](#-five-concurrent-freertos-tasks)
- [📊 MQTT Topics & Data Format](#-mqtt-topics--data-format)
- [📈 Performance & Validation](#-performance--validation)
- [🚀 Quick Start Guide](#-quick-start-guide)
- [📚 Technical Details](#-technical-details)
- [🔌 Pin Configuration](#-pin-configuration)
- [🔍 Troubleshooting](#-troubleshooting)
- [📦 Development Setup](#-development-setup)
- [❓ FAQ](#-faq)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)
- [👥 Authors](#-authors)

---

## 📋 Project Overview

This project implements a **Real-Time Operating System (RTOS) based predictive maintenance system for electric motors** using ESP32 microcontroller. The system continuously monitors motor temperature and vibration in real-time and intelligently detects faults before they cause catastrophic failure.

### 🎯 Problem & Solution

#### The Problem
- Industrial motors account for **45% of global electricity consumption**
- Unexpected motor failures cause **major downtime and equipment damage**
- Existing predictive maintenance systems cost **$1k–$50k** (not accessible to SMEs)
- Traditional bare-metal implementations cannot handle **multi-rate deterministic sensing**

#### Our Solution
| Feature | Benefit |
|---------|---------|
| 💰 **Low-cost (<$15 BOM)** | Affordable for small/medium enterprises |
| 🔓 **Open-source hardware & firmware** | Full transparency and customizable |
| ⚡ **FreeRTOS-based deterministic sensing** | Independent 1Hz + 5Hz task rates, impossible in bare-metal |
| 🧠 **Real-time fault classification** | Priority-ordered thresholds for accurate detection |
| 📱 **Proactive mobile notifications** | Blynk IoT push alerts on fault detection |
| 🌐 **Multi-protocol integration** | MQTT, Google Sheets, MongoDB support |
| 🏭 **Production-ready (4-hour stress test, zero crashes)** | Enterprise-grade reliability |

### ✨ Key Features

✅ **Deterministic Real-Time Sensing**  
   - Independent 1Hz temperature monitoring + 5Hz vibration sampling  
   - FreeRTOS task scheduling ensures predictable response times  

✅ **Intelligent Fault Detection**  
   - Temperature threshold: >75°C (overheat alert)  
   - Vibration threshold: >0.08g (abnormal vibration alert)  
   - Priority-ordered status: Normal → High Vibration → Overheat  

✅ **Multi-Platform Data Integration**  
   - MQTT pub-sub for local monitoring  
   - Blynk IoT for cloud dashboard & mobile alerts  
   - Google Sheets for real-time logging  
   - MongoDB for time-series analysis  

✅ **Stress-Tested Reliability**  
   - 4-hour continuous operation with **zero crashes**  
   - Automatic MQTT broker reconnection  
   - Graceful fault handling and recovery  

✅ **Low Power Consumption**  
   - Efficient sensor polling with task-based scheduling  
   - WiFi power management via FreeRTOS  
   - <2W average system power draw  

---

## 🛠️ Hardware Components

| Component | Model/Spec | Interface | Purpose |
|-----------|-----------|-----------|---------|
| **Microcontroller** | ESP32 WROOM-32 | — | Main processor (Dual-core 240MHz, Wi-Fi) |
| **Temperature Sensor** | NTC Thermistor 10kΩ | ADC (GPIO 34, 12-bit) | Detects motor overheating (threshold: >75°C) |
| **Vibration Sensor** | MPU6050 (6-axis IMU) | I2C (GPIO 21/22, 0x68) | Detects abnormal vibration (threshold: >0.08g) |
| **Resistor** | 10kΩ series | Voltage divider | NTC thermistor circuit |
| **Power Supply** | 5V USB | — | Powers ESP32 and sensors |

**Total System Cost:** <$15

---

## 💻 Software Architecture

### Language & Compilation
- **Language:** Embedded C
- **IDE:** Arduino IDE with ESP32 board support
- **Compiler:** GCC for Xtensa ISA (ESP32 architecture)
- **Libraries:** Standard C library + FreeRTOS + ESP-IDF
- **Code Style:** Pure procedural Embedded C (no object-oriented features)

### Operating System
- **FreeRTOS** (5 concurrent tasks with independent scheduling)
- **Task Scheduler:** vTaskDelay() for deterministic timing
- **Stack per task:** 4096 bytes each

### Communication Stack
- **WiFi 802.11b/g/n** → Home network connectivity
- **MQTT v4** → Mosquitto broker (QoS 0, pub-sub model)
- **Blynk IoT Protocol** → Cloud dashboard + push notifications
- **I2C 400kHz** → MPU6050 sensor bus
- **ADC 12-bit** → NTC thermistor analog reading

### Data Storage & Visualization
- **Google Sheets API** → Real-time CSV logging
- **MongoDB** → Time-series database
- **Node-RED Dashboard** → Local monitoring interface
- **Blynk Mobile App** → Remote alerts + live data

---

## 📁 Project Structure

```
rtos-motor-health-monitoring-esp32/
├── final/
│   └── final.ino                    # MAIN PRODUCTION CODE ⭐
│                                    # Embedded C, 259 lines
│                                    # - 5 concurrent FreeRTOS tasks
│                                    # - MQTT broker integration
│
├── SAFECODE1/
│   └── SAFECODE1.ino                # Extended version with Blynk
│                                    # Embedded C, 288 lines
│
├── Sensortest/
│   └── MPUtest/MPUtest.ino          # Sensor calibration utility
│                                    # Embedded C, 110 lines
│
├── flows_nodered.json               # Node-RED dashboard config
├── docs/images/                     # Diagrams and screenshots
│   ├── blockdiagram.png
│   ├── circuit_schematic.png
│   ├── hardware_prototype.png
│   ├── node-red.png
│   ├── blynk_normal.png
│   └── blynk_fault.png
│
├── PROJECT_REPORT.pdf               # 12-page academic paper
└── README.md                         # This file
```

---

## 🚀 Five Concurrent FreeRTOS Tasks

### 1. **TemperatureTask** (1000ms period)
- Reads NTC thermistor temperature via 12-bit ADC
- Uses Steinhart-Hart equation for accurate conversion
- Output: `g_temp` (degrees Celsius)

### 2. **MPUSensorTask** (200ms period) — 5Hz sampling
- Reads 6-axis accelerometer + gyroscope from MPU6050 via I2C
- 2000-sample auto-calibration on startup removes sensor bias
- Output: `g_vibration` (g-units)

### 3. **FaultTask** (1000ms period)
- Priority-ordered fault detection:
  - Temperature > 75°C → Status 2 (OVERHEAT)
  - Vibration > 0.08g → Status 1 (HIGH VIBRATION)
  - Otherwise → Status 0 (NORMAL)

### 4. **MQTTTask** (2000ms period)
- Maintains MQTT connection and publishes sensor data
- Topics: `motor/temp`, `motor/vibration`, `motor/status`
- Auto-reconnect on broker disconnect

### 5. **BlynkTask** (1000ms period) — SAFECODE1 only
- Updates Blynk IoT cloud dashboard
- Triggers push notifications on state transitions

---

## 📊 MQTT Topics & Data Format

| Topic | Payload | Interval | Meaning |
|-------|---------|----------|---------|
| `motor/temp` | "32.45" | 2000ms | Temperature in °C |
| `motor/vibration` | "0.0342" | 2000ms | Vibration in g-units |
| `motor/status` | "0" / "1" / "2" | 2000ms | 0=Normal, 1=Vibration, 2=Overheat |

---

## 📈 System Performance & Validation

| Metric | Value |
|--------|-------|
| **Temperature sample rate** | 1 Hz |
| **Vibration sample rate** | 5 Hz |
| **MQTT publish rate** | 0.5 Hz |
| **Fault detection latency** | ≤1000ms |
| **Local MQTT latency** | <50ms |
| **Stress test duration** | 4 hours (zero crashes) |
| **FreeRTOS tasks** | 5 |

### Validation Results
✅ Normal operation: Status 0 continuous  
✅ High vibration test: Status 1 detected <1s  
✅ Overheat test: Status 2 detected <1s  
✅ Mobile alerts: Blynk notifications received  

---

## 🚀 Quick Start Guide

### 1. Hardware Assembly
- Connect NTC thermistor to GPIO 34 via 10kΩ resistor
- Connect MPU6050 (SDA→GPIO 21, SCL→GPIO 22)
- Power ESP32 via USB (5V)

### 2. Upload Embedded C Code
```
1. Open Arduino IDE
2. Install ESP32 board: Boards Manager → "esp32" by Espressif
3. Select: Board "ESP32 Dev Module", Port "COMx"
4. Open final/final.ino
5. Update WiFi credentials (line 7):
   const char* ssid = "Your_SSID";
   const char* password = "Your_Password";
6. Update MQTT broker IP (line 11):
   const char* mqtt_server = "Your_Broker_IP";
7. Upload to board
```

### 3. Start MQTT Broker
```bash
mosquitto -p 1883
```

### 4. Import Node-RED Dashboard
```
Node-RED → Import → flows_nodered.json → Deploy
```

### 5. Monitor Data
```bash
mosquitto_sub -h 10.246.24.151 -t "motor/#" -v
```

---

## 📚 Technical Details

### Steinhart-Hart Temperature Formula

The NTC thermistor resistance is converted to temperature using the Steinhart-Hart equation:

```c
R_NTC = R_fixed × (ADC / (ADC_MAX - ADC))
T_K = 1 / ((1/T0) + (1/β)×ln(R_NTC/R0))
T_C = T_K - 273.15

Parameters:
- R0 = 10kΩ @ 25°C
- β = 3950 K (thermistor coefficient)
- T0 = 298.15K (reference temperature)
```

**Accuracy:** ±1°C over 0-100°C range

### Gravity-Compensated Vibration Detection

Tri-axial acceleration from MPU6050 is converted to vibration magnitude:

```c
V_raw = √(ax² + ay² + az²)  // Total acceleration
V_mag = V_raw - 1.0          // Remove gravity (1g offset)

Thresholds:
- V_mag ≤ 0.08g → Normal operation
- V_mag > 0.08g → High vibration alert
```

**Sensitivity:** ±0.01g (16-bit resolution at ±8g range)

---

## 🔌 Pin Configuration

```
GPIO 34  ←→  NTC Thermistor (ADC input)
GPIO 21  ←→  MPU6050 SDA (I2C)
GPIO 22  ←→  MPU6050 SCL (I2C)
GND      ←→  Common ground
5V       ←→  Power supply
```

**Serial:** 115200 baud, 8 bits, 1 stop bit, no parity

---

## 🔍 Troubleshooting

| Issue | Solution |
|-------|----------|
| Cannot connect to MQTT | Verify broker IP, port 1883, WiFi active |
| Vibration always 0 | Check MPU6050 connections (GPIO 21/22) |
| Temperature wrong | Check NTC polarity, 10kΩ resistor in place |
| No Google Sheets data | Verify Node-RED auth and MQTT messages |

---

## 📦 Development Setup

### Prerequisites
- **Arduino IDE** 2.0+ or **PlatformIO**
- **ESP32 Board Support** (Espressif Systems)
- **Python 3.8+** (for MQTT/data processing)
- **Git** (for version control)
- **Mosquitto** (for local MQTT broker)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/Divyanshu-53/rtos-motor-health-monitoring-esp32-predictive-maintenance.git
   cd rtos-motor-health-monitoring-esp32-predictive-maintenance
   ```

2. **Install Arduino IDE Dependencies**
   - Install ESP32 board package via Boards Manager
   - Install libraries:
     - `BlynkSimpleEsp32` (Blynk IoT)
     - `PubSubClient` (MQTT)
     - `Wire` (I2C - builtin)

3. **Setup Mosquitto MQTT Broker**
   ```bash
   # Linux/Mac
   brew install mosquitto
   mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf
   
   # Windows (download installer from mosquitto.org)
   mosquitto -p 1883
   ```

4. **Configure WiFi & MQTT**
   - Edit `final/final.ino` (lines 6-11):
     ```c
     const char* ssid = "Your_WiFi_SSID";
     const char* password = "Your_WiFi_Password";
     const char* mqtt_server = "192.168.x.x";  // Your broker IP
     ```

5. **Upload to ESP32**
   ```bash
   Arduino IDE → Select Board: ESP32 Dev Module
                → Select Port: COMx (Windows) or /dev/ttyUSBx (Linux)
                → Click Upload
   ```

### Building from Source

**Using Arduino IDE:**
- Open `final/final.ino` → Click Verify → Click Upload

**Using PlatformIO:**
```bash
pio project init --board esp32dev
pio run
pio run --target upload
```

---

## ❓ FAQ

### **Q: Can I use a different microcontroller?**
**A:** The code is ESP32-specific (uses ESP-IDF peripherals). Porting to STM32L, ARM Cortex-M, or other MCUs would require hardware abstraction layer rewrite (~40% effort).

### **Q: What's the range of WiFi connectivity?**
**A:** Standard WiFi 802.11n: ~100m line-of-sight, ~30m through walls. Consider WiFi extenders for larger facilities.

### **Q: Can I use cloud MQTT instead of local broker?**
**A:** Yes! Replace `mqtt_server` with cloud broker IP (AWS IoT, Azure IoT Hub, HiveMQ Cloud). Requires SSL/TLS certificate configuration.

### **Q: How do I calibrate the sensors?**
**A:** 
- **MPU6050:** Auto-calibrates 2000 samples on startup (place on level surface, don't move)
- **NTC Thermistor:** Pre-calibrated using Steinhart-Hart equation; adjust `BETA` value for accuracy

### **Q: What happens if WiFi disconnects?**
**A:** MQTT Task auto-retries every 2 seconds. Local sensor data continues collecting; publishes resume when connection restored.

### **Q: Can I extend the system with additional sensors?**
**A:** Yes! Add new FreeRTOS task with its own 4KB stack. Ensure total stack < available heap (~80KB). Use mutual exclusion (mutex) for shared resources.

### **Q: What's the cost of deployment per unit?**
**A:** ~$12-15 for components + $0 for firmware (open-source). At scale, unit cost could reach $8-10.

### **Q: Is this suitable for industrial production?**
**A:** This system is **production-ready for small/medium enterprises** with 24/7 monitoring needs. For OEM integration, add:
  - Industrial-grade vibration sensors (±50g range)
  - Redundant temperature sensors
  - Edge ML for predictive analytics
  - IEC 61508 SIL certification

---

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

### Development Workflow
1. **Fork** the repository
2. **Create a feature branch**: `git checkout -b feature/your-feature-name`
3. **Make changes** with meaningful commit messages
4. **Test thoroughly** on ESP32 hardware
5. **Submit a Pull Request** with description of changes

### Code Style
- **Language:** Embedded C (C99 standard)
- **Naming:** `snake_case` for variables, `UPPER_CASE` for constants
- **Indentation:** 2 spaces (not tabs)
- **Comments:** Only for non-obvious logic
- **Max line length:** 100 characters

### Commit Message Format
```
[AREA] Brief description (50 chars max)

Detailed explanation (if needed)
- Point 1
- Point 2

Fixes #issue_number
```

### Testing Checklist
- [ ] Code compiles without warnings
- [ ] Hardware functions as expected
- [ ] MQTT publishes data correctly
- [ ] Fault detection triggers at thresholds
- [ ] 1-hour stress test completes successfully

### Areas for Contribution
- 🐛 **Bug fixes** (issues marked with `bug` label)
- ✨ **New features** (issues marked with `enhancement`)
- 📚 **Documentation** improvements
- 🧪 **Unit tests** for sensor calibration
- 🔒 **Security hardening** (encryption, authentication)
- 🌐 **Localization** of documentation

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

**Academic Attribution:**  
If you use this project in research or publications, please cite:

```bibtex
@misc{rtos_motor_health_2026,
  author = {Kalal, Divyanshu and Patel, Happy},
  title = {RTOS-Based Predictive Maintenance System for Electric Motors using ESP32},
  year = {2026},
  publisher = {GitHub},
  howpublished = {\url{https://github.com/Divyanshu-53/rtos-motor-health-monitoring-esp32-predictive-maintenance}}
}
```

---

## 👥 Authors & Contributors

| Name | GitHub |
|------|--------|
| **Divyanshu Kalal** | [@Divyanshu-53](https://github.com/Divyanshu-53) |
| **Happy Patel** | [@Happy251005](https://github.com/Happy251005) |

**Project Date:** March 28, 2026  

### Getting Support
- **Issues & Bugs:** [GitHub Issues](https://github.com/Divyanshu-53/rtos-motor-health-monitoring-esp32-predictive-maintenance/issues)
- **Discussions:** [GitHub Discussions](https://github.com/Divyanshu-53/rtos-motor-health-monitoring-esp32-predictive-maintenance/discussions)

---

## 📚 Additional Resources

- 📄 **[Full Project Report](PROJECT_REPORT.pdf)** - 12-page academic paper with detailed analysis
- 🎨 **[Hardware Diagrams](docs/images/)** - Circuit schematics and block diagrams
- 🖼️ **[Screenshots](docs/images/)** - Node-RED dashboard, Blynk mobile app UI
- 📊 **[Test Results](docs/)** - Performance metrics and validation data
- 🔗 **[FreeRTOS Documentation](https://www.freertos.org/)** - Official kernel documentation
- 🔗 **[ESP32 Technical Reference](https://docs.espressif.com/projects/esp-idf/en/latest/)** - Hardware specifications
- 🔗 **[MQTT Protocol Spec](https://mqtt.org/)** - Message Queuing Telemetry Transport standard

---

## 🏆 Recognition & Achievements

✅ **Real-time embedded systems design** using FreeRTOS  
✅ **Multi-rate deterministic sensing** (independent 1Hz + 5Hz sampling)  
✅ **IoT protocol integration** (MQTT, Blynk, Google Sheets, MongoDB)  
✅ **Predictive maintenance algorithms** with fault classification  
✅ **Ultra-low-cost hardware design** (<$15 BOM)  
✅ **Production-ready firmware** (4-hour stress test, zero crashes)  
✅ **Full open-source** hardware & software  

---

## 🙏 Acknowledgments

- **FreeRTOS Community** for excellent real-time kernel
- **Espressif Systems** for outstanding ESP32 platform
- **Arduino Community** for accessible embedded development
- All contributors and users of this project

---

<div align="center">

### ⭐ If this project helped you, please consider giving it a star!

**Status:** ✅ **COMPLETE & PRODUCTION-READY**  

**Repository:** [GitHub - RTOS Motor Health Monitoring ESP32](https://github.com/Divyanshu-53/rtos-motor-health-monitoring-esp32-predictive-maintenance)

*Last Updated: March 30, 2026*

</div>
