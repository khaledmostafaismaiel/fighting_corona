#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal.h>



//preproccesor section

#define MIN_DELAY_BETWEEN_INHALE_AND_EXHALE 100
#define MAX_DELAY_BETWEEN_INHALE_AND_EXHALE 500

#define MIN_DELAY_BETWEEN_2_BREATHS 100
#define MAX_DELAY_BETWEEN_2_BREATHS 500

#define MIN_DELAY_BETWEEN_STEPS 100
#define MAX_DELAY_BETWEEN_STEPS 100

#define MIN_DELAY_BETWEEN_INHALE_AND_EXHALE 100
#define MAX_DELAY_BETWEEN_INHALE_AND_EXHALE 500

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



//methods prototypes

void stepper_move(int dir_pin , int step_pin , char dir , double delay_time);
void init_arm_position(void);
int get_target_number_of_steps();
int get_delay_between_steps(int target_steps);
int get_delay_between_Inhalation_exhalation(int target_steps, int steps_delay);
int get_delay_between_2_breaths(int target_steps, int steps_delay, int delay_after_inhale_exhale);
void test_movement(int target_steps, int steps_delay, int inhale_delay, int breath_breath_delay);
void welcome_message();

//global variable section

LiquidCrystal lcd_screen(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN); //creating lcd_screen variable RS,EN,D4,D5,D6,D7

Adafruit_BMP085 bmp;  //creating sensor pressure variable

int target_number_of_steps = 0;
int delay_between_steps = 0 ;
int delay_between_Inhalation_exhalation = 0;
int delay_between_2_breaths = 0 ;




void setup() {

  lcd_screen.begin(NUMBER_OF_COLUMNS, NUMBER_OF_ROWS);  //init lcd_screen

  bmp.begin(); //init bmp pressure sensor

  pinMode(STEPPER_MOTOR_STEP_PIN , OUTPUT); //init step_pin for stepper motor
  pinMode(STEPPER_MOTOR_DIR_PIN , OUTPUT); //init dir_pin for stepper motor

  pinMode(LED_PIN , OUTPUT);  //init for led
  pinMode(BUZZER_PIN , OUTPUT); //init for buzzer

  pinMode(MIN_END_STOP_PIN , INPUT); //init for MIN_END_STOP_PIN
  pinMode(MAX_END_STOP_PIN , INPUT); //init for MAX_END_STOP_PIN
  pinMode(select_BUTTON_PIN , INPUT); //init for select_BUTTON_PIN
  pinMode(POTENTIOMETER_PIN , INPUT); //init for POTENTIOMETER_PIN


  welcome_message();
  
  init_arm_position();  //init arm position

  delay(500); //to prevent conflict between potentiometer readings

  target_number_of_steps = get_target_number_of_steps();  //get target number of steps to detrmine the volume of breath

  delay(500); //to prevent conflict between potentiometer readings

  init_arm_position();  //init arm position

  delay(500); //to prevent conflict between potentiometer readings

  delay_between_steps = get_delay_between_steps(target_number_of_steps);  //get target delay between steps

  delay(500); //to prevent conflict between potentiometer readings

  init_arm_position();  //init arm position

  delay(500); //to prevent conflict between potentiometer readings

  delay_between_Inhalation_exhalation = get_delay_between_Inhalation_exhalation(target_number_of_steps, delay_between_steps);

  delay(500); //to prevent conflict between potentiometer readings

  init_arm_position();  //init arm position

  delay(500); //to prevent conflict between potentiometer readings

  get_delay_between_2_breaths(target_number_of_steps, delay_between_steps, delay_between_Inhalation_exhalation);

  delay(500); //to prevent conflict between potentiometer readings

  init_arm_position();  //init arm position

  delay(500); //to prevent conflict between potentiometer readings

  test_movement(target_number_of_steps, delay_between_steps, delay_between_Inhalation_exhalation, delay_between_2_breaths); //test the user values before patiant 3 times

  delay(500); //to prevent conflict between potentiometer readings

  init_arm_position();  //init arm position



}



