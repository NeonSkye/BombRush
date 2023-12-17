#include <LiquidCrystal.h>
#include "LedControl.h"

//LCD display data pins
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

//LCD back-light control
const int potPin = A2;
int potValue = 0;
const int contrastPin = 3;
int contrastValue = 0;

// Matrix control pins CLK & LOAD
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

// Joystick pins
const int pinX = A1;
const int pinY = A0;
const int pinSW = 2;


//LED control for matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

//what the physical matrix will show
// Object codes: 1 - WALL, 2 - PLAYER, 3 - BOMB, 5 - PLAYER OVER BOMB
// BOMBS WILL BE INVISIBLE IN FINAL VERSION OF THE GAME
const byte matrixSize = 8;
const byte mapSize = 16;

int gameMap[mapSize][mapSize] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //# # # # # # # # # # # # # # # #
  {1, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //# . . . . . . # # . . . . . . #
  {1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //# . . # . . . # # . . . . . . #
  {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1}, //# . # # # # . . . . # # # # . #
  {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //# . . # . . . . . . . . . . . #
  {1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1}, //# . . # . . . # # . # # # # . #
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //# . . . . . . # # . . . . . . #
  {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //# # # . . # # # # # # . . # # #
  {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //# # # . . # # # # # # . . # # #
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //# . . . . . . # # . . . . . . #
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1}, //# . . . . . . # # . . . . # . #
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, //# # # # # . . . . . . . # . . #
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1}, //# # # # # . . . . . . # . . . #
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1}, //# . . . . . . # # . # . . . . #
  {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //# . . . . . . # # . . . . . . #
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,} //# # # # # # # # # # # # # # # #
};


int roomNumber = 1; // In what room the player is. 1 - TOP LEFT, 2 - TOP RIGHT, 3 - BOTTOM LEFT,  4 - BOTTOM RIGHT
// used to convert from global coordinates to local coordinates. This is what allows separate rooms to exist.
int roomRow = 0;
int roomCol = 0;
int maxRoomRow = 8;
int maxRoomCol = 8;
bool roomSwitch = false;

// What objects should be shown on the real 8x8 LED Matrix.
byte matrixMap[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,}
};

// Updates room number and coordinates.
int roomUpdate(int roomNumber)
{
  switch(roomNumber) {
    case 1:
    roomRow = 0;
    roomCol = 0;
    break;
    case 2:
    roomRow = 0;
    roomCol = 8;
    break;
    case 3:
    roomRow = 8;
    roomCol = 0;
    break;
    case 4:
    roomRow = 8;
    roomCol = 8;
  }
  maxRoomRow = roomRow + 8;
  maxRoomCol = roomCol + 8;
}
// Translates gameMap room to the matrix display.
int matrixWrite(int minRow, int minCol, int maxRow, int maxCol) {
  int matrixRow = 0;
  int matrixCol = 0;
  int mapRow = 0;
  int mapCol = 0;
  for(mapRow = minRow; mapRow < maxRow; mapRow++)
  {
    
    Serial.println();
    {
      for(mapCol = minCol; mapCol < maxCol; mapCol++)
      {
        
        Serial.print(gameMap[mapRow][mapCol]);
        Serial.print(" ");
        matrixMap[matrixRow][matrixCol] = gameMap[mapRow][mapCol];
        if(gameMap[mapRow][mapCol] == 2 || gameMap[mapRow][mapCol] == 5)
        lc.setLed(0, matrixRow, matrixCol, 1);
        matrixCol++;
      }
      matrixCol = 0;
      matrixRow++;
    }
  }
}

// Used in settings to light up the whole matrix.
void matrixShow() {
for (int row = 0; row < matrixSize; row++) {
  for (int col = 0; col < matrixSize; col++) {
    lc.setLed(0, row, col, 1);
    }
  }
}

// The evil twin of the previous function. Used to turn off the whole matrix.
void matrixHide() {
  for (int row = 0; row < matrixSize; row++) {
  for (int col = 0; col < matrixSize; col++) {
    lc.setLed(0, row, col, 0);
    }
  }
}

