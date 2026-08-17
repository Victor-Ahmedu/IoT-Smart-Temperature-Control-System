<img width="3072" height="4080" alt="IMG_20260817_152812" src="https://github.com/user-attachments/assets/4491c95f-cad9-480e-9532-4fc0354eae86" />
<img width="3072" height="4078" alt="IMG_20260817_155939" src="https://github.com/user-attachments/assets/aac4d0e7-4372-4767-91f9-054c0acc96cb" />




# 🌡️ IoT Smart Temperature Control System

An IoT-based smart environment monitoring and control system built with the **NodeMCU ESP8266**, **DHT11 sensor**, **ST7735 TFT display**, and **relay modules**. The system continuously monitors ambient temperature and humidity and automatically controls a **light**, **fan**, and **humidifier** to maintain a comfortable indoor environment.

---

## 📖 Overview

This project demonstrates the application of embedded systems and IoT principles in environmental automation. The NodeMCU ESP8266 collects temperature and humidity data from a DHT11 sensor and controls connected electrical devices through relay modules based on predefined thresholds.

A 1.8-inch ST7735 TFT display provides real-time feedback by showing sensor readings and the operating status of each controlled device.

---

## ✨ Features

- 🌡️ Real-time temperature monitoring
- 💧 Real-time humidity monitoring
- 💡 Automatic light control
- 🌀 Automatic fan control
- 💨 Automatic humidifier control
- 📺 Real-time TFT display interface
- 🖥️ Serial Monitor debugging output
- 🔄 Hysteresis-based temperature control to prevent relay chattering
- ⚡ Active LOW relay support

---

## 🛠 Hardware Components

| Component | Quantity |
|-----------|---------:|
| NodeMCU ESP8266 | 1 |
| DHT11 Temperature & Humidity Sensor | 1 |
| ST7735 TFT Display | 1 |
| 3-Channel Relay Module | 1 |
| Breadboard | 1 |
| Jumper Wires | Several |
| USB Cable | 1 |
| Light Bulb (Load) | 1 |
| DC Fan | 1 |
| Humidifier | 1 |

---

## 💻 Software Requirements

- Arduino IDE
- ESP8266 Board Package
- DHT Sensor Library
- Adafruit GFX Library
- Adafruit ST7735 Library
- SPI Library

---

## 🔌 Pin Connections

| Device | NodeMCU Pin |
|---------|-------------|
| DHT11 Data | D4 |
| Humidifier Relay | D0 |
| Fan Relay | D6 |
| Light Relay | D1 |
| TFT CS | D8 |
| TFT DC | D2 |
| TFT RST | -1 (Not Connected) |

---

## ⚙️ System Operation

### Temperature Control

- Temperature below **28°C**
  - Light turns **ON**
  - Fan turns **OFF**

- Temperature above **32°C**
  - Light turns **OFF**
  - Fan turns **ON**

The hysteresis between 28°C and 32°C prevents frequent switching of the relays.

### Humidity Control

- Humidity below **20%**
  - Humidifier turns **ON**

- Humidity at or above **20%**
  - Humidifier turns **OFF**

---

## 📺 TFT Display

The display shows:

- Temperature
- Humidity
- Humidifier Status
- Fan Status
- Light Status
- Overall System Status

---

## 📂 Project Structure

```text
IoT-Smart-Temperature-Control-System/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── src/
│   └── IoT_Smart_Temperature_Control_System.ino
│
├── images/
│   ├── prototype.jpg
│   ├── tft-display.jpg
│   ├── wiring.jpg
│   └── demo.gif
│
├── circuit/
│   └── wiring-diagram.pdf
│
└── docs/
    └── Project_Report.pdf
```

---

## 🚀 Getting Started

1. Clone this repository.

```bash
git clone https://github.com/Victor-Ahmedu/IoT-Smart-Temperature-Control-System.git
```

2. Open the `.ino` file in Arduino IDE.

3. Install the required libraries.

4. Select:

- Board: **NodeMCU 1.0 (ESP-12E Module)**

5. Upload the program.

6. Open the Serial Monitor (115200 baud).

---


## 🔮 Future Improvements

- Wi-Fi dashboard for remote monitoring
- Mobile application integration
- MQTT support
- ThingSpeak integration
- Blynk integration
- Data logging to cloud
- SMS/Email alerts
- OLED display option
- ESP32 version

---

## 👨‍💻 Author

**Victor Ahmedu**

Electrical & Electronic Engineering Undergraduate

Embedded Systems | IoT | Automation

GitHub: https://github.com/Victor-Ahmedu

---

## 📜 License

This project is licensed under the MIT License.

See the LICENSE file for details.

---

## ⭐ Support

If you found this project useful, please consider giving it a ⭐ on GitHub.
