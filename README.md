# M5Cardputer Productivity Suite

Transform your M5Cardputer into a plug-and-play Productivity Suite with Macro Keypad and other fun stuff!

## Screenshots

![Main screen](https://github.com/aniketkatkar/M5Cardputer-Productivity-Suite/blob/main/M5Cardputer_Productivity_Suite.jpg)


## Features

- Scrollable Menu
- Wifi Clock:
-- Synced with India — in.pool.ntp.org
- Marco Keypad:
-- 16 Custom Macros (a-h, 1-5) with rich text display
-- Scrollable List (; = Up, . = Down)
-- Live Feedback shows last sent macro
-- Special Actions (Ctrl+C, New Google Tab)
-- Backspace/Enter support
-- Full Canvas UI with scroll bar
- Pomodoro Timer:
-- 1 minute
-- 5 minute
-- 10 minute
-- 15 minute
-- 30 minute
-- 1 hour
-- Custom minutes
-- Time up buzzer [WIP]
- Mini Game: [WIP]
- Settings:
-- Connect to Wifi Network
-- Brightness [WIP]
-- System Info [WIP]
-- Battery [WIP]

## Hardware Setup

- Standalone M5Cardputer

## Software Requirements

Arduino IDE 2.x
ESP32 Board Package: 3.x.x
Libraries (Library Manager):
- M5Cardputer (latest)
- No BLE libraries needed
- Board: M5Stack → M5Cardputer

## Key Bindings

- Scroll: 
-- <;> or ▲ →  UP
-- <.> or ▼ →  DOWN
- Macros: a b c d e f g h 1 2 3 4 5 [Can be customized]
- Special Keys:
-- <Enter> : to select a menu item
-- <Esc> : to go back to main menu

## Macro List
| Key | Action                  |
|-----|-------------------------|
| a   | Hello World!            |
| b   | Git commit -m           |
| c   | TODO:                   |
| d   | Date:                   |
| e   | COPY (Ctrl+C)           |
| f   | NEW TAB (Google)        |
| g   | Good morning team       |
| h   | Help Docs               |
| 1   | Priority HIGH           |
| 2   | Priority LOW            |
| 3   | Review Needed           |
| 4   | Internal Only           |
| 5   | Draft Version           |

## Installation

1. Open code in Arduino IDE
2. Select M5Cardputer board
3. Upload (device in download mode: Power OFF + hold G0 + USB)
4. Plug into laptop - ready!

## Customization

cpp for Macro keypad
```
// Add your macros
const char* macros[] = {
  "a:Hello World!", 
  "b:Git commit -m ", 
  "c:TODO: ", 
  "d:Date: "
};
```

## Troubleshooting


❌ No response: Check USB cable (data, not charge-only)

❌ Won't compile: Downgrade to ESP32 core 2.0.11 + restart IDE

❌ Scroll not working: Try `'` or `/` keys instead of `;/.`

❓ Customize: Edit `macros[]` array + reflash


Ready in 30 seconds - perfect for developers, support teams, and power users!