#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <stdio.h>
#include <curses.h> 

#define DELAY (50)
#define SCREEN_W (120)
#define SCREEN_H (50)
#define AVATAR_HEIGHT (3)
#define AVATAR_WIDTH (3)
#define ZOMBIE_HEIGHT (4)
#define ZOMBIE_WIDTH (4)
#define SPIKE_W (3)
#define SPIKE_H (2)
#define EXIT_HEIGHT (4)
#define EXIT_WIDTH (4)
#define PLAT_H (1)
#define PLAT1_W (120)
#define L1_W (40)
#define PLAT2_W (30)
#define PLAT3_W (12)
#define BOULDER_WIDTH (4)
#define BOULDER_HEIGHT (2)
#define L4APLAT_W (22)
#define L4BPLAT_W (90)
#define L4CPLAT_W (80)
#define GATE_W (2)
#define GATE_H (11)
#define KEY_W (3)
#define KEY_H (1)

//game stats
bool game_over = false; //set to true when game over
bool update_screen = true; //set to false to prevent screen update
int avatar_score = 0, level = 1, life = 10;
int milliseconds = 00, seconds = 00, minute = 00;

int pressed = 0; //button press counter

//booleans
bool onGround = true;
bool boulder_onGround = true;
bool moving = true;
bool treasure_collected = false;
bool key_collected = true;
bool gate_unlocked = false;

double gravity = 0.3; 

char * avatar_image =
/**/" o "
/**/"/|\\"
/**/"/ \\";

sprite_id avatar;

char * avatar_image2 =
/**/"\\o/"
/**/" | "
/**/"/ \\";

sprite_id avatar;

char * exit_image = 
/**/ "EXIT"
/**/ ":  :"
/**/ ": .:"
/**/ ":  :";

sprite_id exit_door;

char * zombie_image = 
/**/"ZZZZ"
/**/"  Z "
/**/" Z  "
/**/"ZZZZ";

sprite_id zombie;

char * spike_bat1 = 
/**/" | "
/**/"/ \\";

char * spike_bat2 = 
/**/" | "
/**/"/ \\";
sprite_id spike;

char * msg_image = 
/**/"                Congratulations!                "
/**/"                                                "
/**/"            Your final score was                "
/**/"                                                "
/**/"              Press any key to exit             ";

char * game_over_msg = 
/**/"                    Game Over                   "
/**/"                                                "
/**/"              Press any key to exit             "
/**/"                                                ";

char * L1_plat = 
/**/"========================================";
sprite_id L1_platform;
sprite_id MovePlatform;

char * plat1Bottom_img = "========================================================================================================================";
sprite_id platform1B;

char * L2_Bottomplat = "==============================";
sprite_id Aplatform2B;
sprite_id Bplatform2B;

char * L3_platform = "============";
sprite_id platform3;

char * boulder =
/**/"/\"\"\\"
/**/"\\__/";
sprite_id boulder1;

char * L4_platformA = "======================"; //level four bottom left, 22 characters
sprite_id platform4A; //bottom left

char * L4_platformB = "=========================================================================================="; //gap of 8 in between 4platformA, 90 characters
sprite_id platform4B; //bottom right

char * L4_platformC = "================================================================================"; //80 characters long
sprite_id platform4C1; //1st floating platform bottom
sprite_id platform4C2; //2nd floating platform middle
sprite_id platform4C3; //3rd floating platform top
sprite_id MovePlatform2;

sprite_id treasure; 
sprite_id gatekey;

char * gate =
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]"
/**/"[]";
sprite_id Gate1;


void setup_time(){
    milliseconds++;
    if(milliseconds == 18){
        seconds++;
        milliseconds = 00;
        if(seconds == 60){
            minute++;
            seconds = 00;
        }
    }
    draw_formatted(0, 0, "Time:  %02d:%02d", minute, seconds);
}

void setup_stats(void){
    //Show the lives
    draw_formatted(30, 0, "Lives:  %d", life);

    //Show the level
    draw_formatted(60, 0, "Level:  %d", level);

    //Show the score
    draw_formatted(90, 0, "Score:  %d", avatar_score);
}

void setup_avatar(void) {
    avatar = sprite_create(4, 46, 3, 3, avatar_image);
    sprite_draw( avatar);
}
void setup_exit(void) {
    exit_door = sprite_create(112, 45, 4, 4, exit_image);
    sprite_draw(exit_door);
}

void setup_zomb(void){
	zombie = sprite_create(112, 45, ZOMBIE_WIDTH, ZOMBIE_HEIGHT, zombie_image);
    sprite_turn_to( zombie, 0.35, 0 );
    int degrees = 180;
    sprite_turn( zombie, degrees );
}

void setup_spike(void){
	spike = sprite_create(112, 45, SPIKE_W, SPIKE_H, spike_bat1);
	sprite_turn_to( spike, 1.0, 0 );
    int degrees = 180;
    sprite_turn( spike, degrees );
}

void setup_boulder(void){
	boulder1 = sprite_create(60, 9, BOULDER_WIDTH, BOULDER_HEIGHT, boulder);
	sprite_turn_to( boulder1, 3.0, 0);
}

void setup_border(void){
    int left = 0, right = 121-1, top = 0, bottom = 50-1;
    draw_line(left, top+1, right, top+1, '-');
    draw_line(left, top+2, left, bottom-1, ':');
    draw_line(right-1, top+2, right-1, bottom-1, ':');
}

void level_1(void){
    //platform is floating and is 3.5 times the height of the avatar
    L1_platform = sprite_create(40, 37, 40, 1, L1_plat); //platform is 40 characters long
    sprite_draw(L1_platform);

    platform1B = sprite_create(0, 49, PLAT1_W, PLAT_H, plat1Bottom_img);
    sprite_draw(platform1B);
}

void level_2(void){
	L1_platform = sprite_create(40, 37, 40, 1, L1_plat); //platform is 40 characters long
    sprite_draw(L1_platform);

	Aplatform2B = sprite_create(0, 49, PLAT2_W, PLAT_H, L2_Bottomplat);
	Bplatform2B = sprite_create(90, 49, PLAT2_W, PLAT_H, L2_Bottomplat);
	sprite_draw(Aplatform2B);
	sprite_draw(Bplatform2B);
}