void loop()
{




  for(int i = 0 ; i < target_number_of_steps ; ++i){
  
    stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'D', delay_between_steps);

    get_pressure();
      
    get_temp();

  }


  
  delay(delay_between_Inhalation_exhalation);




  for(int i = 0 ; i < target_number_of_steps ; ++i){
  
    stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'U', delay_between_steps);

      lcd_screen.setCursor(0, 0);//set the cursor to column 0, line1

      get_pressure();
      
      lcd_screen.setCursor(0, 1);

      get_temp();
      
  }



  
  delay(get_delay_between_2_breaths);
  


}





/*
*Function: stepper_move
*
*used to move stepper motor up and down which will control the arm
*
*return void
*/
void stepper_move(int dir_pin , int step_pin , char dir , double delay_time) {


  switch (dir) {

    case 'U':  digitalWrite(dir_pin, HIGH);

      digitalWrite(step_pin, HIGH);
      delayMicroseconds(delay_time);
      digitalWrite(step_pin, LOW);
      delayMicroseconds(delay_time);

      break;

    case 'D':  digitalWrite(dir_pin, LOW);

      digitalWrite(step_pin, HIGH);
      delayMicroseconds(delay_time);
      digitalWrite(step_pin, LOW);
      delayMicroseconds(delay_time);

      break;

    default: ;
      break;

  }
}




/*
*Function: init_arm_position
*
*used to init the arm to a certain position which will only accomplish when arm hit the min_end_stop 
*
*return void
*/
void init_arm_position() {


  /*
    #####please#####
    #####wait!######
  */
  lcd_screen.clear();//clear display
  lcd_screen.setCursor(5, 0); 
  lcd_screen.print("please");
  lcd_screen.setCursor(5, 1);
  lcd_screen.print("wait!");

  while (MIN_END_STOP_PIN == LOW) {
    stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'U', 500);

  }
  
    lcd_screen.clear();//clear display

}



/*
*Function: get_target_number_of_steps
*
*used to detrmine the number of steps which will make the desired volume of oxygen per breath from ambu 
*
*return current_postion
*/
int get_target_number_of_steps() {


  /*
    volume of oxygen
    ######10########
  */
  lcd_screen.clear();//clear display  
  lcd_screen.setCursor(0, 0); 
  lcd_screen.print("volume of oxygen");


  int current_postion = 0 ;
  int current_potentiometer_value = 0;
  int last_potentiometer_value = 0;

  while (digitalRead(select_BUTTON_PIN) == LOW) {
    
    current_potentiometer_value = analogRead(POTENTIOMETER_PIN);  
    
    lcd_screen.setCursor(6, 1);
    lcd_screen.print(String(current_potentiometer_value));
    
    if (current_potentiometer_value > last_potentiometer_value ) {
      if (MAX_END_STOP_PIN == LOW) {
        stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'D', 500);
        ++current_postion;
      }
    } else if (current_potentiometer_value < last_potentiometer_value) {
      if (MIN_END_STOP_PIN == LOW) {
        stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'U', 500);
        --current_postion;
      }
    } else {

    }
    last_potentiometer_value = current_potentiometer_value ;
  }


  lcd_screen.clear();//clear display

  return current_postion ;
}




