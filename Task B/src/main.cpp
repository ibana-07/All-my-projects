// name : ibana arooj
//reg number: 23-ntu-cs-1039

// ESP32 Wokwi Task B 
// Short press → alternate LED toggle (LED1 and LED2)
// Long press (>1.5s) → buzzer tone only
// OLED displays event type

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BUTTON_PIN 15
#define LED1_PIN 2
#define LED2_PIN 5
#define BUZZER_PIN 4

unsigned long pressStart = 0;
bool buttonPressed = false;
int toggleState = 0; // 0 → LED1 ON, LED2 OFF; 1 → LED1 OFF, LED2 ON

void setup() {
  Serial.begin(115200);  // For debugging
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }

  // Initial display and LED state
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Ready...");
  display.display();
  Serial.println("Setup complete");
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && !buttonPressed) {
    // Button just pressed - add debounce
    delay(50);  // Debounce for reliability
    if (digitalRead(BUTTON_PIN) == LOW) {
      buttonPressed = true;
      pressStart = millis();
      Serial.println("Button pressed");
    }
  }

  if (buttonState == HIGH && buttonPressed) {
    // Button released - add debounce
    delay(50);  // Debounce for reliability
    if (digitalRead(BUTTON_PIN) == HIGH) {
      buttonPressed = false;
      unsigned long pressDuration = millis() - pressStart;
      Serial.print("Press duration: ");
      Serial.println(pressDuration);

      display.clearDisplay();
      display.setCursor(0, 0);

      if (pressDuration > 1500) {
        // Long press = buzzer tone only (no LED toggle)
        display.print("Long Press: Buzzer");
        tone(BUZZER_PIN, 1000, 500);
        Serial.println("Long press: Buzzer only");
      } else {
        // Short press = alternate LEDs
        display.print("Short Press: LEDs Toggled");
        toggleState = !toggleState;
        if (toggleState == 0) {
          digitalWrite(LED1_PIN, HIGH);
          digitalWrite(LED2_PIN, LOW);
        } else {
          digitalWrite(LED1_PIN, LOW);
          digitalWrite(LED2_PIN, HIGH);
        }
        Serial.println("Short press: LEDs toggled");
      }

      display.display();
    }
  }
}