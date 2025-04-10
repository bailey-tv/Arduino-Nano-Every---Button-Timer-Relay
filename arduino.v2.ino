// Define hardware pins
const int buttonPin = 2;  // Button input pin (with built-in pull-up, so unpressed is HIGH)
const int relayPin  = 3;  // Relay control output pin
const int ledPin    = 4;  // LED indicator pin

// Timing constants (in milliseconds)
const unsigned long kRelayActiveTime = 6000;  // Relay active time (6 seconds)
const unsigned long kCooldownTime    = 20000; // Cooldown time (20 seconds)
const unsigned long kLongPressTime   = 100;   // Required button press duration (100 ms)

// Define system states
enum SystemState {
  WAITING_FOR_PRESS,
  RELAY_ACTIVE,
  COOLDOWN
};
SystemState state = WAITING_FOR_PRESS;
// Variables for timing
unsigned long stateStartTime = 0;
// Variable for detecting a long, debounced press:
unsigned long buttonPressStartTime = 0;

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
      
      // Debounce + Long press: only trigger if the button has been pressed continuously for at least 100ms.
      if (buttonPressed) {
        // If this is the first detection of the press, record the time.
        if (buttonPressStartTime == 0) {
          buttonPressStartTime = currentMillis;
        }
        // If the button has been held long enough, trigger relay activation.
        else if (currentMillis - buttonPressStartTime >= kLongPressTime) {
          state = RELAY_ACTIVE;
          stateStartTime = currentMillis;  // record when relay was activated
          digitalWrite(ledPin, LOW);         // turn LED off to indicate "active"
          digitalWrite(relayPin, HIGH);      // activate relay
          
          // Reset the press timing variable for subsequent presses.
          buttonPressStartTime = 0;
        }
      } 
      else {
        // Button not pressed: reset press timer (handles bounce and noise)
        buttonPressStartTime = 0;
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
      
      // Once the cooldown period is complete, return to waiting state.
      if (currentMillis - stateStartTime >= kCooldownTime) {
        state = WAITING_FOR_PRESS;
      }
      break;
  }
}
