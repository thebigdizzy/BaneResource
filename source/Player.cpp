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

	speed = 400;

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
	if(left && !right){
		pos_X -= speed * deltaTime;
	}
	if(right && !left){
		pos_X += speed * deltaTime;
	}

	posRect.x = (int)(pos_X + .5f);
	cout << pos_X << endl;
}

void Player::Draw(SDL_Renderer *renderer){
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, 0.0, &center, SDL_FLIP_NONE);
}

void Player::Reset(){

}