void level_3(void){
    Aplatform2B = sprite_create(0, 49, PLAT2_W, PLAT_H, L2_Bottomplat);
	Bplatform2B = sprite_create(90, 49, PLAT2_W, PLAT_H, L2_Bottomplat);
	sprite_draw(Aplatform2B);
	sprite_draw(Bplatform2B);

    platform3 = sprite_create(54, 23, PLAT3_W, PLAT_H, L3_platform);
    sprite_draw(platform3);
}

void level_4(void){
	platform4A = sprite_create(0, 49, L4APLAT_W, PLAT_H, L4_platformA);
	platform4B = sprite_create(30, 49, L4BPLAT_W, PLAT_H, L4_platformB);
	platform4C1 = sprite_create(0, 37, L4CPLAT_W, PLAT_H, L4_platformC);
	platform4C2 = sprite_create(40, 25,L4CPLAT_W, PLAT_H, L4_platformC);
	platform4C3 = sprite_create(0, 12,L4CPLAT_W, PLAT_H, L4_platformC);

	sprite_draw(platform4A);
	sprite_draw(platform4B);
	sprite_draw(platform4C1);
	sprite_draw(platform4C2);
	sprite_draw(platform4C3);
}

void level_5(void){
	sprite_draw(platform4A);
	sprite_draw(platform4B);
	sprite_draw(platform4C1);
	sprite_draw(platform4C3);
}

