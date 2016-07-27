/*
 * platform.h
 *
 *  Created on: Jul 27, 2016
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

class Platform{
public:

	SDL_Texture *texture;

	SDL_Rect posRect;

	float pos_X, pos_Y;

	// starting positions for reset function
	float sPos_X, sPos_Y;

	Platform(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y){

		// string to find the player image
		string platformPath = filePath + "platform.png";

		// set the texture
		texture = IMG_LoadTexture(renderer, platformPath.c_str());

		posRect.x =sPos_X = pos_X = x;
		posRect.y = sPos_Y = pos_Y = y;

		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w/10;
		posRect.h = h/10;

	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
	}

	void Update(float deltaTime){

	}

	void MoveY(float playerSpeed, float deltaTime){
		pos_Y += playerSpeed * deltaTime;

		posRect.y = (int)(pos_Y + .5f);
	}

	void MoveX(float playerSpeed, float deltaTime){
		pos_X += playerSpeed * deltaTime;

		posRect.x = (int)(pos_X + .5f);
	}

	void Reset(){
		pos_X = sPos_X;
		pos_Y = sPos_Y;

		posRect.x = sPos_X;
		posRect.x = sPos_Y;
	}
};
