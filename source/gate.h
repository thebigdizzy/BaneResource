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
	float sX, sY;

	SDL_Point center;
	double angle;

	bool fall1, fall2, fall3;

	enum State {Idle, Lower, Fall};

	State state;

	Gate(){}

	Gate(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y){
		posRect.x = posX = sX = x;
		posRect.y = posY = sY = y;

		sOneRect.x = posRect.x - 60;
		sTwoRect.x = posRect.x - 50;
		sThreeRect.x = posRect.x - 40;
		sOneRect.y = sOneY = sTwoY = sThreeY = posRect.y - 100;

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

	void Reset(){
		posRect.x = posX = sX;
		posRect.y = posY = sY;

		sOneRect.y = sOneY = sTwoY = sThreeY = posRect.y - 100;

		angle = 0;
		state = Idle;
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopyEx(renderer, texture, NULL, &posRect, angle, &center, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, stringTex, NULL, &sOneRect, 100, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, stringTex, NULL, &sTwoRect, 100, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, stringTex, NULL, &sThreeRect, 100, NULL, SDL_FLIP_NONE);
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
		if(sOneRect.y < 3000){
			sOneY += 200 * deltaTime;
			sOneRect.y = (int)(sOneY + .5f);
		}
	}

	void Fall2(float deltaTime){
		if(sTwoRect.y < 3000){
			sTwoY += 200 * deltaTime;
			sTwoRect.y = (int)(sTwoY + .5f);
		}
	}

	void Fall3(float deltaTime){
		if(sThreeRect.y < 3000){
			sThreeY += 200 * deltaTime;
			sThreeRect.y = (int)(sThreeY + .5f);
		}

		if(sThreeRect.y >= 500){
			state = Lower;
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

			//cout << fall1 << fall2 << fall3 << endl;
			break;
		case Lower:
			Fall1(deltaTime);
			Fall2(deltaTime);
			Fall3(deltaTime);
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

		if(!fall1)
			sOneRect.y = posRect.y - 100;
		if(!fall2)
			sTwoRect.y = posRect.y - 100;
		if(!fall3)
			sThreeRect.y = posRect.y - 100;
	}

	void MoveX(float playerSpeed, float deltaTime){
		posX += playerSpeed * deltaTime;

		posRect.x = (int)(posX + .5f);

		if(!fall1)
			sOneRect.x = posRect.x - 60;
		if(!fall2)
			sTwoRect.x = posRect.x - 50;
		if(!fall3)
			sThreeRect.x = posRect.x - 40;
	}
};
