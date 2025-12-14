# Telemetry-Simulation-UART-Streaming-Task

Overview

This project simulates a telemetry transmission system similar to those used in embedded, aerospace, or IoT applications.
A C program generates synthetic IMU and environmental sensor data, frames it with a checksum, and transmits it over a virtual UART interface.
The setup runs on Ubuntu via Windows Subsystem for Linux (WSL) using pseudo-terminals to emulate serial communication.

System Setup

OS: Windows 10/11
Linux Environment: Ubuntu (WSL)
Language: C (GCC)
Communication: Virtual UART using socat

How to Compile and Run
1. Install Required Packages
sudo apt update
sudo apt install gcc socat

2. Create Virtual UART Ports

Open Terminal 1 and run:
socat -d -d pty,raw,echo=0 pty,raw,echo=0


Example output:

PTY is /dev/pts/2
PTY is /dev/pts/3
These act as two ends of a serial cable.

3. Compile the Telemetry Transmitter
Open Terminal 2 and navigate to the source file:
gcc telemetry_tx.c -o tx

4. Run the Transmitter
./tx
The program continuously sends telemetry data to /dev/pts/2.

5. (Optional) Receive and Log Data
You can listen to the other UART end:
cat /dev/pts/3


Or redirect to a file:
cat /dev/pts/3 > output.csv

Telemetry Frame Format
Each frame follows this structure:
$L1,timestamp,ax,ay,az,gx,gy,gz,alt,temp*CHECKSUM
Example:
$L1,1733984032000,0.12,-0.45,0.98,0.01,-0.02,0.05,54.3,29.1*3A

$ → Start of frame

L1 → Frame identifier

* → Checksum delimiter

CHECKSUM → XOR checksum (hex)

AI Tools Used
ChatGPT (OpenAI)

Used for:
Understanding UART communication concepts
Structuring telemetry frame formats
Clarifying C system calls (open, write, termios)
Debugging build and runtime issues in WSL
No AI-generated code was blindly copied; all code was reviewed, understood, and modified as needed.

Assumptions and Simplifications
Simulated Sensor Data
IMU and environmental data are generated using rand() instead of real sensors.
This avoids hardware dependency while demonstrating logic.
Virtual UART
Serial communication is emulated using socat and pseudo-terminals.
Assumes behavior similar to real UART devices.
Simple Checksum
XOR checksum is used instead of CRC for simplicity.
Adequate for demonstration but not production-grade.
Fixed Baud Rate

UART speed is set to 9600 bps.
No dynamic configuration or error handling implemented.

Continuous Transmission
The transmitter runs indefinitely at a fixed 20 Hz rate.
No command or control channel implemented.

Learning Outcome
UART communication fundamentals
Telemetry framing and checksum validation
Linux device files and serial interfaces
Practical use of WSL for embedded software simulation

Possible Extensions
Replace random data with real IMU sensors
Implement CRC-16 for checksum
Add receiver-side validation and parsing
Log telemetry into structured CSV or database
