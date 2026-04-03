/*
    Dumbest possible Exposure timer
    Keys are Hh:Mm:Ss (10's, and 1's") for each Initial commit is all tens 
    
    The timer will run for 1-999999 seconds. About 12 days.
    
    Set selector switch to 'Set' in order to set the time and then switch it to 'run' to ready the countdown timer
    Press Reset while select is on 'set' to zero out timer (BREAKS LOOP AND ENDS EXPOSURE when switch set to 'run' mode)

    Go button runs timer when select switch is set to 'run' - DOES NOT PAUSE - BREAKS LOOP AND ENDS EXPOSURE IF EXPOSURE IS RUNNING)
    Exposure timer Will run 12v circuit till timer reaches zero at which time the exposure will stop and timer will reset to previously set time.

    Using;
    LCD_I2C - Arduino library to control a 16x2 LCD via an I2C adapter based on PCF8574
    Copyright(C) 2020 Blackhack <davidaristi.0504@gmail.com>

*/

#include <LCD_I2C.h>        // SETUP LCD
LCD_I2C lcd(0x27, 16, 2);   // I2C LCD, change according to needs

long SetTime = 0;           // The time that will be displayed on the LCD
long ElaTime = 0;           // The time that is used for the timer countdown

byte RelayGo = 2;           // the digital pin the relay is attached to

byte HourTen = 8;           // Place keepers for the switch data
byte HourOne = 7;
byte MinTens = 6;
byte MinOnes = 5;
byte SecTens = 4;
byte SecOnes = 3;

byte StopGo = 11;       // The switches that control the user selections
byte Select = 12;
byte Resets = 13;

int val1 =0;
int val2 =0;
int val3 =0;
int val4 =0;
int val5 =0;
int val6 =0;

