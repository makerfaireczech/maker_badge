// This example is based on GxEPD2_HelloWorld.ino by Jean-Marc Zingg

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h> // including both doesn't hurt
#include <GxEPD2_3C.h> // including both doesn't hurt
#include <Fonts/FreeMonoBold12pt7b.h>

// Please define you maker badge version. Select A if you get badge earlier than 8/2022 a B if you get it after 8/2022

#define B

// ESP32-S2
#define BUSY 42
#define RST  39
#define DC   40
#define CS   41
#define PWR  16

#ifdef B
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(CS, DC, RST, BUSY)); // GDEM0213B74 128x250, SSD1680
#else
GxEPD2_BW<GxEPD2_213_T5D, GxEPD2_213_T5D::HEIGHT> display(GxEPD2_213_T5D(CS, DC, RST, BUSY)); // GDEW0213T5D
#endif

void setup()
{
  pinMode(PWR, OUTPUT);
   digitalWrite(PWR, LOW); //turn on power for screen
  display.init(115200);
  simpleText();
  display.hibernate();

  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();
//esp_light_sleep_start();
}

const char firstLine[] = "Hello maker";
const char secondLine[] = "I am your";
const char thirdLine[] = "badge.";

void simpleText()
{
  display.setRotation(3);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;

  // center bounding box by transposition of origin:
  display.getTextBounds(firstLine, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x1 = ((display.width() - tbw) / 2) - tbx;
  display.getTextBounds(secondLine, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x2 = ((display.width() - tbw) / 2) - tbx;
  display.getTextBounds(thirdLine, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x3 = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x1, y-20);
    display.print(firstLine);
    display.setCursor(x2, y);
    display.print(secondLine);
    display.setCursor(x3, y + 20);
    display.print(thirdLine);
  }
  while (display.nextPage());
}

void loop() {
  };
