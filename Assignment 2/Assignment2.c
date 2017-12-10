//----------------------------------------
// CAB202 - Assignment 2 - ANSITower
//----------------------------------------
// Author: John Santias - n9983244
//----------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <graphics.h>
#include <lcd.h>
#include <macros.h>
#include <lcd_model.h>
#include <stdbool.h>
#include <sprite.h>
#include <ascii_font.h>
#include "usb_serial.h"
#include "GameSprites.h"

#define LCD_X 84
#define LCD_Y 48
#define BorderLRW 1
#define BorderLRH 104
#define BorderTBW 136
#define BorderTBH 1

#define playerWidth 8
#define playerHeight 10
#define doorWidth 13
#define doorHeigth 10
#define keyWidth 7
#define keyHeight 3
#define monsterWidth 5
#define monsterHeight 6
#define treasureWidth 5
#define treasureHeight 3
#define shieldWidth 8
#define shieldHeight 6
#define bombWidth 4
#define bombHeight 5
#define bowarrowWidth 8
#define bowarrowHeight 5
#define arrowWH 2

#define verticalwallHeight 24
#define verticalwallWidth 3
#define horizwallHeight 3
#define horizwallWidth 32

#define castleHeight 30
#define castleWidth 72

#define FREQ     (8000000.0)
#define PRESCALE (1024.0)

//global variables
bool run = true;
bool gameover = false;
bool intro = true;
bool cdown = true;
bool paused = false;
bool keyCollected = false;
bool shieldCollected = false;
bool bombCollected = false;
bool bowarrowCollected = false;
bool arrowfired = false;
bool used = false;
bool usb = false;
bool placeshield = false;
bool placebomb = false;
bool placebowarrow = false;
bool placetreasure = false;

bool Punder = false;
bool Pabove = false;
bool Pleft = false;
bool Pright = false;

char words[20];

//game stats
int whatLevel;
int Minutes;
int milliseconds = 0;
int seconds;
int Seconds2 = 3; //for countdown
int Score;
int Lives;
int ammo;
int xaxis;
int yaxis;
int time; //seconds
int left;
int top;
int width;
int height;
int playerX;
int playerY;

int posneg; // 0 for positive, 1 for negative

//for printing to USB debugger screen
char buffer[80];

//directions, left = 1, right = 2, up = 3, down = 4
int direction = 0; 

//game token
int token = 0;

//sprites
Sprite player;
Sprite treasure;
Sprite shield;
Sprite bomb;
Sprite key;
Sprite monster;
Sprite door;
Sprite castle;
Sprite bowarrow;
Sprite arrow;

Sprite borderTop;
Sprite borderBottom;
Sprite borderLeft;
Sprite borderRight;

Sprite horizWal[3];
Sprite vertiWal[3];

void setup_usb_serial( void );
void usb_serial_send(char * message);

void debug_string(char* characters){
     unsigned char chars = 0;
     while (*characters != '\0') {
         usb_serial_putchar(*characters);
         characters++;
         chars++;
     }
     char * buffer = "\r\n";
    usb_serial_send( buffer );
}

void setup_border(void){
    draw_line(0, 0, LCD_X, 0, FG_COLOUR); //TOP
    draw_line(0, LCD_Y-1, LCD_X, LCD_Y-1, FG_COLOUR); //BOTTOM
    draw_line(LCD_X, 0, LCD_X-1, LCD_Y, FG_COLOUR); //RIGHT
    draw_line(0, 0, 0, LCD_Y, FG_COLOUR); //LEFT
}

void countdown(void){
    draw_string(12, 10, "GET READY!", FG_COLOUR);
    Seconds2 = 3;
    sprintf(words, "Countdown: %d", Seconds2);
    draw_string(10, 20, words, FG_COLOUR);
    show_screen();
    _delay_ms(300);
    Seconds2--;
    sprintf(words, "Countdown: %d", Seconds2);
    draw_string(10, 20, words, FG_COLOUR);
    show_screen();
    _delay_ms(300);
    Seconds2--;
    sprintf(words, "Countdown: %d", Seconds2);
    draw_string(10, 20, words, FG_COLOUR);
    Seconds2--;
    show_screen();
    _delay_ms(300);
}

void resetdefenceitems(void){
    shieldCollected = false;
    bowarrowCollected = false;
    shieldCollected = false;
}

bool monsterWallCol(){ //monster colliding with wall and castle
    bool collided = true;

    int px = monster.x; //left
    int py = monster.y; //top
    int pr = px + monsterWidth - 1; //right
    int pb = py + monsterHeight - 1; //bottom

    int bx = castle.x;
    int by = castle.y;
    int br = bx + castleWidth - 1;
    int bb = by + castleHeight - 1;
    
    if (pr < bx) { collided = false;}
    if (pb < by) { collided = false;}
    if (br < px) { collided = false;}
    if (bb < py) { collided = false;}

    return collided;
}

