#define ENABLE_GxEPD2_GFX 0 // we won't need the GFX base class
#include <GxEPD2_BW.h>
// Online tool for converting images to byte arrays:
// https://javl.github.io/image2cpp/

#include "Bitmap.h"
#include <WS2812FX.h> // from library manager 
#include "ESP32_RMT_Driver.h"

// ESP32-S2 e-ink pinout
#define BUSY 42
#define RST  39
#define DC   40
#define CS   41
#define PWR  16

bool testing = 0;
const int bootPin = 0;
int bootPinState;
int lastBootPinState = HIGH;
unsigned long nowTime;
bool allPressed, screenDrawn = 0;

// This is borrowed from Debounce example:
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// This is borrowed from NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Which pin on the Arduino is connected to the NeoPixels?
#define ledPin    18
#define ledPower  21
// How many NeoPixels are attached to the board?
#define NUMPIXELS 4

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
WS2812FX neopixels(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

// Instantiate the GxEPD2_BW class for our display type
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(CS, DC, RST, BUSY)); // GDEM0213B74 128x250, SSD1680


void setup() {
  Serial.begin(115200);
  pinMode(bootPin, INPUT_PULLUP);
  pinMode(ledPower, OUTPUT);
  pinMode(PWR, OUTPUT);

  neopixels.init(); // initialize the ws2812fx instances

  neopixels.setBrightness(10); // set the overall LED brightnesses

  rmt_tx_int(RMT_CHANNEL_0, neopixels.getPin()); // assign ws2812fx1 to RMT channel 0

  neopixels.setCustomShow(myCustomShow1); // set the custom show function to forgo the NeoPixel
  digitalWrite(ledPower, LOW); //turn on power for LED´s
  digitalWrite(PWR, LOW); //turn on power for screen
  neopixels.strip_off(); //clear all LED´s 

  // wait for display to become available
  display.init(115200);
  // display Czech maker logo on the display
  drawBitmaps(czm_logo);
  //wait 2 secomds between screens
  delay(2000);
  //display second screen
  drawBitmaps(start_screen);
}

void loop() {
  neopixels.service(); // service each ws2812fx instance
  if (!testing) {
    readBootPin();
  }
  if (testing) {
    readTouchPins();
  }
  if (allPressed && !testing && !screenDrawn) {
    if ((millis() - nowTime) > 1000) {
      finalScreen();
    }
  }
}

//This is function from GxEPD library example
void drawBitmaps(const unsigned char *bitmap) {
  // Configure the display according to our preferences
  display.setRotation(2);
  display.setFullWindow();
  // Display the bitmap image
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(0, 0, bitmap, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
  } while (display.nextPage());
}

// This is borrowed from Debounce example :)
void readBootPin() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(bootPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastBootPinState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != bootPinState) {
      bootPinState = reading;

      // only toggle the LED if the new button state is HIGH
      if (bootPinState == LOW) {
        Serial.println("Button state is: " + String(bootPinState));
        drawBitmaps(second_screen);
        testing = 1;
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastBootPinState = reading;
}

void readTouchPins() {
  uint16_t touchread;
  byte atLeastOneTouch = 0;
  for (int i = 0; i < 5; i++) {
    // (5 - i) because pins order is reversed
    touchread = touchRead(5 - i);
    if (touchread > 25000 ) {
      Serial.printf("Captouch #%0d reading: %d\n", i, touchread);
      if (i != 4) {
        showRGB(i, RED);
      }
      else {
        showRGB(i, GREEN);
        allPressed = 1;
        testing = 0;
      }
      atLeastOneTouch++;
    }
    delay(40);
  }
  // If no touch detected in last cycle, turn off all LEDs
  if (atLeastOneTouch == 0) {
    neopixels.show();
  }
}

void showRGB(int led, unsigned int color) {
  // parameters: index, start, stop, mode, color, speed, reverse
  if (led != 4 ) {
    neopixels.setSegment(0, led, led, FX_MODE_STATIC, color, 1000);
    neopixels.start(); // start'em up
  } else {
    neopixels.setSegment(0, 0, led, FX_MODE_STATIC, color, 1000);
    neopixels.start(); // start'em up
    nowTime = millis();
  }
}

void finalScreen() {
  neopixels.strip_off();
  drawBitmaps(third_screen);
  delay(2000);
  drawBitmaps(czm_logo);
  digitalWrite(ledPower, HIGH);
  screenDrawn = 1;
}

// Custom show functions which will use the RMT hardware to drive the LEDs.
// Need a separate function for each ws2812fx instance.
void myCustomShow1(void) {
  uint8_t *pixels = neopixels.getPixels();
  // numBytes is one more then the size of the ws2812fx's *pixels array.
  // the extra byte is used by the driver to insert the LED reset pulse at the end.
  uint16_t numBytes = neopixels.getNumBytes() + 1;
  rmt_write_sample(RMT_CHANNEL_0, pixels, numBytes, false); // channel 0
}
