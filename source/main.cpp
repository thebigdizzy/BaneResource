// Using SDL2 to create an application window

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
//#include "SDL2_mixer/SDL_mixer.h"
//#include "SDL2_ttf/SDL_ttf.h"

#include <unistd.h> // sleep
#endif

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"
#endif

#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>	// srand, rand
#include <time.h>	// time

// player include
#include "Player.h"

// platform include
#include "platform.h"

// pickup include 
#include "pickUp.h"

// include the dragon
#include "Dragon.h"

// include the pterodactyl
#include "pterodactyl.h"

// include for the dragon's lair gate opening
#include "gate.h"

using namespace std;

// CODE FOR FRAME RATE INDEPENDENCE
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

int main(int argc, char* argv[]) {

	// initialize random seed:
	srand(time(NULL));

#if defined(__APPLE__)

	cout << "Running on Apple" << endl;
	cout << "Added on Apple" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	string imageDir = s_cwd + "/BaneResource/images/";
	string audioDir = s_cwd + "/BaneResource/audio/";

#endif

#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows" << endl;
	cout << "Added on Windows" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	string imageDir = s_cwd + "\\BaneResource\\images\\";
	string audioDir = s_cwd + "\\BaneResource\\audio\\";

#endif


	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
			"Dragons Bane",                  	// window title
			SDL_WINDOWPOS_UNDEFINED,           	// initial x position
			SDL_WINDOWPOS_UNDEFINED,           	// initial y position
			1024,                               // width, in pixels
			768,                               	// height, in pixels
			SDL_WINDOW_OPENGL                  	// flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// the surface contained by the window
	SDL_Surface* screenSurface = NULL;

	// Get window surface
	//screenSurface = SDL_GetWindowSurface(window);

	SDL_Renderer* renderer = NULL;

	// create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	string path = imageDir + "LevelLayout.png";

	// get image for testing
	SDL_Texture *testTex = NULL;
	screenSurface = IMG_Load(path.c_str());

	testTex = SDL_CreateTextureFromSurface(renderer, screenSurface);

	SDL_Rect testRect;
	testRect.x = 0;
	testRect.y = -1536;
	testRect.w = 3072;
	testRect.h = 2304;

	string lGPath = imageDir + "leftGround.png";

	// get image for testing
	SDL_Texture *lGTex = NULL;
	screenSurface = IMG_Load(lGPath.c_str());

	lGTex = SDL_CreateTextureFromSurface(renderer, screenSurface);

	SDL_Rect leftGroundRect;
	leftGroundRect.x = 0;
	leftGroundRect.y = 635;
	leftGroundRect.w = 832;
	leftGroundRect.h = 133;

	string rGPath = imageDir + "rightGround.png";

	// get image for testing
	SDL_Texture *rGTex = NULL;
	screenSurface = IMG_Load(rGPath.c_str());

	rGTex = SDL_CreateTextureFromSurface(renderer, screenSurface);

	SDL_Rect rightGroundRect;
	rightGroundRect.x = 1184;
	rightGroundRect.y = 635;
	rightGroundRect.w = 1888;
	rightGroundRect.h = 133;

	string airWayPath = imageDir + "airWay.png";

	// get image for airWay
	SDL_Texture *airWayTex = NULL;
	screenSurface = IMG_Load(airWayPath.c_str());

	airWayTex = SDL_CreateTextureFromSurface(renderer, screenSurface);

	SDL_Rect airWayRect;
	airWayRect.x = 850;
	airWayRect.y = -300;
	int w, h;
	SDL_QueryTexture(airWayTex, NULL, NULL, &w, &h);
	airWayRect.w = w;
	airWayRect.h = h;

	// airWay x and y pos
	float airWayX_pos = airWayRect.x;
	float airWayY_pos = airWayRect.y;

	// back groung x and y pos
	float X_pos = testRect.x;
	float Y_pos = testRect.y;

	float RX_pos = rightGroundRect.x;
	float RY_pos = rightGroundRect.y;

	float LX_pos = leftGroundRect.x;
	float LY_pos = leftGroundRect.y;

	// initialize the Player object
	Player player(renderer, imageDir, audioDir, 0, 540);

	// initialize the Dragon
	Dragon dragon(renderer, imageDir, audioDir, 600, -700);

	// setup the pterodactyls in a vector
	vector<Pterodactyl> pterodactyl;

	// initialize the pterodactyls
	pterodactyl.push_back(Pterodactyl(renderer, imageDir, audioDir, 950, -100));
	pterodactyl.push_back(Pterodactyl(renderer, imageDir, audioDir, 950, -100));
	pterodactyl.push_back(Pterodactyl(renderer, imageDir, audioDir, 950, -100));
	pterodactyl.push_back(Pterodactyl(renderer, imageDir, audioDir, 950, -100));
	//pterodactyl.push_back(Pterodactyl(renderer, imageDir, audioDir, 950, -100));
	//pterodactyl.push_back(Pterodactyl(renderer, imageDir, audioDir, 950, -100));

	// setup a timer for the spawn rate of the pterodactyls
	float spawnRate = 0;
	bool canSpawn = false;

	// gate variable
	Gate gate(renderer, imageDir, audioDir, 2050,-1150);

	// set up the platforms in a vector
	vector<Platform> platformList;

	// initialize the platforms
	platformList.push_back(Platform(renderer, imageDir, audioDir, 200, 540));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 620, 500));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 620, -200));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 320, -300));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 20, -400));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 1220, -200));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 1620, -300));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 1220, -400));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 1220, -600));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 1220, -800));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 1620, -700));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 920, -500));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 620, -600));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 320, -700));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 20, -800));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 2110, -820));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 2360, -820));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 2610, -820));
	platformList.push_back(Platform(renderer, imageDir, audioDir, 2760, -820));

	// set up a pickup vector
	vector <PickUp> pickUpList;

	// initialize the arrow pickups
	pickUpList.push_back(PickUp(renderer, imageDir, 1, 1700, -380));
	pickUpList.push_back(PickUp(renderer, imageDir, 1, 100, -480));
	pickUpList.push_back(PickUp(renderer, imageDir, 1, 100, -880));

	// initialize the health pickups
	pickUpList.push_back(PickUp(renderer, imageDir, 2, 415, -380));
	pickUpList.push_back(PickUp(renderer, imageDir, 2, 710, -680));
	pickUpList.push_back(PickUp(renderer, imageDir, 2, 1310, -680));
	pickUpList.push_back(PickUp(renderer, imageDir, 2, 2200, -880));

	// initialize the ammo pickups
	pickUpList.push_back(PickUp(renderer, imageDir, 3, 700, 420));
	pickUpList.push_back(PickUp(renderer, imageDir, 3, 700, -280));
	pickUpList.push_back(PickUp(renderer, imageDir, 3, 1300, -280));
	pickUpList.push_back(PickUp(renderer, imageDir, 3, 1000, -580));
	pickUpList.push_back(PickUp(renderer, imageDir, 3, 2250, -890));

	// initialize the bow pickup
	pickUpList.push_back(PickUp(renderer, imageDir, 4, 275, 470));

	// bool for initializing boss Mode
	bool bossInit = true;

	// ***** SDL Event to handle input
	SDL_Event event;

	//***** set up variables for the game states
	enum GameState { MENU, LEVEL1, WIN, LOSE };

	// ***** set up the initial state
	GameState gameState = LEVEL1;

	// bool value to control movement through the states
	bool menu = false, level1 = false, win = false, lose = false, quit = false;

	// mouse position in X and Y value
	int mouseX = 0, mouseY = 0;

	// starting game loop here...
	while(!quit){
		// enter game state machine
		switch(gameState){
		case MENU:
			menu = true;
			cout << "menu" << endl;

			SDL_ShowCursor(1);

			while(menu){
				// set up frame rate for the case using deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						menu = false;
						break;
					}
					switch(event.type){
					// enter case for button down with this code - case SDL_CONTROLLERBUTTONDOWN: & if (event.cdevice.which == 0)
					case SDL_MOUSEBUTTONDOWN:
						if(event.button.button == SDL_BUTTON_LEFT){

						}
						break;
					case SDL_KEYDOWN:
						player.OnButtonPress(event);
						cout << "key down" << endl;
						break;

					default:break;
					} // end switch event type

					mouseX = event.button.x;
					mouseY = event.button.y;
				} // end poll event

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// put in display images here with SDL_RenderCopy()

				// display the test background
				SDL_RenderCopy(renderer, testTex, NULL, &testRect);

				// present the renderer
				SDL_RenderPresent(renderer);
			} // end menu scene loop
			break;
		case LEVEL1:
			level1 = true;
			cout << "level 1" << endl;

			SDL_ShowCursor(0);

			// start level 1 game loop
			while(level1){
				// set up frame rate for the case using deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						level1 = false;
						break;
					}
					switch(event.type){
					// enter case for button down with this code - case SDL_CONTROLLERBUTTONDOWN: & if (event.cdevice.which == 0)
					case SDL_MOUSEBUTTONDOWN:
						if(event.button.button == SDL_BUTTON_LEFT){
							//use this area to shoot an arrow
							player.OnMouseButtonPress();
						}
						if(event.button.button == SDL_BUTTON_RIGHT){
							//use this area to shoot an arrow
							player.ChangeWeapon();
						}
						break;
					case SDL_KEYDOWN:
						if(event.key.keysym.sym == SDLK_ESCAPE){
							quit = true;
							level1 = false;
						}
						if(event.key.keysym.sym == SDLK_RETURN){
							gate.state = gate.Lower;
						}
						// send the button info to the player object
						player.OnButtonPress(event);
						break;
					case SDL_KEYUP:
						player.OnButtonRelease(event);
						break;
					case SDL_MOUSEMOTION:
						mouseX = event.button.x;
						mouseY = event.button.y;
						break;
					default:break;
					} // end switch event type
				} // end poll event

				// update the player
				player.Update(deltaTime);

				// update the dragon
				dragon.Update(deltaTime, player.posRect);

				// update the pterodactyls
				for (int i = 0; i < pterodactyl.size(); i++){
					for (int k = 0; k < player.bulletList.size(); k++){
						if(SDL_HasIntersection(&player.bulletList[k].posRect, &pterodactyl[i].posRect)){
							if(pterodactyl[i].active){
								player.bulletList[k].Reset();
								pterodactyl[i].state = pterodactyl[i].Die;
							}
						}
					}

					pterodactyl[i].Update(deltaTime, player.posRect);
				}

				// update the mouse position
				player.OnMouseEvent(mouseX, mouseY);

				// update the pickups
				for (int i = 0; i < pickUpList.size(); i++) {
					pickUpList[i].update(deltaTime);
				}

				// check for collision with the player and the dragon's fireballs
				for (int i = 0; i < dragon.max; i++) {
					if (dragon.fireball[i]->active) {
						if (SDL_HasIntersection(&player.posRect, &dragon.fireball[i]->posRect)) {
							player.DamageTaken(40);
							dragon.fireball[i]->Reset();
						}
					}
				}

				// check for collision with the pterodactyls
				for(int i = 0; i < pterodactyl.size(); i++){
					if(pterodactyl[i].active){
						if(SDL_HasIntersection(&player.posRect, &pterodactyl[i].posRect)){
							player.DamageTaken(10);
							pterodactyl[i].active = false;
						}
					}
				}

				// check for collison with the ground
				player.groundCollisionLeft = SDL_HasIntersection(&player.feetRect, &leftGroundRect);
				player.groundCollisionRight = SDL_HasIntersection(&player.feetRect, &rightGroundRect);

				// check for collision with the player's arrows and the dragon
				for (int i = 0; i < player.bulletList.size(); i++){
					if(SDL_HasIntersection(&player.bulletList[i].posRect, &dragon.hitRect) && dragon.state == dragon.MoveLtoR){
						player.bulletList[i].Reset();
						dragon.state = dragon.GetHit;
						dragon.health -= 5;
					}
				}

				// "blow" the player up through the airWay
				if(SDL_HasIntersection(&player.posRect, &airWayRect)){
					if(player.vel_Y >= -200){
						player.vel_Y -= 500 * deltaTime;
					}
				}

				// update the lair gate
				gate.Update(deltaTime);

				// check for collision with the platforms
				for(int i = 0; i < platformList.size(); i++){
					player.platform[i] = SDL_HasIntersection(&player.feetRect, &platformList[i].posRect);
				}

				if(player.platform[0]){
					canSpawn = true;
				}

				// spawn the pterodactyls here
				// increment the timer if bossMode is false
				if(!player.bossMode && canSpawn){
					spawnRate += deltaTime;
					if(spawnRate >= 2){
						spawnRate = 0;
						for(int i = 0; i < pterodactyl.size(); i++){
							if(pterodactyl[i].state == pterodactyl[i].Idle){
								pterodactyl[i].state = pterodactyl[i].Dive;
								break;
							}
						}
					}
				}

				// check for collision with the pickups
				for (int i = 0; i < pickUpList.size(); i++) {
					player.pickupNum = pickUpList[i].playerHit(player.posRect);
					if(player.pickupNum == 3 && !player.bow){
						player.pickupNum = 0;
					}
					if (player.pickupNum) {
						break;
					}
				}

				// set lose condition "fall"
				if(!SDL_HasIntersection(&player.posRect, &testRect)){
					//gameState = LOSE;
					//level1 = false;

					player.health--;
				}
				if(!player.bossMode){
					// update the background
					if(player.right == true){
						X_pos -= (player.speed) * deltaTime;
						RX_pos -= (player.speed) * deltaTime;
						LX_pos -= (player.speed) * deltaTime;
						airWayX_pos -= (player.speed) * deltaTime;

						// move the test enemy
						//eMoveX -= player.speed * deltaTime;

						if(testRect.x >= -2000){
							player.speed = 300;
							testRect.x = (int)(X_pos + .5f);
							rightGroundRect.x = (int)(RX_pos + .5f);
							leftGroundRect.x = (int)(LX_pos + .5f);
							airWayRect.x = (int)(airWayX_pos + .5f);

							// move the test enemy
							//enemyRect.x = (int)(eMoveX + .5f);

							// move the lair gate
							gate.MoveX(-player.speed, deltaTime);

							for(int i = 0; i < platformList.size(); i++){
								platformList[i].MoveX(-player.speed, deltaTime);
							}

							for(int i = 0; i < pterodactyl.size(); i++){
								if(pterodactyl[i].state != pterodactyl[i].Idle)
									pterodactyl[i].MoveX(-player.speed, deltaTime);
							}

							for (int i = 0; i < pickUpList.size(); i++) {
								pickUpList[i].MoveX(-player.speed, deltaTime);
							}

							for (int i = 0; i < player.bulletList.size(); i++){
								player.bulletList[i].BulletMoveX(-player.speed, deltaTime);
							}
						} else {
							player.speed = 600;
							X_pos = testRect.x;
							RX_pos = rightGroundRect.x;
							LX_pos = leftGroundRect.x;
							airWayX_pos = airWayRect.x;

							// move the test enemy
							//eMoveX = enemyRect.x;
						}
					}
					if(player.left == true){
						player.speed = 300;
						X_pos += (player.speed) * deltaTime;
						RX_pos += (player.speed) * deltaTime;
						LX_pos += (player.speed) * deltaTime;
						airWayX_pos += (player.speed) * deltaTime;

						// move the test enemy
						//eMoveX += player.speed * deltaTime;

						if(testRect.x <= -2){
							testRect.x = (int)(X_pos + .5f);
							rightGroundRect.x = (int)(RX_pos + .5f);
							leftGroundRect.x = (int)(LX_pos + .5f);
							airWayRect.x = (int)(airWayX_pos + .5f);

							// move the test enemy
							//enemyRect.x = (int)(eMoveX + .5f);

							// move the lair gate
							gate.MoveX(player.speed, deltaTime);

							for(int i = 0; i < platformList.size(); i++){
								platformList[i].MoveX(player.speed, deltaTime);
							}

							for(int i = 0; i < pterodactyl.size(); i++){
								if(pterodactyl[i].state != pterodactyl[i].Idle)
									pterodactyl[i].MoveX(player.speed, deltaTime);
							}

							for (int i = 0; i < pickUpList.size(); i++) {
								pickUpList[i].MoveX(player.speed, deltaTime);
							}

							for (int i = 0; i < player.bulletList.size(); i++){
								player.bulletList[i].BulletMoveX(player.speed, deltaTime);
							}
						} else {
							player.speed = 600;
							X_pos = testRect.x;
							RX_pos = rightGroundRect.x;
							LX_pos = leftGroundRect.x;
							airWayX_pos = airWayRect.x;

							// move the test enemy
							//eMoveX = enemyRect.x;
						}
					}
					if(player.jump && player.vel_Y < 0){
						Y_pos -= player.vel_Y * deltaTime;
						RY_pos -= player.vel_Y * deltaTime;
						LY_pos -= player.vel_Y * deltaTime;
						airWayY_pos -= (player.vel_Y) * deltaTime;

						// move the test enemy
						//eMoveY -= player.vel_Y * deltaTime;

						if(testRect.y < 0){
							testRect.y = (int)(Y_pos + .5f);
							rightGroundRect.y = (int)(RY_pos + .5f);
							leftGroundRect.y = (int)(LY_pos + .5f);
							airWayRect.y = (int)(airWayY_pos + .5f);

							// move the test enemy
							//enemyRect.y = (int)(eMoveY + .5f);

							// move the lair gate
							gate.MoveY(-player.vel_Y, deltaTime);

							for(int i = 0; i < platformList.size(); i++){
								platformList[i].MoveY(-player.vel_Y, deltaTime);
							}

							for(int i = 0; i < pterodactyl.size(); i++){
								if(pterodactyl[i].state != pterodactyl[i].Idle)
									pterodactyl[i].MoveY(-player.vel_Y, deltaTime);
							}

							for (int i = 0; i < pickUpList.size(); i++) {
								pickUpList[i].MoveY(-player.vel_Y, deltaTime);
							}

							for (int i = 0; i < player.bulletList.size(); i++){
								player.bulletList[i].BulletMoveY(-player.vel_Y, deltaTime);
							}
						} else {
							Y_pos = testRect.y;
							RY_pos = rightGroundRect.y;
							LY_pos = leftGroundRect.y;
							airWayY_pos = airWayRect.y;

							// move the test enemy
							//eMoveY = enemyRect.y;
						}
					}
					if((player.jump && player.vel_Y > 0) || (!player.jump && player.falling)){
						Y_pos -= player.vel_Y * deltaTime;
						RY_pos -= player.vel_Y * deltaTime;
						LY_pos -= player.vel_Y * deltaTime;
						airWayY_pos -= (player.vel_Y) * deltaTime;

						// move the test enemy
						//eMoveY -= player.vel_Y * deltaTime;

						if(testRect.y > -1800){
							testRect.y = (int)(Y_pos + .5f);
							rightGroundRect.y = (int)(RY_pos + .5f);
							leftGroundRect.y = (int)(LY_pos + .5f);
							airWayRect.y = (int)(airWayY_pos + .5f);

							// move the test enemy
							//enemyRect.y = (int)(eMoveY + .5f);

							// move the lair gate
							gate.MoveY(-player.vel_Y, deltaTime);

							for(int i = 0; i < platformList.size(); i++){
								platformList[i].MoveY(-player.vel_Y, deltaTime);
							}

							for(int i = 0; i < pterodactyl.size(); i++){
								if(pterodactyl[i].state != pterodactyl[i].Idle)
									pterodactyl[i].MoveY(-player.vel_Y, deltaTime);
							}

							for (int i = 0; i < pickUpList.size(); i++) {
								pickUpList[i].MoveY(-player.vel_Y, deltaTime);
							}

							for (int i = 0; i < player.bulletList.size(); i++){
								player.bulletList[i].BulletMoveY(-player.vel_Y, deltaTime);
							}
						} else {
							Y_pos = testRect.y;
							RY_pos = rightGroundRect.y;
							LY_pos = leftGroundRect.y;
							airWayY_pos = airWayRect.y;

							// move the test enemy
							//eMoveY = enemyRect.y;
						}
					}
				} else {
					if(bossInit){
						bossInit = false;
						canSpawn = false;
						dragon.state = dragon.DropIn;
						player.speed = 600;
						player.posRect.x = player.pos_X = 105;
						player.posRect.y = player.pos_Y = 610;
						testRect.x = X_pos = -2048;
						testRect.y = 0;

						pickUpList[6].posRect.y = pickUpList[6].pos_Y = 670;
						pickUpList[6].posRect.x = pickUpList[6].pos_X = 200;

						pickUpList[11].posRect.y = pickUpList[11].pos_Y = 660;
						pickUpList[11].posRect.x = pickUpList[11].pos_X = pickUpList[6].posRect.w + 200;

						for(int i = 15; i < platformList.size(); i++){
							platformList[i].posRect.y = 715;
						}
						platformList[15].posRect.x = 100;
						platformList[16].posRect.x = platformList[15].posRect.w + 100;
						platformList[17].posRect.x = (platformList[15].posRect.w*2) + 100;
						platformList[18].posRect.x = (platformList[15].posRect.w*3) + 100;
					}
				}

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// put in display images here with SDL_RenderCopy()

				// display the test background
				SDL_RenderCopy(renderer, testTex, NULL, &testRect);

				// draw the lair gate
				gate.Draw(renderer);

				// draw the platforms
				for(int i = 0; i < platformList.size() - 4; i++){
					platformList[i].Draw(renderer);
				}

				SDL_RenderCopy(renderer, rGTex, NULL, &rightGroundRect);
				SDL_RenderCopy(renderer, lGTex, NULL, &leftGroundRect);

				SDL_RenderCopy(renderer, airWayTex, NULL, &airWayRect);

				// draw the pickup items
				for (int i = 0; i < pickUpList.size(); i++) {
					if(pickUpList[i].active)
						pickUpList[i].draw(renderer);
				}

				dragon.Draw(renderer);

				for (int i = 0; i < pterodactyl.size(); i++){
					if(pterodactyl[i].state != pterodactyl[i].Idle)
						pterodactyl[i].Draw(renderer);
				}

				// draw the player
				player.Draw(renderer);

				// present the renderer
				SDL_RenderPresent(renderer);

			} // end level 1 game loop
			break;
		case WIN:
			win = true;
			cout << "win" << endl;

			SDL_ShowCursor(1);

			// start win scene loop
			while(win){
				// set up frame rate for the case using deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						level1 = false;
						break;
					}
					switch(event.type){
					// enter case for button down with this code - case SDL_CONTROLLERBUTTONDOWN: & if (event.cdevice.which == 0)
					case SDL_MOUSEBUTTONDOWN:
						if(event.button.button == SDL_BUTTON_LEFT){
							//use this area to shoot an arrow

						}
						break;
					case SDL_KEYDOWN:
						if(event.key.keysym.sym == SDLK_ESCAPE){
							quit = true;
							win = false;
						}
						break;
					case SDL_KEYUP:
						break;
					default:break;
					} // end switch event type
				} // end poll event

				// Clear SDL renderer
				SDL_RenderClear(renderer);



				// present the renderer
				SDL_RenderPresent(renderer);

			} // end win scene loop

			break;
		case LOSE:
			lose = true;
			cout << "lose" << endl;

			SDL_ShowCursor(1);

			// start lose scene loop
			while(lose){
				// set up frame rate for the case using deltaTime
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						level1 = false;
						break;
					}
					switch(event.type){
					// enter case for button down with this code - case SDL_CONTROLLERBUTTONDOWN: & if (event.cdevice.which == 0)
					case SDL_MOUSEBUTTONDOWN:
						if(event.button.button == SDL_BUTTON_LEFT){
							//use this area to shoot an arrow

						}
						break;
					case SDL_KEYDOWN:
						if(event.key.keysym.sym == SDLK_ESCAPE){
							quit = true;
							lose = false;
						}
						break;
					case SDL_KEYUP:
						break;
					default:break;
					} // end switch event type
				} // end poll event

				// Clear SDL renderer
				SDL_RenderClear(renderer);



				// present the renderer
				SDL_RenderPresent(renderer);

			} // end lose scene loop

			break;
		default:break;
		} // end gameState loop
	} // end entire game loop

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