// Updates the matrix display.
void matrixUpdate() {
for (int row = 0; row < matrixSize; row++) {
  for (int col = 0; col < matrixSize; col++) {
    if (matrixMap[row][col] == 3) lc.setLed(0, row, col, 0);
    if (matrixMap[row][col] < 2 && matrixMap[row][col] != 5)
    lc.setLed(0, row, col, matrixMap[row][col]);
    }
  }
}


// Player blinking LED
unsigned long int playerBlinkDelay = 450;
unsigned long int startPlayerBlink = 0;
unsigned long int statePlayerBlink = 0;
bool playerOnBomb = false;

// Joystick movement stuff
int xVal = 0;
int yVal = 0;
int minThresh = 400;
int maxThresh = 600;
bool joyMoved = false;
unsigned long int playerMove = -1000;
unsigned long int playerMoveDelay = 150;


//LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte bombChar[] = {
  B10000,
  B01000,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110
};



void setup() {
// set up the LCD's number of columns and rows:
lcd.begin(16, 2);
randomSeed(analogRead(5));
lcd.createChar(0, bombChar);
lcd.setCursor(0, 0);
lcd.print(">");
pinMode(pinSW, INPUT_PULLUP);
lc.shutdown(0, false);
lc.setIntensity(0, matrixBrightness);
lc.clearDisplay(0);
Serial.begin(9600);
}

//bomb stuff
bool bombPlaced = false;
bool bombDefused = false;
int bombRoom = 0;
long int timer = 30000; //milliseconds
unsigned long int lastMillis = millis();
unsigned long int timerDelay = 1000; // by default, ticks down the counter 1 second. Can be sped up or slowed down.
int randX = 0;
int randY = 0;
byte swState = LOW;
byte lastSwState = LOW;
int score = 0;
bool gameEnd = false;
bool gameStart = false;
//gameOver function
void gameOver()
{
  matrixHide();
  roomNumber = 1;
  setObject(getObjRow(2), getObjCol(2), 0);
  setObject(getObjRow(3), getObjCol(3), 0);
  gameMap[randX][randY] = 0;
  setObject(getObjRow(5), getObjCol(5), 0);
  gameMap[1][1] = 2;
  bombPlaced = false;
  lcd.clear();
  gameEnd = true;
  lcd.setCursor(0,0);
  lcd.print("Dead! score: ");
  lcd.print(score);
  lcd.setCursor(0,1);
  lcd.print(">Menu");
  timer = 30000;
  roomUpdate(roomNumber);
  score = 0;
  matrixHide();
}

// starts the game (duh)
void startGame() {
gameStart = true;
matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
matrixUpdate();
}

//randomly spawns bomb on gameMap
void bombSpawn()
{
  
  randX = random(0, 15);
  randY = random(0, 15);

  
  if(gameMap[randX][randY] == 1 || gameMap[randX][randY] == 2 || randX == 7 || randX == 8 || randY == 7 || randY == 8){
    bombPlaced = false;
  } else {
    gameMap[randX][randY] = 3; // what room the bomb is in. Used to decide what symbol gets shown.
    if(randX <= 7 && randY <= 7) bombRoom = 1;
    if(randX <= 7 && randY >= 8) bombRoom = 2;
    if(randX >= 8 && randY <= 7) bombRoom = 3;
    if(randX >= 8 && randY >= 8) bombRoom = 4;
    bombPlaced = true;
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
  }

}
// similar to roomRow & roomCol. Used so the coordinates displayed are local to the room the bomb is in, instead of global.
int bombMinX = 0;
int bombMinY = 0;

//menu stuff and flags
const int menuOptions = 4;
int menuArray[menuOptions] = {1, 2, 3, 4};
int menuPointer = 0;
bool inSettings = false;
bool inAbout = false;
int modifySetting = 1;
bool changeMatrixBright = false;
bool changeLCDBright = false;
bool changeSetting = false;

