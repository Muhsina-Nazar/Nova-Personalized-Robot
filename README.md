# NOVA – Intelligent Voice-Controlled Robot

An interactive, AI-powered desktop robot assistant that combines real-time speech recognition, conversational intelligence via Google Gemini, text-to-speech synthesis, and synchronized physical gestures powered by an Arduino microcontroller.

---

## Table of Contents

- [Overview](#overview)
- [Key Features](#key-features)
- [How NOVA Works](#how-nova-works)
- [Technology Stack](#technology-stack)
- [Hardware Components](#hardware-components)
- [Software Architecture](#software-architecture)
- [Project Structure](#project-structure)
- [Installation & Setup](#installation--setup)
- [Running the Project](#running-the-project)
- [Arduino Setup](#arduino-setup)
- [Gemini API Integration](#gemini-api-integration)
- [Use Cases](#use-cases)
- [Limitations](#limitations)
- [Future Improvements](#future-improvements)
- [Author](#author)

---

## Overview

**NOVA** is a personalized, voice-controlled robotic assistant designed for natural human-robot interaction. By integrating a Python-based processing pipeline with Google's Gemini AI and an Arduino hardware controller, NOVA listens to user voice queries, processes contextual responses, speaks back naturally, and physically animates gestures such as waving its arm or nodding its head.

---

## Key Features

- **Voice-Activated Interaction**: Listens to speech through a microphone and transcribes audio to text in real time using Google Speech Recognition.
- **Conversational AI with Google Gemini**: Utilizes the `gemini-1.5-flash-latest` generative model to provide smart, context-aware answers to open-ended user queries.
- **Local Instant Commands**: Rapidly handles built-in utility queries locally without API latency (e.g., current time, date, day of the week, year, and name introduction).
- **Natural Text-to-Speech (TTS)**: Converts generated responses into clear spoken audio using `pyttsx3` with customizable voice profiles and rate settings.
- **Audio Cue Feedback**: Uses `pygame` mixer to play audio cues indicating when the robot begins listening and when it converts speech.
- **Expressive Physical Gestures**: 
  - **Hand Waving**: Automatically triggers arm-waving gestures upon greeting (`"hello"`, `"hi"`) or farewell (`"bye"`, `"exit"`).
  - **Head Nodding**: Triggers head-nodding movements when processing negative response patterns (`no`, `not`, `don't`, `can't`, `won't`).
- **Real-Time Serial Motor Control**: Bridges software logic with hardware servos via serial communication using `cvzone.SerialModule`.

---

## How NOVA Works

The system operates in a continuous interactive loop:

```
┌──────────────┐     ┌──────────────────────┐     ┌──────────────────────┐
│  User Voice  │ ──> │ Speech Recognition   │ ──> │ Intent & AI Analysis │
│ (Microphone) │     │ (Audio to Text)      │     │ (Local / Gemini API) │
└──────────────┘     └──────────────────────┘     └──────────┬───────────┘
                                                             │
                      ┌──────────────────────────────────────┴──────────────────────────────────────┐
                      ▼                                                                             ▼
          ┌────────────────────────┐                                                    ┌────────────────────────┐
          │  Text-to-Speech (TTS)  │                                                    │   Serial Motor Data    │
          │     (pyttsx3/pygame)   │                                                    │     (cvzone/Serial)    │
          └───────────┬────────────┘                                                    └───────────┬────────────┘
                      ▼                                                                             ▼
          ┌────────────────────────┐                                                    ┌────────────────────────┐
          │  Spoken Audio Output   │                                                    │ Arduino Servo Gestures │
          │       (Speaker)        │                                                    │     (Arm / Head)       │
          └────────────────────────┘                                                    └────────────────────────┘
```

1. **Audio Capture**: The robot listens to the ambient environment using `speech_recognition` and adjusts for background noise.
2. **Speech-to-Text**: Voice input is transcribed to lowercase text.
3. **Intent Recognition & Response**:
   - If the user provides greetings or exit keywords, greeting actions and farewell routines are initiated.
   - If the query matches built-in requests (time, date, name), it is processed locally via Python's `datetime`.
   - All other questions are dispatched to Google's Gemini generative AI model.
4. **Physical & Speech Actuation**:
   - If greeting or negative context is detected, serial coordinates are transmitted to the Arduino to actuate servo motors (waving or nodding).
   - The response text is converted to speech using `pyttsx3` and played back through the speaker.

---

## Technology Stack

| Technology / Library | Purpose in Project |
| :--- | :--- |
| **Python** | Primary programming language orchestrating logic, audio, AI, and serial communication |
| **Google Generative AI (`google-generativeai`)** | Generative conversational intelligence using the Gemini model |
| **SpeechRecognition (`speech_recognition`)** | Captures microphone input and transcribes audio to text via Google Speech Recognition |
| **`pyttsx3`** | Offline text-to-speech engine converting response strings into spoken voice |
| **`pygame`** | Audio mixer handling cue playback and synthesized speech output |
| **`cvzone.SerialModule` / `pyserial`** | High-level serial communication layer connecting Python to the Arduino |
| **Arduino (C/C++) & `Servo.h`** | Microcontroller firmware for precise servo motor angle positioning and gesture routines |

---

## Hardware Components

- **Arduino Uno Microcontroller**: Interprets serial commands and generates PWM signals for motors.
- **Servo Motor 1 (Arm / Wave)**: Connected to Pin 9 for arm waving gestures (0°–180° range).
- **Servo Motor 2 (Head / Nod)**: Connected to Pin 11 for head nodding gestures (0°–180° range).
- **Computer / Laptop**: Hosts and executes the Python integration environment.
- **Microphone**: Captures user voice commands.
- **Speaker / Headphones**: Outputs NOVA's spoken responses and system audio cues.
- **USB Cable (Type-A to Type-B)**: Provides serial communication and power between the host computer and Arduino.

---

## Software Architecture

```
+-------------------------------------------------------------------------+
|                              HOST PC (Python)                           |
|                                                                         |
|  +--------------------+     +------------------+     +---------------+  |
|  | SpeechRecognition  | --> | Gemini AI Model  | --> | pyttsx3 &     |  |
|  | (Voice Input)      |     | (NLP Response)   |     | pygame Audio  |  |
|  +--------------------+     +--------+---------+     +---------------+  |
|                                      |                                  |
|                                      v                                  |
|                         +--------------------------+                    |
|                         |  cvzone.SerialModule     |                    |
|                         +------------+-------------+                    |
+--------------------------------------|----------------------------------+
                                       | USB Serial (9600 baud)
                                       v
+-------------------------------------------------------------------------+
|                                ARDUINO UNO                              |
|                                                                         |
|  +----------------------+     +------------------+                      |
|  | Serial.parseInt()    | --> | Servo.h Library  |                      |
|  | (Buffer Parsing)     |     +--------+---------+                      |
|  +----------------------+              |                                |
|                                        v                                |
|                        +-------------------------------+                |
|                        | Pin 9: Arm Servo (Wave)       |                |
|                        | Pin 11: Head Servo (Nod)      |                |
|                        +-------------------------------+                |
+-------------------------------------------------------------------------+
```

---

## Project Structure

```
Nova-Personalized-Robot/
├── Nova-integration.ipynb    # Main Jupyter Notebook integrating speech, Gemini AI, TTS, and serial motor control
├── Nova_arduino.ino          # Arduino firmware sketch controlling the arm and head servo motors
└── README.md                 # Project documentation and setup instructions
```

### File Summary

- **[`Nova-integration.ipynb`](file:///c:/Users/Lenovo/Documents/Nova-Personalized-Robot/Nova-integration.ipynb)**: Implements the central Python controller, handling voice recording, Google Speech Recognition, Gemini API queries, text-to-speech synthesis, and serial message transmission.
- **[`Nova_arduino.ino`](file:///c:/Users/Lenovo/Documents/Nova-Personalized-Robot/Nova_arduino.ino)**: Runs on the Arduino Uno to listen for serial coordinates (`armPos`, `headPos`) and position the servo motors with safety constraints.
- **[`README.md`](file:///c:/Users/Lenovo/Documents/Nova-Personalized-Robot/README.md)**: Comprehensive project overview, technical specifications, and setup instructions.

---

## Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/Muhsina-Nazar/Nova-Personalized-Robot.git
cd Nova-Personalized-Robot
```

### 2. Set Up a Python Virtual Environment

```bash
# Windows
python -m venv venv
venv\Scripts\activate

# Linux / macOS
python3 -m venv venv
source venv/bin/activate
```

### 3. Install Required Dependencies

```bash
pip install speechrecognition pyttsx3 pygame google-generativeai cvzone pyserial pyaudio
```

> **Note for Windows Users**: If installing `pyaudio` encounters compilation issues, install it using `pip install pipwin` followed by `pipwin install pyaudio`, or install pre-built wheels from PyPI.

---

## Running the Project

1. **Connect the Hardware**: Connect the Arduino Uno via USB and confirm its COM port in Device Manager (e.g., `COM3`).
2. **Launch the Controller**:
   Open the notebook in Jupyter Notebook, JupyterLab, or VS Code:
   ```bash
   jupyter notebook Nova-integration.ipynb
   ```
3. **Execute the Cells**:
   - Configure your Gemini API key in the designated initialization cell.
   - Run the initialization cells to establish the serial connection to the Arduino and perform initial motor calibration tests.
   - Run the `main()` function cell to start the live voice-listening loop.

---

## Arduino Setup

### 1. Hardware Wiring

| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **Arm Servo (PWM Signal)** | **Pin 9** | Controls arm waving movement |
| **Head Servo (PWM Signal)** | **Pin 11** | Controls head nodding movement |
| **Servo Power (VCC)** | **5V** | External 5V or Arduino 5V pin |
| **Servo Ground (GND)** | **GND** | Common ground with Arduino |

### 2. Flashing the Firmware

1. Connect the Arduino Uno to your PC via USB.
2. Open **[`Nova_arduino.ino`](file:///c:/Users/Lenovo/Documents/Nova-Personalized-Robot/Nova_arduino.ino)** in the [Arduino IDE](https://www.arduino.cc/en/software).
3. Under **Tools > Board**, select **Arduino Uno**.
4. Under **Tools > Port**, select the appropriate COM port.
5. Click **Upload**.

### 3. Serial Communication Protocol

The Arduino listens over standard Serial at **9600 baud**. The Python script sends two integers representing target angles:
- `[armPos, headPos]` (e.g., `[30, 90]`)
- The Arduino parses the integers with `Serial.parseInt()`, constrains them between `0` and `180`, and writes the angles to the respective servos.

---

## Gemini API Integration

The project integrates Google's **Gemini API** via the `google-generativeai` Python SDK to power conversational capabilities:

- **Model Used**: `gemini-1.5-flash-latest`
- **Role**: Provides natural language understanding and responses for open-ended questions that are not handled by local keyword filters.
- **Security Recommendation**: Supply your API key securely via environment variables or direct configuration during session startup:
  ```python
  import os
  import google.generativeai as genai

  genai.configure(api_key=os.environ.get("GEMINI_API_KEY"))
  ```

---

## Use Cases

- **Human-Robot Interaction (HRI)**: Researching multi-modal interaction combining speech with physical animatronics.
- **Educational Robotics**: Serving as a hands-on learning platform for robotics, embedded systems, and conversational AI.
- **Desktop AI Companion**: Providing hands-free utility assistance (answering questions, providing date/time, offering conversational engagement).
- **Interactive Demonstrations**: Demonstrating end-to-end integration between cloud AI models and physical microcontrollers.

---

## Limitations

- **Internet Connectivity**: Speech-to-text transcription and Gemini API query resolution require an active internet connection.
- **Microphone & Ambient Noise**: Background noise can affect the accuracy of the speech recognizer.
- **Serial Connection Dependency**: The Python host requires an active USB serial connection to the Arduino for motor actuation.
- **Fixed Servo Range**: Movement is limited to two single-axis servo motors (arm waving and head nodding).

---

## Future Improvements

- [ ] Add Computer Vision capabilities (face tracking and emotion detection via OpenCV).
- [ ] Implement wake-word activation (e.g., "Hey Nova") for hands-free triggering.
- [ ] Expand degrees of freedom with additional servo joints for full robotic arm manipulation.
- [ ] Add local offline LLM and offline speech-to-text inference for air-gapped functionality.

---

## Author

**Muhsina Nazar**
