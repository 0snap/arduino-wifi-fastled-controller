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

#include "animations.h"

// inspired by the Fast LED examples!
// see https://github.com/FastLED/FastLED
uint8_t currentAnimation = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Animation array typedef
typedef void (*Animations[])(CRGB*,int);

/*
  * The actual function calls (method names) of animation functions
  */
Animations animations = { wonderland, blueSparks, fire, snow, confetti, halloween, fixedRainbow, movingRainbow, sinelon, juggle, cloudSlowBeatWave };

/*
  * Display names of the animation for the web-view. Keep same order as other array.
  */
String animationNames[] = { "Wonderland", "Blue Sparks", "Fire", "Snow Flakes", "Confetti", "Halloween", "Rainbow", "Moving Rainbow", "Running Light", "Multiple Running Lights", "Clouded Beats" };


void setup(void) {
  Serial.begin(57600);
  Serial.println("setup");

  // start wifi
  startWifi();

  // fastled
  delay(1000);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(Typical8mmPixel);
  FastLED.setBrightness(BRIGHTNESS);
}


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

  animations[currentAnimation](leds, NUM_LEDS);
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void nextPattern() {
  Serial.println("next pattern");
  currentAnimation = (currentAnimation + 1) % ARRAY_SIZE(animations);
}
