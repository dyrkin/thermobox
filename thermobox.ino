#include <Bounce2.h>


#define DEC_BTN_PIN 52
#define INC_BTN_PIN 53

Bounce decBtn = Bounce();
Bounce incBtn = Bounce();

int temp = 20;

void setup() {
  decBtn.attach(DEC_BTN_PIN, INPUT_PULLUP);
  decBtn.interval(25);
  incBtn.attach(INC_BTN_PIN, INPUT_PULLUP);
  incBtn.interval(25);
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
}
