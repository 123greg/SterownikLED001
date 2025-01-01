#include <Arduino.h>
#include <LittleFS.h>
#include <esp32_smartdisplay.h>
#include <ui.h>
#include "read_json.h"

// Definicja zmiennej globalnej
Settings settings;

void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(5000);
#endif

    Serial.begin(115200);
    Serial.setDebugOutput(true);

    // Inicjalizacja systemu plików
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // Sprawdzenie istnienia pliku menu1.json i ewentualne jego utworzenie
    if (!LittleFS.exists("/menu1.json")) {
        Serial.println("menu1.json not found. Creating default settings.");
        createDefaultSettings("/menu1.json");
    } else {
        Serial.println("menu1.json found. Loading settings.");
    }

    // Wczytanie ustawień z pliku JSON
    loadSettings("/menu1.json", &settings);

    smartdisplay_init();
    smartdisplay_lcd_set_backlight(1); // 0=off, 0.5=half, 1=full

    auto disp = lv_disp_get_default();
    // lv_disp_set_rotation(disp, LV_DISP_ROT_90);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    ui_init();
    
}


void loop()
{
    lv_timer_handler();
    delay(5);
}
