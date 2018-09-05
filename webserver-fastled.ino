#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE

// WiFi and webserver
// replace the SSID and password with the credentials of your WiFi
const char* ssid = "gimmewebz";
const char* password = "gimmegoddamninternetznow";
WebServer server(80);

// FastLED
#define DATA_PIN    0
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    304
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  120

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

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
            var button = document.getElementById('button'); \
            button.addEventListener('click', function(e){  \
                var xhr = new XMLHttpRequest(); \
                xhr.open('POST', '/next', true);  \
                xhr.setRequestHeader('Content-Type', 'application/json'); \
                xhr.send(JSON.stringify({ \
                    action: 'next'  \
                }));  \
                xhr.onload = function() { \
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
          <p class='title has-text-white'>LED Master 3000</p> \
          <p class='subtitle'> \
            <br /><br /><br /><br /><br /> \
            <button class='primary has-text-white button is-link is-large' id='button'>next</button> \
          </p> \
        </div></div> \
    <section> \
</body>");
}


void startWifi(void * pvParameters) {
  // based on espressif esp32 WebServer examples, i.e. see
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/examples/HelloServer/HelloServer.ino
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

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
    server.send(200, "text/plain", "OK");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("\nServer listening on port 80");

  vTaskDelete(NULL);
}

void setup(void) {
  Serial.begin(115200);

  // start wifi
  xTaskCreatePinnedToCore(startWifi, "start_WiFi", 10000, NULL, 0, NULL, 0);

  // fastled
  delay(2000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(BRIGHTNESS);
}

typedef void (*Animations[])();
Animations animations = { confetti, rainbow, cloudSlowBeatWave, rainbowBlueTone, sinelon, juggle, rainbowRedTone, bpm };

void loop(void) {
  server.handleClient();

  animations[currentAnimation]();
  
  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void nextPattern() {
  currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
}

void rainbow() {
  // cycle forward with a rainbow
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, ++hue, 5);
  FastLED.setTemperature(UncorrectedTemperature);
}

void rainbowBlueTone() {
  // cycle backward with a rainbow of rather short color slices and blue tone
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, --hue, 23);
  FastLED.setTemperature(ClearBlueSky);
}

void rainbowRedTone() {
  // cycle backward with a rainbow of rather short color slices and red tone
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, --hue, 18);
  FastLED.setTemperature(Tungsten100W);
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
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS-1 );
  static uint8_t hue = 0;
  leds[pos] += CHSV(++hue, 255, 192);
  FastLED.setTemperature(UncorrectedTemperature);
}

void bpm() {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 25;
  CRGBPalette16 palette = OceanColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  static uint8_t hue = 0;
  hue--;
  for (int i = 0; i < NUM_LEDS; i++) {
    int x = beat-hue+i*10;
    if (x < 0) x = NUM_LEDS - x;
    leds[i] = ColorFromPalette(palette, hue+i*2 % NUM_LEDS, x);
  }
  FastLED.setTemperature(UncorrectedTemperature);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for(int i = 0; i < 8; i++) {
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