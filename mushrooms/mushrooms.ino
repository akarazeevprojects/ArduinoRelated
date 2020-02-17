#include <Button.h>
#include <PushButton.h>

PushButton button = PushButton(6);

#include <FastLED.h>

#define LED_A       2
#define LED_B       3
#define LED_C       4
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 20

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGB led_a[1];
CRGB led_b[1];
CRGB led_c[1];

uint8_t startIndex = 0;
int currentMode = 0;
int totalModes = 3;
bool wasHeld = false;

void setup() {
  delay(100);

  FastLED.addLeds<LED_TYPE, LED_A, COLOR_ORDER>(led_a, 1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_B, COLOR_ORDER>(led_b, 1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_C, COLOR_ORDER>(led_c, 1).setCorrection(TypicalLEDStrip);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  button.onRelease(onButtonReleased);
  button.onHoldRepeat(1000, 80, onButtonHeld);
}

void loop() {
  button.update();

  switch (currentMode) {
    case 0:
      startIndex += 1;
      FillLEDsFromPaletteDifferently(startIndex);
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.delay(1000 / UPDATES_PER_SECOND);
      FastLED.show();
      break;
    case 1:
      startIndex += 1;
      FillLEDsFromPaletteColors(startIndex);
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.delay(1000 / UPDATES_PER_SECOND);
      FastLED.show();
      break;
    case 2:
      /* Turn off LEDs */
      FillLEDsWithColor(CRGB::Black);
      FastLED.show();
      break;
  }
}

void onButtonReleased(Button& btn, uint16_t duration) {
  if (!wasHeld) {
    if (currentMode == -1) {
      currentMode = 0;
    } else if (currentMode == -2) {
      currentMode = 1;
    } else {
      currentMode++;
    }
    currentMode %= totalModes;
    Serial.println("button pressed");
  }
  wasHeld = false;
}

void onButtonHeld(Button& btn, uint16_t duration, uint16_t repeatCount) {
  wasHeld = true;
  static uint8_t colorIndex = 0;

  if (currentMode == 0 or currentMode == -1) {
    currentMode = -1;
    FillLEDsFromPaletteDifferently(colorIndex);
    colorIndex += 10;
  } else if (currentMode == 1 or currentMode == -2) {
    currentMode = -2;
    FillLEDsFromPaletteColors(colorIndex);
    colorIndex += 10;
  }

  FastLED.show();
}

void FillLEDsWithColor(CRGB color) {
  led_a[0] = color;
  led_b[0] = color;
  led_c[0] = color;
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t myBrightness = BRIGHTNESS;
  led_a[0] = ColorFromPalette(currentPalette, colorIndex, myBrightness, currentBlending);
  led_b[0] = ColorFromPalette(currentPalette, colorIndex, myBrightness, currentBlending);
  led_c[0] = ColorFromPalette(currentPalette, colorIndex, myBrightness, currentBlending);
}

void FillLEDsFromPaletteDifferently(uint8_t colorIndex) {
  uint8_t myBrightness = BRIGHTNESS;
  uint8_t delta_b = 50;
  uint8_t delta_c = 100;
  led_a[0] = ColorFromPalette(currentPalette, colorIndex, myBrightness, currentBlending);
  led_b[0] = ColorFromPalette(currentPalette, colorIndex + delta_b, myBrightness, currentBlending);
  led_c[0] = ColorFromPalette(currentPalette, colorIndex + delta_c, myBrightness, currentBlending);
}
