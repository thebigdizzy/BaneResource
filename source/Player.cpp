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

	health = 100;

	gui = GUI(renderer, filePath, audioPath);

	for (int i = 0; i < max; i++){
		platform[i] = false;
	}

	playerPath = filePath + "KingArthur.png";

	texture = IMG_LoadTexture(renderer, playerPath.c_str());

	if(texture == NULL){
		printf("Could not get image: %s\n", SDL_GetError());
	}

	posRect.x = x;
	posRect.y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w/12;
	posRect.h = h/12;

	pos_X = x;
	pos_Y = y;
	oldJumpLevel = newJumpLevel = y;
	jump = false;
	left = false;
	right = false;
	falling = false;
	groundCollisionLeft = false;
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
		if(!jump & !falling){
			oldJumpLevel = pos_Y;
			vel_Y = -200;

			for (int i = 0; i < max; i++){
				platform[i] = false;
			}
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
	if((!jump && !falling) || speed == 600){
		if(pos_X <= (1024 - posRect.w) && pos_X >= 0){
			if(left && !right){
				pos_X -= speed/2 * deltaTime;
			}
			if(right && !left){
				pos_X += speed/2 * deltaTime;
			}
		}
	}

	if(pos_X > 1024 - posRect.w){
		pos_X = 1024 - posRect.w;
	}
	if(pos_Y > 600){
		if(pos_X < 0){
			pos_X = 0;
		}
	} else {
		if(pos_X < 100){
			pos_X = 100;
		}
	}
	if(pos_Y > 700 - posRect.h){
		pos_Y = 700 - posRect.h;
	}
	if(pos_Y < 300){
		pos_Y = 300;
	}
	cout << pos_Y << endl;

	// set up "gravity" simulator
	GravitySimulator(deltaTime);

	posRect.y = (int)(pos_Y + .5f);
	posRect.x = (int)(pos_X + .5f);
}

void Player::GravitySimulator(float deltaTime){
	// set up "gravity" simulator
	if(jump || falling){
		vel_Y += .5f;
		pos_Y += vel_Y * deltaTime;
	}

	for (int i = 0; i < max; i++){
		if(platform[i] && (falling)){
			vel_Y = 0;
			falling = false;
			jump = false;
			break;
		}

		if((groundCollisionLeft || groundCollisionRight) && (falling)){
			vel_Y = 0;
			falling = false;
			jump = false;
			break;
		}
	}

	if(!jump && !falling){
		for (int i = 0; i < max; i++){
			if(platform[i] || groundCollisionRight || groundCollisionLeft){
				falling = false;
				break;
			} else {
				falling = true;
			}
		}
	}

	if(vel_Y > 0){
		falling = true;
		jump = false;
	}
	if(vel_Y < 0){
		falling = false;
		jump = true;
	}
}

void Player::Draw(SDL_Renderer *renderer){
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, 0.0, &center, SDL_FLIP_NONE);

	gui.Draw(renderer);
}

void Player::Reset(){

}
