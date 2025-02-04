#include <BleKeyboard.h>
#include <RotaryEncoder.h>

// Pin definitions
#define PIN_ENCODER_A 32
#define PIN_ENCODER_B 33
#define BUTTON_UP 14
#define BUTTON_LEFT 12
#define BUTTON_DOWN 26
#define BUTTON_RIGHT 27
#define BUTTON_IN 25

// Initialize the encoder
RotaryEncoder encoder(PIN_ENCODER_A, PIN_ENCODER_B, RotaryEncoder::LatchMode::TWO03);

// Initialize BleKeyboard
BleKeyboard bleKeyboard;

// Interrupt function to update encoder position
void IRAM_ATTR checkPosition() {
  encoder.tick();
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Rotary Encoder BLE Keyboard");

  // Set up encoder interrupts
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), checkPosition, CHANGE);

  // Configure button pins with pull-up resistors
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_IN, INPUT_PULLUP);

  // Initialize BleKeyboard
  bleKeyboard.begin();
}

void loop() {
  if (bleKeyboard.isConnected()) {
    RotaryEncoder::Direction direction = encoder.getDirection();

    if (direction == RotaryEncoder::Direction::CLOCKWISE) { 
      bleKeyboard.write(KEY_UP_ARROW);
      Serial.println("Key Up");
    } else if (direction == RotaryEncoder::Direction::COUNTERCLOCKWISE) {
      bleKeyboard.write(KEY_DOWN_ARROW);
      Serial.println("Key Down");
    }

    static bool lastButtonUpState = HIGH;
    bool buttonUpState = digitalRead(BUTTON_UP);
    if (buttonUpState != lastButtonUpState) {
      if (buttonUpState == LOW) {
        bleKeyboard.write(KEY_TAB);
        Serial.println("Button Up Pressed");
      }
      lastButtonUpState = buttonUpState;
    }

    static bool lastButtonDownState = HIGH;
    bool buttonDownState = digitalRead(BUTTON_DOWN);
    if (buttonDownState != lastButtonDownState) {
      if (buttonDownState == LOW) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        Serial.println("Button Down Pressed");
      }
      lastButtonDownState = buttonDownState;
    }

    static bool lastButtonLeftState = HIGH;
    bool buttonLeftState = digitalRead(BUTTON_LEFT);
    if (buttonLeftState != lastButtonLeftState) {
      if (buttonLeftState == LOW) {
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
        Serial.println("Button Left Pressed");
      }
      lastButtonLeftState = buttonLeftState;
    }

    static bool lastButtonRightState = HIGH;
    bool buttonRightState = digitalRead(BUTTON_RIGHT);
    if (buttonRightState != lastButtonRightState) {
      if (buttonRightState == LOW) {
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
        Serial.println("Button Right Pressed");
      }
      lastButtonRightState = buttonRightState;
    }

    static bool lastButtonInState = HIGH;
    bool buttonInState = digitalRead(BUTTON_IN);
    if (buttonInState != lastButtonInState) {
      if (buttonInState == LOW) {
        bleKeyboard.write(KEY_RETURN);
        Serial.println("Button In Pressed");
      }
      lastButtonInState = buttonInState;
    }
  } else {
    Serial.println("Waiting for BLE connection...");
  }

  delay(20); // Debounce delay
}
