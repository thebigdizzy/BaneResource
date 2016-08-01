/*
 * Player.cpp
 *
 *  Created on: Jul 25, 2016
 *      Author: danieltrayler
 */
#include "Player.h"

Player::Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y)
{
	filepath = filePath;
	audiopath = audioPath;
	render = renderer;

	active = true;

	bow = false;

	speed = 300;

	pickupNum = 0;

	health = maxHealth = 100;

	ammoCount = 10;

	arrowPU = 0;

	gui = GUI(renderer, filePath, audioPath);

	width = gui.hMiddleRect.w;

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
	pos_X = x;
	pos_Y = y;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w/12;
	posRect.h = h/12;

	lPath = filePath + "rightArm.png";

	leftArm = IMG_LoadTexture(renderer, lPath.c_str());
	SDL_QueryTexture(leftArm, NULL, NULL, &w, &h);
	leftRect.w = w/12;
	leftRect.h = h/12;

	lAngle = 0;
	lShoulder.x = 0;
	lShoulder.y = leftRect.h/2;

	lArmPosY = leftRect.y = y + 40;
	lArmPosX = leftRect.x = x + posRect.w;

	rPath = filePath + "leftArm.png";

	rightArm = IMG_LoadTexture(renderer, rPath.c_str());
	SDL_QueryTexture(rightArm, NULL, NULL, &w, &h);
	rightRect.w = w/12;
	rightRect.h = h/12;

	rArmPosY = rightRect.y = y + 40;
	rArmPosX = rightRect.x = x;

	rAngle = 0;
	rShoulder.x = 0;
	rShoulder.y = rightRect.h/2;

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

void Player::OnMouseButtonPress(){
	ammoCount--;
}

void Player::OnMouseEvent(int x, int y){
	float X = (float)((x) - (rightRect.x));
	float Y = (float)((y) - (rightRect.y));

	rAngle = atan2(Y, X)*180 / 3.14f;

	X = (float)((x) - (leftRect.x));
	Y = (float)((y) - (leftRect.y));

	lAngle = atan2(Y, X)*180 / 3.14f;

	cout << lAngle << " " << rAngle << endl;
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

	// check for collision with pickups
	switch (pickupNum)
	{
	case 1:
		ArrowPickup();
		pickupNum = 0;
		break;
	case 2:
		HealthPickup();
		pickupNum = 0;
		break;
	case 3:
		AmmoPickup();
		pickupNum = 0;
		break;
	case 4:
		// bow pickup
		BowPickup();
		break;
	default:
		break;
	}

	// set up "gravity" simulator
	GravitySimulator(deltaTime);

	rArmPosY = pos_Y + 30;
	rArmPosX = pos_X + 8;

	if(!bow){
		lArmPosY = pos_Y + 30;
		lArmPosX = pos_X + posRect.w - 8;
	} else {
		lArmPosY = pos_Y + 5;
		lArmPosX = pos_X + posRect.w - 10;
	}

	posRect.y = (int)(pos_Y + .5f);
	posRect.x = (int)(pos_X + .5f);

	rightRect.y = (int)(rArmPosY + .5f);
	rightRect.x = (int)(rArmPosX + .5f);
	leftRect.y = (int)(lArmPosY + .5f);
	leftRect.x = (int)(lArmPosX + .5f);
}

void Player::GravitySimulator(float deltaTime){
	// set up "gravity" simulator
	if(jump || falling){
		vel_Y += 200 * deltaTime;
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
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
	SDL_RenderCopyEx(renderer, leftArm, NULL, &leftRect, lAngle, &lShoulder, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, rightArm, NULL, &rightRect, rAngle, &rShoulder, SDL_FLIP_NONE);

	gui.Draw(renderer, ammoCount, arrowPU);
}

void Player::BowPickup(){
	bow = true;

	rPath = filepath + "leftArm.png";

	int w, h;
	rightArm = IMG_LoadTexture(render, rPath.c_str());
	SDL_QueryTexture(rightArm, NULL, NULL, &w, &h);
	rightRect.w = w/12;
	rightRect.h = h/12;

	rArmPosY = rightRect.y = posRect.y;
	rArmPosX = rightRect.x = posRect.x;

	rShoulder.x = 0;
	rShoulder.y = rightRect.h/2;

	lPath = filepath + "rightArmBow.png";

	leftArm = IMG_LoadTexture(render, lPath.c_str());
	SDL_QueryTexture(leftArm, NULL, NULL, &w, &h);
	leftRect.w = w/12;
	leftRect.h = h/12;

	lArmPosY = leftRect.y = posRect.y + 50;
	lArmPosX = leftRect.x = posRect.x + posRect.w - 10;

	lShoulder.x = 0;
	lShoulder.y = leftRect.h/2;
}

void Player::DamageTaken()
{
	health -= 10;
	gui.hMiddleRect.w = ((health / maxHealth) * width);
}

void Player::HealthPickup()
{
	health = 100;
	gui.hMiddleRect.w = ((health / maxHealth) * width);
}

void Player::AmmoPickup()
{
	ammoCount = 10;
}

void Player::ArrowPickup()
{
	arrowPU++;
}

void Player::Reset(){

}
