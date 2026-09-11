#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

#define OLED_SDA 4
#define OLED_SCL 5

#define SW1_PIN 0
#define SW2_PIN 1
#define SW3_PIN 2
#define SW4_PIN 3
#define SW5_PIN 6
#define SW7_PIN 7

#define ENCODER_A  8
#define ENCODER_B  9
#define ENCODER_SW 10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
USBHIDKeyboard Keyboard;

const int keys[] = {SW1_PIN, SW2_PIN, SW3_PIN, SW4_PIN, SW5_PIN, SW7_PIN};
const int numKeys = 6;

int lastKeyStates[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int lastEncoderAState;
int lastEncoderSWState = HIGH;

void drawStatus(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("MACRO PAD READY");
  display.setCursor(0, 16);
  display.println(msg);
  display.display();
}

void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  for (int i = 0; i < numKeys; i++) {
    pinMode(keys[i], INPUT_PULLUP);
  }

  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  Keyboard.begin();
  USB.begin();

  lastEncoderAState = digitalRead(ENCODER_A);
  drawStatus("System Idle");
}

void loop() {
  for (int i = 0; i < numKeys; i++) {
    int currentState = digitalRead(keys[i]);
    if (currentState == LOW && lastKeyStates[i] == HIGH) {
      switch (keys[i]) {
        case SW1_PIN:
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('c');
          delay(10);
          Keyboard.releaseAll();
          drawStatus("Key 1: COPY");
          break;
        case SW2_PIN:
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('v');
          delay(10);
          Keyboard.releaseAll();
          drawStatus("Key 2: PASTE");
          break;
        case SW3_PIN:
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('z');
          delay(10);
          Keyboard.releaseAll();
          drawStatus("Key 3: UNDO");
          break;
        case SW4_PIN:
          Keyboard.press(KEY_LEFT_SHIFT);
          Keyboard.press('a');
          delay(10);
          Keyboard.releaseAll();
          drawStatus("Key 4: SHIFT+A");
          break;
        case SW5_PIN:
          Keyboard.write('b');
          drawStatus("Key 5: 'b'");
          break;
        case SW7_PIN:
          Keyboard.write('c');
          drawStatus("Key 6: 'c'");
          break;
      }
      delay(50);
    }
    lastKeyStates[i] = currentState;
  }

  int currentEncoderAState = digitalRead(ENCODER_A);
  if (currentEncoderAState != lastEncoderAState && currentEncoderAState == LOW) {
    if (digitalRead(ENCODER_B) != currentEncoderAState) {
      Keyboard.write(KEY_MEDIA_VOLUME_UP);
      drawStatus("Vol: UP");
    } else {
      Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
      drawStatus("Vol: DOWN");
    }
  }
  lastEncoderAState = currentEncoderAState;

  int currentEncoderSWState = digitalRead(ENCODER_SW);
  if (currentEncoderSWState == LOW && lastEncoderSWState == HIGH) {
    Keyboard.write(KEY_MEDIA_MUTE);
    drawStatus("Vol: MUTE");
    delay(50);
  }
  lastEncoderSWState = currentEncoderSWState;
}