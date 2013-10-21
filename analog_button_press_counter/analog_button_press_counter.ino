#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

const int SENSOR_PIN = 0; 
const int LOW_HIGH_BORDER = 300;

int counter;
int value;
boolean high;
boolean was_high;

void setup()
{
  lcd.begin(16, 2);
  counter = 0;
  high = false;
  was_high = false;
}

void loop()
{  
  value = analogRead(SENSOR_PIN);
  
  if (value > LOW_HIGH_BORDER) {
    high = true;
  } else {
    high = false;
  }
  
  if (high && !was_high) {
    counter += 1;
    was_high = true;
  } else if (!high && was_high) {
    was_high = false;
  }

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("GO!!! ");
  lcd.print(value);

  lcd.setCursor(0,1);
  lcd.print("#turns: ");
  lcd.print(counter);

  delay(20);
}

