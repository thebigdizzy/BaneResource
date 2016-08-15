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
#include "fireBall.h"

using namespace std;

class Dragon {
public:
	bool active;

	// fireball count
	static const int max = 3;

	bool mRight, mLeft, mUp, mDown;
	SDL_Rect posRect, hitRect;
	SDL_Texture *texture, *hitTexture;
	float posX, posY;
	float fireballTimer;

	int health;

	float Angle;

	// the amount of time the
	int turns;

	// create a vector pointer of fireballs to be used
	vector <FireBall*> fireball;

	enum State { Idle, DropIn, MoveLtoR, GetHit, FlyAway };

	State state;

	Dragon() {}

	Dragon(SDL_Renderer *renderer, string filePath, string audioPath, int x, int y) {
		active = true;

		mLeft = true;
		mRight = false;
		mUp = true;
		mDown = false;

		health = (rand()%21) + 80;

		turns = 0;

		Angle = 0;

		state = Idle;
		posRect.x = posX = x;
		posRect.y = posY = y;

		hitRect.x = posRect.x + 50;
		hitRect.y = posRect.y + 50;

		fireballTimer = 0;

		string path = filePath + "Dragon.png";

		texture = IMG_LoadTexture(renderer, path.c_str());
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w / 3;
		posRect.h = h / 3;

		path = filePath + "diamond.png";
		hitTexture = IMG_LoadTexture(renderer, path.c_str());
		SDL_QueryTexture(hitTexture, NULL, NULL, &w, &h);
		hitRect.w = w;
		hitRect.h = h;

		for (int i = 0; i < max; i++) {
			fireball.push_back(new FireBall(renderer, filePath, -1000, -1000));
		}
	}

	void Draw(SDL_Renderer *renderer) {
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
		SDL_RenderCopy(renderer, hitTexture, NULL, &hitRect);

		for (int i = 0; i < max; i++) {
			if (fireball[i]->active) {
				fireball[i]->Draw(renderer);
			}
		}
	}

	void DropInFunc(float deltaTime) {
		if (posRect.y < 250) {
			posY += 200 * deltaTime;
			posRect.y = (int)(posY + .5f);
		}
		if (posRect.y >= 250) {
			posY = 250;
			posRect.y = (int)posY;
			state = MoveLtoR;
		}
	}

	void FlyOff(float deltaTime) {
		if (posRect.y > -700) {
			posY -= 200 * deltaTime;
			posRect.y = (int)(posY + .5f);
		}
		if (posRect.y <= -700) {
			posY = -700;
			posRect.y = (int)(posY + .5f);
			state = Idle;
		}
	}

	void LookAtThePlayer(SDL_Rect PlayerRect) {
		float X = (float)((PlayerRect.x + (PlayerRect.w / 2)) - (posRect.x + (posRect.w / 2)));
		float Y = (float)((PlayerRect.y + (PlayerRect.h / 3.5f)) - (posRect.y + (posRect.h / 3.5f)));

		Angle = atan2(Y, X) * 180 / 3.14f;
	}

	void MoveSideToSide(float deltaTime) {
		if (active) {
			if (mLeft && posX < 500) {
				mRight = true;
				mLeft = false;
			}
			if (mRight && posX > 680) {
				mLeft = true;
				mRight = false;
			}
			if (mLeft && !mRight) {
				posX -= 100 * deltaTime;
				posRect.x = (int)(posX + .5f);
			}
			if (mRight && !mLeft) {
				posX += 100 * deltaTime;
				posRect.x = (int)(posX + .5f);
			}
		}
	}

	void GotHit(float deltaTime) {

		if (posY < 225) {
			mDown = true;
			mUp = false;
		}
		if (mDown) {
			posY += 200 * deltaTime;
			posRect.y = (int)(posY + .5f);
			if (posRect.y >= 250) {
				posRect.y = posY = 250;
				mDown = false;
				mUp = true;
				state = MoveLtoR;
			}
		}
		else if (mUp) {
			posY -= 200 * deltaTime;
			posRect.y = (int)(posY + .5f);
		}
	}

	void Update(float deltaTime, SDL_Rect PlayerRect) {
		// if the dragons health is lower than or equal to 0, put in FlyAway mode
		if (health <= 0) {
			active = false;
			state = FlyAway;
		}

		// update the active fireballs
		for (int i = 0; i < max; i++) {
			if (fireball[i]->active) {
				fireball[i]->Update(deltaTime);
			}
		}

		switch (state) {
		case Idle:
			// do nothing
			break;
		case DropIn:
			DropInFunc(deltaTime);
			break;
		case MoveLtoR:
			MoveSideToSide(deltaTime);
			// track the player for fireball use
			LookAtThePlayer(PlayerRect);

			// increment the timer for the fireballs
			fireballTimer += deltaTime;

			// shoot a fireball if the timer hits a random number of seconds seconds and reset the timer
			if (fireballTimer > (rand() % 1) + 2) {
				fireballTimer = 0;
				CreateFireBall();
			}
			break;
		case GetHit:
			GotHit(deltaTime);

			// increment the timer for the fireballs
			fireballTimer += deltaTime;
			break;
		case FlyAway:
			FlyOff(deltaTime);
			break;
		default:break;
		}

		// move the hit rectangle with the dragon
		hitRect.x = posRect.x + 155;
		hitRect.y = posRect.y + 40;
	}

	void CreateFireBall() {
		// see if there is a bullet active to fire
		for (int i = 0; i < fireball.size(); i++)
		{
			// see if the bullet is not active
			if (fireball[i]->active == false)
			{
				// dragon the over sound - dragons fine through levels, must pause for QUIT
				//Mix_PlayChannel(-1, fire, 0);

				// set bulelt to active
				fireball[i]->active = true;

				// use some math in the x position to get the bullet close to
				// the center of the dragon using dragon width
				fireball[i]->posRect.x = (posRect.x + (posRect.w / 2));
				fireball[i]->posRect.y = (posRect.y + (posRect.h / 3.5));

				// finishing alighning to the dragon center using the texture width
				fireball[i]->posRect.x = fireball[i]->posRect.x - (fireball[i]->posRect.w / 2);
				fireball[i]->posRect.y = fireball[i]->posRect.y - (fireball[i]->posRect.h / 3.5);

				// set the x and y position of the bullet's float positions
				fireball[i]->pos_X = fireball[i]->posRect.x;
				fireball[i]->pos_Y = fireball[i]->posRect.y;

				fireball[i]->Angle = Angle + 1.57f;

				// set the starting point of the bullet
				fireball[i]->sX = fireball[i]->posRect.x;
				fireball[i]->sY = fireball[i]->posRect.y;

				// once bullet is four , break out of loop
				break;

			}
		}
	}

	~Dragon(){
		for (int i = 0; i < max; i++) {
			delete fireball[i];
		}
	}
};
