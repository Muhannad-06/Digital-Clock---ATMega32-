# ATmega32 Digital Clock & Timer System

A feature-rich, multi-mode digital clock built on the ATmega32 microcontroller. Written in embedded C, this project utilizes a strict layered architecture (MCAL, HAL, APP) and features hardware interrupts, ADC reading, and custom LCD character generation.
## Team
* Muhannad Mahfouz El-Shahiedy
* Mahmoud Emad Ibrahim
* Abdallah Emad Abdallah
## 🌟 Features

The system operates across 4 distinct modes, controlled via a push-button state machine:

*   **Mode 1: Digital Clock**
    *   Displays real-time in `HH:MM:SS` format.
    *   Timekeeping is driven in the background by a synchronous Timer1 CTC interrupt for 1-second accuracy.
*   **Mode 2: Countdown Timer**
    *   Configurable range from 5 to 45 minutes.
    *   Runs completely in the background; you can switch back to the clock while the timer counts down.
    *   **Visual & Audio Alerts:** The last 9 seconds are displayed dynamically on a 7-Segment display. When the timer hits zero, a buzzer rings.
*   **Mode 3: Smart Alarm**
    *   Alarm hour is set dynamically using an analog potentiometer (ADC mapping).
    *   Alarm minute is incremented via a dedicated push-button.
    *   The alarm state is stored globally and triggers a buzzer in the background when it matches the current time.
*   **Mode 4: Calendar / Date**
    *   Displays the current date (`DD/MM/YYYY`).
    *   Features a custom-drawn calendar icon loaded directly into the LCD's CGRAM.

---

## 🏗️ Software Architecture

The codebase is strictly organized into three abstraction layers to ensure reusability, portability, and maintainability:

1.  **MCAL (Microcontroller Abstraction Layer):**
    *   **DIO:** Handles pin directions and digital I/O.
    *   **TIMER:** Manages Timer1 (1Hz Tick) and Timer0/Timer2.
    *   **ADC:** Handles the analog-to-digital conversion for the potentiometer.
    *   **EXTI:** Manages external interrupts for responsive button presses.
2.  **HAL (Hardware Abstraction Layer):**
    *   **LCD:** 4-bit mode driver for the HD44780 16x2 character display.
    *   **7-Segment:** Driver for displaying digits on a common-cathode 7-segment display.
3.  **APP (Application Layer):**
    *   Contains the state machine, logic flow, display-update throttling, and background timekeeping variables.

---

## 🔌 Hardware Configuration & Pin Mapping

This project was simulated in Proteus and is designed for the standard ATmega32 setup. 

### Output Devices
| Component | ATmega32 Pin(s) | Description |
| :--- | :--- | :--- |
| **LCD Data (4-Bit)** | `PA4` - `PA7` | `D4`, `D5`, `D6`, `D7` connected to Port A. |
| **LCD Control** | `PC0` - `PC2` | `RS` (PC0), `RW` (PC1), `EN` (PC2). |
| **7-Segment Display** | `PB0` - `PB6` | Standard 7-segment pins A-G mapped to Port B. |
| **Buzzer** | `PC5` | Active high buzzer output. |

### Input Devices
| Component | ATmega32 Pin | Description |
| :--- | :--- | :--- |
| **Mode Button** | `PD2` (INT0) | Cycles through the 4 application modes. |
| **Set Button** | `PD3` (INT1) | Adjusts timer/alarm minutes. |
| **Start Button** | `PD4` | Starts the countdown timer. |
| **Toggle Button** | `PD6` | Pauses/Resumes timer, or Arms/Disarms the alarm. |
| **Potentiometer** | `PA0` (ADC0) | Adjusts the alarm hour via an analog voltage read. |

> **Note:** All push buttons are configured using the ATmega32's internal pull-up resistors and trigger on a falling edge (connected to GND).

---
