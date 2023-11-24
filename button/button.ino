const int buttonPin = 27;  // Pin connected to the button
const int ledPin = 25;    // Pin connected to the LED

int buttonState = 0;         // Current state of the button
int lastButtonState = 0;     // Previous state of the button
bool ledState = false;       // State of the LED, off by default

void setup() {
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT);    
  digitalWrite(ledPin, ledState); // Initialize the LED in the off state
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read the state of the button

  // Check if button state changed from not pressed to pressed
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      ledState = !ledState;  // Toggle the state of the LED
      digitalWrite(ledPin, ledState); // Set the LED according to ledState
    }
    // Delay a little bit for debounce
    delay(50);
  }

  lastButtonState = buttonState; // Save the current state as the last state for next loop iteration
}
