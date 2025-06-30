/*
 * Step 6+ – Add random face + rolling animation + 90° display rotation
 * Press button → flash random faces → land on one (now rotated view)
 * Faces stored in numbers.h as diceBits[][8][9] PROGMEM
 */

#include <LedControl.h>
#include "numbers.h"  // contains diceBits[] and numFaces

const byte DIN = 11;
const byte CLK = 13;
const byte CS  = 12;
LedControl lc(DIN, CLK, CS, 1);

const byte BTN = 2;

bool lastReading = HIGH;
byte currentFace = 0;

unsigned long lastChange = 0;
const unsigned long DEBOUNCE = 50;

extern const byte numFaces;

/* Convert ASCII "01100110" → binary byte for LED matrix */
byte rowStringToByte(const char *s) {
  byte v = 0;
  for (byte i = 0; i < 8; i++) {
    v <<= 1;
    if (s[i] == '1') v |= 1;
  }
  return v;
}

/* NEW: Fetch and rotate a face from PROGMEM by 90° clockwise */
void showFaceByIndex(byte index) {
  char rowBuf[9];
  byte rotated[8] = {0};  // will hold rotated rows

  // Fetch each row from PROGMEM and convert to byte
  for (byte row = 0; row < 8; row++) {
    memcpy_P(rowBuf, diceBits[index][row], 9);
    byte bits = rowStringToByte(rowBuf);

    // Rotate each bit position 90° clockwise
    for (byte col = 0; col < 8; col++) {
      if (bits & (1 << col)) {
        rotated[col] |= (1 << (7 - row));  // flip row CW into column
        // rotated[7 - col] |= (1 << row);  // rotate 90° CCW
        // rotated[7 - row] |= (1 << (7 - col)); // rotate 180° (filp upside-down)

      }
    }
  }

  // Push rotated data to the LED matrix
  for (byte r = 0; r < 8; r++) {
    lc.setRow(0, r, rotated[r]);
  }
}

/* Show 5 random faces in quick sequence before landing on one */
void rollAnimation() {
  byte lastRoll = currentFace;

  for (byte i = 0; i < 25; i++) {
    byte roll;
    do {
      roll = random(numFaces);
    } while (roll == lastRoll);  // avoid repeating the same face

    lastRoll = roll;
    showFaceByIndex(roll);
    delay(50);  // short delay between flashes
  }

  currentFace = lastRoll;  // store result for later use
}

void setup() {
  lc.shutdown(0, false);       // turn on matrix
  lc.setIntensity(0, 3);       // LED brightness (0–15)
  lc.clearDisplay(0);          // start blank

  pinMode(BTN, INPUT_PULLUP);  // button reads LOW when pressed
  showFaceByIndex(currentFace);  // show face 1 on boot

  randomSeed(analogRead(A0));  // basic entropy for random()
  // this seeds the random number generator
}

void loop() {
  bool reading = digitalRead(BTN);

  // detect state change on button
  if (reading != lastReading) {
    lastChange = millis();
    lastReading = reading;
  }

  // if state has stayed LOW for > debounce time → valid press
  if ((millis() - lastChange) > DEBOUNCE && reading == LOW) {
    rollAnimation();                     // <<< main animation trigger
    while (digitalRead(BTN) == LOW) {}   // wait for release before allowing new roll
  }
}
