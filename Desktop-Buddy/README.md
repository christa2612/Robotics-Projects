# 🖥️ Desktop Buddy – Interactive ESP32 Smart Companion

## 📌 Overview

**Desktop Buddy** is an interactive desktop companion developed during my internship at **Finstein Robotics**. The project combines **embedded systems, graphical user interfaces, animations, interactive applications, and firmware development** into a compact desktop device.

The system runs custom firmware on an **ESP32** and provides multiple interactive features, including a digital clock, animated emotional eyes, Pomodoro productivity timer, virtual pet, and Chrome Dino game.

A menu-driven interface allows the user to navigate between different applications through touch-based interactions. A custom boot sequence and interactive greeting screen were also implemented to create a more engaging user experience.

The project provided practical experience in **ESP32 firmware development, embedded graphical interfaces, touch-based interaction, animation programming, state-based firmware architecture, and interactive system integration**.

---

## 🎯 Objectives

- To develop an interactive multifunctional desktop companion using an ESP32.
- To design and implement an intuitive embedded graphical interface.
- To develop animated facial expressions and interactive behaviors.
- To integrate multiple applications within a single firmware.
- To implement touch-based navigation and user interaction.
- To manage multiple operating modes through structured firmware.
- To gain practical experience in embedded UI development and interactive system design.

---

## ⚙️ Features

The Desktop Buddy integrates multiple interactive applications into a single embedded system.

### 🕒 Digital Clock

The clock application provides:

- Current local time display
- World time functionality
- Touch-based navigation
- Single-tap interaction to return to the main menu

---

### 😊 Emo Eyes

The Desktop Buddy features animated eyes capable of displaying different emotional states.

Available expressions include:

- Happy
- Sad
- Angry
- Love
- Cry
- Weird
- Additional expressive states

Eye movement, blinking, and animations were implemented to create a more engaging and expressive user interaction.

---

### 🍅 Pomodoro Timer

A productivity timer was integrated into the system.

Features include:

- 5-minute default duration
- Adjustable duration from 5 to 60 minutes
- Start and Back controls
- Visual progress bar
- Completion screen displaying **"Great Job!"**
- Access through the main menu

The timer provides a simple productivity-focused application within the embedded system.

---

### 🐾 Virtual Pet

The virtual pet mode allows the user to interact with the Desktop Buddy through touch-based actions.

Features include:

- Pet interaction
- Happiness level
- Happiness progress bar
- Floating heart animations
- Anger indicators
- Interactive visual feedback

User interactions affect the displayed emotional state of the virtual pet.

---

### 🦕 Chrome Dino Game

A playable Dino game was integrated into the firmware.

Features include:

- Touch-based jumping
- Obstacle avoidance
- Game-over state
- High-score tracking
- Double-tap interaction to return to the menu after game completion

The game demonstrates the implementation of interactive controls and game logic within the ESP32 firmware.

---

### ⚙️ Settings

The Settings menu provides:

- Brightness control
- About screen
- High-score reset
- Reset of settings/data

---

## 🎬 Boot Animation

A custom boot animation was developed for the Desktop Buddy.

The boot sequence follows:

```text
Black Screen
      ↓
Eyes Grow In
      ↓
Look Left
      ↓
Look Right
      ↓
Blink × 2
      ↓
Center
      ↓
"Hello !!!"
```

After the boot animation, the device enters the interactive greeting screen.

---

## 👋 Interactive Hello Screen

After booting, the Desktop Buddy displays an animated greeting screen.

The eyes remain active and continue blinking until the user interacts with the display.

```text
Boot Animation
      ↓
Hello Screen
      ↓
Animated / Blinking Eyes
      ↓
User Tap
      ↓
Main Menu
```

This provides an interactive transition between the boot sequence and the main application menu.

---

## 📋 Main Menu

The main menu provides access to the different applications available on the Desktop Buddy.

```text
┌───────────────────────┐
│       MAIN MENU       │
├───────────────────────┤
│  🕒 Clock             │
│  🍅 Pomodoro          │
│  😊 Moods             │
│  🐾 Pet               │
│  🦕 Dino Game         │
│  ⚙️ Settings          │
└───────────────────────┘
```

The menu is scrollable, allowing the user to navigate between different operating modes.

---

## 🧠 Firmware Architecture

The Desktop Buddy firmware is organized around multiple functional states or operating modes.

A simplified representation of the system is:

```text
                    ┌──────────────┐
                    │    ESP32     │
                    │   Firmware   │
                    └──────┬───────┘
                           │
                    ┌──────▼───────┐
                    │  Main Menu   │
                    └──────┬───────┘
                           │
       ┌──────────┬────────┼─────────┬──────────┬──────────┐
       ▼          ▼        ▼         ▼          ▼          ▼
     Clock    Pomodoro   Moods      Pet       Dino     Settings
       │          │        │         │          │          │
       └──────────┴────────┴─────────┴──────────┴──────────┘
                           │
                           ▼
                     User Interaction
```

Each mode contains its own interface and interaction logic while being managed by the central ESP32 firmware.

This state-based approach allows multiple applications to operate within a single embedded system.

---

## 👆 User Interaction

The system uses touch-based interaction for navigation and control.

Different touch interactions are assigned to different functions.

### Single Tap

Used for actions such as:

- Entering the main menu
- Selecting functions
- Returning to menus
- Controlling applications

### Double Tap

Used for specific actions such as:

- Saving a selected mood
- Returning to the menu from the Dino game after game-over/high-score states

The interaction system allows multiple applications to operate within the same device while maintaining simple and intuitive controls.

---

## 🔮 Future Improvements

Possible future enhancements include:

- Voice interaction
- Wi-Fi connectivity
- Weather and live information
- AI-based conversational interaction
- Additional games and applications
- Facial recognition
- More advanced emotional behaviors
- Smartphone synchronization
- Custom notification system
- IoT-based functionality

---

## 📌 Project Outcome

A multifunctional **ESP32-based interactive desktop companion** was developed with multiple applications integrated into a single firmware environment.

The completed system demonstrated:

- Interactive graphical interface
- Touch-based navigation
- Animated expressions
- Productivity timer
- Virtual pet interaction
- Embedded game functionality
- System settings
- Structured firmware architecture

The project demonstrated the practical integration of **embedded systems, interactive UI development, firmware programming, and user-oriented system design**.

---

## 📁 File Access Note

Some project files are large and may not be previewed directly on GitHub. For these files, use the **View raw** option on the GitHub file page to access the original file.

---

## 📂 Project Documentation

- [Code](Code/)
- [Model](Model-Images/)
- [Videos](Working-Videos/)
