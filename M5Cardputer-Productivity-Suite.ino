#include <M5Cardputer.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <WiFi.h>
#include <Preferences.h>
#include <time.h>

// Global objects
USBHIDKeyboard usbKeyboard;
M5Canvas canvas(&M5Cardputer.Display);
String lastSent = "Ready";

// Mode Management
enum Mode { MODE_MENU, MODE_MACRO_PAD, MODE_CLOCK, MODE_POMO, MODE_GAME, MODE_SETTINGS };
Mode currentMode = MODE_MENU;

// Forward Declarations
void drawMenu();
void drawMacroPad();
void handleMacroInput();
void drawSettingsMenu();
void handleSettings();
void autoConnectWifi();
void drawClock();
void handleClock();

#include "Menu.h"
#include "Macros.h"
#include "Settings.h"
#include "Clock.h"

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    canvas.createSprite(240, 135);
    usbKeyboard.begin();
    USB.begin();
    autoConnectWifi(); 
    drawMenu();
}

void loop() {
    M5Cardputer.update();
    
    // PRIORITY ESCAPE: Check more frequently
    if (M5Cardputer.Keyboard.isKeyPressed('`')) { 
        currentMode = MODE_MENU;
        extern enum SettingsState set_state;
        set_state = SET_MENU; 
        drawMenu();
        delay(200);
        return;
    }

    switch (currentMode) {
        case MODE_MENU:
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                if (M5Cardputer.Keyboard.isKeyPressed(';')) { 
                    menuScrollIdx = (menuScrollIdx > 0) ? menuScrollIdx - 1 : totalMenuItems - 1;
                    drawMenu();
                }
                else if (M5Cardputer.Keyboard.isKeyPressed('.')) { 
                    menuScrollIdx = (menuScrollIdx < totalMenuItems - 1) ? menuScrollIdx + 1 : 0;
                    drawMenu();
                }
                else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
                    if (menuScrollIdx == 0) { currentMode = MODE_MACRO_PAD; drawMacroPad(); }
                    else if (menuScrollIdx == 1) { currentMode = MODE_CLOCK; } 
                    else if (menuScrollIdx == 4) { currentMode = MODE_SETTINGS; drawSettingsMenu(); }
                }
            }
            break;
        case MODE_MACRO_PAD: handleMacroInput(); break;
        case MODE_CLOCK: handleClock(); break;
        case MODE_SETTINGS: handleSettings(); break;
    }
    yield(); // Allow background WiFi tasks to process
}