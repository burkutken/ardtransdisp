#include <SPI.h> //not needed

#define J_LEFT_POTX A2    // Left joystick X-axis (Throttle)
#define J_LEFT_POTY A3    // Left joystick Y-axis (Yaw)
#define J_RIGHT_POTX A0   // Right joystick X-axis (Roll)
#define J_RIGHT_POTY A1   // Right joystick Y-axis (Pitch)

int throttle = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

//this is without display

void initial_pos () { 

  throttle = 0;
  yaw      = 127;
  roll     = 127;
  pitch    = 127;

}

bool arePotsSetAsWanted() {
  return throttle < 35 && yaw < 35 && roll < 35 && pitch < 35;
}

// Function to start serial communication and handle LED behavior
void start_serial() {
  Serial.begin(19200);         // Start Serial Communication
  pinMode(LED_BUILTIN, OUTPUT); // Configure LED pin
  
  //if u want u can delete here, its just for configuration with your joysticks to starting up
  if (arePotsSetAsWanted()) {
    digitalWrite(LED_BUILTIN, LOW);  // Turn off LED if pots are correctly set
    Serial.println("Serial started successfully. Joysticks are in the default position.");
  } else {
    Serial.println("Joysticks are not in the default position. Adjust them!");
    // Blink LED to indicate error
    for (int i = 0; i < 10; i++) { // Blink 10 times
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
}


void setup() {

  pinMode(J_LEFT_POTX, INPUT);
  pinMode(J_LEFT_POTY, INPUT);
  pinMode(J_RIGHT_POTX, INPUT);
  pinMode(J_RIGHT_POTY, INPUT);

  initial_pos();
  start_serial();
  
}

void loop() {
  throttle = map(analogRead(J_LEFT_POTX), 0, 1023, 0, 255);
  yaw = map(analogRead(J_LEFT_POTY), 0, 1023, 0, 255);
  roll = map(analogRead(J_RIGHT_POTY), 0, 1023, 0, 255);
  pitch = map(analogRead(J_RIGHT_POTX), 0, 1023, 0, 255);
  
  Serial.print("Throttle = ");
  Serial.print(throttle);
  Serial.print("  Yaw = ");
  Serial.print(yaw);
  Serial.print("  Pitch = ");
  Serial.print(pitch);
  Serial.print("  Roll = ");
  Serial.println(roll);
  delay(100);
}
