#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>


#define DEC_BTN_PIN 52
#define INC_BTN_PIN 53
#define DHT_PIN 39
#define SWITCH_PIN A0
#define DHT_TYPE DHT11

Bounce decBtn = Bounce();
Bounce incBtn = Bounce();
DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal lcd(12, 11, 45, 44, 43, 42);

int keepTemp = 20;
float sensorTemp;

void setup() {
  decBtn.attach(DEC_BTN_PIN, INPUT_PULLUP);
  decBtn.interval(25);
  incBtn.attach(INC_BTN_PIN, INPUT_PULLUP);
  incBtn.interval(25);
  lcd.begin(16, 2);
  lcd.setCursor(2, 0);
  lcd.print(keepTemp);
  dht.begin();
  Serial.begin(9600);
  pinMode(SWITCH_PIN, OUTPUT);
}

void loop() {
  decBtn.update();
  incBtn.update();
  bool keepTempChanged = false;
  bool sensorTempChanged = false;

  if (incBtn.rose()) {
    keepTemp++;
    keepTempChanged = true;
  } else if (decBtn.rose()) {
    keepTemp--;
    keepTempChanged = true;
  }

  if (keepTempChanged) {
    Serial.println(keepTemp);
    lcd.setCursor(2, 0);
    lcd.print(keepTemp);
  }

  float currentTemp = dht.readTemperature();
  sensorTempChanged = currentTemp != sensorTemp;
  if (!isnan(currentTemp) && sensorTempChanged) {
    sensorTemp = currentTemp;
    lcd.setCursor(2, 1);
    lcd.print(currentTemp);
  }

  if (sensorTemp < keepTemp) {
    digitalWrite(SWITCH_PIN, LOW);
  } else {
    digitalWrite(SWITCH_PIN, HIGH);
  }
}