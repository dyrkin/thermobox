#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>


#define DEC_BTN_PIN 52
#define INC_BTN_PIN 53
#define DHT_PIN 39
#define DHTTYPE DHT11

Bounce decBtn = Bounce();
Bounce incBtn = Bounce();
DHT dht(DHT_PIN, DHTTYPE);

LiquidCrystal lcd(12, 11, 45, 44, 43, 42);

int temp = 20;

void setup() {
  decBtn.attach(DEC_BTN_PIN, INPUT_PULLUP);
  decBtn.interval(25);
  incBtn.attach(INC_BTN_PIN, INPUT_PULLUP);
  incBtn.interval(25);
  lcd.begin(16, 2);
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  decBtn.update();
  incBtn.update();

  if (incBtn.rose()) {
    temp++;
    Serial.println(temp);
  } else if (decBtn.rose()) {
    temp--;
    Serial.println(temp);
  }
  lcd.setCursor(2, 0);
  lcd.print(temp);

  float currentTemp = dht.readTemperature();
  if (!isnan(currentTemp)) {
    lcd.setCursor(2, 1);
    lcd.print(currentTemp);
  }

}
