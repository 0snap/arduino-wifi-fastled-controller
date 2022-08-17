// set the board - 1: use it. 0: don't use it. This regulates which libs are loaded.
#define ESP32 1
//#define ESP8266 1

// WiFi and webserver
// replace the SSID and password with the credentials of your WiFi
const char* ssid = "MagentaWLAN-QH63-2.4GHz";
const char* password = "cacaolove<3";

#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  WebServer server(80);
#elif ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#endif

// this name is shown in the browser 'app'
#define STRIP_NAME          "Mittel-Gang"

//#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE


// FastLED
#define DATA_PIN            16
#define LED_TYPE            WS2812B
#define COLOR_ORDER         RGB
#define NUM_LEDS            200
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND   120

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


void setup(void) {
  Serial.begin(57600);
  Serial.println("setup");

  // start wifi
  //  xTaskCreatePinnedToCore(startWifi, "start_WiFi", 10000, NULL, 0, NULL, 0);
  startWifi();

  // fastled
  delay(2000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(BRIGHTNESS);
}

typedef void (*Animations[])();

//Animations animations = { blueSparks, snow, confetti, fixedRainbow, movingRainbow, sinelon, juggle };             // The actual function calls (method names) of animation functions
//String animationNames[] = { "Blue Sparks", "Snow Flakes", "Confetti", "Rainbow", "Moving Rainbow", "Running Light", "6 Running Lights" }; // Display names of the animation for the web-view. Keep same order as other array.
Animations animations = { wonderland, blueSparks, fire, snow, confetti, halloween, fixedRainbow, movingRainbow, sinelon, juggle, cloudSlowBeatWave };             // The actual function calls (method names) of animation functions
String animationNames[] = { "Wonderland", "Blue Sparks", "Fire", "Snow Flakes", "Confetti", "Halloween", "Rainbow", "Moving Rainbow", "Running Light", "Multiple Running Lights", "Clouded Beats" }; // Display names of the animation for the web-view. Keep same order as other array.



/*
 * This serves a very rudimentary responsive webview. Visit this in browser.
 * The webview has a button. When tapped, a post request is made to this esp webserver.
 * The next animation will start.
 */
void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html> \
<head>  \
    <meta charset='utf-8'/> \
    <meta name='viewport' content='width=device-width, initial-scale=1'> \
    <link rel='stylesheet' type='text/css' href='https://cdnjs.cloudflare.com/ajax/libs/bulma/0.7.1/css/bulma.min.css'> \
    <script>  \
        window.onload = function() { \
            var label = document.getElementById('label'); \
            label.innerHTML = '"+ animationNames[currentAnimation] +"'; \
            var button = document.getElementById('button'); \
            button.addEventListener('click', function(e){  \
                button.disabled = true; \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/next', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'next'  \
                }));  \
                xhr.onload = function() { \
                    var label = document.getElementById('label'); \
                    label.innerHTML = this.responseText; \
                    button.disabled = false; \
                    console.log('response:', this.responseText);  \
                } \
            }, false);  \
        } \
    </script> \
</head> \
  \
<body>  \
    <section class='hero is-mobile is-tablet is-desktop has-background-primary is-large' style='height: 100vh;'> \
        <div class='hero-body max-height'><div class='container has-text-centered'> \
          <p class='title has-text-white'>" + STRIP_NAME + "</p> \
          <p class='subtitle'> \
              <br /><br /><br /><br /><br /> \
              <label class='has-text-white'>Animation: </label> <label id='label' class='has-text-white'/> \
            </p> \
          <p class='subtitle'> \
            <br /><br /><br /><br /><br /> \
            <button class='primary has-text-white button is-link is-large' id='button'>next</button> \
          </p> \
        </div></div> \
    <section> \
</body>");
}

void startWifi() {
  // based on espressif esp32 WebServer examples, i.e. see
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/examples/HelloServer/HelloServer.ino

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  Serial.print("Starting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Success! My Subnet IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/next", HTTP_POST, [](){
    nextPattern();
    server.send(200, "text/plain", animationNames[currentAnimation]);
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("\nServer listening on port 80");
}

void loop(void) {
  server.handleClient();

  animations[currentAnimation]();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void nextPattern() {
  Serial.println("next pattern");
  currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
}

void snow() {
  fadeLightBy(leds, NUM_LEDS, 2);
  int pos = random16(NUM_LEDS);

  CRGB snow_white = CRGB(255, 200, 150);
  leds[pos] = snow_white;
  FastLED.setTemperature(Tungsten40W);
  delay(600 / NUM_LEDS);
}

void halloween() {
  CHSV orange = CHSV(25, 200, 200);
  CHSV blue = CHSV(160, 200, 200);

  CRGB grad[NUM_LEDS * 2];
  fill_gradient(grad, 0, blue, NUM_LEDS, orange);
  fill_gradient(grad, NUM_LEDS + 1, orange, NUM_LEDS * 2, blue);

  gradient(grad);
}


void fire() {
  CHSV yellow = CHSV(42, 200, 200);
  CHSV red = CHSV(235, 200, 200);

  CRGB grad[NUM_LEDS * 2];
  fill_gradient(grad, 0, yellow, NUM_LEDS, red);
  fill_gradient(grad, NUM_LEDS + 1, red, NUM_LEDS * 2, yellow);

  gradient(grad);
}

void wonderland() {
  CHSV green = CHSV(85, 200, 200);
  CHSV red = CHSV(220, 200, 200);

  CRGB grad[NUM_LEDS * 2];
  fill_gradient(grad, 0, green, NUM_LEDS, red, LONGEST_HUES);
  fill_gradient(grad, NUM_LEDS + 1, red, NUM_LEDS * 2, green, LONGEST_HUES);

  gradient(grad);
}

void gradient(CRGB *gradient) {
  // http://fastled.io/docs/3.1/group___colorutils.html#ga3144bb2bb66aeed33e20f4fdd6cc1a98
  // https://github.com/FastLED/FastLED/wiki/Pixel-reference

  static uint16_t offset = 0;

  EVERY_N_MILLISECONDS(50) {
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      uint16_t idx = (i+offset) % NUM_LEDS;
      leds[idx] = gradient[i * 2];
    }
    offset = (offset + 1) % NUM_LEDS;
  }

  FastLED.setTemperature(Tungsten40W);
}

void blueSparks() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  fract8 chanceOfGlitter = NUM_LEDS / 2;

  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
    leds[ random16(NUM_LEDS) ] += CRGB::Blue;
  }
}

void movingRainbow() {
  // cycle forward with a rainbow
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, ++hue, 5);
  FastLED.setTemperature(UncorrectedTemperature);
}

void fixedRainbow() {
  fill_rainbow(leds, NUM_LEDS, 0);
  FastLED.setTemperature(UncorrectedTemperature);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  static uint8_t hue = 0;
  leds[pos] += CHSV(--hue + random8(64), 200, 255);
  FastLED.setTemperature(UncorrectedTemperature);
}

void sinelon() {
  // one running dot
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-1 );
  static uint8_t hue = 0;
  leds[pos] += CHSV(++hue, 255, 192);
  FastLED.setTemperature(UncorrectedTemperature);
}

void juggle() {
  // multiple running dots
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t num_dots = 6;
  byte dothue = 0;
  for(int i = 0; i < num_dots; i++) {
    leds[beatsin16(i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.setTemperature(UncorrectedTemperature);
}

void cloudSlowBeatWave() {
  for (int i = 0; i < NUM_LEDS; i++) {
    int colorIndex = beat8(30  + i % 40) + ease8InOutQuad(i + NUM_LEDS);
    leds[i] = ColorFromPalette(CloudColors_p, colorIndex, NUM_LEDS, LINEARBLEND);
  }
  FastLED.setTemperature(UncorrectedTemperature);
}
