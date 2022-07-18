
/* SHOGGOTH EYE TARGETS 
 * for:           Rush Hour Live Escapses
 * by:            Matt Charles of Gratuitous Sets Laboratories
 * written:       01 March 2022
 * last revised:  08 April 2022
 * 
 */

//============== DEFINITIONS & DECLAIRATIONS =================//

//-------------- LIBRARIES -----------------------------------//
/* Call all functionlibraries required by the sketch. 
 */
  #include <Wire.h>                       // I2C library
  #include <Adafruit_PWMServoDriver.h>    // Adafruit PWM servo driver PCA9685

//-------------- HARDWARE INITIALIZATION ---------------------//
/* Hardware setup paramaters.
 */
  Adafruit_PWMServoDriver servoBar1 = Adafruit_PWMServoDriver(0X40);  // 1st servo controller (Adafruit PCA9685)
  Adafruit_PWMServoDriver servoBar2 = Adafruit_PWMServoDriver(0X41);  // 2nd servo controller (Adafruit PCA9685)

//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the ARDNEX2.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */
//.............. Digital Pins ................................//
  #define loadPin     2           // parallel connection to all 74HC165 PISO shift registers, pin 1
  #define dataInPin   3           // serial connection to nearest 74HC165 PISO shift register, pin 9
  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define neoPixelPin 5           // data line to WS2812 (NeoPixel) via 470R resistor
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  #define relay2Pin   8           // trigger pin for relay K2 via 1K resistor & 2N2222 or 2N7000 
  #define relay1Pin   9           // trigger pin for relay K1 via 1K resistor & 2N2222 or 2N7000
  #define buzzerPin  10           // trigger pin for piezoelectric buzzer via 1K resistor & 2N2222 or 2N7000
  #define audioTxPin 11           // data pin for DF Player Mini, pin 11
  #define audioRxPin 12           // data pin for DF Player Mini, pin 10
  #define progLEDPin 13           // trigger pin for PRG LED (if used)
//.............. Analog Pins .................................//
  #define analogInPin A0          // onboard 1K resistor voltage divider
// unused by ArdNex   A1          // OPEN
// unused by ArdNex   A2          // OPEN
// unused by ArdNex   A3          // OPEN
// reserved for I2C   A4          // DAT
// reserved for I2C   A5          // CLK
  #define xOutPin     A6          // 12V signal to higher control system
  #define xInPin      A7          // 12V signal from higher conrtol system
/*  
  #define xAxisPin A1           // X-Axis from joystick
  #define yAxisPin A0           // Y-Axis from joystick
  #define trimPotPin A2         // trim pot for eyelids
  #define blinkPin 2            // blink button (active LOW)
*/

// min/max pulse commands for MG90S
  #define servoMin 100            // pulse command for bottom of servo travel
  #define servoMax 640            // " top

// Full range of motion in Eye Mechs before internal mechanical conflict
  const int xMin = 325;
  const int xMax = 425;
  const int yMin = 325;
  const int yMax = 425;
  const int uMin = 250;
  const int uMax = 400;
  const int lMin = 325;
  const int lMax = 425;

  const String myNameIs = "ShoggothEyeTargets3A 18-Jul-2022";
  
//============================================================//
//============== SETUP =======================================//
//============================================================//

void setup() {

  Serial.begin(9600);
  Serial.println("Setup begun...");
  Serial.println(myNameIs);

//-------------- PINMODES ------------------------------------//
//.............. Shift Registers .............................//
  pinMode (clockPin, OUTPUT);
  pinMode (loadPin, OUTPUT);
  pinMode (dataInPin, INPUT);
  pinMode (latchPin, OUTPUT);
  pinMode (dataOutPin, OUTPUT);
//.............. Onboard Relays ..............................//
  pinMode (relay1Pin, OUTPUT);
  pinMode (relay2Pin, OUTPUT);
//.............. Offboard Communication ......................//
  pinMode (xOutPin, OUTPUT);
  pinMode (xInPin, INPUT);
//.............. Other Pin Modes .............................//
  pinMode (progLEDPin, OUTPUT);
/*
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(trimPotPin, INPUT);
  pinMode(blinkPin, INPUT_PULLUP);
*/

//-------------- SERVO CONTROLERS ----------------------------//
  servoBar1.begin();                // initialize servo PWM controllers
  servoBar1.setPWMFreq(60);         // set freq in Hz
  servoBar2.begin();                // "
  servoBar2.setPWMFreq(60);         // "

  randomSeed(A7);                   // random seed for random eye motion (A7 is not used)

  delay(100);                       // wait

  Serial.println("Setup complete.");
}


