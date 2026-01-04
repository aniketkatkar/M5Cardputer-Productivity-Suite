#ifndef MENU_H
#define MENU_H

extern M5Canvas canvas;
const char* menuItems[] = {"Macro keypad", "Clock", "Pomodoro", "Game", "Settings"};
const int totalMenuItems = 5;
int menuScrollIdx = 0;

void drawMenu() {
    canvas.fillSprite(BLACK);
    canvas.setTextColor(MAGENTA);
    canvas.setTextSize(1.5);
    canvas.drawString("Macro Pod v1.0", 10, 5);
    canvas.setTextSize(1);
    if (WiFi.status() == WL_CONNECTED) {
        canvas.setTextColor(GREEN); canvas.drawString("WiFi: ON", 185, 5);
    } else {
        canvas.setTextColor(RED); canvas.drawString("WiFi: OFF", 185, 5);
    }
    canvas.drawLine(0, 22, 240, 22, WHITE);
    canvas.setTextSize(1.2);
    for (int i = 0; i < totalMenuItems; i++) {
        int yPos = 35 + (i * 18);
        if (i == menuScrollIdx) { canvas.fillRect(5, yPos-2, 230, 16, WHITE); canvas.setTextColor(BLACK); }
        else { canvas.setTextColor(WHITE); }
        canvas.setCursor(15, yPos); canvas.print(menuItems[i]);
    }
    canvas.pushSprite(0, 0);
}
#endif