void PlayerNearMonster(void){
    if (!monsterWallCol()){
        if (monster.x > player.x + 7 + 41 || monster.x < player.x - 3 - 39 || monster.y > player.y + 29 || monster.y < player.y - 25 ){
                monster.x += 0;
                monster.y += 0;
        }
        else {
            if (monster.x <= player.x){
                monster.x += 0.1;
            }
            else if (monster.x >= player.x){
                monster.x -= 0.1;
            }
            if (monster.y >= player.y){
                monster.y -= 0.1;
            }
            else if (monster.y <= player.y){
                monster.y += 0.1;
            }
        }
    }
    else if ((monsterWallCol()&& whatLevel == 1) && monster.x > castle.x + castleWidth){
        monster.x += 2;
    }
    else if ((monsterWallCol()&& whatLevel == 1) && monster.x < castle.x){
        monster.x -=2;
    }
    else if ((monsterWallCol()&& whatLevel == 1) && monster.y < castle.y){
        monster.y -=2;
    }
    else if ((monsterWallCol()&& whatLevel == 1) && monster.y > castle.y){
        monster.y +=2;
    }
}

bool castle_collision(){
    bool collided = true;

    int px = player.x; //left
    int py = player.y; //top
    int pr = px + playerWidth - 1; //right
    int pb = py + playerHeight - 1; //bottom

    int cx = castle.x;
    int cy = castle.y;
    int cr = cx + castleWidth - 1;
    int cb = cy + castleHeight; //9for player to get into the door

    if (pr < cx) { collided = false;}
    if (pb < cy) { collided = false;}
    if (cr < px) { collided = false;}
    if (cb < py) { collided = false;}

    return collided;
}

void playerCastPos(void){
    //player under castle
    if (castle_collision() && ((player.x >= castle.x && player.y >= castle.y + castleHeight) && player.x <= castle.x + castleWidth) ){
        Punder = true;
    }
    //player above castle
    if (castle_collision() && ((player.x >= castle.x && player.y <= castle.y) && player.x <= castle.x + castleWidth) ){
        Pabove = true;
    }

    //player left side of castle
    if (castle_collision() &&((player.y > castle.y && player.y < castle.y + castleHeight) && player.x < castle.x)){
        Pleft = true;
    }
    //player right side of castle
    if (castle_collision() && ((player.y > castle.y && player.y < castle.y + castleHeight) && player.x > castle.x + castleWidth) ){
        Pright = true;
    }
}

bool player_collision(Sprite who, int width, int height){
    bool collided = true;

    int px = player.x; //left
    int py = player.y; //top
    int pr = px + playerWidth; //right
    int pb = py + playerHeight; //bottom

    int kx = who.x;
    int ky = who.y;
    int kr = kx + width - 1;
    int kb = ky + height - 1;

    if (pr < kx) { collided = false;}
    if (pb < ky) { collided = false;}
    if (kr < px) { collided = false;}
    if (kb < py) { collided = false;}

    return collided;
}



bool MonsterShield_collision(){
    bool collided = true;

    int sx = shield.x; //left
    int sy = shield.y; //top
    int sr = sx + shieldWidth - 1; //right
    int sb = sy + shieldHeight - 1; //bottom

    int mx = monster.x;
    int my = monster.y;
    int mr = mx + monsterWidth - 1;
    int mb = my + monsterHeight - 1;
    
    if (sr < mx) { collided = false;}
    if (sb < my) { collided = false;}
    if (mr < sx) { collided = false;}
    if (mb < sy) { collided = false;}

    return collided;
}

bool MonsterBomb_collision(){ //when bomb explodes
    bool collided = true;

    int bx = bomb.x - 40; //left
    int by = bomb.y - 40; //top
    int br = bx + 40 + 40 + bombWidth; //right
    int bb = by + 40  + 40 + bombHeight; //bottom

    int mx = monster.x;
    int my = monster.y;
    int mr = mx + monsterWidth - 1;
    int mb = my + monsterHeight - 1;
    
    if (br < mx) { collided = false;}
    if (bb < my) { collided = false;}
    if (mr < bx) { collided = false;}
    if (mb < by) { collided = false;}

    return collided;
}

