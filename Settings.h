#ifndef SETTINGS_H
#define SETTINGS_H

extern M5Canvas canvas;
enum SettingsState { SET_MENU, SCANNING, WIFI_LIST, ENTER_PASS, VALIDATING };
SettingsState set_state = SET_MENU;

String setOptions[] = {"Connect Wifi", "Screen Brightness", "System Info"};
int setScrollIdx = 0;
int networksFound = 0;
int wifiScrollIdx = 0;
String selectedSSID = "";
String wifiPassword = "";
unsigned long lastCharTime = 0;

void autoConnectWifi() {
    Preferences prefs;
    prefs.begin("wifi_store", true);
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    prefs.end();
    if (ssid != "" && ssid != "NULL") { WiFi.mode(WIFI_STA); WiFi.begin(ssid.c_str(), pass.c_str()); }
}

void drawSettingsMenu() {
    canvas.fillSprite(BLACK);
    canvas.setTextColor(YELLOW);
    canvas.setTextSize(1.5);
    canvas.drawString("SETTINGS", 10, 5);
    canvas.drawLine(0, 22, 240, 22, WHITE);
    for (int i = 0; i < 3; i++) {
        int yPos = 40 + (i * 20);
        if (i == setScrollIdx) { canvas.fillRect(5, yPos-2, 230, 18, WHITE); canvas.setTextColor(BLACK); }
        else { canvas.setTextColor(WHITE); }
        canvas.setCursor(15, yPos); canvas.print(setOptions[i]);
    }
    canvas.pushSprite(0, 0);
}

void drawWifiList() {
    canvas.fillSprite(BLACK);
    canvas.setTextColor(GREEN);
    canvas.setTextSize(1.2);
    canvas.drawString("SELECT NETWORK", 10, 5);
    canvas.drawLine(0, 22, 240, 22, WHITE);
    if (networksFound <= 0) {
        canvas.setTextColor(RED);
        canvas.drawString("No Networks Found!", 10, 60);
    } else {
        for (int i = 0; i < 5; i++) {
            int idx = wifiScrollIdx + i;
            if (idx < networksFound) {
                int yPos = 30 + (i * 18);
                if (idx == wifiScrollIdx) { canvas.fillRect(5, yPos-2, 230, 16, GREEN); canvas.setTextColor(BLACK); }
                else { canvas.setTextColor(WHITE); }
                canvas.setCursor(10, yPos); canvas.print(WiFi.SSID(idx));
            }
        }
    }
    canvas.pushSprite(0, 0);
}

void drawPassInput() {
    canvas.fillSprite(BLACK);
    canvas.setTextColor(WHITE);
    canvas.setCursor(10, 10); canvas.print("SSID: " + selectedSSID);
    canvas.setCursor(10, 40); canvas.print("Password: ");
    String masked = "";
    for(int i=0; i<wifiPassword.length(); i++) {
        if (i == wifiPassword.length()-1 && millis() - lastCharTime < 1000) masked += wifiPassword[i];
        else masked += "*";
    }
    canvas.print(masked + "_");
    canvas.setCursor(10, 100); canvas.print("ENTER to Connect");
    canvas.pushSprite(0, 0);
}

void handleSettings() {
    if (set_state == ENTER_PASS && millis() - lastCharTime > 1000) drawPassInput();
    if (!M5Cardputer.Keyboard.isChange() || !M5Cardputer.Keyboard.isPressed()) return;

    if (set_state == SET_MENU) {
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { setScrollIdx = (setScrollIdx > 0) ? setScrollIdx - 1 : 2; drawSettingsMenu(); }
        else if (M5Cardputer.Keyboard.isKeyPressed('.')) { setScrollIdx = (setScrollIdx < 2) ? setScrollIdx + 1 : 0; drawSettingsMenu(); }
        else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            if (setScrollIdx == 0) {
                set_state = SCANNING;
                canvas.fillSprite(BLACK); canvas.drawString("Deep Scanning...", 60, 60); canvas.pushSprite(0, 0);
                WiFi.disconnect(true); WiFi.mode(WIFI_OFF); delay(200); WiFi.mode(WIFI_STA); delay(500);
                networksFound = WiFi.scanNetworks(false, false, false, 300);
                wifiScrollIdx = 0;
                set_state = (networksFound > 0) ? WIFI_LIST : SET_MENU;
                if(networksFound > 0) drawWifiList(); else drawSettingsMenu();
            } else if (setScrollIdx == 2) {
                Preferences p; p.begin("sys_info", true);
                String c = p.getString("city", "Unknown"); String co = p.getString("country", ""); p.end();
                canvas.fillSprite(BLACK); canvas.setTextColor(CYAN); canvas.drawString("SYSTEM INFO", 10, 10);
                canvas.setTextColor(WHITE); canvas.setCursor(10, 40); canvas.print("Loc: " + c + ", " + co);
                canvas.setCursor(10, 65); canvas.print("IP: " + WiFi.localIP().toString());
                canvas.pushSprite(0, 0); delay(3000); drawSettingsMenu();
            }
        }
    } else if (set_state == WIFI_LIST) {
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { if (wifiScrollIdx > 0) wifiScrollIdx--; drawWifiList(); }
        else if (M5Cardputer.Keyboard.isKeyPressed('.')) { if (wifiScrollIdx < networksFound - 1) wifiScrollIdx++; drawWifiList(); }
        else if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            selectedSSID = WiFi.SSID(wifiScrollIdx); wifiPassword = ""; set_state = ENTER_PASS; drawPassInput();
        }
    } else if (set_state == ENTER_PASS) {
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (status.del && wifiPassword.length() > 0) { wifiPassword.remove(wifiPassword.length()-1); drawPassInput(); }
        else if (status.enter) {
            set_state = VALIDATING;
            canvas.fillSprite(BLACK); canvas.drawString("Connecting...", 80, 60); canvas.pushSprite(0, 0);
            WiFi.begin(selectedSSID.c_str(), wifiPassword.c_str());
            int retry = 0; while (WiFi.status() != WL_CONNECTED && retry < 30) { delay(500); retry++; }
            if (WiFi.status() == WL_CONNECTED) {
                Preferences p; p.begin("wifi_store", false); p.putString("ssid", selectedSSID); p.putString("pass", wifiPassword); p.end();
                set_state = SET_MENU; drawSettingsMenu();
            } else {
                canvas.fillSprite(BLACK); canvas.setTextColor(RED); canvas.drawString("Failed!", 100, 60); canvas.pushSprite(0, 0);
                delay(2000); set_state = ENTER_PASS; drawPassInput();
            }
        } else { for (auto i : status.word) { wifiPassword += i; lastCharTime = millis(); drawPassInput(); } }
    }
}
#endif