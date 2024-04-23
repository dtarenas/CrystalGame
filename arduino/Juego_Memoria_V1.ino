// Memory Game SIMON DICE - Crystal version
// Original program: Sebastian from the electgpl channel
// Modified by: Prof. Garcia and Diego Arenas

/* Variable declaration */
#define Red               2
#define Clear             3
#define Yellow            4
#define Blue              5
#define Bell              7

#define In_Red            A0
#define In_Clear          A1
#define In_Yellow         A2
#define In_Blue           A3
#define In_Next_Attempt   A4


// Speed and level variables
int speed = 600;
int irInputValue = 0;
int currentLevel = 3;     // Initial level
const int INITLEVEL = 3;  // Initial level
const int MAXLEVEL = 15;
int sequence[MAXLEVEL];
int sequenceRead[MAXLEVEL];
/* End of variable declaration */

/* Arduino configuration */
void setup() {
  // Set pin modes
  pinMode(Yellow, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Clear, OUTPUT);
  pinMode(Bell, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600); //Initialize serial monitor
}

/* Main loop */
void loop() {
    // Check if the next attempt button is pressed
    if (digitalRead(In_Next_Attempt) == LOW) {
      // Prepare the level
      prepareLevel();

      // If it's the initial level, generate a new sequence
      //if (currentLevel == INITLEVEL) {
        doNewSequence();
      //}

      // Show the sequence and read the player's input
      showSequence();
      readSequence();
    } 
    else {
      // Start the initial animation and turn off LEDs
      start();
    }
}

// ------------------------  ------------------------
/* Start the initial animation */
void start() {
  // Animate LEDs if the next attempt button is still pressed
  for (int i = 1; i <= 6; i++) {
    if (digitalRead(In_Next_Attempt) == HIGH){
      digitalWrite(Clear, HIGH);
      delay(200);
      digitalWrite(Clear, LOW);
      digitalWrite(Yellow, HIGH);
      delay(200);
      digitalWrite(Yellow, LOW);
      digitalWrite(Blue, HIGH);
      delay(200);
      digitalWrite(Blue, LOW);
      digitalWrite(Red, HIGH);
      delay(200);
      digitalWrite(Red, LOW);
    } else {
      break;
    }
  }
}

/* Prepare the level */
void prepareLevel(){
  Serial.print(1);
  delay(1000);

  // If the next attempt button is still pressed
  if(digitalRead(In_Next_Attempt) == HIGH){
    for (int i = 1; i <= 3; i++){
      ledsOn();
      tone(Bell, 1500); 
      delay(300);
      noTone(Bell); 
      ledsOff();
      delay(300);
    }

    tone(Bell, 3000); 
    delay(800);
    noTone(Bell); 
    delay(500);
  }
}

/* Turn on all LEDs */
void ledsOn(){
  digitalWrite(Clear, HIGH);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Blue, HIGH);
  digitalWrite(Red, HIGH);
}

/* Turn off all LEDs */
void ledsOff() {
  digitalWrite(Clear, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Blue, LOW);
  digitalWrite(Red, LOW);
  delay(300);
}
// -------------------   ---------------   -------

/* Show the sequence */
void showSequence() {
  ledsOff();
  for (int i = 0; i < currentLevel; i++) {
    // Play corresponding tone and light up LED
    if (sequence[i] == Yellow) {
      tone(Bell, 200);
      delay(200);
      noTone(Bell);
    }

    if (sequence[i] == Blue) {
      tone(Bell, 300);
      delay(200);
      noTone(Bell);
    }

    if (sequence[i] == Red) {
      tone(Bell, 400);
      delay(200);
      noTone(Bell);
    }

    if (sequence[i] == Clear) {
      tone(Bell, 500);
      delay(200);
      noTone(Bell);
    }

    // Light up LED and wait
    ColorOn(sequence[i]);
    delay(speed);
    ColorOff(sequence[i]);
    delay(200);
  }
}

/* Read player's input */
void readSequence() {
  int flag = 0;
  for (int i = 0; i < currentLevel; i++) {
    flag = 0;
    while (flag == 0) {
      // Check which button is pressed
      if (digitalRead(In_Clear) == LOW) {
        ColorOn(Clear);
        tone(Bell, 500);
        delay(200);
        noTone(Bell);
        sequenceRead[i] = Clear;
        flag = 1;
        delay(200);
        // Check if the input matches the sequence
        if (sequenceRead[i] != sequence[i]) {
          errorSequence();
          return;
        }
        ColorOff(Clear);
      }
      if (digitalRead(In_Red) == LOW) {
        ColorOn(Red);
        tone(Bell, 400);
        delay(200);
        noTone(Bell);
        sequenceRead[i] = Red;
        flag = 1;
        delay(200);
        if (sequenceRead[i] != sequence[i]) {
          errorSequence();
          return;
        }
        ColorOff(Red);
      }
      if (digitalRead(In_Blue) == LOW) {
        ColorOn(Blue);
        tone(Bell, 300);
        delay(200);
        noTone(Bell);
        sequenceRead[i] = Blue;
        flag = 1;
        delay(200);
        if (sequenceRead[i] != sequence[i]) {
          errorSequence();
          return;
        }
        ColorOff(Blue);
      }
      if (digitalRead(In_Yellow) == LOW) {
        ColorOn(Yellow);
        tone(Bell, 200);
        delay(200);
        noTone(Bell);
        sequenceRead[i] = Yellow;
        flag = 1;
        delay(200);
        if (sequenceRead[i] != sequence[i]) {
          errorSequence();
          return;
        }
        ColorOff(Yellow);
      }
    }
  }

  correctSequence();
}

/* Generate a new sequence */
void doNewSequence() {
  randomSeed(analogRead(5));  // randomSeed(millis());
  for (int i = 0; i < MAXLEVEL; i++) {
    sequence[i] = random(2, 6);
  }
}

/* Handle error sequence */
void errorSequence() {
  Serial.print(7);
  ledsOff();
  delay(250);

  digitalWrite(Red, HIGH);
  playFailingSound();
  delay(1000);
  ledsOff();
  start();
}

/* Handle correct sequence */
void correctSequence() {
  if (currentLevel < MAXLEVEL) {
    currentLevel++;
  }
  speed -= 10;

  Serial.print(8);
  digitalWrite(Clear, HIGH);
  playWinningSound();
  ledsOff();
  delay(1000);
}

/* Play the winning sound */
void playWinningSound() {
  tone(Bell, 2000); // Second highest tone
  delay(100);
  tone(Bell, 2500); // Third even higher tone
  delay(100);
  tone(Bell, 3000); // Fourth highest tone
  delay(100);
  tone(Bell, 3500); // Fifth very high tone
  delay(100);
  tone(Bell, 4000); // Sixth extremely high tone
  delay(300);
  noTone(Bell); // Stop the tone
}

/* Play failing sound */
void playFailingSound() {
  int Tono[] = { 260, 200, 200, 220, 200, 0, 250, 260 };
  int duracionNotas[] = { 4, 8, 8, 4, 4, 4, 4, 4 };
  for (int i = 0; i < 8; i++) {
    int duracionNota = 1000 / duracionNotas[i];
    tone(Bell, Tono[i], duracionNota);
    int pausaEntreNotas = duracionNota * 1.50;
    delay(pausaEntreNotas);
    noTone(Bell);
  }
}

void ColorOn(int color){
  digitalWrite(color, HIGH);
  Serial.print(color);
  delay(100);
}

void ColorOff(int color){
  digitalWrite(color, LOW);
  delay(100);
}
