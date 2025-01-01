#include "read_json.h"
#include <ArduinoJson.h>
#include <LittleFS.h>
//  
//   ------------- read.json.cpp -------------------
//
// Funkcja do wczytywania ustawień z pliku JSON
void loadSettings(const char* filename, Settings* settings) {
    // Montujemy LittleFS
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // Otwieramy plik JSON
    File file = LittleFS.open(filename, "r");
    if (!file) {
        Serial.println("Failed to open file");
        return;
    }

    // Deserializacja JSON
    StaticJsonDocument<2048> doc; // Zwiększony rozmiar dokumentu
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println("Failed to parse JSON");
        file.close();
        return;
    }

    // Główne parametry
    settings->main_effect             = doc["main_effect"]             | 0;
    settings->effect_speed            = doc["effect_speed"]            | 0;
    settings->fade_delay              = doc["fade_delay"]              | 0;
    settings->light_threshold         = doc["light_threshold"]         | 0;

    // Kolory (grupa 1)
    settings->color_H_1               = doc["color_H_1"]               | 0;
    settings->color_S_1               = doc["color_S_1"]               | 100;
    settings->color_V_1               = doc["color_V_1"]               | 100;

    // Kolory (grupa 2)
    settings->color_H_2               = doc["color_H_2"]               | 0;
    settings->color_S_2               = doc["color_S_2"]               | 100;
    settings->color_V_2               = doc["color_V_2"]               | 100;

    // Kolory (grupa 3)
    settings->color_H_3               = doc["color_H_3"]               | 0;
    settings->color_S_3               = doc["color_S_3"]               | 100;
    settings->color_V_3               = doc["color_V_3"]               | 100;

    // Inne parametry
    settings->platform_led_1          = doc["platform_led_1"]          | 0;
    settings->platform_led_2          = doc["platform_led_2"]          | 0;
    settings->platform_led_3          = doc["platform_led_3"]          | 0;
    settings->steps_num               = doc["steps_num"]               | 0;
    settings->led_per_step            = doc["led_per_step"]            | 0;
    settings->bottom_sensor_type      = doc["bottom_sensor_type"]      | 0;
    settings->bottom_sensor_distance  = doc["bottom_sensor_distance"]  | 0;
    settings->top_sensor_type         = doc["top_sensor_type"]         | 0;
    settings->top_sensor_distance     = doc["top_sensor_distance"]     | 0;
    settings->no_stair                = doc["no_stair"]                | 0;
    settings->direction               = doc["direction"]               | 0;

    // Hasła (napisy)
    const char* btPass   = doc["bt_password"]   | "your_bluetooth_password";
    const char* wifiPass = doc["wifi_password"] | "your_wifi_password";

    // Kopiujemy je do tablic w strukturze
    strncpy(settings->bt_password,   btPass,   sizeof(settings->bt_password)   - 1);
    settings->bt_password[sizeof(settings->bt_password) - 1] = '\0'; // Null-terminate

    strncpy(settings->wifi_password, wifiPass, sizeof(settings->wifi_password) - 1);
    settings->wifi_password[sizeof(settings->wifi_password) - 1] = '\0'; // Null-terminate

    // Zamykamy plik
    file.close();

    Serial.println("Settings loaded from JSON.");
}

