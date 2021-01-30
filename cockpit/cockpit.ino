// define pin numbers as constants:
#define SAS 2

// variables:
byte currentSasState;
byte lastSasState = 0;
byte sasState = 0;

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers

void setup() {
  // put your setup code here, to run once:
  
  // set pinmodes:
  pinMode (SAS, INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // initialize serial:
  Serial.begin(9600);
  /* needed for Arduino Leonardo, "Serial" could be zero
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */
}

void loop() {
  // put your main code here, to run repeatedly:

  currentSasState = digitalRead(SAS);

  if(currentSasState != lastSasState)
  {
    lastDebounceTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (currentSasState == HIGH) {
        sasState = 1;
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("SAS enabled");
      } 
      else {
        sasState = 0;
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("SAS disabled");
      }
    }
  }

  lastSasState = currentSasState;
  
}

void sasTrigger(byte sasState) {
  if (sasState == 1) {
    // send keyboard function to KSP; turn on SAS
  }
  else {
    // turn off SAS
  }
  return;
}
