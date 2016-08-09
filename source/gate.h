/*
 * gate.h
 *
 *  Created on: Aug 9, 2016
 *      Author: danieltrayler
 */

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#endif

#include <string>
#include <iostream>
using namespace std;

class Gate{
public:
	SDL_Rect posRect;
	SDL_Texture *texture;
	float posX, posY;

	SDL_Point center;
	double angle;

	enum State {Idle, Lower, Rise};

	State state;

	Gate(){}

	Gate(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y){
		posRect.x = posX = x;
		posRect.y = posY = y;
		string path = filePath + "barrier.png";

		center.x = posRect.x + (posRect.w/2);
		center.y = posRect.y + posRect.h;

		texture = IMG_LoadTexture(renderer, path.c_str());
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w/2;
		posRect.h = h/2;

		angle = 0;

		state = Idle;
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopyEx(renderer, texture, NULL, &posRect, angle, &center, SDL_FLIP_NONE);
	}

	void Update(float deltaTime){
		//angle += deltaTime;
		switch(state){
		case Idle:

			break;
		case Lower:

			break;
		case Rise:

			break;
		default:break;
		}
	}

	void MoveY(float playerSpeed, float deltaTime){
		posY += playerSpeed * deltaTime;

		posRect.y = (int)(posY + .5f);
	}

	void MoveX(float playerSpeed, float deltaTime){
		posX += playerSpeed * deltaTime;

		posRect.x = (int)(posX + .5f);
	}
};