//-------------- GLOBAL CONSTANTS ----------------------------//
/* Define constraits used by various functions.
 * EDIT THESE TO TUNE GAMEPLAY
 */

  const int lookMin = 100;          // minimum time in ms for a pair of eyes to stare
  const int lookMax = 2000;         // maximum time in ms for a pair of eyes to stare
  
  const int frameDelay = 5;         // delay between animation frames in ms
  
  const int healDelay = 15000;      // length of time in ms to wait until an eye "heals"
  const int deadDelay = 300000;     // length of time in ms for the shoggoth to stay "dead" after all the eyes are hit


//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
 
  byte rawData;                     // current input from PISO shift register
  byte oldData;                     // previous input from PISO shift register

  bool isMoving[4];                 // eye is currently in an animation sequense
  bool isShot[4];                   // eye has been shot
  byte lookFrame[4];                // frame number within the animation (out of 100)
  int lookXNew[4];                  // next X-coordinate (-100 to 100)
  int lookXOld[4];                  // prev X-coordinate (-100 to 100)
  int lookYNew[4];                  // next Y-coordinate (-100 to 100)
  int lookYOld[4];                  // prev Y-coordinate (-100 to 100)
  int caffine[4];                   // how far open the eyelids are
  int lookDelay[4];                 // length of time (in ms) to wait until eyes focus on a new point
  unsigned long frameTick[4];       // timestamp of the last frame update
  unsigned long lookTick[4];        // timestamp of the last completed animation
  unsigned long shotTick[4];        // timestamp of the last gunshot hit
  
  int xPulse;                       // pulse ON points used by the updateServos(eye) fuunction
  int lyPulse;                      // "
  int ryPulse;                      // "
  int upPulse;                      // "
  int loPulse;                      // "

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {

//============== INPUT =======================================//

  bool gameOn = digitalRead (xInPin);                         // read the master signal pin
  readReg();                                                  // read the input from the targets

  if (!gameOn){                                               // if the game hasn't started yet...
    xPulse = (xMin+xMax)/2;                                   // set the focus to the middle for each axis
    lyPulse = (yMin+yMax)/2;                                  // "
    ryPulse = (yMin+yMax)/2;                                  // "
    upPulse = uMax;                                           // set the eyelids to close
    loPulse = lMax;                                           // "
    for (int eye = 0; eye < 4; eye++){                        // for each of the four sets of eyes...
      updateServos(eye);                                      // update the servo positions for that eye
    }
  }
  
//============== GAME PLAY ===================================//

  else if (gameOn){                                           // otherwise, if the FEED ME puzzle is complete...
  
//-------------- ANIMATION -----------------------------------// 
    for (int eye = 0; eye < 4; eye++){                        // for each of the 4 sets of eyes...

      gunshots(eye);                                          // determine if the eye has been recently shot
    
      if (isShot[eye]){                                       // if the eye has been shot...
        isMoving[eye] = false;                                // prevent it from moving
        xPulse = (xMin+xMax)/2;                               // set the focus to the middle for each axis
        lyPulse = (yMin+yMax)/2;                              // "
        ryPulse = (yMin+yMax)/2;                              // "
        upPulse = uMax;                                       // set the eyelids to close
        loPulse = lMax;                                       // "
        updateServos(eye);                                    // update the servo positions for that eye
      }

      else if (millis() >= lookTick[eye] + lookDelay[eye]){   // if enough time has passed since the last animation...
        newLook(eye);                                         // generate a new look target point
      }
    
      if (isMoving[eye]){                                     // if the eye is supposed to be moving...
        animate(eye);                                         // run the animation function for that eye
      }
    }
    
//-------------- TARGETS REACTIONS ---------------------------//
    bool allEyesShot = true;                                  // assume all targets have been hit
    for (int eye = 0; eye < 4; eye++){                        // for each of the 4 sets of eyes...
      if (!isShot[eye]){                                      // if that eye has not been hit...
        allEyesShot = false;                                  // then not all the targets have been hit
      }
    }
    if (allEyesShot){                                         // if all the targets have, in fact, been hit...
      digitalWrite (xOutPin, HIGH);                           // engage 12V output
      digitalWrite (relay1Pin, HIGH);                         // engage relay
      digitalWrite (relay2Pin, HIGH);                         // "
      delay (deadDelay);                                      // pause the designated time
    }
    else {                                                    // otherwise...
      digitalWrite (xOutPin, LOW);                            // disengage 12V outpit
      digitalWrite (relay1Pin, LOW);                          // disengage relay
      digitalWrite (relay2Pin, LOW);                          // "
    }
  }

//============== ROUTINE MAINT' ==============================//
  debugTroubleshoot();                                        // serial monitor reports
  cycleReset();                                               // reset variables

}                                                             // CLOSE VOID LOOP
