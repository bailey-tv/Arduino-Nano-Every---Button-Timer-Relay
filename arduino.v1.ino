// Define hardware pins
const int buttonPin = 2;  // Button input pin (with built-in pull-up, so unpressed is HIGH)
const int relayPin  = 3;  // Relay control output pin
const int ledPin    = 4;  // LED indicator pin

// Timing constants (in milliseconds)
const unsigned long kRelayActiveTime = 5000;  // Relay active time (5 seconds)
const unsigned long kCooldownTime    = 15000;  // Cooldown time (15 seconds)

// Define system states
enum SystemState {
  WAITING_FOR_PRESS,
  RELAY_ACTIVE,
  COOLDOWN
};
SystemState state = WAITING_FOR_PRESS;

// Variables for timing
unsigned long stateStartTime = 0;

void setup() {
  // Configure pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Initial state: waiting, relay off, LED on (active)
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // Get the current time for non-blocking timing
  unsigned long currentMillis = millis();
  
  // Read button state (LOW means pressed with pull-up configuration)
  bool buttonPressed = (digitalRead(buttonPin) == LOW);
  
  switch (state) {
    case WAITING_FOR_PRESS:
      // Indicate system is ready (LED on), relay off.
      digitalWrite(ledPin, HIGH);
      digitalWrite(relayPin, LOW);
      
      // If button is pressed, start relay activation
      if (buttonPressed) {
        state = RELAY_ACTIVE;
        stateStartTime = currentMillis;  // record when relay activated
        digitalWrite(ledPin, LOW);   // turn LED off to indicate "active"
        digitalWrite(relayPin, HIGH); // activate relay
      }
      break;
      
    case RELAY_ACTIVE:
      // Stay in this state until the relay active duration expires.
      if (currentMillis - stateStartTime >= kRelayActiveTime) {
        // Turn off the relay
        digitalWrite(relayPin, LOW);
        // Transition to cooldown state
        state = COOLDOWN;
        stateStartTime = currentMillis;  // record when cooldown started
      }
      break;
      
    case COOLDOWN:
      // Keep LED off and ignore the button during cooldown.
      digitalWrite(ledPin, LOW);
      
      // Once cooldown period is complete, go back to waiting state.
      if (currentMillis - stateStartTime >= kCooldownTime) {
        state = WAITING_FOR_PRESS;
        // Reset variables if needed (stateStartTime gets reset on next state change)
        // Now the LED will be turned back on in the WAITING_FOR_PRESS case.
      }
      break;
  }
}
