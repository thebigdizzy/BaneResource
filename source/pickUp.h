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

class PickUp {
public:

	bool active;

	// pickup ID { 1 = arrow pickup, 2 = health pickup, 3 = ammo pickup, 4 = bow pickup }
	int id;

	bool startTimer;
	float timer;

	// movement
	float pos_X, pos_Y;

	SDL_Rect posRect;
	// starting x and y variables for resetting the pickup
	int sX, sY;

	SDL_Texture *texture;

	PickUp(){}

	PickUp(SDL_Renderer *renderer, string filePath, int ID, float x, float y) {
		active = true;

		// pickup ID { 1 = arrow pickup, 2 = health pickup, 3 = ammo pickup, 4 = bow pickup }
		id = ID;

		startTimer = false;
		timer = 0;

		posRect.x = sX = pos_X = x;
		posRect.y = sY = pos_Y = y;

		string path;
		int w, h;

		switch (id)
		{
		case 1:
			path = filePath + "Arrow.png";
			texture = IMG_LoadTexture(renderer, path.c_str());
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			posRect.w = w / 3;
			posRect.h = h / 3;
			break;
		case 2:
			path = filePath + "healthPU.png";
			texture = IMG_LoadTexture(renderer, path.c_str());
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			posRect.w = w / 8;
			posRect.h = h / 8;
			break;
		case 3:
			path = filePath + "Quiver.png";
			texture = IMG_LoadTexture(renderer, path.c_str());
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			posRect.w = w / 3;
			posRect.h = h / 3;
			break;
		case 4:
			path = filePath + "Bow.png";
			texture = IMG_LoadTexture(renderer, path.c_str());
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			posRect.w = w/3;
			posRect.h = h/3;
			break;
		default:
			break;
		}
	}

	int playerHit(SDL_Rect player) {
		if (SDL_HasIntersection(&player, &posRect)) {
			posRect.x = -5000;
			active = false;
			switch (id)
			{
			case 1:
				// return 1 for arrow pickup
				return 1;
				break;
			case 2:
				startTimer = true;
				// return 2 for health pickup
				return 2;
				break;
			case 3:
				startTimer = true;
				// return 3 for ammo pickup
				return 3;
				break;
			case 4:
				// return 4 for bow pickup
				return 4;
				break;
			default:break;
			}
		}
		return 0;
	}

	void reset() {
		active = true;
		timer = 0;
		startTimer = false;
		posRect.x = pos_X;
		posRect.y = pos_Y;
	}

	void draw(SDL_Renderer * renderer) {
		SDL_RenderCopy(renderer, texture, NULL, &posRect);
	}

	void update(float deltaTime) {
		if (startTimer) {
			timer += deltaTime;
			if (timer > 8) {
				// reset the pickup item
				reset();
			}
		}
	}

	void MoveY(float playerSpeed, float deltaTime) {
		pos_Y += playerSpeed * deltaTime;
		if(active)
			posRect.y = (int)(pos_Y + .5f);
	}

	void MoveX(float playerSpeed, float deltaTime) {
		pos_X += playerSpeed * deltaTime;

		if (active)
			posRect.x = (int)(pos_X + .5f);
	}
};
