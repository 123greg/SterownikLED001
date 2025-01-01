#include <Arduino.h>
#include <lvgl.h>
#include <LittleFS.h>
#include "read_json.h"   // Deklaracja struktury Settings i loadSettings(...)
#include "ui.h"          // Deklaracje obiektów (Colorwheel, Slider, Panele, Labele, Przyciski)

// Deklaracja zmiennej globalnej (extern) z read_json.h
// Nie definiujemy ponownie 'Settings settings;'

// Zmienna sterująca tym, która grupa kolorów jest aktualnie w użyciu
// 1 = "Main Color", 2 = "Sec Color", 3 = "STB Color"
static int currentGroup = 1;

// -------------------------------------------------------------
// Funkcja: setPanelColor
//   - Ustawia kolor danego panelu na podstawie H, S, V
// -------------------------------------------------------------
void setPanelColor(lv_obj_t* panel, int hue, int sat, int val) {
    lv_color_t color = lv_color_hsv_to_rgb(hue, sat, val);
    lv_obj_set_style_bg_color(panel, color, LV_PART_MAIN | LV_STATE_DEFAULT);
}


// -------------------------------------------------------------
// Funkcje obsługi przycisków (zmieniają currentGroup)
// -------------------------------------------------------------
void BMainColor(lv_event_t * e)
{
    currentGroup = 1;
    Serial.println(">> Wybrano grupę kolorów 1 (MainColor)");
    // Odświeżenie UI po zmianie grupy
    UstawToo(e);
}

void BSecColor(lv_event_t * e)
{
    currentGroup = 2;
    Serial.println(">> Wybrano grupę kolorów 2 (SecColor)");
    UstawToo(e);
}

void BSTBColor(lv_event_t * e)
{
    currentGroup = 3;
    Serial.println(">> Wybrano grupę kolorów 3 (STBColor)");
    UstawToo(e);
}



// -------------------------------------------------------------
// Funkcja: Save_and_test(lv_event_t * e)
//   - nagrywa dane do pliku - menu1.json 
//   - 
//   - tu powinno byc dodanie czynnosci uruchamiajacych swiecenie LED zgodnie z nowymi parametrami
// -------------------------------------------------------------
//
void Save_and_test(lv_event_t * e)
{
    saveSettings("/menu1.json", &settings);
}


