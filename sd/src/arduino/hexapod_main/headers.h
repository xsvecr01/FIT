/**
 * @file headers.h
 * @brief Header file with constant definitions and external libraries
 * @author Radek Svec
 * Contact: xsvecr01@stud.fit.vutbr.cz
 */
 
#include "src/Adafruit_PWM_Servo_Driver_Library/Adafruit_PWMServoDriver.h"
#include "WiFi.h"
#include "WiFiAP.h"

/********* CONSTANTS *********/
#define S0  5   //coxa
#define S1  18  //femur
#define S2  0   //tibia

#define S3  1   //coxa
#define S4  2   //femur
#define S5  3   //tibia

#define S6  4   //coxa
#define S7  5   //femur
#define S8  6   //tibia

#define S9  7   //coxa
#define S10 8   //femur
#define S11 9   //tibia

#define S12 10  //coxa
#define S13 11  //femur
#define S14 12  //tibia

#define S15 13  //coxa
#define S16 14  //femur
#define S17 15  //tibia

#define FREQ 50  //pwm frequency
#define RESOLUTION 12  //pwm resolution

#define TEN_EIGHTY 0.05555555555555555555555556  // 10/180

#define COXA_LEN 12.5
#define FEMUR_LEN 45
#define TIBIA_LEN 57

#define SERVO_STEP 2

#define FREQ 50  //pwm frequency
#define RESOLUTION 12  //pwm resolution

#define TEN_EIGHTY 0.05555555555555555555555556  // 10/180

#define PORT 51136

enum LegPosition {Front, Middle, Back};
enum State {Sitting, Standing, Walking, Stopped, Rotating};
