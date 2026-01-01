#include <LedControl.h>

// LED Matrix pins
const int DIN = 12;
const int CLK = 10;
const int CS = 11;
LedControl lc = LedControl(DIN, CLK, CS, 1);

// Joystick pins
const int joyX = A3;
const int joyY = A4;

// Game variables
byte grid[8] = {0};  // Represents the 8x8 grid
int snake[40][2];     // Snake body positions (max length 40)
int snakeLength = 2;
int headX = 5, headY = 1;
int dirX = 0, dirY = 1;  // Initial direction: right
int appleX, appleY;
float moveDelay = 300;   // Time between moves (milliseconds)
unsigned long lastMove = 0;


void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0,false); // Wake up the LED matrix 
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8); // Set brightness to medium level 
  /* and clear the display */
  lc.clearDisplay(0);

 // Initialize snake
  snake[0][0] = 5; snake[0][1] = 0;  // Tail
  snake[1][0] = 5; snake[1][1] = 1;  // Head
  
  // Place first apple
  placeApple();
}

void loop() {
  //Handle joystick input 
  readJoystick();

  //Move snake at regular intervals
  if (millis() - lastMove > moveDelay) {
    moveSnake();
    lastMove = millis();
  }

    // Update display
  drawGame();
}


void readJoystick() {
  int xVal = analogRead(joyX);
  int yVal = analogRead(joyY);

 // Change direction 
  if (xVal < 100 && dirY == 0) {    // Left
    dirX = 0; dirY = -1;
  } else if (xVal > 920 && dirY == 0) {  // Right
    dirX = 0; dirY = 1;
  } else if (yVal < 100 && dirX == 0) {  // Up
    dirX = -1; dirY = 0;
  } else if (yVal > 920 && dirX == 0) {  // Down
    dirX = 1; dirY = 0;
  }
}

void moveSnake(){
  //Calculate new head position
  headX += dirX;
  headY += dirY;

  // Wrap around screen edges
  if (headX < 0) headX = 7;
  if (headX > 7) headX = 0;
  if (headY < 0) headY = 7;
  if (headY > 7) headY = 0;
  
  // Check for collision
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i][0] == headX && snake[i][1] == headY) {
      gameOver();
      return;
    }
}

  // Check if the apple was eaten
  if (headX == appleX && headY == appleY) {
    snakeLength++;
    placeApple();
    // Speed up slightly
    if (moveDelay > 100) moveDelay -= 10;
  } else {
    // Remove tail
    for (int i = 0; i < snakeLength - 1; i++) {
      snake[i][0] = snake[i+1][0];
      snake[i][1] = snake[i+1][1];
    }
  }
  
  // Add new head
  snake[snakeLength-1][0] = headX;
  snake[snakeLength-1][1] = headY;
}

void placeApple() {
  bool onSnake;
  do {
    onSnake = false;
    appleX = random(8);
    appleY = random(8);
    
    // Make sure apple doesn't spawn on snake
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i][0] == appleX && snake[i][1] == appleY) {
        onSnake = true;
        break;
      }
    }
  } while (onSnake);
}

void drawGame() {
  // Clear grid
  for (int i = 0; i < 8; i++) {
    grid[i] = 0;
  }
  
  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    grid[snake[i][0]] |= (1 << (7 - snake[i][1]));
  }

  // Draw apple
  grid[appleX] |= (1 << (7 - appleY));
  
  // Update display
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, grid[i]);
  }
}

void gameOver() {
  // Flash display
  for (int i = 0; i < 3; i++) {
    lc.clearDisplay(0);
    delay(200);
    drawGame();
    delay(200);
  }
  
  // Reset game
  snakeLength = 2;
  headX = 5; headY = 1;
  dirX = 0; dirY = 1;
  snake[0][0] = 5; snake[0][1] = 0;
  snake[1][0] = 5; snake[1][1] = 1;
  moveDelay = 300;
  placeApple();
}

