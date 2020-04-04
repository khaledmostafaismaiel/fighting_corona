#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal.h>



            //preproccesor section
            
#define MIN_NUM_OF_BREATHS 10
#define MAX_NUM_OF_BREATHS 20

#define MIN_VOL_PER_BREATH 1
#define MAX_VOL_PER_BREATH 5

#define STEPPER_MOTOR_STEP_PIN 2 
#define STEPPER_MOTOR_DIR_PIN 3 

#define LED_PIN 4
#define BUZZER_PIN 5

#define MIN_END_STOP_PIN A0
#define MAX_END_STOP_PIN A1

#define select_BUTTON_PIN A2

#define POTENTIOMETER_PIN A3


#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 11
#define D5_PIN 10
#define D6_PIN 9
#define D7_PIN 8
#define NUMBER_OF_ROWS 2
#define NUMBER_OF_COLUMNS 16



            //global variable section
            
LiquidCrystal lcd_screen(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN); //creating lcd_screen variable RS,EN,D4,D5,D6,D7

Adafruit_BMP085 bmp;  //creating sensor pressure variable


            //methods prototypes
            
void stepper_move(int dir_pin , int step_pin , char dir ,double delay_time);

int number_of_breaths = 0 ;
int volume_of_oxygen = 0 ;
char PRESSURESHOW[4]; // initializing a character of size 4 for showing the result
char TEMPARATURESHOW[4];  // initializing a character of size 4 for showing the temparature result
double delay_between_steps = 0 ;
double delay_between_Inhalation_exhalation = 0;

void setup() {
  
  lcd_screen.begin(NUMBER_OF_COLUMNS, NUMBER_OF_ROWS);  //init lcd_screen


  //init bmp pressure sensor
  if (!bmp.begin())
  {
    Serial.println("ERROR IN COMMUNICATION BETWEEN BMP_SENSOR AND ARDUINO_BOARD");///if there is an error in communication
    while (1) {
      //if init didnt work correctly flash led and buzzer will turn on and off for ever
      digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);                       // wait for a second
      digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);   
    }
  }
  
  pinMode(STEPPER_MOTOR_STEP_PIN , OUTPUT); //init step_pin for stepper motor
  pinMode(STEPPER_MOTOR_DIR_PIN , OUTPUT); //init dir_pin for stepper motor

  pinMode(LED_PIN , OUTPUT);  //init for led
  pinMode(BUZZER_PIN , OUTPUT); //init for buzzer

  pinMode(MIN_END_STOP_PIN , INPUT); //init for MIN_END_STOP_PIN
  pinMode(MAX_END_STOP_PIN , INPUT); //init for MAX_END_STOP_PIN
  pinMode(select_BUTTON_PIN , INPUT); //init for select_BUTTON_PIN
  pinMode(POTENTIOMETER_PIN , INPUT); //init for POTENTIOMETER_PIN


  //ask user about number of breaths per minute
  while(digitalRead(select_BUTTON_PIN) == LOW){
    number_of_breaths = analogRead(POTENTIOMETER_PIN);
    number_of_breaths = map(potentiometer_value, 0, 1023, MIN_NUM_OF_BREATHS,MAX_NUM_OF_BREATHS); //maping the potentiometer value with MIN_NUM_OF_BREATHS,MAX_NUM_OF_BREATHS
    
    lcd.setCursor(0, 0); // top left
    lcd_screen.print("number of breaths\t",number_of_breaths);
    delay (100);
    lcd_screen.clear();//clear display
  }
  
  delay(500); // to prevnt interruption between the last select_BUTTON_PIN status and the next select_BUTTON_PIN status

  //ask user about volume of oxygen per breath
  while(digitalRead(select_BUTTON_PIN) == LOW){
    volume_of_oxygen = analogRead(POTENTIOMETER_PIN);
    volume_of_oxygen = map(potentiometer_value, 0, 1023, MIN_VOL_PER_BREATH,MAX_VOL_PER_BREATH);  //maping the potentiometer value with MIN_VOL_PER_BREATH,MAX_VOL_PER_BREATH
      
    lcd.setCursor(0, 0); // top left
    lcd_screen.print("volume of oxygen\t",volume_of_oxygen);
    delay (100);
    lcd_screen.clear();//clear display
  }



/*
now we should use number_of_breaths and volume_of_oxygen to help us to find the values of delay_between_steps and delay_between_Inhalation_exhalation
*/


}




void loop()
{

/*
احنا فاللوب بتاع مش محتاجين حاجه غير ان الماتور يعمل الشهيق والزفير ويستنى بينهم وقت معين ويفضل بيعرض قيمه الضغط ودرجه الحراره على الشاشه
*/



/*
شهيق

وبيعرض الضغط ودرجه الحراره على الشاشه

لو حصل والمريض كح الجهاز هيوقف ويشتغل بعد م يبطل

*/



/*

delay

وبيعرض الضغط ودرجه الحراره على الشاشه

*/



/*
زفير

وبيعرض الضغط ودرجه الحراره على الشاشه

لو حصل والمريض كح الجهاز هيوقف ويشتغل بعد م يبطل

*/


/*

delay

وبيعرض الضغط ودرجه الحراره على الشاشه

*/



}






void stepper_move(int dir_pin , int step_pin , char dir ,double delay_time){


  switch(dir_pin){

    case 'U':  digitalWrite(dir_pin,HIGH);
    
              digitalWrite(step_pin,HIGH);
              delayMicroseconds(delay_time);
              digitalWrite(step_pin,LOW);
              delayMicroseconds(delay_time);
              
              break;

    case 'D':  digitalWrite(dir_pin,LOW);
            
              digitalWrite(step_pin,HIGH);
              digitalWrite(step_pin,LOW);
              delayMicroseconds(delay_time);

              break; 
               
    default: break;
  
  }


}
