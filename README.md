Bare-Metal Arduino Reaction Timer

A microsecond-precision digital chronometer built on the ATmega328P using direct register programming.

Overview

This project explores the low-level capabilities of the ATmega328P microcontroller by bypassing the standard Arduino abstraction layer. Instead of using functions like digitalRead() and millis(), which introduce latency, this reaction timer uses direct port manipulation, hardware interrupts, and 16-bit Timer registers to achieve theoretical precision down to 4 microseconds.

The result is a highly responsive, non-blocking system that measures human reaction time with far greater accuracy than a typical loop()-based implementation.

Key Features : 

Zero-Latency Input: Utilizes External Hardware Interrupts (INT0/INT1) to capture button presses instantly, eliminating polling delays.

Precision Timing: Configured the 16-bit Timer1 directly (TCCR1B, OCR1A) to function as both a random delay generator and a high-resolution stopwatch.

Overflow Handling: Implemented custom ISR logic to track timer overflows, enabling accurate measurement of durations longer than the 16-bit limit (approx 1.048s) without losing precision.

Hybrid Architecture: Designed a system that uses bare-metal C++ for critical timing paths while leveraging standard libraries for the non-critical OLED interface.

🛠️ Hardware Architecture

Microcontroller: ATmega328P (Arduino Uno)
Display: 128x32 I2C OLED (SSD1306 Driver)
Inputs: 2x Tactile Push Buttons (Start, React)



Link to Live Wokwi Simulation : https://wokwi.com/projects/449257966648675329


Check out the full source code and simulation. If you're interested in embedded systems engineering, let's connect on LinkedIn.