void loop() {
  potValue = analogRead(potPin);
  contrastValue = map(potValue, 0,  1023, 0, 128);
  xVal = analogRead(pinX);
  yVal = analogRead(pinY);
  swState = digitalRead(pinSW);
  analogWrite(contrastPin, contrastValue);
  if(inAbout == true) { // ABOUT menu section
    if(millis() - lastMillis >= 450) {
      
    lcd.autoscroll();
    lcd.setCursor(0,0);
    lcd.print("BombRush!");
    lcd.setCursor(0,1);
    lcd.print("https://github.com/NeonSkye/BombRush");
    lastMillis = millis(); 
    }
    if(swState != lastSwState) { // if button is pressed, go back to main menu
      if (swState == LOW)
      inAbout = false;
      lastSwState = swState;
      lcd.clear();
      lcd.noAutoscroll();
    }
  } else
  if(inSettings == true) { // SETTINGS 
    if(changeSetting == false) 
    { // MAIN SETTINGS MENU
    lcd.setCursor(0, menuPointer%2);
    lcd.print(">");
   
  if(swState != lastSwState) { // Check what option was selected in settings. 
          if(swState == LOW){
            lcd.clear();
            switch(menuArray[menuPointer]) {
            case 1:
            lcd.clear();
            menuPointer = 0;
            changeMatrixBright = true;
            changeSetting = true;
            joyMoved = false;
            
            break;
            case 2:
            lcd.clear();
            menuPointer = 0;
            changeLCDBright = true;
            changeSetting = true;
            joyMoved = false;
            
            break;
            case 3:
            lcd.clear();
            menuPointer = 0;
            changeLCDBright = true;
            changeSetting = true;
            joyMoved = false;
            break;
            case 4:
            lcd.clear();
            menuPointer = 0;
            changeSetting = false;
            inSettings = false;
            break;
            }

          }
        lcd.clear();
        lastSwState = swState;
      }
  if(xVal > maxThresh && joyMoved == false) // MOVE DOWN IN MENU
  {
    joyMoved = true;
    menuPointer++;
    lcd.clear();
    }
  if (xVal < minThresh && joyMoved == false) // MOVE UP IN MENU
  {
    joyMoved = true;
    menuPointer--;
    lcd.clear();
  }
    if(menuPointer == -1) { // LOOP DOWN
    menuPointer = 3;
  } else
  if (menuPointer <= 1) { // Page 1
  lcd.setCursor(1,0);
  lcd.setCursor(15, 0);
  lcd.print("|");
  lcd.setCursor(1, 0);
  lcd.print("Matrix Bright");
  lcd.setCursor(1, 1);
  lcd.print("W.I.P option");
  } else if (menuPointer >= 2 && menuPointer < 4) { // Page 2
    lcd.setCursor(15, 1);
    lcd.print("|");
    lcd.setCursor(1,0);
    lcd.print("W.I.P option");
    lcd.setCursor(1, 1);
    lcd.print("Exit");
  } else if (menuPointer == menuOptions + 1) { // LOOP UP
    menuPointer = 0;
  }
  if(xVal >= minThresh && xVal <= maxThresh && yVal >= minThresh && yVal <= maxThresh) {
  joyMoved = false;
  }
  } else { // IN SPECIFIC SETTING
    menuPointer = 0;
    if(xVal > maxThresh && joyMoved == false) 
    {
    joyMoved = true;
    modifySetting = modifySetting - 1;
    lcd.clear();
    }
  if (xVal < minThresh && joyMoved == false) 
  {
    joyMoved = true;
    modifySetting = modifySetting + 1;
    lcd.clear();
  }
    if(changeMatrixBright == true){
    if(modifySetting >= 10) 
    {   
    modifySetting = 10;
}
    if(modifySetting <= 0) modifySetting = 0; 
    matrixShow();
    lcd.setCursor(0,0);
    lcd.print(matrixBrightness);
    matrixBrightness = modifySetting;
    lc.setIntensity(0, matrixBrightness);
    }
    if(changeLCDBright == true) {
      lcd.print("W.I.P");
    }
    if(swState != lastSwState) { // When button is pressed, save changes and go back to settings.
    if(swState == LOW){
    matrixHide();
    changeMatrixBright = false;
    changeLCDBright = false;
    changeSetting = false;
    menuPointer = 0;
    lcd.clear();
    }
    lcd.clear();
    lastSwState = swState;
    }
  if(xVal >= minThresh && xVal <= maxThresh && yVal >= minThresh && yVal <= maxThresh) {
  joyMoved = false;
  }
  }
} else
  if(gameStart == false) { // Main Menu

  xVal = analogRead(pinX);
  yVal = analogRead(pinY);
  lcd.setCursor(0,menuPointer%2);
  lcd.print(">");
  if(xVal > maxThresh && joyMoved == false) { // MOVE DOWN 
  joyMoved = true;
  menuPointer++;
  lcd.clear();
  }
  if (xVal < minThresh && joyMoved == false) { // MOVE UP
  joyMoved = true;
  menuPointer--;
  lcd.clear();
  }
  if(menuPointer == -1) { // LOOP DOWN
    menuPointer = 3;
  } else
  if (menuPointer <= 1) { // Page 1
  lcd.setCursor(15, 0);
  lcd.print("|");
  lcd.setCursor(1,0);
  lcd.print("Start");
  lcd.setCursor(1, 1);
  lcd.print("Settings");
  } else if (menuPointer >= 2 && menuPointer < 4) {
    lcd.setCursor(15, 1);
    lcd.print("|");
    lcd.setCursor(1,0);
    lcd.print("Settings");
    lcd.setCursor(1, 1);
    lcd.print("About");
  } else if (menuPointer == menuOptions) { // LOOP UP
    menuPointer = 0;
  }
  swState = digitalRead(pinSW);
  if(swState != lastSwState) { // Enables menu flags based on what was pressed.
          if(swState == LOW){
            switch(menuArray[menuPointer]) {
              case 1: // Start
            gameStart = true;
            lcd.clear();
            break;
              case 2: // Settings (page 1)
            menuPointer = 0;
            inSettings = true;
            joyMoved = false;
            lcd.clear();
            break;
              case 3: // Settings (page 2)
            menuPointer = 0;
            inSettings = true;
            joyMoved = false;
            lcd.clear();
              case 4: // About (page 2)
            inAbout = true;
            menuPointer = 0;
            joyMoved = false;
            lcd.clear();
            break;
            }

          }
        lcd.clear();
        lastSwState = swState;
      }
    if(xVal >= minThresh && xVal <= maxThresh && yVal >= minThresh && yVal <= maxThresh) {
    joyMoved = false;
    }
  }
  else
  {
    if(gameEnd == false)  
    { // This is the main game loop
    
    if(bombPlaced == false){ // Spawn a bomb
      bombSpawn();
    } else {
      lcd.setCursor(0,0);
      lcd.write(byte(0));
      lcd.print(" at: ");
      switch(bombRoom) {
      case 1:
      lcd.print("+ ");
      bombMinX = 0;
      bombMinY = 0;
      break;
      case 2:
      lcd.print("= ");
      bombMinX = 0;
      bombMinY = 8;
      break;
      case 3:
      lcd.print("- ");
      bombMinX = 8;
      bombMinY = 0;
      break;
      case 4:
      lcd.print("/ ");
      bombMinX = 8;
      bombMinY = 8;
      break;
    }
    lcd.print(randX - bombMinX);
    lcd.print(" ");
    lcd.print(randY - bombMinY);
    }
    swState = digitalRead(pinSW);
    if(swState != lastSwState) {
    if(swState == LOW && bombPlaced == true && getObjRow(5) != -1) { // This only works if there's a player on a bomb object (5).
      bombPlaced = false;
      score = score + 1;
      timer = timer + 5000; // + 5 Seconds whenever a bomb is defused.
      setObject(getObjRow(5),getObjCol(5), 2);
      lcd.clear();
      }
    lastSwState = swState;
    }

    lcd.setCursor(0, 1);


    xVal = analogRead(pinX);
    yVal = analogRead(pinY);
    if(getObjRow(2) > -1) { // If the player object isn't on a bomb, move player object.
      blinkPlayer(getObjRow(2), getObjCol(2));
      if(xVal > maxThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(2, 2, getObjRow(2), getObjCol(2)); // GO DOWN
        playerMove = millis();
        joyMoved = true;
      }
      if (xVal < minThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(2, 1, getObjRow(2), getObjCol(2)); // GO UP
        playerMove = millis();
        joyMoved = true;
      }
      if (yVal > maxThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(2, 3, getObjRow(2), getObjCol(2)); // GO LEFT
        playerMove= millis();
        joyMoved = true;
      }
      if (yVal < minThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(2, 4, getObjRow(2), getObjCol(2)); // GO RIGHT
        playerMove= millis();
        joyMoved = true;
      }
    }
    else { // There is a player object on a bomb instead, move this special player object. This way bombs won't disappear simply by walking over them.
        if(xVal > maxThresh && millis() - playerMove >= playerMoveDelay){
        movePlayer(5, 2, getObjRow(5), getObjCol(5)); // GO DOWN
        playerMove = millis();
        joyMoved = true;
        }
      if (xVal < minThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(5, 1, getObjRow(5), getObjCol(5)); // GO UP
        playerMove = millis();
        joyMoved = true;
        }
      if (yVal > maxThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(5, 3, getObjRow(5), getObjCol(5)); // GO LEFT
        playerMove= millis();
        joyMoved = true;
        }
      if (yVal < minThresh && millis() - playerMove >= playerMoveDelay) {
        movePlayer(5, 4, getObjRow(5), getObjCol(5)); // GO RIGHT
        playerMove= millis();
        joyMoved = true;
        }
      }
    if (timer >= 0) {
    lcd.setCursor(0,1);
    lcd.print(timer/1000);
    if(millis() - lastMillis >= timerDelay) {
      lcd.clear();
      timer = timer - 1000;
      lastMillis = millis();
        }
    lcd.print("   score:");
    lcd.print(score);
      }
    else {
      gameOver();
      }

      if(score >= 25) timerDelay = 850; // Game gets harder the more bombs you pick.
      if(score >= 50) timerDelay = 650;
      if(score >= 100) timerDelay = 325;

    if(xVal >= minThresh && xVal <= maxThresh && yVal >= minThresh && yVal <= maxThresh) {
        joyMoved = false;
        playerMove = -1000;
        }
    }
    else {
      swState = digitalRead(pinSW);
        if(swState != lastSwState) {
          if(swState == LOW){
            gameEnd = false;
            gameStart = false;
            lcd.clear();
          }
        lastSwState = swState;
      }
    }
  }
}

