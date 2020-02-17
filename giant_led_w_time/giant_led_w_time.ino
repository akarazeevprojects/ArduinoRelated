#include "LEDTime.h"

#include <FastLED.h>
#define LED_PIN     D3
#define NUM_LEDS    4
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 20

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

#include <Button.h>
#include <PushButton.h>

PushButton button = PushButton(D5);

int currentMode = 0;
int totalModes = 2;
bool wasHeld = false;

void setup() {
  delay(1000);
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  connectWiFi();
  timeClient.begin();

  button.onRelease(onButtonReleased);
  button.onHoldRepeat(1000, 80, onButtonHeld);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.dim(false);
  display.clearDisplay();
  Serial.println("OK let's go");

  delay(1000);
}

void loop() {
  displayDate();
  button.update();

  switch (currentMode) {
    case -1:
      /* Do nothing */
      break;
    case 0:
      static uint8_t startIndex = 0;
      startIndex += 1;
      FillLEDsFromPaletteColors(startIndex);
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.delay(1000 / UPDATES_PER_SECOND);
      FastLED.show();
      break;
    case 1:
      /* Turn off LEDs */
      FillLEDsWithColor(CRGB::Black);
      FastLED.show();
      break;
  }
}

void onButtonReleased(Button& btn, uint16_t duration) {
  if (!wasHeld) {
    currentMode++;
    currentMode %= totalModes;
    Serial.println("button pressed");
  }
  wasHeld = false;
}

void onButtonHeld(Button& btn, uint16_t duration, uint16_t repeatCount) {
  wasHeld = true;
  currentMode = -1;
  static uint8_t colorIndex = 0;
  FillLEDsFromPaletteColors(colorIndex);
  colorIndex += 10;
  FastLED.show();
}

void FillLEDsWithColor(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
  }
}
