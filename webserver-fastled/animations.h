#ifndef ANIMATION_FUNCS
#define ANIMATION_FUNCS

  #include <FastLED.h>
  FASTLED_USING_NAMESPACE

  // functions that project an animation to the LED strip:

  void snow(CRGB *leds, int num_leds);
  void halloween(CRGB *leds, int num_leds);
  void fire(CRGB *leds, int num_leds);
  void wonderland(CRGB *leds, int num_leds);
  void gradient(CRGB *gradient, CRGB *leds, int num_leds);
  void blueSparks(CRGB *leds, int num_leds);
  void movingRainbow(CRGB *leds, int num_leds);
  void fixedRainbow(CRGB *leds, int num_leds);
  void confetti(CRGB *leds, int num_leds);
  void sinelon(CRGB *leds, int num_leds);
  void juggle(CRGB *leds, int num_leds);
  void cloudSlowBeatWave(CRGB *leds, int num_leds);

#endif