int blinkPlayer (int row, int col) { // Blinks the player LED.
  if (millis() - startPlayerBlink >= playerBlinkDelay) {
    statePlayerBlink = !statePlayerBlink;
    lc.setLed(0, row, col, statePlayerBlink);
    startPlayerBlink = millis();
  }
}

int getObjRow(int object)  { // Gets the row of an object. (This only works on Player, Bomb, Player on bomb). ((Why would you get the row of a wall?))
  int result = -1;
  for (int row = 0; row < matrixSize; row++) {
    for(int col = 0; col < matrixSize; col++)  
    {
      if(matrixMap[row][col] == object)  
      {
        result = row;
        break;
      }

    }
  }
  return result;
}

int getObjCol(int object)  { // The twin of getObjRow. Gets the column.
  int result = -1;
  for (int row = 0; row < matrixSize; row++) {
    for(int col = 0; col < matrixSize; col++)  
    {
      if(matrixMap[row][col] == object)  
      {
        result = col;
        break;
      }

    }
  }
  return result;
}

int movePlayer(int object, int direction, int row, int col){ // Movement logic for the player.
  if(object == 5) {
    if(bombDefused == false) {
  matrixMap[row][col] = 3;
  //lc.setLed(0, row, col, 0); // for debugging. Bombs will be invisible so this is not needed.
  gameMap[row+roomRow][col+roomCol] = 3;
  }
  else {
    matrixMap[row][col] = 0;
    gameMap[row+roomRow][col+roomCol] = 0;
    lc.setLed(0, row, col, 0);
  }
  }
  else {
    matrixMap[row][col] = 0;
    lc.setLed(0, row, col, 0);
    gameMap[row+roomRow][col+roomCol] = 0;
  }
  
switch(direction)
{
  case 1: // UP

  if (row == 0)
  {
    if (roomNumber == 4) {
    roomNumber = 2;
    roomUpdate(roomNumber);
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
    }
    else if(roomNumber == 3) {
    roomNumber = 1;
    roomUpdate(roomNumber);
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
    }
    setObject(7, col, object);
  }
  else if(object == 2)
  {
    if (matrixMap[row-1][col] == 1)
    {
      matrixMap[row][col] = object;
    } else if(matrixMap[row-1][col] == 3)
      {
        setObject(row-1, col, 5);
      }
      else setObject(row-1, col, object);
  }

  else if(object == 5)
  {
    if (matrixMap[row-1][col] == 1)
    {
      matrixMap[row][col] = object;
    } 
      else setObject(row-1, col, 2);
  }
  break;
  case 2: // GO DOWN

  if (row == 7)
  {
    if (roomNumber == 1)
    {
      roomNumber = 3;
      roomSwitch = true;
      roomUpdate(roomNumber);
      matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
      matrixUpdate();
    }
    
    else if(roomNumber == 2)
    {
      roomNumber = 4;
      roomSwitch = true;
      roomUpdate(roomNumber);
      matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
      matrixUpdate();
    }
    setObject(0, col, object);
    
  }
  else if(object == 2)
  {
    if (matrixMap[row+1][col] == 1)
    {
      matrixMap[row][col] = object;
    } else if(matrixMap[row+1][col] == 3)
      {
        setObject(row+1, col, 5);
      }
      else setObject(row+1, col, object);
  }

  else if(object == 5)
  {
    if (matrixMap[row+1][col] == 1)
    {
      matrixMap[row][col] = object;
    } 
      else setObject(row+1, col, 2);
  }
  break;

  case 3: // GO LEFT

  if (col == 0)
  {
    if (roomNumber == 2) {
    roomNumber = 1;
    roomUpdate(roomNumber);
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
    }
    else if(roomNumber == 4) {
    roomNumber = 3;
    roomUpdate(roomNumber);
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
    }
    setObject(row, 7, object);
  }
  else if(object == 2)
  {
    if (matrixMap[row][col-1] == 1)
    {
      matrixMap[row][col] = object;
    } else if(matrixMap[row][col-1] == 3)
      {
        setObject(row, col-1, 5);
      }
      else setObject(row, col-1, object);
  }

  else if(object == 5)
  {
    if (matrixMap[row][col-1] == 1)
    {
      matrixMap[row][col] = object;
    } 
      else setObject(row, col-1, 2);
  }
  break;

  case 4: // GO RIGHT

  if (col == 7)
  {
    if (roomNumber == 1){
    roomNumber = 2;
    roomUpdate(roomNumber);
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
    }
    else if(roomNumber == 3){
    roomNumber = 4;
    roomUpdate(roomNumber);
    matrixWrite(roomRow, roomCol, maxRoomRow, maxRoomCol);
    matrixUpdate();
    }
    setObject(row, 0, object);
  }
  else if(object == 2)
  {
    if (matrixMap[row][col+1] == 1)
    {
      matrixMap[row][col] = object;
    } else if(matrixMap[row][col+1] == 3)
      {
        setObject(row, col+1, 5);
      }
      else setObject(row, col+1, object);
  }

  else if(object == 5)
  {
    if (matrixMap[row][col+1] == 1)
    {
      matrixMap[row][col] = object;
    } 
      else setObject(row, col+1, 2);
  }
  break;
}

}
int setObject (int row, int col, int object) // sets an object in the local room.
{
  matrixMap[row][col] = object;
  lc.setLed(0, row, col, 1);
  gameMap[row+roomRow][col+roomCol] = object;
}
