#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "IZZI-1947"
#define WIFI_PASSWORD "AcirK4HK"
#define FIREBASE_HOST "https://iotapp-5608d-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyAcQny59ysfpBkhQ3xGjyztIeDJcFo0G4o"

FirebaseData fbdo;

const int segmentPins[8] = {23, 16, 25, 27, 14, 17, 33, 26};  // A-G and DP
const int digitPins[8]   = {32, 15, 2, 4, 13, 5, 18, 19};     // Digit select

const byte digitSegments[10] = {
  B00111111, B00000110, B01011011, B01001111,
  B01100110, B01101101, B01111101, B00000111,
  B01111111, B01101111
};

int numero = 0;  // This will be updated by Firebase
unsigned long lastFirebaseRead = 0;
unsigned long lastDisplayRefresh = 0;
int currentDigit = 0;

// This is the ONLY function used in loop()
void displayNumber(int number) {
  static int digits[8];

  // Read Firebase every 1000 ms
  if (millis() - lastFirebaseRead >= 1000) {
    if (Firebase.getInt(fbdo, "/ESP32IoTAPP_DISPLAY/DISPLAY")) {
      numero = fbdo.intData();
      Serial.print("Firebase: ");
      Serial.println(numero);
    } else {
      Serial.print("Firebase error: ");
      Serial.println(fbdo.errorReason());
    }
    lastFirebaseRead = millis();
  }

  // Refresh one digit every 2 ms
  if (millis() - lastDisplayRefresh >= 2) {
    int temp = numero;
    for (int i = 7; i >= 0; i--) {
      digits[i] = temp % 10;
      temp /= 10;
    }

    for (int i = 0; i < 8; i++) digitalWrite(digitPins[i], HIGH);  // Turn off all

    byte segments = digitSegments[digits[7 - currentDigit]];
    for (int s = 0; s < 8; s++) {
      digitalWrite(segmentPins[s], (segments >> s) & 0x01);
    }

    digitalWrite(digitPins[currentDigit], LOW);  // Enable digit
    currentDigit = (currentDigit + 1) % 8;
    lastDisplayRefresh = millis();
  }
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(" connected!");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  displayNumber(numero);  // 💡 As requested: only this line
}