/*
*Function: get_delay_between_steps
*
*used to detrmine the delay_between_steps which will control the speed of stepper motor which will influnce the speed of inhale and exhale
*
*return target_delay
*/
int get_delay_between_steps(int target_steps) {

  /*
    #####speed######
    #######10#######
  */
  lcd_screen.clear();//clear display
  lcd_screen.setCursor(5, 0); 
  lcd_screen.print("speed");

  
  
  int exit_condition = 0 ;
  int target_delay = 0 ;

  while (exit_condition == 0) {

    for (int i = 0 ; (i < target_steps) && (MAX_END_STOP_PIN == LOW); ++i) {

      target_delay = analogRead(POTENTIOMETER_PIN);
      target_delay = map(target_delay, 0, 1023, MIN_DELAY_BETWEEN_STEPS, MAX_DELAY_BETWEEN_STEPS); //maping the potentiometer value with MIN_DELAY_BETWEEN_STEPS,MAX_DELAY_BETWEEN_STEPS
      
      lcd_screen.setCursor(6, 1);
      lcd_screen.print(String(target_delay));
      
      stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'D', target_delay);

      if (digitalRead(select_BUTTON_PIN) != LOW) {
        exit_condition = 1 ;  //to break from while loop
        break;
      }

    }

    delay(200);

    init_arm_position();  //init arm position

  }

  lcd_screen.clear();//clear display

  return target_delay ;
}



/*
*Function: get_delay_between_Inhalation_exhalation
*
*used to detrmine the delay_between_Inhalation_exhalation
*
*return target_delay
*/
int get_delay_between_Inhalation_exhalation(int target_steps, int steps_delay) {


  /*
    after inhale####
    wait###10#######
  */
  lcd_screen.clear();//clear display
  lcd_screen.setCursor(5, 0); 
  lcd_screen.print("after inhale");

  
  int target_delay = 0 ;
  int exit_condition = 0 ;

  while (exit_condition == 0) {

    for (int i = 0 ; (i < target_steps) ; ++i) {

      target_delay = analogRead(POTENTIOMETER_PIN);
      target_delay = map(target_delay, 0, 1023, MIN_DELAY_BETWEEN_INHALE_AND_EXHALE, MAX_DELAY_BETWEEN_INHALE_AND_EXHALE); //maping the potentiometer value with MIN_DELAY_BETWEEN_INHALE_AND_EXHALE,MAX_DELAY_BETWEEN_INHALE_AND_EXHALE

      lcd_screen.setCursor(0, 1);
      lcd_screen.print(String("wait   ") + String(target_delay));
      
      stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'D', steps_delay);

      if (digitalRead(select_BUTTON_PIN) != LOW) {
        exit_condition = 1 ;  //to break from while loop
        break;
      }

    }

    delay(target_delay);

    if(exit_condition == 0){
      for (int i = 0 ; i < target_steps ; ++i) {
  
        target_delay = analogRead(POTENTIOMETER_PIN);
        target_delay = map(target_delay, 0, 1023, MIN_DELAY_BETWEEN_INHALE_AND_EXHALE, MAX_DELAY_BETWEEN_INHALE_AND_EXHALE); //maping the potentiometer value with MIN_DELAY_BETWEEN_INHALE_AND_EXHALE,MAX_DELAY_BETWEEN_INHALE_AND_EXHALE

        lcd_screen.setCursor(0, 1);
        lcd_screen.print(String("wait   ") + String(target_delay));
        
        stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'U', steps_delay);
  
        if (digitalRead(select_BUTTON_PIN) != LOW) {
          exit_condition = 1 ;  //to break from while loop
          break;
  
        }
      }
    }
  }

  lcd_screen.clear();//clear display

  return target_delay ;
}


