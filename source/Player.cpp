/*
 * Player.cpp
 *
 *  Created on: Jul 25, 2016
 *      Author: danieltrayler
 */
#include "Player.h"

Player::Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	active = true;

	speed = 300;

	playerPath = filePath + "KingArthur.png";

	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	if(texture == NULL){
		printf("Could not get image: %s\n", SDL_GetError());
	}

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w/8;
	posRect.h = h/8;

	pos_X = x;
	pos_Y = y;
	oldJumpLevel = newJumpLevel = y;
	jump = false;
	left = false;
	right = false;
	falling = false;
	groundCollisionLeft = true;
	groundCollisionRight = false;

	vel_Y = 0;

	xDir = 0;
	xDirOld = 1;

	center.x = posRect.w/2;
	center.y = posRect.h/2;
}

void Player::OnButtonPress(SDL_Event event)
{
	switch(event.key.keysym.sym){
	case SDLK_a:
		left = true;
		break;
	case SDLK_d:
		right = true;
		break;
	case SDLK_SPACE:
		if(!jump){
			oldJumpLevel = pos_Y;
			groundCollisionRight = false;
			groundCollisionLeft = false;
			jump = true;
			vel_Y = -1000;
		}
		break;
	default:break;
	}
}

void Player::OnButtonRelease(SDL_Event event){
	switch(event.key.keysym.sym){
	case SDLK_a:
		left = false;
		break;
	case SDLK_d:
		right = false;
		break;
	default:break;
	}
}

void Player::OnMouseEvent(int x, int y){

}

void Player::Update(float deltaTime){
	// move the player
	if(!jump && (groundCollisionRight || groundCollisionLeft)){
		if(pos_X <= (1024 - posRect.w) && pos_X >= 0){
			if(left && !right){
				pos_X -= speed * deltaTime;
			}
			if(right && !left){
				pos_X += speed * deltaTime;
			}
			//cout << posRect.x << endl;
		}
	}

	if(pos_X > 1024 - posRect.w){
		pos_X = 1024 - posRect.w;
	}

	if(pos_X < 0){
		pos_X = 0;
	}

	if(pos_Y > 768 - posRect.h){
		pos_Y = 768 - posRect.h;
	}
	if(pos_Y < 0){
		pos_Y = 0;
	}
	//cout << groundCollision << endl;

	// set up "gravity" simulator
	if(jump || !groundCollisionRight && !groundCollisionLeft){
		vel_Y += .5f;
		pos_Y += vel_Y * deltaTime;

		if((groundCollisionLeft || groundCollisionRight) && jump && vel_Y > -100){
			oldJumpLevel = newJumpLevel;
			newJumpLevel = pos_Y;
			jump = false;
		}
	} else {
		pos_Y = newJumpLevel;
	}

	posRect.y = (int)(pos_Y + .5f);
	posRect.x = (int)(pos_X + .5f);
}

void Player::Draw(SDL_Renderer *renderer){
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, 0.0, &center, SDL_FLIP_NONE);
}

void Player::Reset(){

}
