# snake-game
I built a classic Snake game where the snake moves around the screen and must eat apples to grow longer and earn points. The game ends if the snake hits the walls or its own body.

For the hardware part I used:
  ~ 1 joystick module (with X/Y axes)                                                            ~ an 8x8 LED Matrix controlled by MAX7219 chip                                                 ~ an Arduino Uno Rev3 Board

In the software part, the display uses the MAX7219 LED driver library and shows the snake as individual lit LEDs. The game implements wrapping boundaries - when the snake reaches one edge, it reappears on the opposite side. Each apple eaten increases the snake's length by one unit and slightly increases game speed. If the snake collides with itself, the game flashes the display and resets. Apples spawn randomly in positions not occupied by the snake's body.
