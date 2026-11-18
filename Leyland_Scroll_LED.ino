/*
 * ============================================================================
 * Project:     Leyland Bus Illuminated Scroll Emblem (ESP32-C3)
 * Description: One-time cursive handwriting stroke startup animation with
 *              433 MHz RF remote control and non-volatile flash memory (NVS).
 * Hardware:    - ESP32-C3-MINI-1
 *              - 84x SK6812-WWA (Warm White / Cool White / Amber) LEDs
 *              - RXB6 433 MHz Superheterodyne Receiver
 *              - PC817 Opto-isolated Side-Light Trigger (Active LOW)
 * Libraries:   - Adafruit NeoPixel (by Adafruit)
 *              - rc-switch (by sui77)
 *              - Preferences (built-in ESP32 core)
 * ============================================================================
 */

#include <Adafruit_NeoPixel.h>
#include <RCSwitch.h>
#include <Preferences.h>

// ==========================================
// 1. HARDWARE PIN DEFINITIONS (ESP32-C3)
// ==========================================
#define LED_PIN         4   // Data Out to SK6812-WWA LED chain (via 74AHCT1G125)
#define RF_RX_PIN       5   // Data In from 433MHz RXB6 Receiver
#define TRIGGER_PIN     6   // Opto-isolated Side-Light sense (Active LOW)
#define NUM_LEDS        84  // Total LEDs routed along the cursive "Leyland" path

// ==========================================
// 2. REMOTE CONTROL RF CODES (433 MHz)
// Open Serial Monitor (115200 baud) and press your keyfob buttons to view
// their serial codes, then update these four constants.
// ==========================================
const unsigned long RF_BTN_A_POWER  = 1048577; // Toggle On/Off & Replay Sweep
const unsigned long RF_BTN_B_COLOR  = 1048578; // Cycle 3000K / 4500K / 6000K
const unsigned long RF_BTN_C_BRIGHT = 1048580; // Cycle 25% / 50% / 75% / 100%
const unsigned long RF_BTN_D_SPEED  = 1048584; // Cycle Sweep Speed (0.8s / 1.4s / 2.2s)

// ==========================================
// 3. GLOBAL OBJECTS & PERSISTENT SETTINGS
// ==========================================
// SK6812-WWA uses 3 bytes per pixel (Warm White, Cool White, Amber)
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
RCSwitch mySwitch = RCSwitch();
Preferences prefs;

// Saved user preferences
bool badgeEnabled    = true;
uint8_t colorMode    = 0;   // 0 = 3000K Warm, 1 = 4500K Neutral, 2 = 6000K Cool
uint8_t brightIndex  = 3;   // 0 = 25%, 1 = 50%, 2 = 75%, 3 = 100%
uint8_t speedIndex   = 1;   // 0 = 0.8s, 1 = 1.4s, 2 = 2.2s

const uint8_t BRIGHTNESS_LEVELS[4] = {64, 128, 192, 255};
const uint16_t SWEEP_DURATIONS[3]  = {800, 1400, 2200}; // Total milliseconds

// State tracking
bool lastTriggerState = HIGH;
bool sweepPlayed      = false;
unsigned long lastRfTime = 0;

// ==========================================
// 4. HELPER: GET ACTIVE WWA COLOR
// ==========================================
uint32_t getActiveColor(uint8_t scale = 255) {
  uint8_t ww = 0, cw = 0, amb = 0;

  switch (colorMode) {
    case 0: // 3000K Vintage Incandescent Warm White + hint of Amber
      ww  = 240;
      cw  = 0;
      amb = 35;
      break;
    case 1: // 4500K Balanced Neutral White
      ww  = 160;
      cw  = 160;
      amb = 0;
      break;
    case 2: // 6000K Crisp Modern White
      ww  = 0;
      cw  = 255;
      amb = 0;
      break;
  }

  // Scale channels for smooth trailing tip fade during animation
  ww  = (ww * scale) / 255;
  cw  = (cw * scale) / 255;
  amb = (amb * scale) / 255;

  // Map to GRB byte order used by 3-channel SK6812-WWA
  return strip.Color(cw, ww, amb);
}

// ==========================================
// 5. STATIC STEADY-STATE ILLUMINATION
// ==========================================
void applyStaticIllumination() {
  if (!badgeEnabled) {
    strip.clear();
    strip.show();
    return;
  }

  strip.setBrightness(BRIGHTNESS_LEVELS[brightIndex]);
  uint32_t col = getActiveColor(255);

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, col);
  }
  strip.show();
}

