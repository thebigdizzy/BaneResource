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

	// test the health decrement with this
	string enemyPath = imageDir + "KingArthur.png";
	SDL_Texture *enemy = NULL;
	enemy = IMG_LoadTexture(renderer, enemyPath.c_str());
	SDL_Rect enemyRect;
	enemyRect.x = 500;
	enemyRect.y = 550;
	SDL_QueryTexture(enemy, NULL, NULL, &w, &h);
	enemyRect.w = w/12;
	enemyRect.h = h/12;
	float eMoveX = enemyRect.x;
	float eMoveY = enemyRect.y;
	bool enemyActive = true;
	float enemyTimer = 0;

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
	pickUpList.push_back(PickUp(renderer, imageDir, 4, 200, 500));

	// ***** SDL Event to handle input
	SDL_Event event;

	//***** set up variables for the game states
	enum GameState { MENU, LEVEL1, WIN, LOSE };

	// ***** set up the initial state
	GameState gameState = LEVEL1;

	// bool value to control movement through the states
	bool menu = false, level1 = false, win = false, lose = false, quit = false;

	// mouse position in X and Y value
	int mouseX, mouseY;

	// starting game loop here...
	while(!quit){
		// enter game state machine
		switch(gameState){
		case MENU:
			menu = true;
			cout << "menu" << endl;
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
							//gameState = LEVEL1;
							//menu = false;
							//cout << "mouse button down" << endl;
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
						break;
					case SDL_KEYDOWN:
						if(event.key.keysym.sym == SDLK_ESCAPE){
							quit = true;
							level1 = false;
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

				// update the mouse position
				player.OnMouseEvent(mouseX, mouseY);

				// update the pickups
				for (int i = 0; i < pickUpList.size(); i++) {
					pickUpList[i].update(deltaTime);
				}

				// check for collison with the ground
				player.groundCollisionLeft = SDL_HasIntersection(&player.posRect, &leftGroundRect);
				player.groundCollisionRight = SDL_HasIntersection(&player.posRect, &rightGroundRect);

				// "blow" the player up through the airWay
				if(SDL_HasIntersection(&player.posRect, &airWayRect)){
					if(player.vel_Y >= -200){
						player.vel_Y -= .7f;
					}
				}

				if (!enemyActive) {
					enemyTimer += deltaTime;
					if (enemyTimer > 2) {
						enemyActive = true;
						enemyTimer = 0;
					}
				}

				// take health away from the player when he collides with the "enemy"
				if (SDL_HasIntersection(&player.posRect, &enemyRect) && enemyActive) {
					player.DamageTaken();
					enemyActive = false;
				}

				// check for collision with the platforms
				for(int i = 0; i < platformList.size(); i++){
					player.platform[i] = SDL_HasIntersection(&player.posRect, &platformList[i].posRect);
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

				// update the background
				if(player.right == true){
					X_pos -= (player.speed) * deltaTime;
					RX_pos -= (player.speed) * deltaTime;
					LX_pos -= (player.speed) * deltaTime;
					airWayX_pos -= (player.speed) * deltaTime;

					// move the test enemy
					eMoveX -= player.speed * deltaTime;

					if(testRect.x >= -2000){
						player.speed = 300;
						testRect.x = (int)(X_pos + .5f);
						rightGroundRect.x = (int)(RX_pos + .5f);
						leftGroundRect.x = (int)(LX_pos + .5f);
						airWayRect.x = (int)(airWayX_pos + .5f);

						// move the test enemy
						enemyRect.x = (int)(eMoveX + .5f);

						for(int i = 0; i < platformList.size(); i++){
							platformList[i].MoveX(-player.speed, deltaTime);
						}

						for (int i = 0; i < pickUpList.size(); i++) {
							pickUpList[i].MoveX(-player.speed, deltaTime);
						}
					} else {
						player.speed = 600;
						X_pos = testRect.x;
						RX_pos = rightGroundRect.x;
						LX_pos = leftGroundRect.x;
						airWayX_pos = airWayRect.x;

						// move the test enemy
						eMoveX = enemyRect.x;
					}
				}
				if(player.left == true){
					player.speed = 300;
					X_pos += (player.speed) * deltaTime;
					RX_pos += (player.speed) * deltaTime;
					LX_pos += (player.speed) * deltaTime;
					airWayX_pos += (player.speed) * deltaTime;

					// move the test enemy
					eMoveX += player.speed * deltaTime;

					if(testRect.x <= -2){
						testRect.x = (int)(X_pos + .5f);
						rightGroundRect.x = (int)(RX_pos + .5f);
						leftGroundRect.x = (int)(LX_pos + .5f);
						airWayRect.x = (int)(airWayX_pos + .5f);

						// move the test enemy
						enemyRect.x = (int)(eMoveX + .5f);

						for(int i = 0; i < platformList.size(); i++){
							platformList[i].MoveX(player.speed, deltaTime);
						}

						for (int i = 0; i < pickUpList.size(); i++) {
							pickUpList[i].MoveX(player.speed, deltaTime);
						}
					} else {
						player.speed = 600;
						X_pos = testRect.x;
						RX_pos = rightGroundRect.x;
						LX_pos = leftGroundRect.x;
						airWayX_pos = airWayRect.x;

						// move the test enemy
						eMoveX = enemyRect.x;
					}
				}
				if(player.jump && player.vel_Y < 0){
					Y_pos -= player.vel_Y * deltaTime;
					RY_pos -= player.vel_Y * deltaTime;
					LY_pos -= player.vel_Y * deltaTime;
					airWayY_pos -= (player.vel_Y) * deltaTime;

					// move the test enemy
					eMoveY -= player.vel_Y * deltaTime;

					if(testRect.y < 0){
						testRect.y = (int)(Y_pos + .5f);
						rightGroundRect.y = (int)(RY_pos + .5f);
						leftGroundRect.y = (int)(LY_pos + .5f);
						airWayRect.y = (int)(airWayY_pos + .5f);

						// move the test enemy
						enemyRect.y = (int)(eMoveY + .5f);

						for(int i = 0; i < platformList.size(); i++){
							platformList[i].MoveY(-player.vel_Y, deltaTime);
						}

						for (int i = 0; i < pickUpList.size(); i++) {
							pickUpList[i].MoveY(-player.vel_Y, deltaTime);
						}
					} else {
						Y_pos = testRect.y;
						RY_pos = rightGroundRect.y;
						LY_pos = leftGroundRect.y;
						airWayY_pos = airWayRect.y;

						// move the test enemy
						eMoveY = enemyRect.y;
					}
				}
				if((player.jump && player.vel_Y > 0) || (!player.jump && player.falling)){
					Y_pos -= player.vel_Y * deltaTime;
					RY_pos -= player.vel_Y * deltaTime;
					LY_pos -= player.vel_Y * deltaTime;
					airWayY_pos -= (player.vel_Y) * deltaTime;

					// move the test enemy
					eMoveY -= player.vel_Y * deltaTime;

					if(testRect.y > -1800){
						testRect.y = (int)(Y_pos + .5f);
						rightGroundRect.y = (int)(RY_pos + .5f);
						leftGroundRect.y = (int)(LY_pos + .5f);
						airWayRect.y = (int)(airWayY_pos + .5f);

						// move the test enemy
						enemyRect.y = (int)(eMoveY + .5f);

						for(int i = 0; i < platformList.size(); i++){
							platformList[i].MoveY(-player.vel_Y, deltaTime);
						}

						for (int i = 0; i < pickUpList.size(); i++) {
							pickUpList[i].MoveY(-player.vel_Y, deltaTime);
						}
					} else {
						Y_pos = testRect.y;
						RY_pos = rightGroundRect.y;
						LY_pos = leftGroundRect.y;
						airWayY_pos = airWayRect.y;

						// move the test enemy
						eMoveY = enemyRect.y;
					}
				}

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// put in display images here with SDL_RenderCopy()

				// display the test background
				SDL_RenderCopy(renderer, testTex, NULL, &testRect);

				SDL_RenderCopy(renderer, rGTex, NULL, &rightGroundRect);
				SDL_RenderCopy(renderer, lGTex, NULL, &leftGroundRect);

				SDL_RenderCopy(renderer, airWayTex, NULL, &airWayRect);

				// draw the test enemy
				SDL_RenderCopy(renderer, enemy, NULL, &enemyRect);

				// draw the platforms
				for(int i = 0; i < platformList.size(); i++){
					platformList[i].Draw(renderer);
				}

				// draw the pickup items
				for (int i = 0; i < pickUpList.size(); i++) {
					pickUpList[i].draw(renderer);
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
