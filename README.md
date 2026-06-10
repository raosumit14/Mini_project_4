# ESP32 GPS & Pressure Sensor Data Logger

## Overview

An embedded data logging system built using the ESP32 microcontroller that collects GPS location and pressure sensor readings in real time and stores them on a microSD card for future analysis.

This project demonstrates sensor interfacing, UART communication, SPI communication, and embedded data acquisition techniques.

---

## Features

- Real time GPS tracking
- Pressure sensor monitoring
- Data logging to microSD card
- Timestamped measurements
- Portable embedded system

---

## Hardware Components

| Component | Description |
|------------|------------|
| ESP32-WROOM | Main Controller |
| Neo 6M GPS Module | Location & Time Data |
| Hx710B Pressure Sensor | Pressure Measurement |
| SD Card Module | Data Storage Interface |
| MicroSD Card | Log Storage |
| Power Supply | System Power |

---

## System Architecture

```text
GPS Module
     │
     ├── Latitude
     ├── Longitude
     ├── Time

Pressure Sensor
     │
     └── Pressure Data

            ↓

        ESP32

            ↓

      SD Card Module

            ↓

      CSV Log File
```

##  Communication Protocols

### UART
Used for communication with the GPS module.

### SPI
Used for communication with the SD card module.

### GPIO / ADC
Used for interfacing the pressure sensor.

---

##  Working Principle

1. ESP32 initializes all peripherals.
2. GPS module provides location and timing information.
3. Pressure sensor measures pressure values.
4. ESP32 processes sensor data.
5. Data is formatted into a log record.
6. Log records are stored on the microSD card.
7. Data can later be analyzed on a computer.

---

## Sample Logged Data

```csv
Timestamp,Latitude,Longitude,Pressure
2026-06-01 10:15:02,31.3260,75.5762,101.3
2026-06-01 10:15:07,31.3261,75.5764,101.5
2026-06-01 10:15:12,31.3263,75.5767,101.4
```

---

## Project Images
<img width="1600" height="1200" alt="WhatsApp Image 2026-06-10 at 3 56 31 PM" src="https://github.com/user-attachments/assets/b126c26f-44bd-4097-b31c-04f17ef81250" />
<img width="1200" height="1600" alt="WhatsApp Image 2026-06-10 at 3 57 35 PM" src="https://github.com/user-attachments/assets/d4a7bcac-1237-42d5-b4bd-f8a41d5872f8" />
<img width="1068" height="815" alt="WhatsApp Image 2026-06-10 at 3 58 49 PM" src="https://github.com/user-attachments/assets/f2aadfec-1c3c-4f03-a1f7-e4a067bbb5c5" />
<img width="1280" height="681" alt="image" src="https://github.com/user-attachments/assets/dca04ca6-2b7f-4b5f-8208-704f9b394ec7" />


##  Demo Video
https://youtube.com/shorts/_Tw0hG10OkQ?feature=share

---

## Applications

- Environmental Monitoring
- Weather Stations
- Dump Vehicle Tracking
- Industrial Waste Monitoring
- Research Projects

---

## Future Improvements

- Wi-Fi Data Upload
- Cloud Dashboard
- Battery Monitoring
- Multi Sensor Support
- Real Time Visualization

---

## Skills Demonstrated

- Embedded Systems Design
- ESP32 Programming
- Sensor Interfacing
- UART Communication
- SPI Communication
- Data Logging
- File Handling
- IoT Development

---

## Author

**Sumit**
