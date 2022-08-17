#include "animations.h"

void snow(CRGB *leds, int num_leds) {
  fadeLightBy(leds, num_leds, 2);
  int pos = random16(num_leds);

  CRGB snow_white = CRGB(255, 200, 150);
  leds[pos] = snow_white;
  FastLED.setTemperature(Tungsten40W);
  delay(600 / num_leds);
}

void halloween(CRGB *leds, int num_leds) {
  CHSV orange = CHSV(25, 200, 200);
  CHSV blue = CHSV(160, 200, 200);

  CRGB grad[num_leds * 2];
  fill_gradient(grad, 0, blue, num_leds, orange);
  fill_gradient(grad, num_leds + 1, orange, num_leds * 2, blue);

  gradient(grad, leds, num_leds);
}

void fire(CRGB *leds, int num_leds) {
  CHSV yellow = CHSV(42, 200, 200);
  CHSV red = CHSV(235, 200, 200);

  CRGB grad[num_leds * 2];
  fill_gradient(grad, 0, yellow, num_leds, red);
  fill_gradient(grad, num_leds + 1, red, num_leds * 2, yellow);

  gradient(grad, leds, num_leds);
}

void wonderland(CRGB *leds, int num_leds) {
  CHSV green = CHSV(85, 200, 200);
  CHSV red = CHSV(220, 200, 200);

  CRGB grad[num_leds * 2];
  fill_gradient(grad, 0, green, num_leds, red, LONGEST_HUES);
  fill_gradient(grad, num_leds + 1, red, num_leds * 2, green, LONGEST_HUES);

  gradient(grad, leds, num_leds);
}

void gradient(CRGB *gradient, CRGB *leds, int num_leds) {
  // http://fastled.io/docs/3.1/group___colorutils.html#ga3144bb2bb66aeed33e20f4fdd6cc1a98
  // https://github.com/FastLED/FastLED/wiki/Pixel-reference

  static uint16_t offset = 0;

  EVERY_N_MILLISECONDS(50) {
    for (uint16_t i = 0; i < num_leds; i++) {
      uint16_t idx = (i+offset) % num_leds;
      leds[idx] = gradient[i * 2];
    }
    offset = (offset + 1) % num_leds;
  }

  FastLED.setTemperature(Tungsten40W);
}

void blueSparks(CRGB *leds, int num_leds) {
  fadeToBlackBy(leds, num_leds, 10);
  fract8 chanceOfGlitter = num_leds / 2;

  if( random8() < chanceOfGlitter) {
    leds[ random16(num_leds) ] += CRGB::White;
    leds[ random16(num_leds) ] += CRGB::Blue;
  }
}

void movingRainbow(CRGB *leds, int num_leds) {
  // cycle forward with a rainbow
  static uint8_t hue = 0;
  fill_rainbow(leds, num_leds, ++hue, 5);
  FastLED.setTemperature(UncorrectedTemperature);
}

void fixedRainbow(CRGB *leds, int num_leds) {
  fill_rainbow(leds, num_leds, 0);
  FastLED.setTemperature(UncorrectedTemperature);
}

void confetti(CRGB *leds, int num_leds) {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, num_leds, 10);
  int pos = random16(num_leds);
  static uint8_t hue = 0;
  leds[pos] += CHSV(--hue + random8(64), 200, 255);
  FastLED.setTemperature(UncorrectedTemperature);
}

void sinelon(CRGB *leds, int num_leds) {
  // one running dot
  fadeToBlackBy(leds, num_leds, 20);
  int pos = beatsin16(13, 0, num_leds-1 );
  static uint8_t hue = 0;
  leds[pos] += CHSV(++hue, 255, 192);
  FastLED.setTemperature(UncorrectedTemperature);
}

void juggle(CRGB *leds, int num_leds) {
  // multiple running dots
  fadeToBlackBy(leds, num_leds, 20);
  uint8_t num_dots = 6;
  byte dothue = 0;
  for(int i = 0; i < num_dots; i++) {
    leds[beatsin16(i+7, 0, num_leds-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.setTemperature(UncorrectedTemperature);
}

void cloudSlowBeatWave(CRGB *leds, int num_leds) {
  for (int i = 0; i < num_leds; i++) {
    int colorIndex = beat8(30  + i % 40) + ease8InOutQuad(i + num_leds);
    leds[i] = ColorFromPalette(CloudColors_p, colorIndex, num_leds, LINEARBLEND);
  }
  FastLED.setTemperature(UncorrectedTemperature);
}