void pause(void){
    setup_border();
    draw_string(LCD_X / 6, 3, "P a u s e d", FG_COLOUR); //title

    sprintf(words, "Floor: %d", whatLevel);
    draw_string(LCD_X / 6, 13, words, FG_COLOUR); //floor

    sprintf(words, "Time: %02d:%02d", Minutes, time);
    draw_string(LCD_X / 6, 21, words, FG_COLOUR); //Time

    sprintf(words, "Score: %d", Score);
    draw_string(LCD_X / 6, 29, words, FG_COLOUR); //Score

    sprintf(words, "Lives: %d", Lives);
    draw_string(LCD_X / 6, 37, words, FG_COLOUR); //Lives
}

void loadingscreen(void){
    clear_screen();
    draw_string(LCD_X / 2 - 30, 10, "L O A D I N G", FG_COLOUR);
    sprintf(words, "Floor: %d", whatLevel);
    draw_string(LCD_X / 5, 23, words, FG_COLOUR); //floor
    sprintf(words, "Score: %d", Score);
    draw_string(LCD_X / 5, 33, words, FG_COLOUR); //Score
    LCD_CMD(lcd_set_display_mode, lcd_display_inverse);
    show_screen();
    _delay_ms(600);
}

void resetlooks(void){
    lcd_clear();
    clear_screen();
    LCD_CMD(lcd_set_display_mode, lcd_display_normal);
    setup_border();
}

unsigned int rand_number(unsigned int min, unsigned int max)
{
    const unsigned int range = 1 + max - min;
    const unsigned int a = RAND_MAX / range;
    const unsigned int limit = a * range;
    int s;
    do{
        s = rand();
    } while (s >= limit);

    return min + (s / a);
}

void rand_position(void){
    treasure.x = rand_number(borderLeft.x + 3,  borderRight.x);
    treasure.y = rand_number(borderTop.y + 3,  borderBottom.y);
    monster.x = rand_number(borderLeft.x + 3,  borderRight.x);
    monster.y = rand_number(borderTop.y + 3,  borderBottom.y);
    key.x = rand_number(borderLeft.x + 3,  borderRight.x);
    key.y = rand_number(borderTop.y + 3,  borderBottom.y);
    door.x = rand_number(borderLeft.x + 3,  borderRight.x);
    door.y = rand_number(borderTop.y + 3,  borderBottom.y);
    if (placebomb){
        bomb.x = rand_number(borderLeft.x + 3,  borderRight.x);
        bomb.y = rand_number(borderTop.y + 3,  borderBottom.y);
    }
    if (placebowarrow){
        bowarrow.x = rand_number(borderLeft.x + 3,  borderRight.x);
        bowarrow.y = rand_number(borderTop.y + 3,  borderBottom.y);
    }
    if (placeshield){
        shield.x = rand_number(borderLeft.x + 3,  borderRight.x);
        shield.y = rand_number(borderTop.y + 3,  borderBottom.y);
    }
    if (placetreasure){
        treasure.x = rand_number(borderLeft.x + 3,  borderRight.x);
        shield.y = rand_number(borderTop.y + 3,  borderBottom.y);
    }
}

void spritescrollmap(void){
    int xaxis = rand_number(borderLeft.x + BorderLRW, borderRight.x); //normal width of map
    int yaxis = rand_number(borderTop.y + BorderTBH, borderBottom.y); //height of the map
    if (xaxis < castle.x){
        monster.x -= xaxis;
        key.x -= xaxis;
        door.x -= xaxis;
        bomb.x -= xaxis;
        bowarrow.x -= xaxis;
        shield.x -= xaxis;
        borderTop.x -= xaxis;
        borderBottom.x -= xaxis;
        borderLeft.x -= xaxis;
        borderRight.x -= xaxis;
        for (int i = 0; i < 3; i++){
            horizWal[i].x -= xaxis;
            vertiWal[i].x -= xaxis;
        }
    }
    if (xaxis >= castle.x){
        monster.x += xaxis;
        key.x += xaxis;
        door.x += xaxis;
        bomb.x += xaxis;
        bowarrow.x += xaxis;
        shield.x += xaxis;
        borderTop.x += xaxis;
        borderBottom.x += xaxis;
        borderLeft.x += xaxis;
        borderRight.x += xaxis;
        for (int i = 0; i < 3; i++){
            horizWal[i].x += xaxis;
            vertiWal[i].x += xaxis;
        }
    }
    if (yaxis < castle.y){
        monster.y -= yaxis;
        key.y -= yaxis;
        door.y -= yaxis;
        bomb.y -= yaxis;
        bowarrow.y -= yaxis;
        shield.y -= yaxis;
        borderTop.y -= yaxis;
        borderBottom.y -= yaxis;
        borderLeft.y -= yaxis;
        borderRight.y -= yaxis;
        for (int i = 0; i < 3; i++){
            horizWal[i].y -= yaxis;
            vertiWal[i].y -= yaxis;
        }
    }
    if (yaxis >= castle.y){
        monster.y += yaxis;
        key.y += yaxis;
        door.y += yaxis;
        bomb.y += yaxis;
        bowarrow.y += yaxis;
        shield.y += yaxis;
        borderTop.y += yaxis;
        borderBottom.y += yaxis;
        borderLeft.y += yaxis;
        borderRight.y += yaxis;
        for (int i = 0; i < 3; i++){
            horizWal[i].y += yaxis;
            vertiWal[i].y += yaxis;
        }
    }

    //always set the castle to its position
    if (whatLevel == 0){
        castle.y = borderTop.y + BorderTBH + 27;
        castle.x = borderLeft.x + BorderLRW + 36;
    }
}