void process_collision( sprite_id who_moved){
	if (level == 1){
		double ax = (sprite_x(L1_platform)), ay = (sprite_y(L1_platform));
		double by = (sprite_y(platform1B));
	    double cx = (sprite_x(avatar)), cy = (sprite_y(avatar));
	    double ex = (sprite_x(boulder1)), ey = (sprite_y(boulder1));
		bool collided = true;
		bool collided2 = true;

		if ( ax >= cx + AVATAR_WIDTH ) collided = false;
	    if ( ay >= cy + AVATAR_HEIGHT ) collided = false;
	    if ( cx >= ax + L1_W ) collided = false;
	    if ( cy >= ay + PLAT_H) collided = false;
	    if ( cy > by - PLAT_H - 2 ) collided = true;
	    if (collided) {
	    	//if avatar hit platform
	    	if (who_moved == avatar){
	    		double x = sprite_dx(avatar), y = sprite_dy(avatar);

	    		//if avatar hits platform from below.
	    		if (cy >= ay + PLAT_H && y < 0){ 
	    			y = -y;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ax && x > 0) && cy <= ay - 1){
	    			x = 0;
	    			y += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((cx == ax + L1_W - 1 && x < 0) && (cy <= ay - 1 )){
	    			x = 0;
	    			y += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			y = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, x, y);
	    	}
	    }
	    	if ( ax >= ex + BOULDER_WIDTH ) collided2 = false;
		    if ( ay >= ey + BOULDER_HEIGHT ) collided2 = false;
		    if ( ex >= ax + L1_W ) collided2 = false;
		    if ( ey >= ay + PLAT_H) collided2 = false;
		    if ( ey > by - PLAT_H - 2 ) collided2 = true;
		if (collided2) {
	    	if (who_moved == boulder1){
	    		double x = sprite_dx(boulder1), y = sprite_dy(boulder1);
	    		//if boulder hits platform from below.
	    		if (ey >= ay + PLAT_H && y < 0){ 
	    			y = -y;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if boulder hits left edge of platform
	    		else if ((ex + BOULDER_WIDTH - 1 == ex && x > 0) && ey <= ay - 1){
	    			x = 0;
	    			y += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if boulder hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((ex == ax + L1_W - 1 && x < 0) && (ey <= ay - 1 )){
	    			x = 0;
	    			y += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if boulder lands on top of platform
	    		else {
	    			y = 0;
	    			boulder_onGround = true;
	    		}
	    		sprite_back(boulder1);
	    		sprite_turn_to (boulder1, x, y);
	    	}
	    }
	}

	if (level == 2){
		double ax = (sprite_x(L1_platform)), ay = (sprite_y(L1_platform)); //floating platform
		double by = (sprite_y(Aplatform2B)); //bottom left platform
		double dy = (sprite_y(Bplatform2B)); //bottom right platform
	    double cx = (sprite_x(avatar)), cy = (sprite_y(avatar)); //avatar's location
		bool collided = true;

		if ( ax >= cx + AVATAR_WIDTH ) collided = false;
	    if ( ay >= cy + AVATAR_HEIGHT ) collided = false;
	    if ( cx >= ax + L1_W ) collided = false; 
	    if ( cy >= ay + PLAT_H) collided = false;
	    if ( cy > by - PLAT_H - 2 && cx < PLAT2_W) collided = true;
	    if ( cy > dy - PLAT_H - 2 && cx > 120 - PLAT2_W) collided = true; //screen - platform width

	    if (collided) {
	    	//if avatar hit platform
	    	if (who_moved == avatar){
	    		double dx = sprite_dx(avatar), dy = sprite_dy(avatar);

	    		//if avatar hits platform from below.
	    		if (cy >= ay + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ax && dx > 0) && cy <= ay - 1){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((cx == ax + L1_W - 1 && dx < 0) && (cy <= ay - 1 )){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	}
	if (level == 3){
		double ax = (sprite_x(MovePlatform)), ay = (sprite_y(MovePlatform)); //floating platform
		double by = (sprite_y(Aplatform2B)); //bottom left platform
		double dy = (sprite_y(Bplatform2B)); //bottom right platform
	    double cx = (sprite_x(avatar)), cy = (sprite_y(avatar)); //avatar's location
	    double ex = (sprite_x(platform3)), ey = (sprite_y(platform3)); //platform with exit door

		bool collided1 = true; // for the first platform

		if ( ax >= cx + AVATAR_WIDTH ) collided1 = false; //platform is on the right of the avatar
	    if ( ay >= cy + AVATAR_HEIGHT ) collided1 = false; //platform is below avatar
	    if ( cx >= ax + L1_W) collided1 = false; // if avatar is below platform 1
	    if ( cy >= ay + PLAT_H) collided1 = false; // if avatar is below platform 1
	    if ( cy > by - PLAT_H - 2 && cx < PLAT2_W) collided1 = true; //if avatar is above bottom left platform
	    if ( cy > dy - PLAT_H - 2 && cx > 120 - PLAT2_W) collided1 = true; //screen - platform width, if avatar is above bottom right platform
	    // if ( cx >= ex && cx <= ax) collided = false;
	    // if ( cy >= ey && cy <= ay - 3) collided = false; // if avatar is below the height of the floating platform with exit door
	    // if ( cy == ey + 1) collided = true; //

	    if (collided1) {
	    	//if avatar hit platform
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= ay + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}

	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ax && dx > 0) && cy <= ay - 1){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((cx == ax + L1_W - 1 && dx < 0) && (cy <= ay - 1 )){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    bool collided2 = true;

	    if ( ex >= cx + AVATAR_WIDTH ) collided2 = false; //if avatar is on the left of the platform
	    if ( ey >= cy + AVATAR_HEIGHT ) collided2 = false; // if avatar is higher than the platform
	    if ( cy >= ey + PLAT_H) collided2 = false; // if avatar is lower than the platform
	    if ( cx >= ex + PLAT3_W) collided2 = false; // is avatar is on on the right of the platform

	    if (collided2){
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= ey + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ex && dx > 0) && cy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((cx == ex + PLAT3_W - 1 && dx < 0) && cy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	}
	if (level == 4){
		double ax = (sprite_x(platform4C1)), ay = (sprite_y(platform4C1)); //floating platform
		double by = (sprite_y(platform4A)); //bottom left platform
		double dy = (sprite_y(platform4B)); //bottom right platform
	    double cx = (sprite_x(avatar)), cy = (sprite_y(avatar)); //avatar's location
	    double ex = (sprite_x(platform4C2)), ey = (sprite_y(platform4C2)); //platform with exit door
	    double fx = (sprite_x(platform4C3)), fy = (sprite_y(platform4C3));
	    double gx = (sprite_x(boulder1)), gy = (sprite_y(boulder1));


		bool collided1 = true; // for the first platform

		if ( ax >= cx + AVATAR_WIDTH ) collided1 = false; //platform is on the right of the avatar
	    if ( ay >= cy + AVATAR_HEIGHT ) collided1 = false; //platform is below avatar
	    if ( cx >= ax + L4CPLAT_W ) collided1 = false; // if avatar is below platform 1
	    if ( cy >= ay + PLAT_H) collided1 = false; // if avatar is below platform 1
	    if ( cy > by - PLAT_H - 2 && cx < L4APLAT_W ) collided1 = true; //if avatar is above bottom left platform
	    if ( cy > dy - PLAT_H - 2 && cx > 120 - L4BPLAT_W ) collided1 = true; //screen - platform width, if avatar is above bottom right platform
	    // if ( cx >= ex && cx <= ax) collided = false;
	    // if ( cy >= ey && cy <= ay - 3) collided = false; // if avatar is below the height of the floating platform with exit door
	    // if ( cy == ey + 1) collided = true; //

	    if (collided1) {
	    	//if avatar hit platform
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= ay + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}

	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ax && dx > 0) && cy <= ay - 1){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((cx == ax + L1_W - 1 && dx < 0) && (cy <= ay - 1 )){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    bool collided2 = true;

	    if ( ex >= cx + AVATAR_WIDTH ) collided2 = false; //if avatar is on the left of the platform
	    if ( ey >= cy + AVATAR_HEIGHT ) collided2 = false; // if avatar is higher than the platform
	    if ( cy >= ey + PLAT_H) collided2 = false; // if avatar is lower than the platform
	    if ( cx >= ex + L4CPLAT_W) collided2 = false; // is avatar is on on the right of the platform

	    if (collided2){
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= ey + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ex && dx > 0) && cy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((cx == ex + L4CPLAT_W - 1 && dx < 0) && cy <= ey - 1){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    bool collided3 = true;

	    if ( fx >= cx + AVATAR_WIDTH ) collided3 = false; //if avatar is on the left of the platform
	    if ( fy >= cy + AVATAR_HEIGHT ) collided3 = false; // if avatar is higher than the platform
	    if ( cy >= fy + PLAT_H) collided3 = false; // if avatar is lower than the platform
	    if ( cx >= fx + L4CPLAT_W) collided3 = false; // is avatar is on on the right of the platform

	    if (collided3){
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= fy + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == fx && dx > 0) && cy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((cx == fx + L4CPLAT_W - 1 && dx < 0) && cy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    bool collided4 = true;
		if ( ax >= gx + BOULDER_WIDTH ) collided4 = false; //platform is on the right of the avatar
	    if ( ay >= gy + BOULDER_HEIGHT ) collided4 = false; //platform is below avatar
	    if ( gx >= ax + L4CPLAT_W ) collided4 = false; // if avatar is below platform 1
	    if ( gy >= ay + PLAT_H) collided4 = false; // if avatar is below platform 1
	    if ( gy > by - PLAT_H - 1 && gx < L4APLAT_W ) collided4 = true; //if avatar is above bottom left platform
	    if ( gy > dy - PLAT_H - 1 && gx > 120 - L4BPLAT_W ) collided4 = true; //screen - platform width, if avatar is above bottom right platform

	    if (collided4) {
	    	//if avatar hit platform
	    	if (who_moved == boulder1){
				double dx = sprite_dx(boulder1), dy = sprite_dy(boulder1);
	    		//if avatar hits platform from below.
	    		if (gy >= ay + L4CPLAT_W && dy < 0){ 
	    			dy = -dy;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ax && dx > 0)){
	    			// dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		// else if ((gx == ax + L4CPLAT_W - 1 && dx > 0) && (gy <= ay - 1 )){
	    		// 	dy += gravity;
	    		// 	boulder_onGround = false;
	    		// }

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			boulder_onGround = true;
	    		}

	    		sprite_back(boulder1);
	    		sprite_turn_to (boulder1, dx, dy);
	    	}
	    }
	    bool collided5 = true;

	    if ( ex >= gx + BOULDER_WIDTH ) collided5 = false; //if avatar is on the left of the platform
	    if ( ey >= gy + BOULDER_HEIGHT ) collided5 = false; // if avatar is higher than the platform
	    if ( gy >= ey + PLAT_H) collided5 = false; // if avatar is lower than the platform
	    if ( gx >= ex + L4CPLAT_W) collided5 = false; // is avatar is on on the right of the platform

	    if (collided5){
	    	if (who_moved == boulder1){
				double dx = sprite_dx(boulder1), dy = sprite_dy(boulder1);
	    		//if avatar hits platform from below.
	    		if (dy >= ey + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((gx + BOULDER_WIDTH - 1 == ex && dx > 0) && gy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((gx == ex + L4CPLAT_W - 1 && dx < 0) && gy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			boulder_onGround = true;
	    		}

	    		sprite_back(boulder1);
	    		sprite_turn_to(boulder1, dx, dy);
	    	}
	    }
	    bool collided6 = true;

	    if ( fx >= gx + BOULDER_WIDTH ) collided6 = false; //if avatar is on the left of the platform
	    if ( fy >= gy + BOULDER_HEIGHT ) collided6 = false; // if avatar is higher than the platform
	    if ( gy >= fy + PLAT_H) collided6 = false; // if avatar is lower than the platform
	    if ( gx >= fx + L4CPLAT_W) collided6 = false; // is avatar is on on the right of the platform

	    if (collided6){
	    	if (who_moved == boulder1){
				double dx = sprite_dx(boulder1), dy = sprite_dy(boulder1);
	    		//if avatar hits platform from below.
	    		if (dy >= fy + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((gx + BOULDER_WIDTH - 1 == ex && dx > 0) && gy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((gx == fx + L4CPLAT_W - 1 && dx < 0) && gy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			boulder_onGround = true;
	    		}

	    		sprite_back(boulder1);
	    		sprite_turn_to(boulder1, dx, dy);
	    	}
	    }
	}
    if (level == 5){
		double ax = (sprite_x(platform4C1)), ay = (sprite_y(platform4C1)); //floating platform
		double by = (sprite_y(platform4B)); //bottom left platform
		double dy = (sprite_y(platform4A)); //bottom right platform
	    double cx = (sprite_x(avatar)), cy = (sprite_y(avatar)); //avatar's location
	    double ex = (sprite_x(MovePlatform2)), ey = (sprite_y(MovePlatform2)); //platform with exit door
	    double fx = (sprite_x(platform4C3)), fy = (sprite_y(platform4C3));

		bool collided1 = true; // for the first platform

		if ( ax >= cx + AVATAR_WIDTH ) collided1 = false; //platform is on the right of the avatar
	    if ( ay >= cy + AVATAR_HEIGHT ) collided1 = false; //platform is below avatar
	    if ( cx >= ax + L4CPLAT_W ) collided1 = false; // if avatar is below platform 1
	    if ( cy >= ay + PLAT_H) collided1 = false; // if avatar is below platform 1
	    if ( cy > by - PLAT_H - 2 && cx < L4BPLAT_W ) collided1 = true; //if avatar is above bottom left platform
	    if ( cy > dy - PLAT_H - 2 && cx > 120 - L4APLAT_W ) collided1 = true; //screen - platform width, if avatar is above bottom right platform
	    // if ( cx >= ex && cx <= ax) collided = false;
	    // if ( cy >= ey && cy <= ay - 3) collided = false; // if avatar is below the height of the floating platform with exit door
	    // if ( cy == ey + 1) collided = true; //

	    if (collided1) {
	    	//if avatar hit platform
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= ay + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}

	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ax && dx > 0) && cy <= ay - 1){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((cx == ax + L1_W - 1 && dx < 0) && (cy <= ay - 1 )){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    bool collided2 = true;

	    if ( ex >= cx + AVATAR_WIDTH ) collided2 = false; //if avatar is on the left of the platform
	    if ( ey >= cy + AVATAR_HEIGHT ) collided2 = false; // if avatar is higher than the platform
	    if ( cy >= ey + PLAT_H) collided2 = false; // if avatar is lower than the platform
	    if ( cx >= ex + L4CPLAT_W) collided2 = false; // is avatar is on on the right of the platform

	    if (collided2){
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= ey + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == ex && dx > 0) && cy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((cx == ex + L4CPLAT_W - 1 && dx < 0) && cy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    bool collided3 = true;

	    if ( fx >= cx + AVATAR_WIDTH ) collided3 = false; //if avatar is on the left of the platform
	    if ( fy >= cy + AVATAR_HEIGHT ) collided3 = false; // if avatar is higher than the platform
	    if ( cy >= fy + PLAT_H) collided3 = false; // if avatar is lower than the platform
	    if ( cx >= fx + L4CPLAT_W) collided3 = false; // is avatar is on on the right of the platform

	    if (collided3){
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);
	    		//if avatar hits platform from below.
	    		if (cy >= fy + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + AVATAR_WIDTH - 1 == fx && dx > 0) && cy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((cx == fx + L4CPLAT_W - 1 && dx < 0) && cy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			onGround = true;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    double gx = (sprite_x(Gate1)), gy = (sprite_y(Gate1));

	    bool collided4 = true;

	    if ( gx >= cx + AVATAR_WIDTH ) collided4 = false; //if avatar is on the left of the platform
	    if ( gy >= cy + AVATAR_HEIGHT ) collided4 = false; // if avatar is higher than the platform
	    if ( cy >= gy + GATE_H) collided4 = false; // if avatar is lower than the platform
	    if ( cx >= gx + GATE_W) collided4 = false;

	    if (collided4){
	    	if (who_moved == avatar){
				double dx = sprite_dx(avatar), dy = sprite_dy(avatar);

	    		//if avatar hits left edge of gate
	    		if ((cx + AVATAR_WIDTH - 1 == gx)){
	    			dx = 0;
	    			dy += gravity;
	    			onGround = false;
	    		}

	    		sprite_back(avatar);
	    		sprite_turn_to (avatar, dx, dy);
	    	}
	    }
	    double hx = (sprite_x(boulder1)), hy = (sprite_y(boulder1));

	    bool collided5 = true;
		if ( ax >= hx + BOULDER_WIDTH ) collided5 = false; //platform is on the right of the avatar
	    if ( ay >= hy + BOULDER_HEIGHT ) collided5 = false; //platform is below avatar
	    if ( hx >= ax + L4CPLAT_W ) collided5 = false; // if avatar is below platform 1
	    if ( hy >= ay + PLAT_H) collided5 = false; // if avatar is below platform 1
	    if ( hy > by - PLAT_H - 1 && hx < L4BPLAT_W ) collided5 = true; //if avatar is above bottom left platform
	    if ( hy > dy - PLAT_H - 1 && hx > 120 - L4APLAT_W ) collided5 = true; //screen - platform width, if avatar is above bottom right platform

	    if (collided5) {
	    	//if avatar hit platform
	    	if (who_moved == boulder1){
				double dx = sprite_dx(boulder1), dy = sprite_dy(boulder1);
	    		//if avatar hits platform from below.
	    		if (hy >= ay + L4CPLAT_W && dy < 0){ 
	    			dy = -dy;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((cx + BOULDER_WIDTH - 1 == ax && dx > 0)){
	    			// dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		// if avatar hits right edge of platform  && cx >= ax + L1_W)
	    		else if ((hx == ax + L4CPLAT_W - 1 && dx > 0) && (hy <= ay - 1 )){
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			boulder_onGround = true;
	    		}

	    		sprite_back(boulder1);
	    		sprite_turn_to (boulder1, dx, dy);
	    	}
	    }
	    bool collided6 = true;

	    if ( ex >= hx + BOULDER_WIDTH ) collided6 = false; //if avatar is on the left of the platform
	    if ( ey >= hy + BOULDER_HEIGHT ) collided6 = false; // if avatar is higher than the platform
	    if ( hy >= ey + PLAT_H) collided6 = false; // if avatar is lower than the platform
	    if ( hx >= ex + L4CPLAT_W) collided6 = false; // is avatar is on on the right of the platform

	    if (collided6){
	    	if (who_moved == boulder1){
				double dx = sprite_dx(boulder1), dy = sprite_dy(boulder1);
	    		//if avatar hits platform from below.
	    		if (dy >= ey + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((hx + BOULDER_WIDTH - 1 == ex && dx > 0) && hy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((hx == ex + L4CPLAT_W - 1 && dx < 0) && hy <= ey - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			boulder_onGround = true;
	    		}

	    		sprite_back(boulder1);
	    		sprite_turn_to(boulder1, dx, dy);
	    	}
	    }
	    bool collided7 = true;

	    if ( fx >= hx + BOULDER_WIDTH ) collided7 = false; //if avatar is on the left of the platform
	    if ( fy >= hy + BOULDER_HEIGHT ) collided7 = false; // if avatar is higher than the platform
	    if ( hy >= fy + PLAT_H) collided7 = false; // if avatar is lower than the platform
	    if ( hx >= fx + L4CPLAT_W) collided7 = false; // is avatar is on on the right of the platform

	    if (collided7){
	    	if (who_moved == boulder1){
				double dx = sprite_dx(boulder1), dy = sprite_dy(boulder1);
	    		//if avatar hits platform from below.
	    		if (dy >= fy + PLAT_H && dy < 0){ 
	    			dy = -dy;
	    			boulder_onGround = false;
	    		}
	    		
	    		//if avatar hits left edge of platform
	    		else if ((hx + BOULDER_WIDTH - 1 == ex && dx > 0) && hy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar hits right edge of platform 
	    		else if ((hx == fx + L4CPLAT_W - 1 && dx < 0) && hy <= fy - 2){
	    			dx = 0;
	    			dy += gravity;
	    			boulder_onGround = false;
	    		}

	    		//if avatar lands on top of platform
	    		else {
	    			dy = 0;
	    			boulder_onGround = true;
	    		}

	    		sprite_back(boulder1);
	    		sprite_turn_to(boulder1, dx, dy);
	    	}
	    }
	}
}

void reset_avatar(void){
    sprite_move_to(avatar, 4, 46);
}

void reset_zom(void){
    sprite_move_to(zombie, 112, 45);
}
void reset_boulder(void){
	sprite_move_to(boulder1, 60, 10);
	sprite_turn_to(boulder1, +1.33, 0);
	sprite_step(boulder1);
}

bool collision(){

    bool collided = true;

    int hx = round(sprite_x(avatar));
    int hy = round(sprite_y(avatar));
    int hr = hx + AVATAR_WIDTH - 1;
    int hb = hy + AVATAR_HEIGHT - 1;

    int zx = round(sprite_x(zombie));
    int zy = round(sprite_y(zombie));
    int zr = zx + ZOMBIE_WIDTH - 1;
    int zb = zy + ZOMBIE_HEIGHT - 1;

    if (hr < zx) { collided = false;}
    if (hb < zy) { collided = false;}
    if (zr < hx) { collided = false;}
    if (zb < hy) { collided = false;}

    if (hx > screen_width()){
    	collided = true;
    }

    if (hy > screen_height()){
    	collided = true;
    }

    return collided;
}

bool spike_collision(){
	bool collided = true;

    int hx = round(sprite_x(avatar));
    int hy = round(sprite_y(avatar));
    int hr = hx + AVATAR_WIDTH - 1;
    int hb = hy + AVATAR_HEIGHT - 1;

    int zx = round(sprite_x(spike));
    int zy = round(sprite_y(spike));
    int zr = zx + SPIKE_W - 1;
    int zb = zy + SPIKE_H - 1;

    if (hr < zx) { collided = false;}
    if (hb < zy) { collided = false;}
    if (zr < hx) { collided = false;}
    if (zb < hy) { collided = false;}

    return collided;
}

bool boulder_collision(){
	bool collided = true;
	int hx = round(sprite_x(avatar));
    int hy = round(sprite_y(avatar));
    int hr = hx + AVATAR_WIDTH - 1;
    int hb = hy + AVATAR_HEIGHT - 1;

	int bx = round(sprite_x(boulder1));
    int by = round(sprite_y(boulder1));
    int br = bx + BOULDER_WIDTH - 1;
    int bb = by + BOULDER_HEIGHT - 2;

    if (hr < bx) { collided = false;}
    if (hb < by) { collided = false;}
    if (br < hx) { collided = false;}
    if (bb < hy) { collided = false;}

    return collided;
}

bool treasure_collision(){
    bool collided = true;

    int hx = round(sprite_x(avatar));
    int hy = round(sprite_y(avatar));
    int hr = hx + AVATAR_WIDTH - 1;
    int hb = hy + AVATAR_HEIGHT - 1;

    int tx = round(sprite_x(treasure));
    int ty = round(sprite_y(treasure));
    int tr = tx + 1;
    int tb = ty + 1;

    if (hr < tx) { collided = false;}
    if (hb < ty) { collided = false;}
    if (tr < hx) { collided = false;}
    if (tb < hy) { collided = false;}

    return collided;
}
bool gate_collision(){

    bool collided = false;

    int hx = round(sprite_x(avatar)); 
    int hy = round(sprite_y(avatar)); 
    int hr = hx + AVATAR_WIDTH - 1; 

    int gx = round(sprite_x(Gate1)); 
    int gy = round(sprite_y(Gate1));
    if (hr == gx - 3 && hy >= gy){
    	collided = true;
    }

    return collided;
}

bool key_collision(){

    bool collided = true;

    int hx = round(sprite_x(avatar));
    int hy = round(sprite_y(avatar));
    int hr = hx + AVATAR_WIDTH - 1;
    int hb = hy + AVATAR_HEIGHT - 1;

    int kx = round(sprite_x(gatekey));
    int ky = round(sprite_y(gatekey)); 
    int kr = kx + KEY_W;
    int kb = ky + KEY_H;

    if (hr < kx) { collided = false;}
    if (hb < ky) { collided = false;}
    if (kr < hx) { collided = false;}
    if (kb < hy) { collided = false;}

    return collided;
}

bool exit_collide(){

    bool exit = true;

    int hx = round(sprite_x(avatar)); // top
    int hy = round(sprite_y(avatar)); //left
    int hr = hx + AVATAR_WIDTH - 1; //right
    int hb = hy + AVATAR_HEIGHT - 1; //bottom

    int ex = round(sprite_x(exit_door));
    int ey = round(sprite_y(exit_door)); 
    int er = ex + EXIT_WIDTH;
    int eb = ey + EXIT_HEIGHT;

    if (hr < ex) { exit = false;} //collide on left
    if (hb < ey) { exit = false;} // collide on top
    if (er < hx) { exit = false;} //collide on right
    if (eb < hy) { exit = false;} // collide on bottom

    return exit;
}

void setup(void){
	if (level == 1){
		level_1();
	    setup_exit();
	    setup_avatar();
	    setup_zomb();
	    setup_boulder();
	    setup_spike();
	}

	if (level == 2){
		level_2();
		setup_avatar();
		setup_exit();
		setup_spike();
		char * change = spike_bat1;
		if(milliseconds == 5){
	        sprite_set_image(spike, spike_bat2);
	        change = spike_bat2;
	    }
	    if(milliseconds == 5 && change == spike_bat2){
	        sprite_set_image(spike, spike_bat1);
	        change = spike_bat1;
	    }
	}

	if (level == 3){
		level_3();
		setup_avatar();
		setup_exit();
	}

	if (level == 4){
		level_4();
		setup_avatar();
		setup_exit();
		setup_boulder();
	}

	if (level == 5){
		level_5();
		setup_avatar();
		setup_exit();
		setup_boulder();
	}
	setup_border();
    show_screen();
}

void process(void){
	if (avatar_score < 0){
		avatar_score = 0; // keeps avatar score positive
	}

    int key = get_char();

    if (onGround){
		sprite_set_image(avatar, avatar_image);
	}

	if (!onGround || key == 'l'){
		sprite_set_image(avatar, avatar_image2);
	}

    int ax = round(sprite_x(avatar));
    // int ay = round(sprite_y(avatar));
    double dy = sprite_dy(avatar);
    double dx = sprite_dx(avatar);
    double by = sprite_dy(boulder1);
    double bx = sprite_dx(boulder1);
    bool moving = false;

	if (ax <= 3 || ax > screen_width() - 5){ //makes sure avatar is in between screen
		dx = 0;
		pressed = 0;
		sprite_back(avatar);
	}
	if (key == KEY_RIGHT && onGround){ //move to right
		// dy = 0;
		if (dx == -1.0 || dx == -1.75){ //if avatar is moving to the left
			pressed = 0;
			moving = true;
			if (dx == -1.75){
				dx = -1.0;
			}
			else {
				dx = 0;
			}
		}
		if (moving == false){ //if avatar was not moving left or is still, move right
			dx = 1.0;
			pressed++;
		}
		if (pressed > 1){ //double movement to the right
			dx = +1.75;
		}
		sprite_step(avatar);
	}
	if (key == KEY_LEFT && onGround){
		// dy = 0;
		if (dx == 1.0 || dx == 1.75){
			pressed = 0;
			moving = true;
			if (dx == 1.75){
				dx = 1.0;
			}
			else {
				dx = 0;
			}
		}
		if (moving == false){
			dx = -1.0;
			pressed++;
		}
		if (pressed > 1){
			dx = -1.75;
		}
	}
	sprite_turn_to(avatar, dx, dy);
	sprite_step(avatar);
	process_collision(avatar);
	process_collision(boulder1);

	dy += gravity; //sticks avatar to the ground

	if (onGround){ //makes sure w can't be pressed again if avatar is in air
		if (key == KEY_UP){
			if(onGround){
				dy = -2;
				onGround = false;
			}
			if (!onGround){
				dy = -2;
			} 
		}
	}
	sprite_turn_to(avatar, dx, dy);
	sprite_step(avatar);

	by += gravity; //sticks boulder to the ground
	 //makes sure w can't be pressed again if avatar is in air
		// if(boulder_onGround){
		// 	by = -2;
		// 	boulder_onGround = false;
		// }
	if (!boulder_onGround){
		by = -2;
	} 

	sprite_turn_to(boulder1, bx, by);
	sprite_step(boulder1);

	process_collision(avatar);
	process_collision(L1_platform);
	process_collision(platform1B);
	process_collision(Aplatform2B);
	process_collision(Bplatform2B);
	process_collision(platform3);
	process_collision(boulder1);


    if ( key == -1 && level == 1) {
        sprite_step( zombie );
        int zx = round(sprite_x(zombie));
        double dx = sprite_dx( zombie );
        double dy = sprite_dy( zombie );

        // test to see if zombie hits the left and right sides
        if ( zx == 0 || zx == 120 - ZOMBIE_WIDTH) {
            dx = -dx; 
        }
        // for zombie to change direction
        if ( dx != sprite_dx(zombie)){
            sprite_back( zombie );
            sprite_turn_to(zombie, dx, dy);
        }
    }

    if ( key == -1 && level == 2) {
        sprite_step( spike );
        int zx = round(sprite_x(spike));
        double dx = sprite_dx( spike );
        double dy = sprite_dy( spike );

        // test to see if zombie hits the left and right sides
        if ( zx <= 0 || zx >= 120 - SPIKE_W) {
            dx = -dx; 
        }
        // for zombie to change direction
        if ( dx != sprite_dx(spike)){
            sprite_back( spike );
            sprite_turn_to(spike, dx, dy);
        }
    }
    if ( key == -1 && level == 3) {
        sprite_step( MovePlatform );
        double px = round(sprite_x(MovePlatform));
        double dx = sprite_dx( MovePlatform );
        double dy = sprite_dy( MovePlatform );

        // test to see if platform hits the left and right sides
        if ( px == 0 || px == 120 - L1_W) {
            dx = -dx; 
        }
        // for platform to change direction
        if ( dx != sprite_dx(MovePlatform)){
            sprite_back( MovePlatform );
            sprite_turn_to(MovePlatform, dx, dy);
        }
    }
    if ( key == -1 && (level == 4 || level == 5)) {
        sprite_step( boulder1 );
        int zx = round(sprite_x(boulder1));
        double dx = sprite_dx( boulder1 );
        double dy = sprite_dy( boulder1 );

        // test to see if boulder hits the left and right sides
        if ( zx <= 0 || zx >= 120 - BOULDER_WIDTH) {
            dx = -dx; 
        }
        // for boulder to change direction
        if ( dx != sprite_dx(boulder1)){
            sprite_back( boulder1 );
            sprite_turn_to(boulder1, dx, dy);
        }
    }
    if ( key == -1 && level == 5) {
        sprite_step( MovePlatform2 );
        double px = round(sprite_x(MovePlatform2));
        double dx = sprite_dx( MovePlatform2);
        double dy = sprite_dy( MovePlatform2);

        // test to see if platform hits the left and right sides
        if ( px == 0 || px == 120 - L4CPLAT_W) {
            dx = -dx; 
        }
        // for platform to change direction
        if ( dx != sprite_dx(MovePlatform2)){
            sprite_back( MovePlatform2 );
            sprite_turn_to(MovePlatform2, dx, dy);
        }
    }
    if (level == 1){
    	sprite_move_to(boulder1, 130, 0);
    	sprite_hide(boulder1);
    	sprite_move_to(spike, 130, 0);
    	sprite_hide(spike);
    }

    if (level == 4){
    	double bx = round(sprite_x(boulder1));
    	double by = round(sprite_y(boulder1));
    	double x_gap = 21;
    	double y_gap = 46;

    	if (bx <= x_gap && by >= y_gap){
    		sprite_move_to(boulder1, 22, 0);
    	}
    }

    if (level == 5){
    	double bx = round(sprite_x(boulder1));
    	double by = round(sprite_y(boulder1));
    	double x_gap = 90;
    	double y_gap = 50;


    	if (bx >= x_gap && by >= y_gap){
    		sprite_move_to(boulder1, SCREEN_W / 2, 0);
    	}
    }

    if ( collision() || boulder_collision() || spike_collision()) {
        life--;
        pressed = 0;
        sprite_turn_to(avatar, 0, 0);
        reset_avatar();
        if (level == 1){
			reset_zom();
        }
        if (level == 2){
        	if (treasure_collected){
        		sprite_move_to(treasure, SCREEN_W / 2, SCREEN_H / 2);
        		sprite_show(treasure);
				avatar_score = avatar_score - 50;
        	}
        	sprite_move_to(spike, 112, 46);
        	sprite_turn_to(spike, -1, 0);
        	sprite_step(spike);
        	char * change = spike_bat1;
		    if(milliseconds == 5){
	        sprite_set_image(spike, spike_bat2);
	        change = spike_bat2;
		    }
		    if(milliseconds == 5 && change == spike_bat2){
		        sprite_set_image(spike, spike_bat1);
		        change = spike_bat1;
		    }
        }
        if (level == 3){
        	sprite_move_to(MovePlatform, 40, 37);
        	sprite_turn_to(MovePlatform, 1, 0);
        	if (treasure_collected){
        		sprite_move_to(treasure, 112, 46);
        		sprite_show(treasure);
				avatar_score = avatar_score - 50;
        	}
        }
        if (level == 4){
        	reset_boulder();
        	sprite_turn_to(MovePlatform, +1, 0);
        	sprite_move_to(boulder1, SCREEN_W / 2, 10);
	    	sprite_show(boulder1);
        }
        if (level == 5){
        	reset_boulder();
        	sprite_move_to(MovePlatform2, 0, 25);
        	sprite_turn_to(MovePlatform2, +1, 0);
        	if (treasure_collected){
        		sprite_move_to(treasure, SCREEN_W / 2, SCREEN_H / 2 - 9);
        		sprite_show(treasure);
				avatar_score = avatar_score - 50;
        	}
        	if (key_collected){
        		sprite_move_to(gatekey, 112, 6);
        		sprite_show(gatekey);
        	}
        	if (gate_unlocked){
        		sprite_move_to(Gate1, 80, 38);
        		sprite_show(Gate1);
        	}
        	treasure_collected = false;
        	key_collected = false;
        	gate_unlocked = false;

        }
    }

    if (exit_collide() || key == 'l'){
    	level++;
    	pressed = 0;
    	avatar_score = avatar_score + 100;
    	if (key == 'l'){
    		avatar_score = avatar_score - 100; // pressing l can't add score
    	}
    	sprite_turn_to(avatar, 0, 0);
    	reset_avatar();
    	if (level == 2){
			sprite_destroy(zombie);
			sprite_destroy(platform1B);
			sprite_destroy(L1_platform);
			treasure = sprite_create(SCREEN_W / 2, SCREEN_H / 2, 1, 1, "$");
			sprite_draw(treasure);
			sprite_move_to(spike, 112, 46);
			sprite_show(spike);

			char * change = spike_bat1;
		    if(milliseconds == 5){
	        sprite_set_image(spike, spike_bat2);
	        change = spike_bat2;
		    }
		    if(milliseconds == 5 && change == spike_bat2){
		        sprite_set_image(spike, spike_bat1);
		        change = spike_bat1;
		    }
        }

        if (level == 3){
        	sprite_destroy(spike);
			sprite_destroy(L1_platform);
			sprite_move_to(exit_door, 58, 19);

			MovePlatform = sprite_create(40, 37, L1_W, PLAT_H, L1_plat); //platform is 40 characters long
		    sprite_draw(MovePlatform);
	    	sprite_turn_to(MovePlatform, +1, 0);
			sprite_step(MovePlatform);	

			if (treasure_collected){
				treasure_collected = false;
			}
			sprite_show(treasure);
			sprite_move_to(treasure, 112, 46);
        }

        if (level == 4){
        	reset_boulder();
        	sprite_destroy(MovePlatform);
        	sprite_move_to(exit_door, 4, 8);
        	sprite_move_to(boulder1, 60, 10);
        	sprite_show(boulder1);

        	// boulder1 = sprite_create(60, 10, BOULDER_WIDTH, BOULDER_HEIGHT, boulder);
        	// sprite_draw(boulder1);
        	// sprite_turn_to(boulder1, +2, 0);
        	// sprite_step(boulder1);
        }

        if (level == 5){
        	reset_boulder();
        	sprite_step(boulder1);
        	sprite_move_to(platform4C1, 40, 37);
        	sprite_destroy(platform4C2);
        	sprite_move_to(platform4C3, 40, 12);
        	sprite_move_to(platform4A, 98, 49);
        	sprite_move_to(platform4B, 0, 49);
        	sprite_move_to(exit_door, 112, 45);

        	MovePlatform2 = sprite_create(0, 25, L4CPLAT_W, PLAT_H, L4_platformC); //platform is 40 characters long
		    sprite_draw(MovePlatform2);
	    	sprite_turn_to(MovePlatform2, +1, 0);
			sprite_step(MovePlatform2);	

			if (treasure_collected){
				treasure_collected = false;
			}
			sprite_show(treasure);
			sprite_move_to(treasure, SCREEN_W / 2, SCREEN_H / 2 - 9);

			gatekey = sprite_create(112, 6, 3, 1, "0-+");
			sprite_draw(gatekey);

			Gate1 = sprite_create(80, 38, GATE_W, GATE_H, gate);
			sprite_draw(Gate1);
        }
    }

    //clears the screen
    clear_screen(); 

    setup_stats();
    setup_time();
    //draw the boulder
    sprite_draw(boulder1);
    //draw the level
    if (level == 1){
    	level_1();

	    //draw the border
	    int left = 0, right = 121-1, top = 0, bottom = 50-1;
	    draw_line(left, top+1, right, top+1, '-');
	    draw_line(left, top+2, left, bottom-1, ':');
	    draw_line(right-1, top+2, right-1, bottom-1, ':');

	    //draw the avatar
	    sprite_draw(avatar);

	    //draw the zombie
	    sprite_draw(zombie);

	    //draw the exit door
	    sprite_draw(exit_door);

	    //draw the boulder
	    sprite_draw(boulder1);

	    //draw the spike
	    sprite_draw(spike);
	}

	if (level == 2){
		level_2();
		//draw the border
	    int left = 0, right = 121-1, top = 0, bottom = 50-1;
	    draw_line(left, top+1, right, top+1, '-');
	    draw_line(left, top+2, left, bottom-1, ':');
	    draw_line(right-1, top+2, right-1, bottom-1, ':');

	    //draw the avatar
	    sprite_draw(avatar);

	    //draw the exit door
	    sprite_draw(exit_door);

	    //draw the spike
	    sprite_draw(spike);
	    char * change = spike_bat1;
	    if(milliseconds == 5){
	        sprite_set_image(spike, spike_bat2);
	        change = spike_bat2;
	    }
	    if(milliseconds == 5 && change == spike_bat2){
	        sprite_set_image(spike, spike_bat1);
	        change = spike_bat1;
	    }

	    //draw the treasure
	    sprite_draw(treasure);
	    if (treasure_collision()){
	    		sprite_move_to(treasure, 120, 0);
		    	sprite_hide(treasure);
		    	avatar_score = avatar_score + 50;
		    	treasure_collected = true;
	    }

	}

	if (level == 3){
		level_3();
		//draw the border
	    int left = 0, right = 121-1, top = 0, bottom = 50-1;
	    draw_line(left, top+1, right, top+1, '-');
	    draw_line(left, top+2, left, bottom-1, ':');
	    draw_line(right-1, top+2, right-1, bottom-1, ':');

	    //draw the moving platform
	    sprite_draw(MovePlatform);

	    //draw the avatar
	    sprite_draw(avatar);

	    //draw the exit door
	    sprite_draw(exit_door);

	    //draw the treasure
	    sprite_draw(treasure);
	    if (treasure_collision()){
	    		sprite_move_to(treasure, 120, 0);
		    	sprite_hide(treasure);
		    	avatar_score= avatar_score + 50;
		    	treasure_collected = true;
	    }
	}

	if (level == 4){
		level_4();
		//draw the border
	    int left = 0, right = 121-1, top = 0, bottom = 50-1;
	    draw_line(left, top+1, right, top+1, '-');
	    draw_line(left, top+2, left, bottom-1, ':');
	    draw_line(right-1, top+2, right-1, bottom-1, ':');

	    //draw the boulder
	    // sprite_draw(boulder1);

	    //draw the avatar
	    sprite_draw(avatar);

	    //draw the exit door
	    sprite_draw(exit_door);

	    //draw the boulder
	    sprite_draw(boulder1);
	}

	if (level == 5){
		level_5();
		//draw the border
	    int left = 0, right = 121-1, top = 0, bottom = 50-1;
	    draw_line(left, top+1, right, top+1, '-');
	    draw_line(left, top+2, left, bottom-1, ':');
	    draw_line(right-1, top+2, right-1, bottom-1, ':');

	    //draw the moving platform
	    sprite_draw(MovePlatform2);

	    //draw the avatar
	    sprite_draw(avatar);

	    //draw the exit door
	    sprite_draw(exit_door);

	    //draw the boulder
	    // sprite_draw(boulder1);

	    //draw the treasure, above the middle platform
	    sprite_draw(treasure);
	    if (treasure_collision()){
	    		sprite_move_to(treasure, 120, 0);
		    	sprite_hide(treasure);
		    	avatar_score= avatar_score + 50;
		    	treasure_collected = true;
	    }

	    //draw the gate key
	    sprite_draw(gatekey);
	    if (key_collision()){
	    		sprite_move_to(gatekey, 120, 0);
		    	sprite_hide(gatekey);
		    	key_collected = true;
	    }

	    //draw the gate
	    sprite_draw(Gate1);
	    if (gate_collision() && key_collected == true){
	    		sprite_move_to(Gate1, 120, 0);
		    	sprite_hide(Gate1);
		    	gate_unlocked = true;
	    }
	}

	if (level >= 6){
        clear_screen();
        int message_width = strlen(msg_image)/5;
        sprite_id msg = sprite_create( ( 120 - message_width ) / 2, ( 50 - 2 ) / 2, message_width, 5, msg_image);
        draw_formatted(69, 26, "%d", avatar_score);
        sprite_draw(msg);
        show_screen();
        game_over = true;
        wait_char();
        return;
    }
    if (life == 0){
        clear_screen();
        int message_width = strlen(game_over_msg)/4;
        sprite_id msg2 = sprite_create( ( 120 - message_width ) / 2, ( 50 - 2 ) / 2, message_width, 4, game_over_msg);
        sprite_draw(msg2);
        show_screen();
        game_over = true;
        wait_char();
        return;

    }
}

int main(void) {
    setup_screen();
    override_screen_size(120, 50);
    setup();    
    show_screen();
    while ( !game_over ) {
        process();
        if ( update_screen ) {
            show_screen();
        }

        timer_pause(DELAY);
    }
    return 0;
}