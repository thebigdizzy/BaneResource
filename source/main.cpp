// Using SDL2 to create an application window

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
//#include "SDL2_mixer/SDL_mixer.h"
//#include "SDL2_ttf/SDL_ttf.h"
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
#include <unistd.h> // sleep

// player include
#include "Player.h"

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

	// back groung x and y pos
	float X_pos = testRect.x;
	float Y_pos = testRect.y;

	// initialize the Player object
	Player player(renderer, imageDir, audioDir, 0,525);

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

				// Fill the surface white
				//SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));

				// Update the surface
				//SDL_UpdateWindowSurface(window);

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

					mouseX = event.button.x;
					mouseY = event.button.y;

					player.OnMouseEvent(mouseX, mouseY);
					default:break;
					} // end switch event type
				} // end poll event

				// update the player
				player.Update(deltaTime);

				// update the background
				if((player.posRect.x >= 1024 - player.posRect.w) && player.right == true){

					X_pos -= (player.speed) * deltaTime;

					if(testRect.x >= -512){
						testRect.x = (int)(X_pos + .5f);
					} else {
						X_pos = testRect.x;
					}
				}
				cout << X_pos << endl;

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// put in display images here with SDL_RenderCopy()

				// display the test background
				SDL_RenderCopy(renderer, testTex, NULL, &testRect);

				// draw the player
				player.Draw(renderer);

				// Fill the surface white
				//SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));

				// Update the surface
				//SDL_UpdateWindowSurface(window);

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
						win = false;
						break;
					}
					switch(event.type){
					// enter case for button down with this code - case SDL_CONTROLLERBUTTONDOWN: & if (event.cdevice.which == 0)
					case SDL_MOUSEBUTTONDOWN:
						if(event.button.button == SDL_BUTTON_LEFT){
							//gameState = LOSE;
							//win = false;
						}
						break;

					mouseX = event.button.x;
					mouseY = event.button.y;
					default:break;
					} // end switch event type
				} // end poll event

				// Clear SDL renderer
				//SDL_RenderClear(renderer);

				// put in display images here with SDL_RenderCopy()
				// Fill the surface white
				SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));

				// Update the surface
				SDL_UpdateWindowSurface(window);

				// present the renderer
				//SDL_RenderPresent(renderer);

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
						lose = false;
						break;
					}
					switch(event.type){
					// enter case for button down with this code - case SDL_CONTROLLERBUTTONDOWN: & if (event.cdevice.which == 0)
					case SDL_MOUSEBUTTONDOWN:
						if(event.button.button == SDL_BUTTON_LEFT){
							//quit = true;
							//lose = false;
						}
						break;

					mouseX = event.button.x;
					mouseY = event.button.y;
					default:break;
					} // end switch event type
				} // end poll event

				// Clear SDL renderer
				//SDL_RenderClear(renderer);

				// put in display images here with SDL_RenderCopy()
				// Fill the surface white
				SDL_FillRect( screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 42, 254));

				// Update the surface
				SDL_UpdateWindowSurface(window);

				// present the renderer
				//SDL_RenderPresent(renderer);

			} // end lose scene loop

			break;
		default:break;
		} // end gameState loop
	} // end entire game loop

	// The window is open: could enter program loop here (see SDL_PollEvent())

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
