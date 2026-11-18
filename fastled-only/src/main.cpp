#include <Arduino.h>
#include <FastLED.h>

// XIAO ESP32-C5: D1 = GPIO0. Connect D1 to the first WS2812B's DIN.
constexpr uint8_t LED_COUNT = 1;
CRGB leds[LED_COUNT];

void showColor(const char* name, const CRGB& color) {
    leds[0] = color;
    Serial.print("before show: ");
    Serial.println(name);
    FastLED.show();
    Serial.print("after show: ");
    Serial.println(name);
    delay(1000);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("FastLED-only test starting");

    FastLED.addLeds<WS2812B, 9 , GRB>(leds, LED_COUNT);
    FastLED.addLeds<WS2812B, 10, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(80);
}

void loop() {
    showColor("RED", CRGB::Red);
    showColor("GREEN", CRGB::Green);
    showColor("BLUE", CRGB::Blue);
    showColor("WHITE", CRGB::White);
    showColor("OFF", CRGB::Black);
}
