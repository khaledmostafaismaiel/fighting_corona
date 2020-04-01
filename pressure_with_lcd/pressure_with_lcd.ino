#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "Arduino.h"
#include <SoftwareSerial.h>


#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 11
#define D5_PIN 10
#define D6_PIN 9
#define D7_PIN 8
#define NUMBER_OF_ROWS 2
#define NUMBER_OF_COLUMNS 16

LiquidCrystal lcd_screen(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);//RS,EN,D4,D5,D6,D7


#define LED_PIN
#define BUZZER_PIN
#define KNOB_PIN



Adafruit_BMP085 bmp;

char PRESSURESHOW[4];// initializing a character of size 4 for showing the result
char TEMPARATURESHOW[4];// initializing a character of size 4 for showing the temparature result

SoftwareSerial BTserial(4, 6); // RX , TX

void setup() {
  
  lcd_screen.begin(NUMBER_OF_COLUMNS, NUMBER_OF_ROWS);

  lcd_screen.print("Fighting Corona");
  
  lcd_screen.setCursor(0, 1);
  
  lcd_screen.print("Temp. & Pressure");
  
  lcd_screen.setCursor(0, 2);
  
  delay (3000);

  lcd_screen.clear();//clear display
  
  Serial.begin(9600);
  BTserial.begin(9600);

  if (!bmp.begin())
  {
    
    Serial.println("ERROR IN COMMUNICATION BETWEEN BMP_SENSOR AND ARDUINO_BOARD");///if there is an error in communication
    
    while (1) {
    
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);   
      
    }
      
  }
  
}


void loop()
{

  lcd_screen.print("Pressure= ");
  
  String PRESSUREVALUE = String(bmp.readPressure());
  PRESSUREVALUE.toCharArray(PRESSURESHOW, 4);
  
  lcd_screen.print(PRESSURESHOW);
  lcd_screen.print("hPa ");
  
  lcd_screen.setCursor(0, 1);


  
  lcd_screen.print("Temparature=");// print name
  
  String TEMPARATUREVALUE = String(bmp.readTemperature());
  TEMPARATUREVALUE.toCharArray(TEMPARATURESHOW, 4);

  lcd_screen.print(TEMPARATURESHOW);
  lcd_screen.print("C ");
  
  lcd_screen.setCursor(0, 0);//set the cursor to column 0, line1
  
  delay(500);

  //BTserial.write(PRESSURESHOW);
  BTserial.write(TEMPARATURESHOW);

}
