#include <StopWatch.h>
#include <LiquidCrystal.h>

const int SENSOR_PIN = 0; 
const int LOW_HIGH_BORDER = 100;
const int ITERATIONS_TO_KEEP_DISPLAYING_POWER = 5;
const int ITERATIONS_TO_BLINK_ON = 15;
const int ITERATIONS_TO_BLINK_OFF = 8;
const int NO_ACTION_TIME_BEFORE_TIME_IS_STOPPED = 5000;

LiquidCrystal lcd(12,11,5,4,3,2);
StopWatch sw;    // MILLIS (default)
int counter;
int iterationOfShowingPower;
int iterationsOfBlinking;
int valueToShow;
boolean was_high;
boolean started;
boolean paused;
unsigned long time[2] = {0,0};
float frequency;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  
  Serial.begin(9600);
}

void loop() {  
  int value = analogRead(SENSOR_PIN);
  char buffer[16];
  boolean high;
  
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
      started = true;
      sw.start();
    }

    time[1] = sw.elapsed();
    frequency = 1.0 / (time[1] - time[0]) * 60000;
    time[0] = time[1];

    valueToShow = value;
    counter += 1;
    was_high = true;
  } else if (!high && was_high) {
    was_high = false;
  }
  
  if (sw.elapsed() - time[0] > NO_ACTION_TIME_BEFORE_TIME_IS_STOPPED) {
    if (!paused) {
      sw.stop();
      paused = true;
      iterationsOfBlinking = 1;
    }
  } else {
    sw.start();
    paused = false;    
  }
  
  if (paused) {
    if (iterationsOfBlinking > 0) {
      iterationsOfBlinking++;
      if (iterationsOfBlinking >= ITERATIONS_TO_BLINK_ON) {
        iterationsOfBlinking = -1;
      }      
    } else {
      iterationsOfBlinking--;
      if (iterationsOfBlinking <= -ITERATIONS_TO_BLINK_OFF) {
        iterationsOfBlinking = 1;
      }      
    }    
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
  lcd.print("t:");
  if (started) {
    if (paused && iterationsOfBlinking > 0) {
      lcd.print("     ");
    } else {
      printTime(sw.elapsed());
    }
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

void printTime(long time){
   long minutes = time / 60000.0;
   long seconds = time / 1000 % 60;

   lcd.print(zeroPaddedTimePart(minutes));
   lcd.print(":");
   lcd.print(zeroPaddedTimePart(seconds));
}

String zeroPaddedTimePart(long part) {
  String pad = "";
  if (part < 10) pad += ("0");
  return pad + part;
}

