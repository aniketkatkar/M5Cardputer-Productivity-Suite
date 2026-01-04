#ifndef CLOCK_H
#define CLOCK_H

#include <HTTPClient.h>
#include <ArduinoJson.h>

extern M5Canvas canvas;
String city = "Loading Loc...";
String country = "";
bool timeSynced = false;
unsigned long lastGeoCheck = 0;

void syncTimeAndLocation() {
    if (WiFi.status() != WL_CONNECTED) return;
    
    // Sync Time (non-blocking call)
    configTime(19800, 0, "0.in.pool.ntp.org", "pool.ntp.org");

    // Geolocation with Timeout
    HTTPClient http;
    http.setTimeout(2000); // 2 second timeout - prevents hanging
    http.begin("http://ip-api.com/json/");
    int httpCode = http.GET();
    if (httpCode == 200) {
        String payload = http.getString();
        JsonDocument doc;
        if (!deserializeJson(doc, payload)) {
            city = doc["city"].as<String>();
            country = doc["country"].as<String>();
            Preferences prefs; prefs.begin("sys_info", false);
            prefs.putString("city", city); prefs.putString("country", country); prefs.end();
        }
    }
    http.end();
    timeSynced = true;
}

void drawClock() {
    canvas.fillSprite(BLACK);
    canvas.setTextColor(YELLOW);
    canvas.setTextSize(1.5);
    canvas.drawString("INTERNET CLOCK", 10, 5);
    
    if (WiFi.status() == WL_CONNECTED) {
        canvas.setTextColor(GREEN); canvas.setTextSize(1);
        canvas.drawString("WiFi: ON", 185, 5);
    } else {
        canvas.setTextColor(RED); canvas.setTextSize(1);
        canvas.drawString("WiFi: OFF", 185, 5);
    }
    
    canvas.drawLine(0, 22, 240, 22, WHITE);
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        canvas.setTextColor(RED);
        canvas.setCursor(60, 65);
        canvas.print("Syncing Time...");
    } else {
        canvas.setTextColor(WHITE);
        canvas.setTextSize(4);
        canvas.setCursor(25, 45);
        canvas.printf("%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        
        canvas.setTextSize(1.5);
        canvas.setTextColor(CYAN);
        canvas.setCursor(10, 105);
        canvas.print(city + ", " + country);
    }
    canvas.pushSprite(0, 0);
}

void handleClock() {
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 1000) {
        // Only try to sync if connected and not yet synced
        if (!timeSynced && WiFi.status() == WL_CONNECTED) {
            syncTimeAndLocation();
        }
        drawClock();
        lastUpdate = millis();
    }
}
#endif