// -------------------------------------------------------------
// Funkcja: Ustaw(lv_event_t * e)
//   - Ustawia labele (H i V) oraz kolory paneli
//   - Ustawia koło kolorów (wyłącznie barwa H, jasność = 100 na kole)
//   - Ustawia suwak jasności (V)
// -------------------------------------------------------------
void UstawToo(lv_event_t * e)
{    
    // 1. W zależności od currentGroup, pobierz H, S, V
    int hue = 0, sat = 100, val = 100; // Domyślne wartości
    switch(currentGroup) {
        case 1:
            hue = settings.color_H_1;
            sat = settings.color_S_1;
            val = settings.color_V_1;
            break;
        case 2:
            hue = settings.color_H_2;
            sat = settings.color_S_2;
            val = settings.color_V_2;
            break;
        case 3:
            hue = settings.color_H_3;
            sat = settings.color_S_3;
            val = settings.color_V_3;
            break;
    }

    // 2. Ustaw label z Hue
    char text_buffer_h[16];
    snprintf(text_buffer_h, sizeof(text_buffer_h), "H: %d", hue);
    lv_label_set_text(ui_TxtColorOpis1, text_buffer_h);

    // 3. Ustaw label z Value
    char text_buffer_v[16];
    snprintf(text_buffer_v, sizeof(text_buffer_v), "V: %d", val);
    lv_label_set_text(ui_TxtBrigh1, text_buffer_v);

    // 4. Ustaw wszystkie trzy panele na podstawie danych z JSON
    lv_obj_t* panels[3] = { ui_PanelColor11, ui_PanelColor22, ui_PanelColor33 };
    int hues[3] = { settings.color_H_1, settings.color_H_2, settings.color_H_3 };
    int sats[3] = { settings.color_S_1, settings.color_S_2, settings.color_S_3 };
    int vals[3] = { settings.color_V_1, settings.color_V_2, settings.color_V_3 };

    for (int i = 0; i < 3; i++) {
        setPanelColor(panels[i], hues[i], sats[i], vals[i]);
    }

    // 5. Wylicz kolor dla ui_PanelColor1 i ustaw go
    lv_color_t color = lv_color_hsv_to_rgb(
        hue,   // 0..359
        sat,   // 0..100
        val    // 0..100
    );
    lv_obj_set_style_bg_color(ui_PanelColor1, color, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 6. Warunkowo aktualizuj odpowiedni panel (opcjonalne)
    switch(currentGroup) {
        case 1:
            setPanelColor(ui_PanelColor11, hue, sat, val);
            break;
        case 2:
            setPanelColor(ui_PanelColor22, hue, sat, val);
            break;
        case 3:
            setPanelColor(ui_PanelColor33, hue, sat, val);
            break;
    }

    // 7. Ustaw koło kolorów (HSV)
    lv_color_hsv_t hsv;
    hsv.h = hue;   // 0..359
    hsv.s = sat;   // 0..100
    hsv.v = 100;   // zawsze pełna jasność na kółku
    lv_colorwheel_set_hsv(ui_Colorwheel1, hsv);

    // 8. Ustaw slider (dla jasności)
    lv_slider_set_value(ui_SliderV1, val, LV_ANIM_OFF);

}

// -------------------------------------------------------------
// Funkcja: color_wheel_event1(lv_event_t * e)
//   - Zmienia barwę (H) w aktualnie wybranej grupie
//   - Nie zmienia V, bo suwak steruje jasnością
// -------------------------------------------------------------
void color_wheel_event1(lv_event_t * e)
{
    lv_obj_t * colorwheel = lv_event_get_target(e);
    lv_color_hsv_t hsv_now = lv_colorwheel_get_hsv(colorwheel);

    // Odczytane hue (0..359), sat (0..100), v (0..100)
    int newHue = hsv_now.h;
    int newSat = hsv_now.s; 

    // Zapisujemy TYLKO barwę i nasycenie do settings (V pozostaje kontrolowane przez suwak)
    switch(currentGroup) {
        case 1:
            settings.color_H_1 = newHue;
            settings.color_S_1 = newSat;
            break;
        case 2:
            settings.color_H_2 = newHue;
            settings.color_S_2 = newSat;
            break;
        case 3:
            settings.color_H_3 = newHue;
            settings.color_S_3 = newSat;
            break;
    }

    // Aktualizujemy label z wartością H
    char text_buffer_h[16];
    snprintf(text_buffer_h, sizeof(text_buffer_h), "H: %d", newHue);
    lv_label_set_text(ui_TxtColorOpis1, text_buffer_h);

    // Przeliczamy nowy kolor (S z kółka, V z settings)
    int currentVal = 0;
    switch(currentGroup) {
        case 1:
            currentVal = settings.color_V_1;
            break;
        case 2:
            currentVal = settings.color_V_2;
            break;
        case 3:
            currentVal = settings.color_V_3;
            break;
    }

    lv_color_t color = lv_color_hsv_to_rgb(
        newHue,
        newSat,
        currentVal
    );

    // Ustaw kolor dla ui_PanelColor1
    lv_obj_set_style_bg_color(ui_PanelColor1, color, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Warunkowo aktualizuj odpowiedni panel
    switch(currentGroup) {
        case 1:
            setPanelColor(ui_PanelColor11, newHue, newSat, currentVal);
            break;
        case 2:
            setPanelColor(ui_PanelColor22, newHue, newSat, currentVal);
            break;
        case 3:
            setPanelColor(ui_PanelColor33, newHue, newSat, currentVal);
            break;
    }
}

// -------------------------------------------------------------
// Funkcja: sliderV1_event(lv_event_t * e)
//   - Zmienia jasność (V) w aktualnie wybranej grupie
//   - Nie zmienia H ani S, bo te zmienia kółko
// -------------------------------------------------------------
void sliderV1_event(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int newVal = lv_slider_get_value(slider);

    // Zapisujemy nową wartość jasności w odpowiedniej grupie
    switch(currentGroup) {
        case 1:
            settings.color_V_1 = newVal;
            break;
        case 2:
            settings.color_V_2 = newVal;
            break;
        case 3:
            settings.color_V_3 = newVal;
            break;
    }

    // Wyświetlamy w labelu aktualną wartość V
    char text_buffer_v[16];
    snprintf(text_buffer_v, sizeof(text_buffer_v), "V: %d", newVal);
    lv_label_set_text(ui_TxtBrigh1, text_buffer_v);

    // Pobieramy H i S z odpowiedniej grupy
    int currentHue = 0, currentSat = 100;
    switch(currentGroup) {
        case 1:
            currentHue = settings.color_H_1;
            currentSat = settings.color_S_1;
            break;
        case 2:
            currentHue = settings.color_H_2;
            currentSat = settings.color_S_2;
            break;
        case 3:
            currentHue = settings.color_H_3;
            currentSat = settings.color_S_3;
            break;
    }

    // Przeliczamy nowy kolor
    lv_color_t color = lv_color_hsv_to_rgb(
        currentHue,
        currentSat,
        newVal
    );

    // Ustaw kolor dla ui_PanelColor1
    lv_obj_set_style_bg_color(ui_PanelColor1, color, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Warunkowo aktualizuj odpowiedni panel
    switch(currentGroup) {
        case 1:
            setPanelColor(ui_PanelColor11, currentHue, currentSat, newVal);
            break;
        case 2:
            setPanelColor(ui_PanelColor22, currentHue, currentSat, newVal);
            break;
        case 3:
            setPanelColor(ui_PanelColor33, currentHue, currentSat, newVal);
            break;
    }
}

// -------------------------------------------------------------
// Puste placeholdery dla ewentualnych innych zdarzeń
// -------------------------------------------------------------
void slider_event(lv_event_t * e)
{
    // ...
}