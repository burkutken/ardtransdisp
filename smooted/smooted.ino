#include <SPI.h>
#include <Wire.h>


// Define potentiometer pins for the joysticks
#define J_LEFT_POTX A2   // Left joystick X-axis (Throttle)
#define J_LEFT_POTY A3   // Left joystick Y-axis (Yaw)
#define J_RIGHT_POTX A0  // Right joystick X-axis (Roll)
#define J_RIGHT_POTY A1  // Right joystick Y-axis (Pitch)

// Joystick variables
int throttle = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

// Define smoothing parameters
const int SMOOTHING_FACTOR = 5; // Number of samples for averaging
int throttleSamples[SMOOTHING_FACTOR] = {0};
int yawSamples[SMOOTHING_FACTOR] = {0};
int rollSamples[SMOOTHING_FACTOR] = {0};
int pitchSamples[SMOOTHING_FACTOR] = {0};

// Deadband threshold
const int DEAD_BAND = 5;

// Function to calculate the moving average
int movingAverage(int *samples, int newValue) {
  for (int i = 0; i < SMOOTHING_FACTOR - 1; i++) {
    samples[i] = samples[i + 1];
  }
  samples[SMOOTHING_FACTOR - 1] = newValue;

  int sum = 0;
  for (int i = 0; i < SMOOTHING_FACTOR; i++) {
    sum += samples[i];
  }
  return sum / SMOOTHING_FACTOR;
}

// Function to apply a deadband
int applyDeadband(int value, int center, int threshold) {
  if (abs(value - center) <= threshold) {
    return center;
  }
  return value;
}

void setup() {
  Serial.begin(19200);         // Start Serial Communication
  pinMode(LED_BUILTIN, OUTPUT); // Configure LED pin

  // Initialize joystick pins
  pinMode(J_LEFT_POTX, INPUT);
  pinMode(J_LEFT_POTY, INPUT);
  pinMode(J_RIGHT_POTX, INPUT);
  pinMode(J_RIGHT_POTY, INPUT);
}

void loop() {
  // Read analog joystick values
  int rawThrottle = analogRead(J_LEFT_POTX);
  int rawYaw = analogRead(J_LEFT_POTY);
  int rawRoll = analogRead(J_RIGHT_POTY);
  int rawPitch = analogRead(J_RIGHT_POTX);

  // Apply smoothing
  throttle = movingAverage(throttleSamples, map(rawThrottle, 0, 1023, 0, 255));
  yaw = movingAverage(yawSamples, map(rawYaw, 0, 1023, 0, 255));
  roll = movingAverage(rollSamples, map(rawRoll, 0, 1023, 0, 255));
  pitch = movingAverage(pitchSamples, map(rawPitch, 0, 1023, 0, 255));

  // Apply deadband
  yaw = applyDeadband(yaw, 127, DEAD_BAND);
  roll = applyDeadband(roll, 127, DEAD_BAND);
  pitch = applyDeadband(pitch, 127, DEAD_BAND);

  // Output the smoothed and processed values
  Serial.print("Throttle = ");
  Serial.print(throttle);
  Serial.print("  Yaw = ");
  Serial.print(yaw);
  Serial.print("  Roll = ");
  Serial.print(roll);
  Serial.print("  Pitch = ");
  Serial.println(pitch);

  // Simulate control signals (for debugging, replace with your drone's communication protocol)
  delay(100);
}
