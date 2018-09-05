# Controller for FastLED Stripe via WiFi+webserver

The code is made for the esp32 board.

This project basically combines some of the nice FastLED example animations with the espressif webserver, that will automatically hook into local WiFi. You can talk to the webserver (eg. mobile web view) or simply send post requests. On request, the next animation is chosen and will be shown on the LED stripe.

## Credits

#### FastLED
Most of the FastLED animations are not made by me. All credits belong to the awesome FastLED team! See here for the [FastLED project](https://github.com/FastLED), the [documentation](http://fastled.io) and for some [awesome examples that I used in this code](https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino).

#### ESP 32 WebSerber / WiFi
The code for connecting to WiFi and setting up the HTTP webserver is heavily inspired / taken from [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32). All credits for this great work belong to the respective creators! :)
For esp related development go check out their docs / [repos](https://github.com/espressif)!

## Setup

- ESP 32 board. I use the MH-ET Live MiniKit
- LED stripe, at best WS2811 / WS2812. I like the longer ones (300 LEDs +)
- follow the install instructions if you work with the arduino ide: https://github.com/espressif/arduino-esp32#installation-instructions
- install the necessary GCC extension for `xtensa-elf` (on arch linux that would be `aur/xtensa-elf-binutils` and `aur/xtensa-elf-gcc`)
- check out the sources of this project and replace the WiFi SSID / password with your credentials


## Usage

When you start the ESP it should print its subnet IP address to serial. Open that IP with another device in the same subnet (eg. smartphone, laptop etc). You should see a page with a button, which says `next`. Hit that button. The next animation should be shown.