void randomlyplacewalls(void){
    for (int i = 0; i < 3; i++){
            horizWal[i].x = rand_number(borderLeft.x + BorderLRW, borderRight.x - horizwallWidth);
            horizWal[i].y = rand_number(borderTop.y + BorderTBH, borderBottom.y - 4);
            rand_number(borderTop.y + BorderTBH, borderBottom.y - 3);
        }
    vertiWal[0].x = rand_number(horizWal[0].x - horizwallWidth, horizWal[0].x + horizwallWidth);
    vertiWal[0].y = rand_number(horizWal[0].y - horizwallHeight, horizWal[0].y + horizwallHeight);
    //for vert walls
    for (int i = 1; i < 3; i++){
        vertiWal[i].x = rand_number(borderLeft.x + BorderLRW, borderRight.x - verticalwallWidth);
        vertiWal[i].y = rand_number(borderTop.y + BorderTBH, borderBottom.y - verticalwallHeight);
        rand_number(borderTop.y + BorderTBH, borderBottom.y - 3);
    }
}

void setup(void){
    set_clock_speed(CPU_8MHz);

    //Timer 0 in normal mode
    TCCR0A = 0;
    TCCR0B = 5; 
    TIMSK0 = 1; 

    //enable timer overflow
    sei();

    setup_usb_serial();

    //game stats
    whatLevel = 0;
    Minutes = 0;
    time = 0;
    Score = 0;
    Lives = 3;

    //draw border
    setup_border();

    draw_string(LCD_X / 2 - 26, LCD_Y / 2 - 20, "ANSI TOWER", FG_COLOUR);
    draw_string(LCD_X / 2 - 31, LCD_Y / 2 - 10, "John Santias", FG_COLOUR);
    draw_string(LCD_X / 2 - 27, LCD_Y / 2 + 0, " n9983244", FG_COLOUR);
    draw_string(LCD_X / 2 - 27, LCD_Y / 2 + 10, "  READY?", FG_COLOUR);

    sprite_init(&borderTop, -20, -(BorderTBH) - 54, BorderTBW, BorderTBH, TB);
    sprite_init(&borderBottom, -20, 48, BorderTBW, BorderTBH, TB);
    sprite_init(&borderLeft, -20 - BorderLRW, -(BorderTBH) - 54, BorderLRW, BorderLRH, LR);
    sprite_init(&borderRight, 115, -(BorderTBH) - 54, BorderLRW, BorderLRH, LR);

    sprite_init(&player, LCD_X / 2 - 5, LCD_Y/2 - 5, playerWidth, playerHeight, SpritePlayer);
    sprite_init(&castle, LCD_X / 2 - 36, -27, castleWidth, castleHeight, level1);
    sprite_init(&door, castle.x + castleWidth/2 - 7, castle.y + 20, doorWidth, doorHeigth, SpriteDoor);
    
    for (int i = 0; i < 3; i++){
        sprite_init(&horizWal[i], 1000100, LCD_Y - 6, horizwallWidth, horizwallHeight, HorizWall);
        sprite_init(&vertiWal[i], 1000100, LCD_Y - 6, verticalwallWidth, verticalwallHeight, VertWall);
    }
    
    sprite_init(&key, 4, castle.y + 35, keyWidth, keyHeight, SpriteKey);
    sprite_init(&monster, LCD_X - 9, castle.y + 35 , monsterWidth, monsterHeight, SpriteMonster);
    sprite_init(&treasure, 1000000, LCD_Y -6 , treasureWidth, treasureHeight, SpriteTreasure);
    sprite_init(&shield, 1000000, LCD_Y -6 , shieldWidth, shieldHeight, SpriteShield);
    sprite_init(&bowarrow, 1000000, LCD_Y -6 , bowarrowWidth, bowarrowHeight, SpriteBow);
    sprite_init(&arrow, 1000000, LCD_Y -6 , arrowWH, arrowWH, SpriteArrow);
    sprite_init(&bomb, 1000000, LCD_Y -6 , bombWidth, bombHeight, SpriteBomb);

    //sprite initial speed values
    player.dx = 3.0;
    player.dy = 2.0;
    arrow.dx = 6;
    arrow.dy = 4;

    //enable output to LED0 and LED1
    SET_BIT(DDRB, 2); //LED0
    SET_BIT(DDRB, 3); //LED1
    //turn off LED0 and LED1.
    PORTB = 0;

    //enable input from the switches SW1 and SW2
    CLEAR_BIT(DDRF, 6); //left button
    CLEAR_BIT(DDRF, 5); //right button

    //enable input from Pot0 and Pot1
    CLEAR_BIT(DDRF, 0); //left
    CLEAR_BIT(DDRF, 1); //right

    //enable input from the analog sticks
    CLEAR_BIT(DDRD, 1); //UP
    CLEAR_BIT(DDRD, 0); //RIGHT
    CLEAR_BIT(DDRB, 1); //LEFT
    CLEAR_BIT(DDRB, 7); //DOWN
    CLEAR_BIT(DDRB, 0); //CENTRE

    //initialise default contrast setting
    lcd_init(LCD_DEFAULT_CONTRAST);

    show_screen();
}
volatile int overflow_counter = 0;
ISR(TIMER0_OVF_vect) {
    overflow_counter++;
}

