#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TF-Luna LiDAR Settings
HardwareSerial mySerial(1);  // Use UART1 for TF Luna
#define NUM_SAMPLES 5        // Number of samples for averaging
int distanceReadings[NUM_SAMPLES];
int distanceIndex = 0;       // Index for distance readings

// Sound Sensor Settings
#define SOUND_SENSOR_PIN 34  // MAX4466 connected to GPIO34 (ADC1)
int soundReadings[NUM_SAMPLES];
int soundIndex = 0;          // Index for sound readings

// Calibration values for sound sensor
const float minVoltage = 0.1;  // Voltage for quiet sound 
const float maxVoltage = 2.0;  // Voltage for loud sound (e.g., 90 dB)
const float minDB = 0.005;     // Minimum dB level (quiet sound)
const float maxDB = 90.0;      // Maximum dB level (loud sound)

// Global variable to hold the average distance
int avgDistance = 0;

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("OLED not found");
        while (1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Initialize TF-Luna LiDAR
    mySerial.begin(115200, SERIAL_8N1, 16, 17);

    // Initialize distance and sound readings arrays
    for (int i = 0; i < NUM_SAMPLES; i++) {
        distanceReadings[i] = 0;
        soundReadings[i] = 0;
    }
}

void loop() {
    // Read TF Luna LiDAR data
    if (mySerial.available() >= 9) {
        uint8_t data[9];
        mySerial.readBytes(data, 9);

        // Check if the data is valid (header bytes: 0x59 0x59)
        if (data[0] == 0x59 && data[1] == 0x59) {
            // Calculate distance from the data bytes
            int distance = data[2] + (data[3] << 8);

            // Store the distance in the circular buffer
            distanceReadings[distanceIndex] = distance;
            distanceIndex = (distanceIndex + 1) % NUM_SAMPLES;

            // Calculate the average distance
            int sumDistance = 0;
            for (int i = 0; i < NUM_SAMPLES; i++) {
                sumDistance += distanceReadings[i];
            }
            avgDistance = sumDistance / NUM_SAMPLES;  // Update global avgDistance

            // Print the average distance to the Serial Monitor
            Serial.print("Distance: ");
            Serial.print(avgDistance);
            Serial.println(" cm");
        }
    }

    // Read Sound Level
    int soundValue = analogRead(SOUND_SENSOR_PIN);
    float voltage = (soundValue / 4095.0) * 3.3;  

    // Map voltage to dB using calibration
    float dB = map(voltage, minVoltage, maxVoltage, minDB, maxDB);

    // Store sound readings in the circular buffer
    soundReadings[soundIndex] = dB;
    soundIndex = (soundIndex + 1) % NUM_SAMPLES;

    // Calculate the average sound level
    int sumSound = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sumSound += soundReadings[i];
    }
    float avgSound = sumSound / NUM_SAMPLES;

    // Print the average sound level to the Serial Monitor
    Serial.print("Sound Level: ");
    Serial.print(avgSound);
    Serial.println(" dB");

    // Clear the display and set the cursor for sound level
    display.clearDisplay();
    display.setCursor(10, 0);  // Set cursor to top
    display.print("Sound Level: ");
    display.print(avgSound);
    display.print(" dB");

    // Set the cursor for distance
    display.setCursor(10, 30);  // Set cursor to below sound level
    display.print("Distance: ");
    display.print(avgDistance);
    display.print(" cm");

    display.display();  // Update the display
}