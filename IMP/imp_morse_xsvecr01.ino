/**
 * project: IMP - Dekodér Morseovy abecedy
 * author: Radek Švec
 * login: xsvecr01
 * date: 9.12. 2020
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// set LCD address
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);

// button and led pins
const int BTN = 6;
const int LED = 7;

// timing constants
const int DOT_TIME = 200;
const int LINE_TIME = 3*DOT_TIME;
const int CHAR_SPACE = LINE_TIME;
const int WORD_SPACE = 7*DOT_TIME;

// variables
int lastState = HIGH;
int currentState;
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
String input = "";
bool spaced = true;
int lcdC = 0;


// -..- ... ...- . -.-. .-. ----- .----
// decode input from Morse code to ASCII character
char morseDecode(String code)
{
  if(code == ".-")
    return 'A';
  else if(code == "-...")
    return 'B';
  else if(code == "-.-.")
    return 'C';
  else if(code == "-..")
    return 'D';
  else if(code == ".")
    return 'E';
  else if(code == "..-.")
    return 'F';
  else if(code == "--.")
    return 'G';
  else if(code == "....")
    return 'H';
  else if(code == "..")
    return 'I';
  else if(code == ".---")
    return 'J';
  else if(code == "-.-")
    return 'K';
  else if(code == ".-..")
    return 'L';
  else if(code == "--")
    return 'M';
  else if(code == "-.")
    return 'N';
  else if(code == "---")
    return 'O';
  else if(code == ".--.")
    return 'P';
  else if(code == "--.-")
    return 'Q';
  else if(code == ".-.")
    return 'R';
  else if(code == "...")
    return 'S';
  else if(code == "-")
    return 'T';
  else if(code == "..-")
    return 'U';
  else if(code == "...-")
    return 'V';
  else if(code == ".--")
    return 'W';
  else if(code == "-..-")
    return 'X';
  else if(code == "-.--")
    return 'Y';
  else if(code == "--..")
    return 'Z';
  
  else if(code == ".----")
    return '1';
  else if(code == "..---")
    return '2';
  else if(code == "...--")
    return '3';
  else if(code == "....-")
    return '4';
  else if(code == ".....")
    return '5';
  else if(code == "-....")
    return '6';
  else if(code == "--...")
    return '7';
  else if(code == "---..")
    return '8';
  else if(code == "----.")
    return '9';
  else if(code == "-----")
    return '0';
  else
    return 0;
}

// setup LCD configuration and pin modes
void setup()
{
  Wire.begin();
  lcd.begin(16,2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
}


// main program cycle
void loop()
{
  // save button current state
  currentState = digitalRead(BTN);
  
  // button is pressed
  if(lastState == HIGH && currentState == LOW)
  {
    // erase display if full
    if(lcdC > 15)
    {
      lcd.clear();
      lcdC = 0;
    }

    digitalWrite(LED, HIGH);
    pressedTime = millis();

    long spaceDuration = pressedTime - releasedTime;    
  }

  // button is released
  else if(lastState == LOW && currentState == HIGH) 
  {
    digitalWrite(LED, LOW);
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    // decide if press is short - dot, or long - line
    if(pressDuration < DOT_TIME)
    {
      input += '.';
    }
    else if(pressDuration < LINE_TIME)
    {
      input += '-';
    }
  }

  long spaceDuration = millis() - releasedTime;

  // should decode and print character?
  if(input != "" && spaceDuration > CHAR_SPACE && spaceDuration <= WORD_SPACE)
  {
    char c = morseDecode(input);
    // wrong input
    if(c != 0)
    {
      lcd.print(c);
      lcdC++;
      spaced = false;
    }
    input = "";
  }
  // print word space
  if(!spaced && spaceDuration > WORD_SPACE)
  {
    spaced = true;
    lcd.print("_");
    lcdC++;
  }
  // clear display
  if(spaceDuration > 4*WORD_SPACE)
  {
    lcd.clear();
    lcdC = 0;
  }
  
  // save the the last state
  lastState = currentState;
}
