# Installation & Setup Guide

Complete step-by-step guide to get the RTOS Motor Health Monitoring System up and running.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Hardware Assembly](#hardware-assembly)
3. [Software Installation](#software-installation)
4. [Configuration](#configuration)
5. [Verification](#verification)
6. [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Required Hardware
- **ESP32 WROOM-32** development board
- **USB-A to Micro-USB cable** (for programming)
- **NTC Thermistor** (10kΩ, B-value 3950K)
- **MPU6050** 6-axis IMU sensor
- **10kΩ Resistor** (voltage divider for NTC)
- **Breadboard or PCB** (for connections)
- **Jumper wires** (male-to-male)
- **5V Power Supply** (USB or external)

### Required Software
- **Arduino IDE 2.0+** (or PlatformIO)
- **Git** (for cloning repository)
- **Python 3.8+** (for MQTT/Node-RED)
- **Mosquitto MQTT Broker**
- **Node-RED** (optional, for dashboard)

### System Requirements
- **OS:** Windows 10+, macOS 10.13+, or Linux (Ubuntu 18.04+)
- **RAM:** 2GB minimum
- **Storage:** 500MB free space
- **Network:** WiFi connectivity required

---

## Hardware Assembly

### Step 1: Identify Components

```
┌─────────────────────────────────────────┐
│         ESP32 WROOM-32 Pinout          │
│                                         │
│  3V3 ─────────────────────────── GND    │
│  EN  ─────────────────────────── GPIO23 │
│  SVP ─────────────────────────── GPIO22 │ ← SCL (I2C)
│  SVN ─────────────────────────── GPIO21 │ ← SDA (I2C)
│  GPIO34 ──────────────────────── GPIO19 │
│   ↑                                      │
│   └─ ADC (NTC Thermistor)               │
│                                         │
│  GND ─────────────────────────── GPIO5  │
│  GPIO35 ─────────────────────── GPIO18  │
│  GPIO32 ─────────────────────── GPIO17  │
│  GPIO33 ─────────────────────── GPIO16  │
│  GPIO25 ─────────────────────── GPIO4   │
│  GPIO26 ─────────────────────── GPIO0   │
│  GPIO27 ─────────────────────── GPIO2   │
│  GPIO14 ─────────────────────── GPIO15  │
│  GPIO12 ─────────────────────── GPIO13  │
│  GND ─────────────────────────── GPIO11 │
│  VIN ─────────────────────────── GPIO10 │
└─────────────────────────────────────────┘
```

### Step 2: Connect NTC Thermistor

**Circuit Design:**
```
    5V
    │
    ├─ [10kΩ Resistor] ─┬─ GPIO34 (ADC input)
    │                   │
  [NTC 10kΩ]           │
    │                   │
    └─────────────────┬─┘
                      │
                     GND
```

**Connections:**
1. Connect one end of NTC thermistor to **5V**
2. Connect other end to **GND**
3. Connect the junction between NTC and 10kΩ resistor to **GPIO34**
4. 10kΩ resistor: one end to **5V**, other end to junction and **GND**

### Step 3: Connect MPU6050 (I2C)

**I2C Bus Pinout:**
```
MPU6050          ESP32
─────────        ─────────
VCC      ←→      3V3
GND      ←→      GND
SDA      ←→      GPIO21
SCL      ←→      GPIO22
INT      ←→      (optional, leave floating)
```

**Connections:**
1. MPU6050 **VCC** → ESP32 **3V3** (3.3V power)
2. MPU6050 **GND** → ESP32 **GND** (common ground)
3. MPU6050 **SDA** → ESP32 **GPIO21** (I2C data)
4. MPU6050 **SCL** → ESP32 **GPIO22** (I2C clock)

### Step 4: Connect USB Power

1. Connect **Micro-USB cable** to ESP32's USB port
2. Connect USB cable to your computer
3. ESP32 should power on (LED should light up)

### Step 5: Physical Assembly

1. **Secure all components** on breadboard/PCB
2. **Double-check all connections** (especially GND)
3. **Use heat shrink** or tape to insulate solder connections
4. **Mount sensors** close to motor for accurate readings
5. **Keep wires short** to reduce noise

---

## Software Installation

### Step 1: Install Arduino IDE

**Windows:**
1. Download from https://www.arduino.cc/en/software
2. Run installer (arduino-ide_*.exe)
3. Accept license and select components
4. Choose installation path
5. Complete installation

**macOS:**
1. Download .dmg file
2. Drag Arduino IDE to Applications folder
3. Open and complete setup

**Linux (Ubuntu):**
```bash
sudo apt update
sudo apt install arduino
```

### Step 2: Install ESP32 Board Support

1. Open **Arduino IDE**
2. Go to **File → Preferences**
3. In "Additional Boards Manager URLs", paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board Manager**
6. Search "ESP32"
7. Click **Install** for "ESP32 by Espressif Systems"
8. Wait for installation (2-5 minutes)
9. Close Board Manager

### Step 3: Install Required Libraries

1. Go to **Sketch → Include Library → Manage Libraries**
2. Search and install each:
   - **"PubSubClient"** by Nick O'Leary (v2.8.0+)
   - **"BlynkSimpleEsp32"** by Blynk Labs (v0.7.1+)
   - **"Wire"** (built-in, usually pre-installed)

3. Click Install for each library
4. Wait for completion

### Step 4: Clone Repository

```bash
# Create a workspace folder
mkdir ~/MotorMonitoring
cd ~/MotorMonitoring

# Clone repository
git clone https://github.com/Divyanshu-53/rtos-motor-health-monitoring-esp32.git
cd rtos-motor-health-monitoring-esp32
```

### Step 5: Install Mosquitto MQTT Broker

**Windows:**
1. Download from https://mosquitto.org/download/
2. Run installer
3. Accept defaults
4. Complete installation
5. Mosquitto will run as a service

**macOS (using Homebrew):**
```bash
brew install mosquitto
brew services start mosquitto
```

**Linux (Ubuntu):**
```bash
sudo apt update
sudo apt install mosquitto
sudo systemctl start mosquitto
```

---

## Configuration

### Step 1: Configure WiFi

Open `final/final.ino` in Arduino IDE:

```cpp
// Line 6-7: Update WiFi credentials
const char* ssid = "Your_WiFi_SSID";           // Your home WiFi name
const char* password = "Your_WiFi_Password";   // Your WiFi password
```

### Step 2: Configure MQTT Broker

```cpp
// Line 11: Update MQTT broker IP
const char* mqtt_server = "192.168.x.x";  // Your computer's IP address
const int mqtt_port = 1883;                // Default MQTT port (don't change)
```

**Finding your IP address:**

**Windows (Command Prompt):**
```cmd
ipconfig
Look for "IPv4 Address" (usually 192.168.x.x)
```

**macOS/Linux (Terminal):**
```bash
ifconfig
Look for "inet" address under your WiFi interface
```

### Step 3: Optional - Configure Blynk

If using Blynk dashboard (SAFECODE1.ino):

```cpp
// Line 6-8
#define BLYNK_TEMPLATE_ID "Your_Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Template_Name"
#define BLYNK_AUTH_TOKEN "Your_Auth_Token"
```

Get these from Blynk IoT console: https://blynk.cloud/

---

## Uploading Code

### Step 1: Configure Arduino IDE

1. **Select Board:**
   - **Tools → Board → esp32 → "ESP32 Dev Module"**

2. **Select Port:**
   - **Tools → Port → COM3** (or your device's port)
   - If no port visible, driver may need installation

3. **Set Upload Speed:**
   - **Tools → Upload Speed → 115200**

### Step 2: Compile & Upload

1. Click **Verify** (checkbox icon) to compile
   - Should show "Compilation complete"
   - Take note of any warnings

2. Click **Upload** (arrow icon) to program
   - Terminal should show progress
   - Wait for "Hard resetting via RTS pin..." message
   - Should complete in 10-30 seconds

3. If successful, LED on ESP32 blinks

---

## Verification

### Step 1: Check Serial Output

1. Click **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. Should see startup messages:
   ```
   Connecting WiFi...........
   WiFi connected
   IP address: 192.168.x.x
   Connecting MQTT...connected
   ```

### Step 2: Verify MQTT Topics

**Windows Command Prompt:**
```cmd
mosquitto_sub -h 192.168.x.x -t "motor/#" -v
```

**macOS/Linux Terminal:**
```bash
mosquitto_sub -h 192.168.x.x -t "motor/#" -v
```

Should show continuous updates:
```
motor/temp 28.45
motor/vibration 0.032
motor/status 0
motor/temp 28.47
...
```

### Step 3: Test Fault Detection

1. **Overheat Test:**
   - Hold NTC thermistor under warm water (>75°C)
   - Serial monitor should show "Status: 2"
   - MQTT topic: `motor/status` should be "2"

2. **Vibration Test:**
   - Gently tap MPU6050 sensor
   - Serial monitor should show "Status: 1"
   - MQTT topic: `motor/status` should be "1"

---

## Troubleshooting

### Cannot Upload Code

**Problem:** "Failed to connect to ESP32"
**Solution:**
- Check USB cable (try different cable)
- Restart Arduino IDE
- Hold **BOOT** button while uploading
- Update CH340 driver (common on cheap boards)

**Problem:** "Port COM3 not found"
**Solution:**
- Try different USB port on computer
- Install CH340 driver: https://www.wch-ic.com/downloads/
- Restart computer

### WiFi Connection Failed

**Problem:** "Cannot connect to WiFi"
**Solution:**
- Verify SSID and password are correct
- Check WiFi 2.4GHz (ESP32 doesn't support 5GHz)
- Look for "Connecting WiFi" in serial monitor
- Restart ESP32

### MQTT Connection Failed

**Problem:** "MQTT failed, rc=X"
**Solution:**
- Check Mosquitto is running: `mosquitto -v`
- Verify broker IP is correct: `ipconfig`
- Check firewall allows port 1883
- Verify no IP conflicts

### Sensors Not Reading

**Problem:** "Temperature always 0 or 4095"
**Solution:**
- Check ADC connections (GPIO34)
- Verify 10kΩ resistor is present
- Check NTC polarity

**Problem:** "Vibration always 0"
**Solution:**
- Check I2C connections (GPIO21/22)
- Verify MPU6050 address: `i2cdetect` tool
- I2C pull-up resistors may be needed (4.7kΩ)

### Frequent Crashes/Resets

**Problem:** "Watchdog timer warnings"
**Solution:**
- Serial prints slow down loop
- Reduce sensor polling rate
- Check MQTT connection timeouts
- Restart ESP32

---

## Next Steps

1. **Import Node-RED Dashboard:** `flows_nodered.json`
2. **Setup Blynk Mobile App:** Configure if using SAFECODE1.ino
3. **Connect to Google Sheets:** Via Node-RED
4. **Monitor 24/7:** Let system run continuously
5. **Analyze Data:** Check for patterns and anomalies

---

**Estimated Time:** 45 minutes for complete setup  
**Support:** For issues, check README.md troubleshooting or open GitHub issue

**Happy Monitoring! 🎉**