// ==========================================
// 6. ANIMATION: ONE-TIME HANDWRITING SWEEP
// ==========================================
void playHandwritingSweep() {
  if (!badgeEnabled) return;

  strip.clear();
  strip.setBrightness(BRIGHTNESS_LEVELS[brightIndex]);
  strip.show();

  uint16_t totalTime = SWEEP_DURATIONS[speedIndex];
  const uint8_t tailLength = 6; // Soft 6-LED "wet ink" gradient tip

  unsigned long startTime = millis();
  bool complete = false;

  while (!complete) {
    unsigned long elapsed = millis() - startTime;
    float progress = (float)elapsed / (float)totalTime;

    if (progress >= 1.0f) {
      progress = 1.0f;
      complete = true;
    }

    // Current leading pen position along the 0..NUM_LEDS path
    float headPos = progress * (NUM_LEDS + tailLength);

    for (int i = 0; i < NUM_LEDS; i++) {
      if (i <= headPos - tailLength) {
        // Fully illuminated stroke behind the pen tip
        strip.setPixelColor(i, getActiveColor(255));
      } else if (i < headPos) {
        // Smooth fading gradient at the leading pen tip
        float factor = 1.0f - ((headPos - i) / (float)tailLength);
        uint8_t brightnessScale = (uint8_t)(factor * 255.0f);
        strip.setPixelColor(i, getActiveColor(brightnessScale));
      } else {
        // Unwritten section ahead of the pen
        strip.setPixelColor(i, 0);
      }
    }

    strip.show();
    delay(8); // ~120 FPS smooth interpolation
  }

  // Lock into steady 100% static illumination after sweep finishes
  applyStaticIllumination();
}

// ==========================================
// 7. SAVE & LOAD SETTINGS IN FLASH (NVS)
// ==========================================
void saveSettings() {
  prefs.begin("leyland_led", false);
  prefs.putBool("enabled", badgeEnabled);
  prefs.putUChar("color", colorMode);
  prefs.putUChar("bright", brightIndex);
  prefs.putUChar("speed", speedIndex);
  prefs.end();
}

void loadSettings() {
  prefs.begin("leyland_led", true);
  badgeEnabled = prefs.getBool("enabled", true);
  colorMode    = prefs.getUChar("color", 0);
  brightIndex  = prefs.getUChar("bright", 3);
  speedIndex   = prefs.getUChar("speed", 1);
  prefs.end();
}

// ==========================================
// 8. SETUP & MAIN LOOP
// ==========================================
void setup() {
  Serial.begin(115200);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  loadSettings();

  strip.begin();
  strip.clear();
  strip.show();

  // Initialize 433MHz RF Receiver
  mySwitch.enableReceive(digitalPinToInterrupt(RF_RX_PIN));

  // If wired directly to switched ignition (or TRIGGER_PIN bridged to GND),
  // play the startup handwriting animation immediately on boot.
  if (digitalRead(TRIGGER_PIN) == LOW) {
    playHandwritingSweep();
    sweepPlayed = true;
  }
}

void loop() {
  // 1. Check Side-Light / Ignition Trigger Line (Active LOW via optocoupler)
  bool currentTrigger = digitalRead(TRIGGER_PIN);

  if (currentTrigger == LOW && lastTriggerState == HIGH) {
    // Lights just switched ON -> run one-time handwriting animation
    delay(50); // Debounce automotive relay chatter
    if (digitalRead(TRIGGER_PIN) == LOW && !sweepPlayed) {
      playHandwritingSweep();
      sweepPlayed = true;
    }
  } else if (currentTrigger == HIGH && lastTriggerState == LOW) {
    // Lights switched OFF -> turn off LEDs and reset trigger for next startup
    strip.clear();
    strip.show();
    sweepPlayed = false;
  }
  lastTriggerState = currentTrigger;

  // 2. Listen for 433 MHz Keyfob Commands
  if (mySwitch.available()) {
    unsigned long receivedCode = mySwitch.getReceivedValue();

    // Print unknown codes to Serial Monitor so you can pair any new keyfob
    Serial.print("RF Code Received: ");
    Serial.println(receivedCode);

    // 300ms debounce so a single button press doesn't double-trigger
    if (millis() - lastRfTime > 300) {
      lastRfTime = millis();

      if (receivedCode == RF_BTN_A_POWER) {
        badgeEnabled = !badgeEnabled;
        saveSettings();
        if (badgeEnabled) {
          playHandwritingSweep();
        } else {
          applyStaticIllumination();
        }
      } 
      else if (receivedCode == RF_BTN_B_COLOR) {
        colorMode = (colorMode + 1) % 3;
        saveSettings();
        applyStaticIllumination();
      } 
      else if (receivedCode == RF_BTN_C_BRIGHT) {
        brightIndex = (brightIndex + 1) % 4;
        saveSettings();
        applyStaticIllumination();
      } 
      else if (receivedCode == RF_BTN_D_SPEED) {
        speedIndex = (speedIndex + 1) % 3;
        saveSettings();
        playHandwritingSweep(); // Preview the new sweep speed immediately
      }
    }

    mySwitch.resetAvailable();
  }
}
