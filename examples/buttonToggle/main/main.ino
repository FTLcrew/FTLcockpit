// Button Toggle test

#define SAS_BUTTON 3
#define RCS_BUTTON 4

#define SAS_LED 8
#define RCS_LED 9

byte SASstate = LOW;
int lastSASstate;
int currentSASstate;

byte RCSstate = LOW;
int lastRCSstate;
int currentRCSstate;

// Debouncing for button pressing errors. To improve: deglitching, edge detection, non blocking operation
const int DEBOUCNE_DELAY = 50; // Debounce time; increase if output flickers
unsigned long lastStateChange = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(SAS_BUTTON, INPUT_PULLUP);
  pinMode(SAS_LED, OUTPUT);

  pinMode(RCS_BUTTON, INPUT_PULLUP);
  pinMode(RCS_LED, OUTPUT);

  currentSASstate = digitalRead(SAS_BUTTON);
  currentRCSstate = digitalRead(RCS_BUTTON);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastStateChange > DEBOUCNE_DELAY) {
    
    lastSASstate = currentSASstate; // Saves the last state
    currentSASstate = digitalRead(SAS_BUTTON);

    if(lastSASstate == HIGH && currentSASstate == LOW) {

      lastStateChange = millis();      
      Serial.println("SAS toggled");

      SASstate = !SASstate;
      digitalWrite(SAS_LED, SASstate);
    }
  }
}