// Funkcja do tworzenia domyślnych ustawień
void createDefaultSettings(const char* filename) {
    // Montujemy LittleFS
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // Sprawdzamy, czy plik już istnieje
    if (LittleFS.exists(filename)) {
        Serial.println("Plik już istnieje. Nie tworzę nowego.");
        return;
    }

    // Tworzymy nowy plik
    File file = LittleFS.open(filename, "w");
    if (!file) {
        Serial.println("Nie mogę utworzyć pliku JSON");
        return;
    }

    // Tworzymy dokument JSON
    StaticJsonDocument<2048> doc;

    // Ustawienia główne
    doc["main_effect"]             = 3;
    doc["effect_speed"]            = 50;
    doc["fade_delay"]              = 500;
    doc["light_threshold"]         = 200;

    // Kolory (grupa 1)
    doc["color_H_1"]               = 160;
    doc["color_S_1"]               = 100;
    doc["color_V_1"]               = 70;

    // Kolory (grupa 2)
    doc["color_H_2"]               = 40;
    doc["color_S_2"]               = 100;
    doc["color_V_2"]               = 60;

    // Kolory (grupa 3)
    doc["color_H_3"]               = 220;
    doc["color_S_3"]               = 100;
    doc["color_V_3"]               = 60;

    // Inne parametry
    doc["platform_led_1"]          = 12;
    doc["platform_led_2"]          = 22;
    doc["platform_led_3"]          = 33;
    doc["steps_num"]               = 10;
    doc["led_per_step"]            = 10;
    doc["bottom_sensor_type"]      = 0;
    doc["bottom_sensor_distance"]  = 75;
    doc["top_sensor_type"]         = 0;
    doc["top_sensor_distance"]     = 75;
    doc["no_stair"]                = 1;
    doc["direction"]               = 0;

    // Hasła
    doc["bt_password"]             = "your_bluetooth_password";
    doc["wifi_password"]           = "your_wifi_password";

    // Serializacja do pliku
    if (serializeJsonPretty(doc, file) == 0) {
        Serial.println("Nie mogę zapisać do pliku JSON");
    } else {
        Serial.println("Domyślne ustawienia zapisane do menu1.json");
    }

    // Zamykamy plik
    file.close();
}


// Funkcja do zapisywania ustawień do pliku JSON
void saveSettings(const char* filename, const Settings* settings) {
    // Montujemy LittleFS
    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    // Otwieramy plik JSON do zapisu
    File file = LittleFS.open(filename, "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    // Tworzymy dokument JSON
    StaticJsonDocument<2048> doc;

    // Ustawienia główne
    doc["main_effect"]             = settings->main_effect;
    doc["effect_speed"]            = settings->effect_speed;
    doc["fade_delay"]              = settings->fade_delay;
    doc["light_threshold"]         = settings->light_threshold;

    // Kolory (grupa 1)
    doc["color_H_1"]               = settings->color_H_1;
    doc["color_S_1"]               = settings->color_S_1;
    doc["color_V_1"]               = settings->color_V_1;

    // Kolory (grupa 2)
    doc["color_H_2"]               = settings->color_H_2;
    doc["color_S_2"]               = settings->color_S_2;
    doc["color_V_2"]               = settings->color_V_2;

    // Kolory (grupa 3)
    doc["color_H_3"]               = settings->color_H_3;
    doc["color_S_3"]               = settings->color_S_3;
    doc["color_V_3"]               = settings->color_V_3;

    // Inne parametry
    doc["platform_led_1"]          = settings->platform_led_1;
    doc["platform_led_2"]          = settings->platform_led_2;
    doc["platform_led_3"]          = settings->platform_led_3;
    doc["steps_num"]               = settings->steps_num;
    doc["led_per_step"]            = settings->led_per_step;
    doc["bottom_sensor_type"]      = settings->bottom_sensor_type;
    doc["bottom_sensor_distance"]  = settings->bottom_sensor_distance;
    doc["top_sensor_type"]         = settings->top_sensor_type;
    doc["top_sensor_distance"]     = settings->top_sensor_distance;
    doc["no_stair"]                = settings->no_stair;
    doc["direction"]               = settings->direction;

    // Hasła
    doc["bt_password"]             = settings->bt_password;
    doc["wifi_password"]           = settings->wifi_password;

    // Serializacja JSON do pliku
    if (serializeJsonPretty(doc, file) == 0) {
        Serial.println("Failed to write to file");
    } else {
        Serial.println("Settings saved to JSON.");
    }

    // Zamykamy plik
    file.close();
}