void process(void){
    clear_screen();
    setup_border();
    time = ( overflow_counter * 256.0 + TCNT0 ) * PRESCALE  / FREQ;

    if (Lives <= 0){
        gameover = true;
    }
    else{
        if (Lives >= 1){
            playerCastPos();
            sprite_draw(&player);
            sprite_draw(&door);
            sprite_draw(&key);
            sprite_draw(&monster);
            if (whatLevel == 0){
                sprite_draw(&castle);
            }
            sprite_draw(&bomb);
            sprite_draw(&treasure);
            sprite_draw(&bowarrow);
            sprite_draw(&arrow);
            sprite_draw(&shield);
            sprite_draw(&borderTop);
            sprite_draw(&borderTop);
            sprite_draw(&borderBottom);
            sprite_draw(&borderLeft);
            sprite_draw(&borderRight);

            for (int i = 0; i < 3; i++){
                sprite_draw(&vertiWal[i]);
                sprite_draw(&horizWal[i]);
            }

            PlayerNearMonster();
            // if player collides with the key
            if (player_collision(key, keyWidth, keyHeight)){
                // key.is_visible = 0;
                keyCollected = true;
                debug_string("Key Collected\r\n");
            }
            // if player collides with the treasure
            if (player_collision(treasure, treasureWidth, treasureHeight)){
                treasure.is_visible = 0;
                treasure.x = 10000;
                Score = Score + 10;
                debug_string("Treasure Collected\r\n");
            }
            // if player collides with the shield
            if (player_collision(shield, shieldWidth, shieldHeight)){
                shieldCollected = true;
                bombCollected = false;
                bowarrowCollected = false;
                debug_string("Shield Collected\r\n");
            }

            // if player collides with the bomb
            if (player_collision(bomb, bombWidth, bombHeight)){
                bombCollected = true;
                shieldCollected = false;
                bowarrowCollected = false;
                debug_string("Bomb Collected\r\n");
            }

            // if player collides with the bow and arrow
            if (player_collision(bowarrow, bowarrowWidth, bowarrowHeight)){
                bowarrowCollected = true;
                shieldCollected = false;
                bombCollected = false;
                ammo = 5;
                debug_string("Bow and Arrow Collected\r\n");
            }

            // if player collides with the monster
            if (player_collision(monster, monsterWidth, monsterHeight)){
                Lives--;
                keyCollected = false;
                spritescrollmap();
                debug_string("Player collided with monster\r\n");
            }

            //if shield collides with the monster, destroy both
            if (MonsterShield_collision()){
                shieldCollected = false;
                shield.is_visible = 0;
                shield.x = 1000000;
                monster.is_visible = 0;
                monster.x = 110000;
                Score = Score + 10;
                debug_string("Shield hit and killed the monster\r\n");
            }

            if (bowarrowCollected && (BIT_IS_SET(PINF, 6) ||  BIT_IS_SET(PINF, 5))){
                arrowfired = true;
                ammo -= 1;
            }

            if (bowarrowCollected && ammo == 0){ 
                bowarrow.x = 1000000;
                bowarrowCollected = false; 
            }

            if (arrowfired){
                if (direction == 1){ arrow.x -= 5; }
                else if (direction == 2){ arrow.x += 5; }
                else if (direction == 3){ arrow.y -= 5; }
                else if (direction == 4){ arrow.y += 5; }
            }

            if (Lives == 0){
                debug_string("Game over\r\n");
            }

            if (bombCollected && (BIT_IS_SET(PINF, 6) ||  BIT_IS_SET(PINF, 5))){
                if (direction == 1){ //left
                    bomb.x = player.x - 18;
                    used = true;
                }
                else if (direction == 2){ //right
                    bomb.x = player.x + 18 + playerWidth;
                    used = true;
                }
                else if (direction == 3){ //up
                    bomb.y = player.y - 20;
                    used = true;
                }
                else if (direction == 4){ //down
                    bomb.y = player.y + 20 + playerHeight;
                    used = true;
                }
                if (used){
                    SET_BIT(PORTB, 2);
                    SET_BIT(PORTB, 3);
                    _delay_ms(100);
                    CLEAR_BIT(PORTB, 2);
                    CLEAR_BIT(PORTB, 3);
                    bombCollected = false;
                    if (MonsterBomb_collision()){
                        monster.x = 10000;
                        Score = Score + 10;
                        debug_string("Bomb killed monster\r\n");
                    }
                    used = false;
                    bomb.is_visible = 0;
                    bomb.x = 1000000;
                }
            }

            if (castle_collision()){
                castle.y -= 3;
                monster.y -= 3;
                key.y -= 3;
                door.y -= 3;
                bomb.y -= 3;
                bowarrow.y = 3;
                shield.y -= 3;
                borderTop.y -= 3;
                borderBottom.y -= 3;
                borderLeft.y -= 3;
                borderRight.y -= 3;
            }

            //press center button to pause
            if (BIT_IS_SET(PINB, 0)){
                clear_screen();
                pause();
            }

            if (player_collision(door, doorWidth, doorHeigth + 2) && keyCollected){
                debug_string("Going to next level\r\n");
                Score = Score + 100;
                whatLevel++;
                loadingscreen();
                _delay_ms(2000);
                LCD_CMD(lcd_set_display_mode, lcd_display_normal);
                keyCollected = false;
                rand_position();
                castle.is_visible = 0;
                castle.x = 100000;
                monster.is_visible = 1;
                shield.is_visible = 1;
                bowarrow.is_visible = 1;
                treasure.is_visible = 1;
                bomb.is_visible = 1;
                rand_position();
                randomlyplacewalls();
                int decide1 = rand() % 100;
                int decide2 = rand() % 100;
                int decide3 = rand() % 100;
                int decide4 = rand() % 100;
                if (decide1 <= 30){
                    placebomb = true;
                    rand_position();
                    placebomb = false;
                }
                else{ bomb.x = 1000000;}

                if (decide2 <= 30 || whatLevel == 1){
                    placebowarrow = true;
                    rand_position();
                    placebowarrow = false;
                }
                else{ 
                    bowarrow.x = 1000000;
                    arrow.x = 1000000;
                }
                if (decide3 <= 30){
                    placeshield = true;
                    rand_position();
                    placeshield = false;
                }
                if (decide4 <= 30){
                    placetreasure = true;
                    rand_position();
                    placetreasure = false;
                }
            }

            //Up joystick, move up
            if (BIT_IS_SET(PIND, 1) && player.y > borderTop.y + 2){
                // player.y -= player.dy;
                if (!castle_collision()){
                    castle.y += player.dy;
                    treasure.y += player.dy;
                    monster.y += player.dy;
                    key.y += player.dy;
                    door.y += player.dy;
                    bomb.y += player.dy;
                    bowarrow.y += player.dy;
                    shield.y += player.dy;
                    borderTop.y += player.dy;
                    borderBottom.y += player.dy;
                    borderLeft.y += player.dy;
                    borderRight.y += player.dy;
                    for (int i = 0; i < 3; i++){
                        horizWal[i].y += player.dy;
                        vertiWal[i].y += player.dy;
                    }
                    direction = 3;
                    if (keyCollected){
                        key.x = player.x;
                        key.y = player.y + 14;
                        key.y -= player.dy;
                    }
                    if (shieldCollected){
                        shield.x = player.x;
                        shield.y = player.y - 6;
                        shield.y -= player.dy;
                    }
                    if (bombCollected){
                        bomb.x = player.x + 2;
                        bomb.y = player.y - 5;
                        bomb.y -= player.dy;
                    }
                    if (bowarrowCollected){
                        bowarrow.x = player.x;
                        bowarrow.y = player.y -5;
                        bowarrow.y -= player.dy;
                        arrow.x = bowarrow.x;
                        arrow.y = bowarrow.y + 1;
                    }
                }

                else if (castle_collision() && Punder){
                    castle.y -= 3;
                    monster.y -= 3;
                    key.y -= 3;
                    door.y -= 3;
                    bomb.y -= 3;
                    bowarrow.y = 3;
                    shield.y -= 3;
                    borderTop.y -= 3;
                    borderBottom.y -= 3;
                    borderLeft.y -= 3;
                    borderRight.y -= 3;
                }
            }

            //Down joystick, move down
            if (BIT_IS_SET(PINB, 7) && player.y + 10 < borderBottom.y - 1){
                // player.y += player.dy;
                if (!castle_collision()){
                    castle.y -= player.dy;
                    treasure.y -= player.dy;
                    monster.y -= player.dy;
                    key.y -= player.dy;
                    door.y -= player.dy;
                    bomb.y -= player.dy;
                    bowarrow.y -= player.dy;
                    shield.y -= player.dy;
                    borderTop.y -= player.dy;
                    borderBottom.y -= player.dy;
                    borderLeft.y -= player.dy;
                    borderRight.y -= player.dy;
                    for (int i = 0; i < 3; i++){
                        horizWal[i].y -= player.dy;
                        vertiWal[i].y -= player.dy;
                    }
                    direction = 4;
                    if (keyCollected){
                        key.x = player.x;
                        key.y = player.y - 5;
                        key.y += player.dy;
                    }
                    if (shieldCollected){
                        shield.x = player.x;
                        shield.y = player.y + 13;
                        shield.y -= player.dy;
                    }
                    if (bombCollected){
                        bomb.x = player.x + 2;
                        bomb.y = player.y + 15;
                        bomb.y -= player.dy;
                    }
                    if (bowarrowCollected){
                        bowarrow.x = player.x;
                        bowarrow.y = player.y + 13;
                        bowarrow.y -= player.dy;
                        arrow.x = bowarrow.x;
                        arrow.y = bowarrow.y + 1;
                    }
                }
                else if (castle_collision() && Pabove){
                    castle.y += 5;
                    monster.y += 5;
                    key.y += 5;
                    door.y += 5;
                    bomb.y += 5;
                    bowarrow.y += 5;
                    shield.y += 5;
                    borderTop.y += 5;
                    borderBottom.y += 5;
                    borderLeft.y += 5;
                    borderRight.y += 5;
                }
            }

            //Left joystick, move left
            if (BIT_IS_SET(PINB, 1) && player.x > borderLeft.x + 2){
                // player.x -= player.dx;
                if (!castle_collision()){
                    castle.x += player.dx;
                    treasure.x += player.dx;
                    monster.x += player.dx;
                    key.x += player.dx;
                    door.x += player.dx;
                    bomb.x += player.dx;
                    bowarrow.x += player.dx;
                    shield.x += player.dx;
                    borderTop.x += player.dx;
                    borderBottom.x += player.dx;
                    borderLeft.x += player.dx;
                    borderRight.x += player.dx;
                    for (int i = 0; i < 3; i++){
                        horizWal[i].x += player.dx;
                        vertiWal[i].x += player.dx;
                    }
                    direction = 1;
                    if (keyCollected){
                        key.x = player.x + 12;
                        key.y = player.y + 7;
                        key.x -= player.dx;
                    }
                    if (shieldCollected){
                        shield.x = player.x - 12;
                        shield.y = player.y + 4;
                        shield.y -= player.dy;
                    }
                    if (bombCollected){
                        bomb.x = player.x - 5;
                        bomb.y = player.y + 5;
                        bomb.y -= player.dy;
                    }
                    if (bowarrowCollected){
                        bowarrow.x = player.x - 10;
                        bowarrow.y = player.y + 4;
                        bowarrow.y -= player.dy;
                        arrow.x = bowarrow.x;
                        arrow.y = bowarrow.y + 1;
                    }
                }
                else if (castle_collision() && Pright){
                    castle.x -= 5;
                    monster.x -= 5;
                    key.x -= 5;
                    door.x -= 5;
                    bomb.x -= 5;
                    bowarrow.x -= 5;
                    shield.x -= 5;
                    borderTop.x -= 5;
                    borderBottom.x -= 5;
                    borderLeft.x -= 5;
                    borderRight.x -= 5;
                }
            }

            //Right joystick, move right
            if (BIT_IS_SET(PIND, 0) && player.x + 9 < borderRight.x){
                // player.x += player.dx;
                if (!castle_collision()){
                    castle.x -= player.dx;
                    treasure.x -= player.dx;
                    monster.x -= player.dx;
                    key.x -= player.dx;
                    door.x -= player.dx;
                    bomb.x -= player.dx;
                    bowarrow.x -= player.dx;
                    shield.x -= player.dx;
                    borderTop.x -= player.dx;
                    borderBottom.x -= player.dx;
                    borderLeft.x -= player.dx;
                    borderRight.x -= player.dx;
                    for (int i = 0; i < 3; i++){
                        horizWal[i].x -= player.dx;
                        vertiWal[i].x -= player.dx;
                    }
                    direction = 2;
                    if (keyCollected){
                        key.x = player.x - 7;
                        key.y = player.y + 7;
                        key.x -= player.dx;
                    }
                    if (shieldCollected){
                        shield.x = player.x + 12;
                        shield.y = player.y + 4;
                        shield.y -= player.dy;
                    }
                    if (bombCollected){
                        bomb.x = player.x + 11;
                        bomb.y = player.y + 2;
                        bomb.y -= player.dy;
                    }
                    if (bowarrowCollected){
                        bowarrow.x = player.x + 10;
                        bowarrow.y = player.y + 4;
                        bowarrow.y -= player.dy;
                        arrow.x = bowarrow.x;
                        arrow.y = bowarrow.y + 1;
                    }
                }
                else if (castle_collision() && Pleft){
                    castle.x += 5;
                    monster.x += 5;
                    key.x += 5;
                    door.x += 5;
                    bomb.x += 5;
                    bowarrow.x += 5;
                    shield.x += 5;
                    borderTop.x += 5;
                    borderBottom.x += 5;
                    borderLeft.x += 5;
                    borderRight.x += 5;
                }
            }
        }
        show_screen();
    }
}

