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
	bowR = false;
	bowL = false;

	speed = 300;

	pickupNum = 0;

	health = maxHealth = 100;

	ammoCount = 0;

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

	feetRect.h = 1;
	feetRect.w = posRect.w;
	feetRect.x = posRect.x;
	feetRect.y = posRect.y + posRect.h;

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

	weaponID = 0;
	canShoot = false;

	//create the player's bullet pool
	for (int i = 0; i < 10; i++)
	{
		//add to bulletList
		bulletList.push_back(Bullet(renderer, filePath, -1000, -1000));
	}

	//create the player's bullet pool
	for (int i = 0; i < 3; i++)
	{
		//add to bulletList
		arrowList.push_back(Bullet(renderer, filePath, -1000, -1000));
	}
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
	if(ammoCount > 0 || arrowPU > 0){
		CreateBullet();
	}
}

void Player::OnMouseEvent(int x, int y){

	gui.cursorRect.x = x - (gui.cursorRect.w/2);
	gui.cursorRect.y = y + (gui.cursorRect.h) - 15;

	float X = (float)((x) - (rightRect.x));
	float Y = (float)((y) - (rightRect.y));

	if(X < 0){
		bowL = true;
		bowR = false;
	} else {
		bowL = false;
		bowR = true;
	}

	rAngle = atan2(Y, X) * 180 / 3.14f;

	X = (float)((x) - (leftRect.x));
	Y = (float)((y) - (leftRect.y));

	lAngle = atan2(Y, X) * 180 / 3.14f;
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
	feetRect.x = posRect.x;
	feetRect.y = posRect.y + posRect.h;

	// update the bullet list
	for (int i = 0; i < bulletList.size(); i++){
		if(bulletList[i].posRect.x > 1024 || bulletList[i].posRect.x < 0 ||
				bulletList[i].posRect.y > 768 || bulletList[i].posRect.y < 0){
			bulletList[i].Reset();
		}

		bulletList[i].Update(deltaTime);
	}

	// update the arrow list
		for (int i = 0; i < arrowList.size(); i++){
			if(arrowList[i].posRect.x > 1024 || arrowList[i].posRect.x < 0 ||
					arrowList[i].posRect.y > 768 || arrowList[i].posRect.y < 0){
				arrowList[i].Reset();
			}

			arrowList[i].Update(deltaTime);
		}
}

void Player::GravitySimulator(float deltaTime){
	// set up "gravity" simulator
	if(jump || falling){
		vel_Y += 200 * deltaTime;
		pos_Y += vel_Y * deltaTime;
	}

	for (int i = 0; i < max; i++){
		if(platform[i] && (falling)){
			vel_Y = -1;
			falling = false;
			jump = false;
			break;
		}

		if((groundCollisionLeft || groundCollisionRight) && (falling)){
			vel_Y = -1;
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
	if(vel_Y < -1){
		falling = false;
		jump = true;
	}
}

void Player::Draw(SDL_Renderer *renderer){
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
	SDL_RenderCopyEx(renderer, leftArm, NULL, &leftRect, lAngle, &lShoulder, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, rightArm, NULL, &rightRect, rAngle, &rShoulder, SDL_FLIP_NONE);

	gui.Draw(renderer, ammoCount, arrowPU);

	for (int i = 0; i < bulletList.size(); i++){
		bulletList[i].Draw(renderer);
	}

	for (int i = 0; i < arrowList.size(); i++){
		arrowList[i].Draw(renderer);
	}
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
	if(arrowPU > 2){
		canShoot = true;
	}
}

void Player::ChangeWeapon(){
	if(weaponID){
		weaponID = 0;
	} else {
		weaponID = 1;
	}
}

void Player::CreateBullet()
{
	switch(weaponID){
	case 0:
	{
		// see if there is a bullet active to fire
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (ammoCount) {
				// see if the bullet is not active
				if (bulletList[i].active == false)
				{
					ammoCount--;
					// player the over sound - players fine through levels, must pause for QUIT
					//Mix_PlayChannel(-1, fire, 0);

					// set bulelt to active
					bulletList[i].active = true;

					// use some math in the x position to get the bullet close to
					// the center of the player using player width
					bulletList[i].posRect.x = (posRect.x + (posRect.w / 2));
					bulletList[i].posRect.y = (posRect.y + (posRect.h / 2));

					// finishing alighning to the player center using the texture width
					bulletList[i].posRect.x = bulletList[i].posRect.x - (bulletList[i].posRect.w / 2);
					bulletList[i].posRect.y = bulletList[i].posRect.y - (bulletList[i].posRect.h / 2);

					// set the x and y position of the bullet's float positions
					bulletList[i].pos_X = bulletList[i].posRect.x;
					bulletList[i].pos_Y = bulletList[i].posRect.y;

					bulletList[i].Angle = lAngle;

					// set the starting point of the bullet
					bulletList[i].sX = bulletList[i].posRect.x;
					bulletList[i].sY = bulletList[i].posRect.y;

					// once bullet is four , break out of loop
					break;

				}
			}
		}
		break;
	}
	case 1:
	{
		// see if there is a bullet active to fire
		for (int i = 0; i < arrowList.size(); i++)
		{
			if (arrowPU && canShoot) {
				// see if the bullet is not active
				if (arrowList[i].active == false)
				{
					arrowPU--;
					// player the over sound - players fine through levels, must pause for QUIT
					//Mix_PlayChannel(-1, fire, 0);

					// set bulelt to active
					arrowList[i].active = true;

					// change the size of the arrow
					arrowList[i].posRect.w = bulletList[0].posRect.w * 4;
					arrowList[i].posRect.h = bulletList[0].posRect.h * 4;

					// use some math in the x position to get the bullet close to
					// the center of the player using player width
					arrowList[i].posRect.x = (posRect.x + (posRect.w / 2));
					arrowList[i].posRect.y = (posRect.y + (posRect.h / 2));

					// finishing alighning to the player center using the texture width
					arrowList[i].posRect.x = arrowList[i].posRect.x - (arrowList[i].posRect.w / 2);
					arrowList[i].posRect.y = arrowList[i].posRect.y - (arrowList[i].posRect.h / 2);

					// set the x and y position of the bullet's float positions
					arrowList[i].pos_X = arrowList[i].posRect.x;
					arrowList[i].pos_Y = arrowList[i].posRect.y;

					arrowList[i].Angle = lAngle;

					// set the starting point of the bullet
					arrowList[i].sX = arrowList[i].posRect.x;
					arrowList[i].sY = arrowList[i].posRect.y;

					// once bullet is four , break out of loop
					break;
				}
			}
		}
		break;
	}
	default:break;
	}
}

void Player::Reset(){

}
