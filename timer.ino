// Define pin numbers for the LED and potentiometer
const byte LED_PIN = 13;
const byte POTENTIOMETER_PIN = A4;

void setup()
{
  // Configure LED pin as output and potentiometer pin as input
  pinMode(LED_PIN, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
  
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Serial communication initialized.");
  
  // Read initial frequency from the potentiometer
  double initialFrequency = readPotentiometer();
  Serial.print("Initial frequency: ");
  Serial.print(initialFrequency);
  Serial.println(" Hz");
  
  // Start timer with the initial frequency
  startTimer(initialFrequency);
  Serial.println("Timer started.");
}

void loop()
{ 
  // Continuously read the potentiometer value and update timer frequency
  double frequency = readPotentiometer();
  Serial.print("Current frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");
  
  startTimer(frequency);
  
  // Delay for 2 seconds
  delay(2000);
}

// Function to configure and start the timer with a specified frequency
void startTimer(double timerFrequency){
  // Disable interrupts for configuring timer registers
  noInterrupts();
  
  // Calculate OCR1A value (Output Compare Register A)
  unsigned long ocrValue = F_CPU / (2 * 1024 * timerFrequency) - 1; // Assuming prescaler is set to 1024
  
  // Set OCR1A value
  OCR1A = ocrValue;
  
  // Configure Timer1 for CTC (Clear Timer on Compare Match) mode
  TCCR1A = 0; // Clear Timer1 control register A
  TCCR1B = (1 << WGM12); // Set WGM12 bit for CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  
  // Enable Timer1 compare match A interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  // Re-enable interrupts
  interrupts();
}

// Interrupt service routine for handling timer compare match event
ISR(TIMER1_COMPA_vect){
   // Toggle LED pin state
   digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
   Serial.println("LED toggled.");
}

// Function to read potentiometer value and map it to a frequency range
double readPotentiometer() {
  // Read analog value from the potentiometer
  int potValue = analogRead(POTENTIOMETER_PIN);
  
  // Map potentiometer value to a frequency range
  double frequency = map(potValue, 0, 1023, 0.1, 10.0); // Example range: 0.1 Hz to 10 Hz
  
  // Return mapped frequency
  return frequency;
}