int main(void){
    while (run){
        lcd_clear();
        setup();

        while (intro){
            if (BIT_IS_SET(PINF, 6) || BIT_IS_SET(PINF, 5)){
                token = 1;
                time = 0;
                overflow_counter = 0;
                gameover = false;
                lcd_clear();
                clear_screen();
                cdown = false;
            }

            while (!cdown){
                setup_border();
                countdown();
                if (Seconds2 == 0){
                    lcd_clear();
                    clear_screen();
                    cdown = true;
                    intro = false;
                }
            }
        }
        while (token == 1){
            while (!gameover){
                if (time == 60){
                    Minutes++;
                    overflow_counter = 0;
                    time = 0;
                }
                milliseconds++;
                if (milliseconds == 4){
                    snprintf(buffer, 56, "Time: %02d:%02d  Score: %d  Floor: %d  Lives: %d \r\n", Minutes, time, Score, whatLevel, Lives);
                    usb_serial_send( buffer );
                    milliseconds = 0;
                }
                process();
                _delay_ms(100);
                
            }

            if (gameover){
                clear_screen();

                //game over screen
                draw_string(0, 5, " G A M E O V E R ", FG_COLOUR);
                draw_string(LCD_X / 2 - 26, 35, " Continue? ", BG_COLOUR);
                sprintf(words, "Final Floor: %d", whatLevel);
                draw_string(4, 16, words, FG_COLOUR); //floor
                sprintf(words, "Score: %d", Score);
                draw_string(4, 26, words, FG_COLOUR); //Score
                LCD_CMD(lcd_set_display_mode, lcd_display_inverse);
                show_screen();
                if (BIT_IS_SET(PINF, 6) || BIT_IS_SET(PINF, 5)){
                    intro = true;
                    cdown = true;
                    token = 0;
                    Seconds2 = 3;
                    time = 0;
                    milliseconds = 0;
                    Minutes = 0;
                    resetdefenceitems();
                    lcd_clear();
                    clear_screen();
                }
            }
        }
    }
    return 0;
}

// ---------------------------------------------------------
//  USB serial business.
// ---------------------------------------------------------

void usb_serial_send(char * message) {
    usb_serial_write((uint8_t *) message, strlen(message));
}

void setup_usb_serial(void) {
    // Set up LCD and display message
    lcd_init(LCD_DEFAULT_CONTRAST);
    draw_string(10, 10, "Connect USB...", FG_COLOUR);
    show_screen();

    usb_init();
    if (!usb){
        while ( !usb_configured() ) {
        // Block until USB is ready.
        }
    }
    
    usb = true;
    char * buffer = "Welcome\r\n";
    usb_serial_send( buffer );
    clear_screen();
    draw_string(10, 10, "USB connected", FG_COLOUR);
    show_screen();
    _delay_ms(100);
    clear_screen();
}