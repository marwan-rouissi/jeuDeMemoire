/* Constants - define pin numbers for LEDs,
   buttons and speaker, and also the game tones: */
const byte ledPins[] = {4, 5, 6, 7};
const byte buttonPins[] = {8, 9, 10, 11};
#define SPEAKER_PIN 3

#define MAX_GAME_LENGTH 100


/* Global variables - store the game state */
byte gameSequence[MAX_GAME_LENGTH] = {0};
byte gameIndex = 0;
byte lvl = 1;

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13, A0, A1, A2, A3);

/**
   Set up the Arduino board and initialize Serial communication
*/
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(SPEAKER_PIN, OUTPUT);
  // The following line primes the random number generator.
  // It assumes pin A4 is floating (disconnected):
  randomSeed(analogRead(A4));
}

/**
   Lights the given LED
*/
void lightLed(byte ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
}

/**
   Plays the current sequence of notes that the user has to repeat
*/

void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLed(currentLed);
    delay(50);
  }
}

/**
    Waits until the user pressed one of the buttons,
    and returns the index of that button
*/
byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        return i;
      }
    }
    delay(1);
  }
}

/**
  Play the game over sequence, and report the game score
*/
void gameOver() {
  tone(SPEAKER_PIN, 500);
  delay(1000);
  noTone(SPEAKER_PIN);
  lcd.setCursor(0,0);
  lcd.print("Game over!");
  lcd.setCursor(0,1);
  lcd.print("your score:");
  lcd.setCursor(12,1);
  lcd.print(gameIndex - 1);
  gameIndex = 0;
  lvl = 1;
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("New Game");
  delay(2000);
  lcd.clear();
}


/**
   Get the user's input and compare it with the expected sequence.
*/
bool checkUserSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    lightLed(actualButton);
    if (expectedButton != actualButton) {
      return false;
    }
  }

  return true;
}


/**
   The main game loop
*/
void loop() {
  // Add a random color to the end of the sequence
  gameSequence[gameIndex] = random(0, 4);
  gameIndex++;
  lvl++;
  
  lcd.setCursor(0,0);
  lcd.print("Level:");
  lcd.setCursor(7,0);
  lcd.print(lvl-1);
  lcd.setCursor(0,1);
  lcd.print("your score:");
  lcd.setCursor(12,1);
  lcd.print(gameIndex - 1);
  
  if (gameIndex >= MAX_GAME_LENGTH) {
    gameIndex = MAX_GAME_LENGTH - 1;
    lvl++;
  }

  playSequence();
  if (!checkUserSequence()) {
    gameOver();
  }

  delay(300);

}