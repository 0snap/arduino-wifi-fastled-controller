# Controller for FastLED Stripe via WiFi+webserver (or a button)

The code is made such that it can be used for both `esp32` and `esp8266` boards.

This project combines some of the nice FastLED examples and other home-made animations with a super-basic webserver. The webserver hooks into local WiFi and exposes a simple form to switch the animation on the LED strip (i.e., send POST requests if you do that programatically). WiFi libraries are loaded for the board type that you configure.

In addition to the webserver controller you can change animations via a simple button click, given you solder a button to your board using PIN `22` (that can be changed in the code).

## Setup

- ESP 32 board. I use the `AZ Delivery ESP32 D1 Mini NodeMCU`
- OR: ESP 8266 board. I use the `NodeMCU 1.0` board
- LED stripe, at best WS2811 / WS2812. I like the longer ones (300 LEDs +), usually sold as "fairy lights".
- follow the install instructions or consider some datasheets:
    - https://github.com/espressif/arduino-esp32#installation-instructions
    - https://github.com/esp8266/Arduino#installing-with-boards-manager
    - https://arduino-esp8266.readthedocs.io/en/2.4.2/
    - https://www.espressif.com/en/support/download/documents
- install the necessary GCC extension for `xtensa-elf` (on arch linux that would be `aur/xtensa-elf-binutils` and `aur/xtensa-elf-gcc`)


## Usage

1. Open the the `webserver-fastled.ino` file. Define the board type you are using by setting the one `#define` to `1` and uncomment the other one
2. Enable / disable WiFi webserver and / or the button mode via the corresponding "#define"s
3. *WiFi only:* adjust the WiFi settings for you local WiFi
4. Tinker with all the other `#define`s in the code. Change the length of your LED stripe or the LED_TYPE etc...
5. Flash the code to your board (see the setup section above to install all required dependencies first)
6. *WiFi only* Open the serial monitor, check the prints for the EPS's IP address in your local network. Then open that address in a browser to change the animation of the LED stripe.
7. Push the button!
8. Have fun!


## Credits

#### FastLED
Most of the FastLED animations are not (solely) made by me. All credits belong to the awesome FastLED team! See here for the [FastLED project](https://github.com/FastLED), the [documentation](http://fastled.io) and for some [awesome examples that I used in this code](https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino). Their code is licensed under the [MIT](https://opensource.org/licenses/MIT) license.

#### ESP 8266 WebServer / WiFi
The code for connecting to WiFi and setting up the HTTP webserver is heavily inspired by the [esp8266/Arduino webserver examples](https://github.com/esp8266/Arduino/blob/74819a763bfb6e9890a57411dcea4aba221a778d/libraries/ESP8266WebServer/examples/HelloServer/HelloServer.ino). All credits for this great work belong to the respective creators! :)
For esp8266 related development go check out their docs / [repos](https://github.com/esp8266)!
Their code is licensed under the [LGPL-2.1 License](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)

#### ESP 32 WebSerber / WiFi
The code for connecting to WiFi and setting up the HTTP webserver is heavily inspired by the[espressif/arduino-esp32 webserver examples](https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/examples/HelloServer/HelloServer.ino). All credits for this great work belong to the respective creators! :)
For esp32 related development go check out their docs / [repos](https://github.com/espressif)!
Their code is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.en.html)
