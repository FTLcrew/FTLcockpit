/* FTL Cockpit
   Software to control Kerbal Space Program with the FTL Cockpit
*/

#include "KerbalSimpit.h"

// Input setup
#define SAS_PIN 3 // SAS button pin to control state
#define SAS_LED 8 // SAS LED pin
int sasLastState;
int sasCurrentState;

// Store the current action status, as recevied by simpit
byte currentActionStatus = 0;

// Declare a KerbalSimpit object that will communicate using the "Serial" device
KerbalSimpit ftlCockpit(Serial);

// This boolean tracks the desired LED state.
bool state = false;

// A timestamp of the last time we sent an echo packet.
unsigned long lastSent = 0;
// How often to send echo packets (in ms)
unsigned int sendInterval = 1000;

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set up the built in LED, and turn it on.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Set up the switch with builtin pullup.
  pinMode(SAS_PIN, INPUT_PULLUP);
  pinMode(SAS_LED, OUTPUT);
  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!ftlCockpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message in KSP to indicate handshaking is complete.
  ftlCockpit.printToKSP("Connected", PRINT_TO_SCREEN);
  
  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.
  ftlCockpit.inboundHandler(messageHandler);
  
  // Send a message to the plugin registering for the Action status channel.
  // The plugin will now regularly send Action status  messages while the
  // flight scene is active in-game.
  ftlCockpit.registerChannel(ACTIONSTATUS_MESSAGE);
}

void loop() {
  unsigned long now = millis();
  
  // If we've waited long enough since the last message, send a new one.
  if (now - lastSent >= sendInterval) {
    // If the last message was "high", send "low"
    // and vice-versa.
    if (state) {
      ftlCockpit.send(ECHO_REQ_MESSAGE, "low", 4);
    } else {
      ftlCockpit.send(ECHO_REQ_MESSAGE, "high", 5);
    }
    // Update when we last sent a message.
    lastSent = now;
    // Update the state variable to match the message we just sent.
    state = !state;
  }
  
  // Get the SAS switch state
  sasLastState = sasCurrentState;
  sasCurrentState = digitalRead(SAS_PIN);

  // Update the SAS to match the state, only if a change is needed to avoid
  // spamming commands.
  if(sasLastState != sasCurrentState){
    if(sasCurrentState && !(currentActionStatus & SAS_ACTION)){
      ftlCockpit.printToKSP("Activate SAS!");
      ftlCockpit.activateAction(SAS_ACTION);
      digitalWrite(SAS_LED, HIGH);
    }
    else if(sasCurrentState && (currentActionStatus & SAS_ACTION)){
      ftlCockpit.printToKSP("Desactivate SAS!");
      ftlCockpit.deactivateAction(SAS_ACTION);
      digitalWrite(SAS_LED, LOW);
    }
  }

  // Delay for debouncing -> change to milis
  delay(20);
  ftlCockpit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
  /*case ECHO_RESP_MESSAGE:
    // The message payload will be either "low" or "high".
    // We use the strcmp function to check what the string payload
    // is, and set the LED status accordingly.
    if (strcmp(msg, "low")) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  */
  case ACTIONSTATUS_MESSAGE:
    if (msgSize == 1) {
      currentActionStatus = msg[0];

      //Let the LED_BUILIN match the current SAS state
      if(currentActionStatus & SAS_ACTION){
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    break;
  }
}
