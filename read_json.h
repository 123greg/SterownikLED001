#ifndef READ_JSON_H
#define READ_JSON_H

#include <Arduino.h>
// 
// 
//   ------------- read.json.h -------------------
// 
// Struktura do przechowywania zmiennych
typedef struct {
    int main_effect; // Wybrany efekt LED
    int effect_speed; // Szybkość efektu
    int fade_delay; // Czas świecenia LED
    int light_threshold; // Jasność, przy której mają zadziałać sensory
    int floor_set; // pietro obecnie przetwarzane 0=G-F, max 3
    int color_H_1; 
    int color_S_1; 
    int color_V_1; 
    int color_H_2; 
    int color_S_2; 
    int color_V_2; 
    int color_H_3; 
    int color_S_3; 
    int color_V_3; 
    int platform_led_1; // Ilość LED w pierwszej platformie
    int platform_led_2; // Ilość LED w drugiej platformie
    int platform_led_3; // Ilość LED w trzeciej platformie
    int steps_num; // Ilość zwykłych stopni poza platformami
    int led_per_step; // Ilość LED na zwykły stopień
    int bottom_sensor_type; // Typ sensora na dole (0=PIR/1=TOF)
    int bottom_sensor_distance; // Odległość działania dolnego sensora TOF
    int top_sensor_type; // Typ sensora na górze (0=PIR/1=TOF)
    int top_sensor_distance; // Odległość działania górnego sensora TOF
    int no_stair; // Ilość par schodów w domu
    int v_direction;// Kierunek efektów (0=dół->góra, 1=góra->dół)
    int h_direction;// kierunek poziomy dzialania wybranego efektu LED: (1)Midle-Site, (2)Site-Midle, (3)L-R, (4)R-L

    // Zmienne komunikacyjne
    char bt_password[32]; // Hasło Bluetooth
    char wifi_password[64]; // Hasło WiFi
} Settings;

extern Settings settings;   // Deklaracja zmiennej globalnej

// Deklaracja funkcji do wczytania zmiennych
void loadSettings(const char* filename, Settings* settings);

// Deklaracja funkcji do zapisu domyślnych ustawień
void createDefaultSettings(const char* filename);

// Deklaracja funkcji do zapisu ustawień do pliku JSON
void saveSettings(const char* filename, const Settings* settings);

#endif // READ_JSON_H