/*
*Function: get_delay_between_2_breaths
*
*used to detrmine the delay_between_2_breaths
*
*return target_delay
*/
int get_delay_between_2_breaths(int target_steps, int steps_delay, int inhale_delay) {

  /*
    after breath####
    wait###10#######
  */
  lcd_screen.clear();//clear display
  lcd_screen.setCursor(5, 0); 
  lcd_screen.print("after breath");

  
  int target_delay = 0 ;
  int exit_condition = 0 ;
  while (exit_condition == 0) {

    for (int i = 0 ; i < target_steps ; ++i) {

      target_delay = analogRead(POTENTIOMETER_PIN);
      target_delay = map(target_delay, 0, 1023, MIN_DELAY_BETWEEN_2_BREATHS, MAX_DELAY_BETWEEN_2_BREATHS); //maping the potentiometer value with MIN_DELAY_BETWEEN_2_BREATHS,MAX_DELAY_BETWEEN_2_BREATHS

      lcd_screen.setCursor(0, 1);
      lcd_screen.print(String("wait   ") + String(target_delay));
        
      stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'D', steps_delay);

      if(digitalRead(select_BUTTON_PIN) == LOW){
        exit_condition = 1 ;  //to break from while loop
        break ;
      }
    
    }


    if(exit_condition == 0 ){
      delay(inhale_delay);
    }

    
    if(exit_condition == 0){
      
      for (int i = 0 ; i < target_steps ; ++i) {
  
        target_delay = analogRead(POTENTIOMETER_PIN);
        target_delay = map(target_delay, 0, 1023, MIN_DELAY_BETWEEN_2_BREATHS, MAX_DELAY_BETWEEN_2_BREATHS); //maping the potentiometer value with MIN_DELAY_BETWEEN_2_BREATHS,MAX_DELAY_BETWEEN_2_BREATHS

        lcd_screen.setCursor(0, 1);
        lcd_screen.print(String("wait   ") + String(target_delay));
          
        stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'U', steps_delay);
      }
    }

    if(exit_condition == 0 ){
      delay(target_delay);
    }

  }


  lcd_screen.clear();//clear display

  return target_delay ;

}




/*
*Function: test_movement
*
*used to test the user choises if its suitable for the patient or not 
*
*return void
*/
void test_movement(int target_steps, int steps_delay, int inhale_delay, int breath_breath_delay) {

  /*
    #####wait!#####
    ###for test####
  */

  lcd_screen.clear();//clear display
  lcd_screen.setCursor(5, 0); 
  lcd_screen.print("wait!");
  lcd_screen.setCursor(3, 1);
  lcd_screen.print("for test");
 
  
  for (int i = 0 ; i < 3 ; ++i) {

    for (int j = 0 ; j < target_steps ; ++j) {
      stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'D', steps_delay);
    }

    delay(inhale_delay);

    for (int k = 0 ; k < target_steps ; ++k) {
      stepper_move(STEPPER_MOTOR_DIR_PIN, STEPPER_MOTOR_STEP_PIN, 'U', steps_delay);
    }

    delay(breath_breath_delay);


  }

  lcd_screen.clear();//clear display

}

/*
*Function: get_pressure
*
*used to get the pressure of the patient
*
*return pressure
*/
int get_pressure(){

  char PRESSURESHOW[4]; // initializing a character of size 4 for showing the result
  lcd_screen.clear();//clear display
  lcd_screen.setCursor(0, 0);//set the cursor to column 0, line1
  lcd_screen.print("Pressure= ");
  
  String PRESSUREVALUE = String(bmp.readPressure());
  PRESSUREVALUE.toCharArray(PRESSURESHOW, 4);
  
  lcd_screen.print(PRESSURESHOW);
  lcd_screen.print("hPa ");
   
  return bmp.readPressure() ;
}


/*
*Function: get_pressure
*
*used to get the temp of the patient
*
*return temp
*/
int get_temp(){

  char TEMPARATURESHOW[4];  // initializing a character of size 4 for showing the temparature result

  lcd_screen.setCursor(0, 1);//set the cursor to column 0, line1

  lcd_screen.print("Temparature=");// print name
  
  String TEMPARATUREVALUE = String(bmp.readTemperature());
  TEMPARATUREVALUE.toCharArray(TEMPARATURESHOW, 4);

  lcd_screen.print(TEMPARATURESHOW);
  lcd_screen.print("C ");
  
 return bmp.readTemperature() ;
}


void welcome_message(){
  /*
    ###Fighting#####
    ####corona######
  */

  lcd_screen.clear();//clear display
  lcd_screen.setCursor(2, 0); 
  lcd_screen.print("Fighting");
  lcd_screen.setCursor(3, 1);
  lcd_screen.print("Corona");
  delay (2000);
  lcd_screen.clear();//clear display

}
