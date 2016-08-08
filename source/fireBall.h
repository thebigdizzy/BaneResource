/*
* fireBall.h
*
*  Created on: Aug 2, 2016
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

class FireBall {
public:
	// bool for the state of the fireBall
	bool active;

	// fireBall texture
	SDL_Texture *texture;

	// fireBall rectangle for position (X, Y) and size (W, H)
	SDL_Rect posRect;

	// distance
	float distance;

	// fireBall speed
	float speed;

	// player angle passed in
	float Angle;

	// rotating angle for the fireBall
	float rotate;

	// fireBall float positions to prevent precision loss
	float pos_X, pos_Y, sX, sY;

	// fireBall creation method. requires the renderer, and path to the needed image, an x position, and a y position
	FireBall(SDL_Renderer *renderer, string filePath, float x, float y) {
		// set the fireBall initial state
		active = false;

		// set fireBall speed
		speed = 200;

		Angle = 0;
		rotate = 0;
		distance = 0;
		sX = sY = -1000;

		string path = filePath + "FireBall.png";

		// create the texture from the passed renderer and created surface
		texture = IMG_LoadTexture(renderer, path.c_str());

		// set the width and height of the fireBall's rect
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		posRect.w = w / 3;
		posRect.h = h / 3;

		// set the x and y pos of the fireBall rect
		// finish aligning to the player center using the texture width
		posRect.x = x - (posRect.w / 2);
		posRect.y = y;

		// set the float positon values of the texture for precision loss
		pos_X = x;
		pos_Y = y;
	}

	void FireBallMoveX(float Speed, float deltaTime) {
		pos_X += (Speed)* deltaTime;

		// update the fireBall position with code to account for precision loss
		posRect.x = (int)(pos_X + .5f);
	}

	void FireBallMoveY(float Speed, float deltaTime) {
		pos_Y += (Speed)* deltaTime;

		// update the fireBall position with code to account for precision loss
		posRect.y = (int)(pos_Y + .5f);
	}

	// fireBall update - requires deltaTime be passed
	void Update(float deltaTime) {
		// update the rotation angle
		if (rotate <= 360) {
			rotate += 500 * deltaTime;
		}
		else {
			rotate = 0;
		}


		float radians = (Angle * 3.14) / 180;

		float move_x = speed * cos(radians);
		float move_y = speed * sin(radians);

		pos_X += (move_x)* deltaTime;
		pos_Y += (move_y)* deltaTime;

		// update fireBall position with code to account for precision loss
		posRect.x = (int)(pos_X + .5f);
		posRect.y = (int)(pos_Y + .5f);

		if (posRect.x > 1024 || posRect.x < 0 || posRect.y > 768 || posRect.y < 0) {
			Reset();
		}
	}

	// fireBall draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer) {
		SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rotate, NULL, SDL_FLIP_NONE);
	}

	// fireBall reset
	void Reset() {
		// reset the x position off the screen
		posRect.x = -1000;

		// update the pos+X for precision
		pos_X = posRect.x;

		// deactivate the fireBall
		active = false;
	}
};