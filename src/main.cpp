#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Display pins (TFT GC9A01)
#define TFT_CS   6
#define TFT_RST  5
#define TFT_DC   4
#define TFT_MOSI 3
#define TFT_CLK  2

// Button pin
#define BUTTON_PIN 8

// Display object
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Color array
uint16_t colors[] = {
  ST7789_RED,
  ST7789_GREEN,
  ST7789_BLUE,
  ST7789_YELLOW,
  ST7789_CYAN,
  ST7789_MAGENTA,
  ST7789_WHITE
};
int colorIndex = 0;
int colorCount = sizeof(colors) / sizeof(colors[0]);

// Button state
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Initializing...");
  
  // Button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Display setup
  tft.init(240, 240);
  tft.setRotation(0);
  tft.fillScreen(ST7789_BLACK);
  
  Serial.println("Setup complete!");
  
  // Show initial color
  updateDisplay();
}

void updateDisplay() {
  tft.fillScreen(colors[colorIndex]);
  Serial.print("Color changed to index: ");
  Serial.println(colorIndex);
}

void loop() {
  // Read button with debouncing
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastButtonState) {
      if (reading == LOW) {  // Button pressed (active low)
        colorIndex = (colorIndex + 1) % colorCount;
        updateDisplay();
      }
      lastButtonState = reading;
    }
  }
  
  delay(10);
}
