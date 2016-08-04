/*
 * Dragon.h
 *
 *  Created on: Aug 3, 2016
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

class Dragon {
public:
	bool active;

	bool mRight, mLeft, mUp, mDown;
	SDL_Rect posRect;
	SDL_Texture *texture;
	float posX, posY;

	int health;

	// the amount of time the
	int turns;

	enum State {Idle, DropIn, MoveLtoR, GetHit};

	State state;

	Dragon(){}

	Dragon(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y){
		active = true;

		mLeft = true;
		mRight = false;
		mUp = true;
		mDown = false;

		health = 100;

		turns = 0;

		state = Idle;
		posRect.x = posX = x;
		posRect.y = posY = y;

		string path = filePath + "Dragon.png";

		texture = IMG_LoadTexture(renderer, path.c_str());
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w/3;
		posRect.h = h/3;
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
	}

	void DropInFunc(float deltaTime){
		if(posRect.y < 250){
			posY += 200 *deltaTime;
			posRect.y = (int)(posY + .5f);
		}
		if(posRect.y >= 250){
			posY = 250;
			posRect.y = (int)posY;
			state = MoveLtoR;
		}
	}

	void MoveSideToSide(float deltaTime){
		if(active){
			if(mLeft && posX < 500){
				mRight = true;
				mLeft = false;
			}
			if(mRight && posX > 680){
				mLeft = true;
				mRight = false;
			}
			if(mLeft && !mRight){
				posX -= 100 * deltaTime;
				posRect.x = (int)(posX + .5f);
			}
			if(mRight && !mLeft){
				posX += 100 * deltaTime;
				posRect.x = (int)(posX + .5f);
			}
		}
	}

	void GotHit(float deltaTime){
		if(posY < 200){
			mDown = true;
			mUp = false;
		}
		if(mDown){
			posY += 300 * deltaTime;
			posRect.y = (int)(posY + .5f);
			if(posRect.y >= 250){
				posRect.y = posY = 250;
				mDown = false;
				mUp = true;
				state = MoveLtoR;
			}
		} else if(mUp){
			posY -= 300 * deltaTime;
			posRect.y = (int)(posY + .5f);
		}
	}

	void Update(float deltaTime){

		if(health <= 0){
			active = false;
		}

		switch(state){
		case Idle:
			// do nothing
			break;
		case DropIn:
			DropInFunc(deltaTime);
			break;
		case MoveLtoR:
			MoveSideToSide(deltaTime);
			break;
		case GetHit:
			GotHit(deltaTime);
			break;
		default:break;
		}
	}
};
