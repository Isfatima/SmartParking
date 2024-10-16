// Arduino Car Parking System

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <LedControl.h>

// LCD configuration
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change the HEX address

// Servo configuration
Servo myservo1;

// IR sensors
int IR1 = 2;
int IR2 = 4;

// Number of parking slots
int Slot = 6;  // Enter Total number of parking Slots

// Flags
int flag1 = 0;
int flag2 = 0;

// LED pins
const int ledPin1 = 6;
const int ledPin2 = 8;
const int ledPin3 = 9;
const int ledPin4 = 10;
const int ledPin5 = 11;
const int ledPin6 = 12;
const int ledPin7 = 13;
const int ldrPin = A0;  // The number of the LDR pin

// MAX7219 configuration
const int DIN_PIN = 7;
const int CS_PIN = 1;
const int CLK_PIN = 5;

// Create an instance of the LedControl library
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// Define the bitmaps for the arrows
byte leftArrow[8] = {B00010000,B00110000,B01110000,B11111111,B11111111,B01110000,B00110000,B00010000};

byte rightArrow[8] = {B00001000,B00001100,B00001110,B11111111,B11111111,B00001110,B00001100,B00001000};

byte upArrow[8] = {B00011000,B00111100,B01111110,B11111111,B00011000,B00011000,B00011000,B00011000};

void setup() 
{
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize pins
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  
  // Initialize servo
  myservo1.attach(3);
  myservo1.write(100);

  // Display startup message on LCD
  lcd.setCursor(0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();

  // Initialize serial communication
  //Serial.begin(9600);

  // Initialize LED pins
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ldrPin, INPUT);

  // Initialize the MAX7219
  lc.shutdown(0, false);  // Wake up the display
  lc.setIntensity(0, 8);  // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);     // Clear the display register
}

void loop() 
{ 
  // Read the status of the LDR value
  int ldrStatus = analogRead(ldrPin);
  leds(ldrStatus);
  
  // Check IR1 sensor
  if(digitalRead(IR1) == LOW && flag1 == 0) 
  {
    if(Slot > 0) {
      flag1 = 1;
      if(flag2 == 0) {
        myservo1.write(0); 
        Slot = Slot - 1;
        //showArrows();  // Show arrows when the motor is activated
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    SORRY :(    ");  
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  "); 
      delay(3000);
      lcd.clear(); 
    }
  }

  // Reset flags and close servo
  if(flag1 == 1 && flag2 == 1) 
  {
    delay(1000);
    myservo1.write(100);
    //showArrows();
     flag1 = 0;
    flag2 = 0;
  }

  // Check IR2 sensor
  if(digitalRead(IR2) == LOW && flag2 == 0) 
  {
    flag2 = 1;
    if(flag1 == 0)
    {
      myservo1.write(0); 
      Slot = Slot + 1;
    }
  }

  // Reset flags and close servo
  if(flag1 == 1 && flag2 == 1) 
  {
    delay(1000);
    myservo1.write(100);
    showArrows();
    flag1 = 0;
    flag2 = 0;
  }

  // Display remaining slots on LCD
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor(0, 1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);

  // Read the status of the LDR value
//  int ldrStatus = analogRead(ldrPin);
  leds(ldrStatus);
  
}

void showArrows() 
{
  lc.shutdown(0, false); // Power on the MAX7219

  // Display left arrow
  displayArrow(leftArrow);
  delay(1000);  // Display for 1 second

  // Display right arrow
  displayArrow(rightArrow);
  delay(1000);  // Display for 1 second

  // Display up arrow
  displayArrow(upArrow);
  delay(1000);  // Display for 1 second

  // Clear the display
  lc.clearDisplay(0);
  lc.shutdown(0, true); // Power off the MAX7219
}

void displayArrow(byte arrow[])
{
  for (int row = 0; row < 8; row++) 
  {
    lc.setRow(0, row, arrow[row]);
  }
} 

void leds(int ldrStatus)
{
  // Control LEDs based on LDR status
  if (ldrStatus <= 30) 
  {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledPin5, HIGH);
    digitalWrite(ledPin6, HIGH);
    digitalWrite(ledPin7, HIGH);
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, LOW);
    digitalWrite(ledPin6, LOW);
    digitalWrite(ledPin7, LOW);
  }
}

Ultrasonic Code:
#include "SR04.h"
 
// Define pins for three ultrasonic sensors
#define TRIG_PIN1 12
#define ECHO_PIN1 11
#define TRIG_PIN2 10
#define ECHO_PIN2 9
#define TRIG_PIN3 8
#define ECHO_PIN3 7
// Initialize SR04 objects for each sensor
SR04 sr04_1 = SR04(ECHO_PIN1, TRIG_PIN1);
SR04 sr04_2 = SR04(ECHO_PIN2, TRIG_PIN2);
SR04 sr04_3 = SR04(ECHO_PIN3, TRIG_PIN3);
 
const int buzzerPin = 6; // Buzzer pin
const int thresholdDistance = 3; // Threshold distance in cm

const int flamePin = 5;
int Flame = HIGH;
int redled = 4;
void setup() {
   Serial.begin(9600);
   delay(1000);
   pinMode(buzzerPin, OUTPUT);

  pinMode (buzzerPin, OUTPUT);
  pinMode (redled, OUTPUT);
 
  pinMode (flamePin, INPUT);
  Serial.begin(9600);
}
void loop() {
  
   long distance1 = sr04_1.Distance();
   long distance2 = sr04_2.Distance();
   long distance3 = sr04_3.Distance();
   Serial.print("Distance 1: ");
   Serial.print(distance1);
   Serial.println(" cm");
 
   Serial.print("Distance 2: ");
   Serial.print(distance2);
   Serial.println(" cm");
 
   Serial.print("Distance 3: ");
   Serial.print(distance3);
   Serial.println(" cm");
   // Check if any sensor detects an object within the threshold distance
   if (distance1 < thresholdDistance || distance2 < thresholdDistance || distance3 < thresholdDistance) {
      digitalWrite(buzzerPin, HIGH); // Turn on buzzer
   } else {
      digitalWrite(buzzerPin, LOW); // Turn off buzzer
   }
  delay(1000);
  Flame = digitalRead(flamePin);
  if(Flame == LOW)
  {
    digitalWrite (buzzerPin, HIGH);
    digitalWrite(redled, HIGH);
    delay(2000);
  }
  else
  {
    digitalWrite (buzzerPin, LOW);
    digitalWrite(redled, LOW);
  }
}
