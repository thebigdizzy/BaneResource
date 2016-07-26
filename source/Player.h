/*
 * Player.h
 *
 *  Created on: Jul 25, 2016
 *      Author: danieltrayler
 */

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

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Player{
public:
	bool active;

	// bools for movement
	bool left, right, jump, groundCollisionLeft, groundCollisionRight, falling;

	// string to find the player image
	string playerPath;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir, xDirOld;

	float speed;
	float pos_X, pos_Y;
	float vel_Y;
	float newJumpLevel, oldJumpLevel;

	SDL_Point center;

	// audio effects here

	// constructor
	Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	// movement of the player
	void OnButtonPress(SDL_Event event);

	// movement of the player
	void OnButtonRelease(SDL_Event event);

	// movement of the mouse
	void OnMouseEvent(int x, int y);

	void Update (float deltaTime);

	void Draw(SDL_Renderer *renderer);

	void Reset();


};
