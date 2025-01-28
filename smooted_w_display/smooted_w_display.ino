#include <U8glib.h>

// Initialize the OLED display
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK); // Use U8G_I2C_OPT_NONE if needed

// Define joystick pins
#define J_LEFT_POTX A2   // Throttle
#define J_LEFT_POTY A3   // Yaw
#define J_RIGHT_POTX A0  // Pitch
#define J_RIGHT_POTY A1  // Roll

// Joystick variables
int throttle = 0;
int yaw = 0;
int roll = 0;
int pitch = 0;

// Smoothing parameters
const int SMOOTHING_FACTOR = 5;
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

// Function to display the opening screen
void displayOpeningScreen() {
  unsigned long startTime = millis();
  
  // Loop for 3 seconds
  while (millis() - startTime < 3000) {
    u8g.firstPage();
    do {
      // Display "Technwave" in the center of the screen
      u8g.setFont(u8g_font_fur17); // Larger font for opening screen
      u8g.setPrintPos((128 - u8g.getStrWidth("technwave")) / 2, 32); // Center the text
      u8g.print("technwave");
    } while (u8g.nextPage());
  }
}

// Function to display joystick values on the OLED
void displayJoystickValues() {
  u8g.firstPage();
  do {
    // Draw titles and values on the screen
    u8g.setFont(u8g_font_6x10); // Set font size
    u8g.drawStr(0, 10, "Joystick Values:");
    
    u8g.setPrintPos(0, 25);
    u8g.print("Throttle: ");
    u8g.print(throttle);

    u8g.setPrintPos(0, 35);
    u8g.print("Yaw: ");
    u8g.print(yaw);

    u8g.setPrintPos(0, 45);
    u8g.print("Pitch: ");
    u8g.print(pitch);

    u8g.setPrintPos(0, 55);
    u8g.print("Roll: ");
    u8g.print(roll);
  } while (u8g.nextPage());
}

void setup() {
  // Initialize the OLED display
  u8g.setFont(u8g_font_6x10);
  u8g.setColorIndex(1); // Set color to black on white
  
  // Display the opening screen
  displayOpeningScreen();

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

  // Display joystick values on the OLED
  displayJoystickValues();

  // Delay for stability
  delay(100);
}
