/*
 * Player.h
 *
 *  Created on: Jul 25, 2016
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

#include "GUI.h"
#include "bullet.h"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Player{
public:
	bool active;
	bool bossMode;

	string filepath, audiopath;
	SDL_Renderer *render;

	// bullet variables
	vector <Bullet> bulletList, arrowList;

	// player health
	float health, maxHealth;
	int width;

	// ammo variables
	int ammoCount;

	// arrow pickup 
	int arrowPU;
	bool canShoot;

	int weaponID;

	// keep track of which pickup the player hits
	int pickupNum;

	// GUI object
	GUI gui;

	// bools for movement
	bool left, right, jump, groundCollisionLeft, groundCollisionRight, falling, bowR, bowL;

	// bool for the bow pickup
	bool bow;

	static const int max = 20;

	bool platform[max];

	// string to find the player image
	string playerPath, rPath, lPath;

	SDL_Texture *texture, *rightArm, *leftArm;

	SDL_Rect posRect, rightRect, leftRect, feetRect;

	float xDir, xDirOld;

	float speed;
	float pos_X, pos_Y, lArmPosX, lArmPosY, rArmPosX, rArmPosY;
	float vel_Y;
	float newJumpLevel, oldJumpLevel;

	float rAngle, lAngle;

	SDL_Point rShoulder, lShoulder;

	// audio effects here

	// constructor
	Player(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y);

	// movement of the player
	void OnButtonPress(SDL_Event event);

	// movement of the player
	void OnButtonRelease(SDL_Event event);

	// movement of the mouse
	void OnMouseEvent(int x, int y);

	void OnMouseButtonPress();

	void Update (float deltaTime);

	void GravitySimulator(float deltaTime);

	void Draw(SDL_Renderer *renderer);

	void BowPickup();

	void DamageTaken();

	void HealthPickup();

	void AmmoPickup();

	void ArrowPickup();

	void ChangeWeapon();

	void CreateBullet();

	void Reset();
};
