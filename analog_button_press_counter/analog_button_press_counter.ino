#include <Time.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);

const int SENSOR_PIN = 0; 
const int LOW_HIGH_BORDER = 100;
const int ITERATIONS_TO_KEEP_DISPLAYING_POWER = 5;

int counter;
int iterationOfShowingPower;
int valueToShow;
boolean high;
boolean was_high;
boolean started;
unsigned long time[2] = {0,0};
volatile unsigned long per = 0;
float frequency;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  
  Serial.begin(9600);
}

void loop() {  
  int value;
  char buffer[16];

  value = analogRead(SENSOR_PIN);
  
  if (value > LOW_HIGH_BORDER) {
    if (value > valueToShow) {
      valueToShow = value;
    }
    high = true;
    iterationOfShowingPower = 0;
  } else {
    iterationOfShowingPower++;
    if (iterationOfShowingPower >= ITERATIONS_TO_KEEP_DISPLAYING_POWER) {
      valueToShow = 0;
    }
    high = false;
  }
  
  if (high && !was_high) {
    if (!started) {
      setTime(0,0,0,0,0,0);
      started = true;
    }

    time[1] = millis();
    per = time[1] - time[0];
    frequency = 1.0 / per*1000*60;
    time[0] = time[1];

    valueToShow = value;
    counter += 1;
    was_high = true;
  } else if (!high && was_high) {
    was_high = false;
  }

  //The display is going to show the following values:
  // Top left: Highest measurement in the last turn
  // Top right: Elapsed time
  // Bottom left: NUmber of turns
  // Bottom right: Turn Frequency

// Top Left  
  lcd.setCursor(0,0);
  lcd.print("p:");
  lcd.print(valueToShow);
  lcd.print("   ");

// Top Right
  lcd.setCursor(9,0);
  lcd.print("t");
  if (started) {
    printDigits(minute());
    printDigits(second());
  }

// Bottom Lift
  lcd.setCursor(0,1);
  lcd.print("t:");
  lcd.print(counter);
  lcd.print("  ");

// Bottom Right
  lcd.setCursor(11,1);
  lcd.print("f:");
  lcd.print(dtostrf(frequency,3,0,buffer));

  delay(40);
}

void printDigits(int digits){
  // utility function for clock display: prints preceding colon and leading 0
  lcd.print(":");
  if(digits < 10) lcd.print('0');
  lcd.print(digits);
}