int Mode =0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SETUP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup(){

//Set up all of the pins that are in use

// Set up the pin that will control the relay and make sure it is switched off.
    pinMode(RelayGo, OUTPUT);
    digitalWrite(RelayGo, LOW);
// The pins for control in the initial loop (Set high default to trigger on low)
    pinMode(StopGo, INPUT_PULLUP);  
    pinMode(Select, INPUT_PULLUP);  
    pinMode(Resets, INPUT_PULLUP);  
// The pins which will be used to dial up the exposure time
    pinMode(HourTen, INPUT_PULLUP);  
    pinMode(HourOne, INPUT_PULLUP);  
    pinMode(MinTens, INPUT_PULLUP);  
    pinMode(MinOnes, INPUT_PULLUP);  
    pinMode(SecTens, INPUT_PULLUP);  
    pinMode(SecOnes, INPUT_PULLUP);  


    lcd.begin(); // Start the LCD
    lcd.clear(); 
    lcd.backlight(); // Turn on backlight
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MAIN CODE LOOP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop()
{    
    int Mode = (digitalRead(Select));           // Check state of the selector switch and activate the relevant function
    if (Mode == LOW){
        ReadButton();                           //THIS CODE ONLY EXISTS TO KICK DEVICE TO EITHER PROGRAM OR RUN MODE
    }                                           //READS SWITCH AND THEN GOES TO RELEVANT FUNCTION
    else {
        Timer();                                // If Selector is high go to function for adding time using buttons Send Time()
    }
    delay(100);                                 // DEBOUNCE ISN'T NEEDED, BUT DOESN'T HURT
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FUNCTIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// COUNTDOWN TIMER FUNCTION
void Timer () //pause button (or reset or mode change) just lears the exposure for now.
{
    ElaTime = SetTime;
       while (digitalRead(Select)==HIGH){       //Give condition to escape function and return to main
        ElaTime = SetTime;                      // This means that the timer will reset after exposure and it'll keep that
        lcd.clear();                            // information so that exposures can be repeated easily
        lcd.setCursor(0, 0);
        lcd.print("RUN TIME      GO");          // formatting stuff
        lcd.setCursor(0, 1);
        lcd.print("SET:  ");                    
        lcd.setCursor(5, 1);                    // DISPLAY SetTime (elapsed time = set time here).
        lcd.print(ElaTime);
        delay(75);                              //Debounce


       if (digitalRead(StopGo) == LOW) {
        digitalWrite(RelayGo, HIGH);            //TURN ON RELAY
        for (long i=ElaTime; i>0; i--) {        //Create a loop the length of the time that's been set
            delay(500);                         //Using two shorter delays to make button press quicker
            if (digitalRead(Resets) == LOW){    //Should probably attachinterupt to throw out to a pause function
                i=0;                            //But maybe fix later - Can't be bothered now.
            }                                   
            if (digitalRead(StopGo) == LOW){    //GO, Reset, and mode change buttons will break loop as needed
                i=0;
            }
            if (digitalRead(Select) == LOW){
                i=0;
            }
            delay(500);
            lcd.clear();                        // Show the number which has been set
            lcd.setCursor(0, 0);
            lcd.print("SET:");                   
            lcd.setCursor(5, 0);                // Formatting stuff
            lcd.print(SetTime);
            lcd.setCursor(12, 0);               // DISPLAY SetTime.
            lcd.print("STOP");
            lcd.setCursor(0, 1);
            lcd.print("RUN:");                  
            lcd.setCursor(5, 1);                // DISPLAY RunTime.
            lcd.print(ElaTime);
            ElaTime--;                          //Countdown decrement
            }
        digitalWrite(RelayGo, LOW);             //LOOP END - TURN OFF RELAY
        delay(1000);                            // one second delay at end of function
        }
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReadButton ()          // read the pins to add the time into the system
{
    byte Del = 65;          //I can't be bothered manually entering delays - this changes them all - it's basically a debounce to be used everywhere

    int Ones = 0;           //Set up some variables to hold button state data while in this mode
    int Tens = 0;           //This will reset if coming back into this mode - I'm fine with that
    int Hund = 0;
    int Thou = 0;
    int TenThous = 0;
    int HundThou = 0;

   while (digitalRead(Select)==LOW){                //Give condition to escape loop in function and return to main
        lcd.clear();                                //First clear the screen
                                                    
// Ones of seconds                                  //Do the following until the 'select' switch is switched back to run
        if (digitalRead(SecOnes) == LOW) {          //For each button that is being being pressed
            if (Ones<0) {                           // If the number in the variable for that button is above zero and if it's 8 or less
                Ones=0;                             // Then add one to the number
                delay(Del);                         // Otherwise reset the number to zero
            }                                       
            else if (Ones>8) {                      
                Ones=0;                             
                delay(Del);                         
            }
            else {          
                Ones++;
                delay(Del);                         //(The delay will only run once as only one of these statements is true)
            }                                       //move to next button                                            
        }  
// Tens of seconds
        if (digitalRead(SecTens) == LOW) {          //Rinse and repeat for every button
            if (Tens<0) {                           
                Tens=0;                             
                delay(Del);                         
            }                                       
            else if (Tens>8) {                      
                Tens=0;                             
                delay(Del);                         
            }
            else {          
                Tens++;
                delay(Del);                         
            }                                       //move to next button                                            
        }  
// Hundreds of seconds
        // s of seconds
        if (digitalRead(MinOnes) == LOW) {          
            if (Hund<0) {                           
                Hund=0;                             
                delay(Del);                         
            }                                       
            else if (Hund>8) {                      
                Hund=0;                             
                delay(Del);                         
            }
            else {          
                Hund++;
                delay(Del);                         
            }                                       //move to next button                                             
        }  

// Thousands of seconds
        if (digitalRead(MinTens) == LOW) {        
            if (Thou<0) {                         
                Thou=0;                         
                delay(Del);                        
            }                                     
            else if (Thou>8) {                    
                Thou=0;                            
                delay(Del);                         
            }
            else {          
                Thou++;
                delay(Del);                       
            }                                       //move to next button                                           
        }  
// Tens of thousands of seconds
        if (digitalRead(HourOne) == LOW) {         
            if (TenThous<0) {                          
                TenThous=0;                           
                delay(Del);                        
            }                                      
            else if (TenThous>8) {                     
                TenThous=0;                             
                delay(Del);                         
            }
            else {          
                TenThous++;
                delay(Del);                        
            }                                      //move to next button                                  
        }  
    // Hundreds of thousands of seconds
        if (digitalRead(HourTen) == LOW) {         
            if (HundThou<0) {                          
                HundThou=0;                            
                delay(Del);                         
            }                                      
            else if (HundThou>8) {                  
                HundThou=0;                            
                delay(Del);                         
            }
            else {          
                HundThou++;
                delay(Del);                        
            }                                       //move to next button                                   
        }  
            
        //Do the maths to turn the capture variables to a real number which is saved as a Long integer
        //Without a Long declaration the tens of thousands will overflow - it's a 'C' error rather than an arduino thing
        SetTime = ((Ones*1L)+(Tens*10L)+(Hund*100L)+(Thou*1000L)+(TenThous*10000L)+(HundThou*100000L)); //Note Long declaration

        lcd.setCursor(0, 0);
        lcd.print("SET:  ");    // 
        lcd.setCursor(8, 0);    // DISPLAY SetTime.
        lcd.print(SetTime);
        delay(Del);

    if (digitalRead(Resets)==LOW){      //If the reset button is pressed
        Ones = 0;                       //reset all of the variables
        Tens = 0;                       //and go back to head of loop
        Hund = 0;
        Thou = 0;
        TenThous = 0;
        HundThou = 0;
        }
     } 
} 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// STUFF AND JUNK WHAT MIGHT GET HANDY LATER (NOTES)

/*
void Pause()
{
 Do the pause
 Return to Where you came from
}
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
