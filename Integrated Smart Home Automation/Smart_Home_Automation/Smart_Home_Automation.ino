#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <ESP32Servo.h>

// Wi-Fi / ThingSpeak
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WiFiClient client;
unsigned long channelID = 3348508;
const char *writeAPIKey = "M0BEO6JAG7SCH7KT";

// Pin definitions
const int LDR_PIN = 34;
const int PIR_PIN = 5;
const int LIGHT_LED_PIN = 2;
const int CLIMATE_LED_PIN = 18;
const int BUZZER_PIN = 4;
const int DHT_PIN = 15;
const int DOOR_BUTTON_PIN = 27;
const int SERVO_PIN = 32;
const int MODE_BUTTON_PIN = 25;

// Sensor / actuator objects
DHT dht(DHT_PIN, DHT22);
Servo doorServo;

// System thresholds
const int lightThreshold = 2000;
const float temperatureThreshold = 30.0;

// Timing constants
const unsigned long modeDebounceMs = 250;
const unsigned long doorDebounceMs = 250;
const unsigned long doorOpenDurationMs = 3000;
const unsigned long sensorReadIntervalMs = 500;
const unsigned long serialIntervalMs = 1000;
const unsigned long thingSpeakIntervalMs = 20000;
const unsigned long normalBuzzOnMs = 300;
const unsigned long normalBuzzOffMs = 900;
const unsigned long securityBuzzOnMs = 100;
const unsigned long securityBuzzOffMs = 100;

// Global state
bool securityMode = false;
bool lastModeButtonState = HIGH;
bool lastDoorButtonState = HIGH;
bool doorIsOpen = false;
bool buzzerState = false;
bool motionDetected = false;

unsigned long lastModeToggleMs = 0;
unsigned long lastDoorPressMs = 0;
unsigned long doorOpenedAtMs = 0;
unsigned long lastSensorReadMs = 0;
unsigned long lastSerialMs = 0;
unsigned long lastThingSpeakMs = 0;
unsigned long lastBuzzToggleMs = 0;

int lightValue = 0;
float temperatureValue = NAN;
float humidityValue = NAN;

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

void updateModeButton(unsigned long now) {
  bool currentState = digitalRead(MODE_BUTTON_PIN);

  if (lastModeButtonState == HIGH && currentState == LOW && now - lastModeToggleMs >= modeDebounceMs) {
    securityMode = !securityMode;
    lastModeToggleMs = now;

    Serial.print("Mode changed to: ");
    Serial.println(securityMode ? "SECURITY" : "NORMAL");
  }

  lastModeButtonState = currentState;
}

void updateDoorButton(unsigned long now) {
  bool currentState = digitalRead(DOOR_BUTTON_PIN);

  if (lastDoorButtonState == HIGH && currentState == LOW && now - lastDoorPressMs >= doorDebounceMs) {
    lastDoorPressMs = now;
    doorOpenedAtMs = now;
    doorIsOpen = true;
    doorServo.write(90);
    Serial.println("Door opening");
  }

  lastDoorButtonState = currentState;
}

void updateDoorAutoClose(unsigned long now) {
  if (doorIsOpen && now - doorOpenedAtMs >= doorOpenDurationMs) {
    doorServo.write(0);
    doorIsOpen = false;
    Serial.println("Door closing");
  }
}

void readSensors(unsigned long now) {
  if (now - lastSensorReadMs < sensorReadIntervalMs) {
    return;
  }

  lastSensorReadMs = now;
  lightValue = analogRead(LDR_PIN);
  temperatureValue = dht.readTemperature();
  humidityValue = dht.readHumidity();
  motionDetected = digitalRead(PIR_PIN) == HIGH;

  if (isnan(temperatureValue) || isnan(humidityValue)) {
    Serial.println("DHT sensor read failed");
  }
}

void updateLighting() {
  digitalWrite(LIGHT_LED_PIN, lightValue < lightThreshold ? HIGH : LOW);
}

void updateClimateControl() {
  bool climateActive = !isnan(temperatureValue) && temperatureValue > temperatureThreshold;
  digitalWrite(CLIMATE_LED_PIN, climateActive ? HIGH : LOW);
}

void updateBuzzer(unsigned long now) {
  unsigned long onTime = securityMode ? securityBuzzOnMs : normalBuzzOnMs;
  unsigned long offTime = securityMode ? securityBuzzOffMs : normalBuzzOffMs;

  if (!motionDetected) {
    buzzerState = false;
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  unsigned long interval = buzzerState ? onTime : offTime;
  if (now - lastBuzzToggleMs >= interval) {
    buzzerState = !buzzerState;
    lastBuzzToggleMs = now;
    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
  }
}

void printStatus(unsigned long now) {
  if (now - lastSerialMs < serialIntervalMs) {
    return;
  }

  lastSerialMs = now;
  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print(" | Temp: ");
  Serial.print(temperatureValue);
  Serial.print(" C | Humidity: ");
  Serial.print(humidityValue);
  Serial.print(" % | Motion: ");
  Serial.print(motionDetected ? 1 : 0);
  Serial.print(" | Mode: ");
  Serial.print(securityMode ? "SECURITY" : "NORMAL");
  Serial.print(" | Door: ");
  Serial.println(doorIsOpen ? "OPEN" : "CLOSED");
}

void uploadToThingSpeak(unsigned long now) {
  if (now - lastThingSpeakMs < thingSpeakIntervalMs) {
    return;
  }

  lastThingSpeakMs = now;

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  ThingSpeak.setField(1, temperatureValue);
  ThingSpeak.setField(2, humidityValue);
  ThingSpeak.setField(3, lightValue);
  ThingSpeak.setField(4, motionDetected ? 1 : 0);
  ThingSpeak.setField(5, securityMode ? 1 : 0);

  int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);
  Serial.print("ThingSpeak status: ");
  Serial.println(statusCode);
}

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LIGHT_LED_PIN, OUTPUT);
  pinMode(CLIMATE_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DOOR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);

  dht.begin();
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);

  connectWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  unsigned long now = millis();

  updateModeButton(now);
  updateDoorButton(now);
  updateDoorAutoClose(now);
  readSensors(now);
  updateLighting();
  updateClimateControl();
  updateBuzzer(now);
  printStatus(now);
  uploadToThingSpeak(now);
}
