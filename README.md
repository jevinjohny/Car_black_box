# 🧠 Blackbox

### Embedded Firmware Control System

A Car Black Box System developed using the PIC18F4580 microcontroller and Embedded C to monitor and record critical vehicle events in real-time. This system functions similarly to an aircraft black box, providing valuable data for accident analysis, driver behavior monitoring, and vehicle diagnostics.

---

## 🚀 Key Features

- 📟 **Display-Driven Interface**
  - LCD screen output for live system status
  - Structured dashboard and menu presentation

- ⌨️ **User Input Handling**
  - Matrix keypad interaction
  - Menu navigation and control toggling

- ⚙️ **Peripheral Integration**
  - ADC-based sensing
  - I2C communication
  - UART communication
  - Real-time clock support

- 🧩 **Modular Firmware Structure**
  - Separated driver and application logic
  - Easy to extend for new features

---

## 🧠 System Overview

### 1️⃣ Main Application
- Coordinates initialization of peripherals
- Runs the dashboard or menu based on system state
- Serves as the central control loop

### 2️⃣ Dashboard Logic
- Displays live information to the user
- Presents the main operational view

### 3️⃣ Menu Logic
- Handles navigation and interaction flows
- Enables user configuration and control

---

## 🔧 Requirements

- MPLAB X
- XC8 compiler
- PIC development toolchain

## ▶️ Build

1. Open the project in MPLAB X.
2. Build the default configuration.
3. Program the target device.

## 📁 Main Files

- [newmain.c](newmain.c) — main application flow
- [dashboard.c](dashboard.c) — dashboard logic
- [menu.c](menu.c) — menu logic
- [clcd.c](clcd.c), [adc.c](adc.c), [ds1307.c](ds1307.c), [uart.c](uart.c) — peripheral drivers
