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
	SDL_Rect posRect, sOneRect, sTwoRect, sThreeRect;
	SDL_Texture *texture, *stringTex;
	float posX, posY, sOneY, sTwoY, sThreeY;

	SDL_Point center;
	double angle;

	bool fall1, fall2, fall3;

	enum State {Idle, Lower, Fall};

	State state;

	Gate(){}

	Gate(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y){
		posRect.x = posX = x;
		posRect.y = posY = y;

		sOneRect.x = posRect.x;
		sTwoRect.x = posRect.x + 10;
		sThreeRect.x = posRect.x + 20;
		sOneRect.y = sOneY = sTwoY = sThreeY = posRect.y - 50;

		string path = filePath + "barrier.png";

		texture = IMG_LoadTexture(renderer, path.c_str());
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w/2;
		posRect.h = h/2;

		path = filePath + "platform.png";

		stringTex = IMG_LoadTexture(renderer, path.c_str());
		SDL_QueryTexture(stringTex, NULL, NULL, &w, &h);
		sOneRect.w = w/15;
		sOneRect.h = h/15;
		sTwoRect.w = w/15;
		sTwoRect.h = h/15;
		sThreeRect.w = w/15;
		sThreeRect.h = h/15;

		center.x = (posRect.w/2);
		center.y = posRect.h;

		angle = 0;

		fall1 = fall2 = fall3 = false;

		state = Idle;
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopyEx(renderer, texture, NULL, &posRect, angle, &center, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, stringTex, NULL, &sOneRect, 80, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, stringTex, NULL, &sTwoRect, 80, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, stringTex, NULL, &sThreeRect, 80, NULL, SDL_FLIP_NONE);
	}

	void Open(float deltaTime){
		if(angle > -135){
			angle -= 20 * deltaTime;
		} else {
			state = Fall;
		}
	}

	void FallDown(float deltaTime){
		if(angle > -180){
			angle -= 10 * deltaTime;
		}

		posY += 100 * deltaTime;
		posRect.y = (int)(posY + .5f);

		if(posY > 1000){
			state = Idle;
		}
	}

	void Fall1(float deltaTime){
		if(sOneY < 3000){
			sOneY += 200 * deltaTime;
			sOneRect.y = (int)(sOneY + .5f);
		}
	}

	void Fall2(float deltaTime){
		if(sTwoY < 3000){
			sTwoY += 200 * deltaTime;
			sTwoRect.y = (int)(sTwoY + .5f);
		}
	}

	void Fall3(float deltaTime){
		if(sThreeY < 3000){
			sThreeY += 200 * deltaTime;
			sThreeRect.y = (int)(sThreeY + .5f);
		}
	}

	void Update(float deltaTime){
		switch(state){
		case Idle:
			if(fall1){
				// string falling function
				Fall1(deltaTime);
			}
			if(fall2){
				Fall2(deltaTime);
			}
			if(fall3){
				Fall3(deltaTime);
			}
			break;
		case Lower:
			Open(deltaTime);
			break;
		case Fall:
			FallDown(deltaTime);
			break;
		default:break;
		}
	}

	void MoveY(float playerSpeed, float deltaTime){
		posY += playerSpeed * deltaTime;

		posRect.y = (int)(posY + .5f);

		sOneRect.y = sTwoRect.y = sThreeRect.y = posRect.y - 10;
	}

	void MoveX(float playerSpeed, float deltaTime){
		posX += playerSpeed * deltaTime;

		posRect.x = (int)(posX + .5f);

		sOneRect.x = posRect.x;
		sTwoRect.x = posRect.x + 5;
		sThreeRect.x = posRect.x + 10;
	}
};
