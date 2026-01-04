#ifndef MACROS_H
#define MACROS_H

extern USBHIDKeyboard usbKeyboard;
extern M5Canvas canvas;
extern String lastSent;

const char* macros[] = {
  "a:Hello World!", "b:Git commit -m ", "c:TODO: ", "d:Date: ",
  "e:COPY (Ctrl+C)", "f:NEW TAB (Google)", "g:Good morning team",
  "1:Priority HIGH", "2:Priority LOW", "3:Review Needed",
  "4:Internal Only", "5:Draft Version", "h:Help Docs",
  "i:Issue Tracker", "j:Jenkins Build", "k:Kubernetes Log"
};
const int numMacros = 16;
int macroScrollIdx = 0;
const int macroVisibleRows = 5;

void drawMacroPad() {
    canvas.fillSprite(BLACK);
    canvas.setTextColor(CYAN);
    canvas.setTextSize(1.5);
    canvas.drawString("MODE: MACRO PAD", 10, 5);
    
    if (WiFi.status() == WL_CONNECTED) {
        canvas.setTextColor(GREEN); canvas.setTextSize(1);
        canvas.drawString("WiFi", 200, 8);
    }

    canvas.drawLine(0, 22, 240, 22, WHITE);

    for (int i = 0; i < macroVisibleRows; i++) {
        int idx = macroScrollIdx + i;
        if (idx < numMacros) {
            int yPos = 30 + (i * 15);
            canvas.setTextColor(macros[idx][0] == 'e' || macros[idx][0] == 'f' ? YELLOW : WHITE);
            canvas.setCursor(10, yPos);
            canvas.printf("[%c] %s", macros[idx][0], String(macros[idx]).substring(2).c_str());
        }
    }

    if (numMacros > macroVisibleRows) {
        int barHeight = (macroVisibleRows * 75) / numMacros;
        int barPos = (macroScrollIdx * 75) / numMacros;
        canvas.drawRect(230, 30, 4, 75, DARKGREY);
        canvas.fillRect(230, 30 + barPos, 4, barHeight, WHITE);
    }

    canvas.fillRect(0, 110, 240, 25, DARKGREEN);
    canvas.setTextColor(WHITE);
    canvas.setCursor(10, 118);
    canvas.print("Sent: " + lastSent);
    canvas.pushSprite(0, 0);
}

void handleMacroInput() {
    if (M5Cardputer.Keyboard.isKeyPressed(';')) { 
        if (macroScrollIdx > 0) macroScrollIdx--;
        drawMacroPad();
    }
    else if (M5Cardputer.Keyboard.isKeyPressed('.')) { 
        if (macroScrollIdx < numMacros - macroVisibleRows) macroScrollIdx++;
        drawMacroPad();
    }
    else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
        usbKeyboard.write(KEY_RETURN);
        lastSent = "[ENTER]";
        drawMacroPad();
    }
    else if (M5Cardputer.Keyboard.isKeyPressed(8)) {
        usbKeyboard.write(KEY_BACKSPACE);
        lastSent = "[BACKSPACE]";
        drawMacroPad();
    }

    for (int i = 0; i < numMacros; i++) {
        if (M5Cardputer.Keyboard.isKeyPressed(macros[i][0])) {
            if (macros[i][0] == 'e') {
                usbKeyboard.press(KEY_LEFT_CTRL); usbKeyboard.press('c');
                delay(50); usbKeyboard.releaseAll();
                lastSent = "Ctrl+C";
            } else if (macros[i][0] == 'f') {
                usbKeyboard.press(KEY_LEFT_CTRL); usbKeyboard.press('t');
                delay(150); usbKeyboard.releaseAll();
                delay(250);
                usbKeyboard.print("https://www.google.com");
                usbKeyboard.write(KEY_RETURN);
                lastSent = "New Tab";
            } else {
                String text = String(macros[i]).substring(2);
                usbKeyboard.print(text);
                lastSent = text;
            }
            drawMacroPad();
            delay(100);
        }
    }
}
#endif