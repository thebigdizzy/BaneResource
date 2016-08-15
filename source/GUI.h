/*
 * GUI.h
 *
 *  Created on: Jul 28, 2016
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

using namespace std;

class GUI{
public:
	SDL_Texture *hTopTexture, *hMiddleTexture, *hBottomTexture, *glow;

	// health rectangles
	SDL_Rect hTopRect, hMiddleRect, hBottomRect;

	SDL_Texture *aBackTexture, *aTexture, *Zero, *One, *Two, *Three, *Four, *Five, *Six, *Seven, *Eight, *Nine, *Ten;

	// arrow pickup rectangles
	SDL_Rect aBackRect, aOneRect, aTwoRect, aThreeRect, ammoRect;

	// cursor rectangles
	SDL_Rect cursorRect;

	SDL_Texture *cursor;

	GUI(){}

	GUI(SDL_Renderer *renderer, string filePath, string audioPath){

		//cursor
		string path = filePath + "cursor.png";

		cursor = IMG_LoadTexture(renderer, path.c_str());

		// health bar
		path = filePath + "healthTop.png";

		hTopTexture = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "healthMiddle.png";

		hMiddleTexture = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "healthBottom.png";

		hBottomTexture = IMG_LoadTexture(renderer, path.c_str());

		// arrow pickups
		path = filePath + "arrowOutline.png";

		aBackTexture = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "arrowFill.png";

		aTexture = IMG_LoadTexture(renderer, path.c_str());

		// ammo count
		path = filePath + "quiver0.png";

		Zero = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver1.png";

		One = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver2.png";

		Two = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver3.png";

		Three = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver4.png";

		Four = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver5.png";

		Five = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver6.png";

		Six = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver7.png";

		Seven = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver8.png";

		Eight = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver9.png";

		Nine = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "quiver10.png";

		Ten = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "glow.png";
		glow = IMG_LoadTexture(renderer, path.c_str());

		// set up the cursor rectangle
		cursorRect.x = cursorRect.y = 0;

		int w, h;
		SDL_QueryTexture(cursor, NULL, NULL, &w, &h);
		cursorRect.w = w/3;
		cursorRect.h = h/3;

		// set up the health rectangles
		hTopRect.x = 200;
		hTopRect.y = 20;


		SDL_QueryTexture(hTopTexture, NULL, NULL, &w, &h);
		hTopRect.w = w/3;
		hTopRect.h = h/3;

		hMiddleRect.x = 275;
		hMiddleRect.y = 27;

		SDL_QueryTexture(hMiddleTexture, NULL, NULL, &w, &h);
		hMiddleRect.w = w/3;
		hMiddleRect.h = h/3;

		hBottomRect.x = 275;
		hBottomRect.y = 27;

		SDL_QueryTexture(hBottomTexture, NULL, NULL, &w, &h);
		hBottomRect.w = w/3;
		hBottomRect.h = h/3;

		// set up the arrow pickup rectangles
		aBackRect.x = 20;
		aBackRect.y = 19;
		SDL_QueryTexture(aBackTexture, NULL, NULL, &w, &h);
		aBackRect.w = w/3;
		aBackRect.h = h/3;

		aOneRect.x = 21;
		aOneRect.y = 20;
		SDL_QueryTexture(aTexture, NULL, NULL, &w, &h);
		aOneRect.w = w/3;
		aOneRect.h = h/3;

		aTwoRect.x = 56;
		aTwoRect.y = 20;
		aTwoRect.w = w/3;
		aTwoRect.h = h/3;

		aThreeRect.x = 90;
		aThreeRect.y = 20;
		aThreeRect.w = w/3;
		aThreeRect.h = h/3;

		// set up the ammo counter rectangle
		ammoRect.x = 900;
		ammoRect.y = 650;
		SDL_QueryTexture(Ten, NULL, NULL, &w, &h);
		ammoRect.w = w/3;
		ammoRect.h = h/3;
	}

	void DrawGlow(int weaponID, SDL_Renderer *renderer){
		if(!weaponID){
			SDL_RenderCopy(renderer, glow, NULL, &ammoRect);
		} else {
			SDL_RenderCopy(renderer, glow, NULL, &aBackRect);
		}
	}

	void Draw(SDL_Renderer *renderer, int ammoCount, int arrowPU) {

		// draw the cursor
		SDL_RenderCopy(renderer, cursor, NULL, &cursorRect);

		// draw the health gui
		SDL_RenderCopy(renderer, hBottomTexture, NULL, &hBottomRect);
		SDL_RenderCopy(renderer, hMiddleTexture, NULL, &hMiddleRect);
		SDL_RenderCopy(renderer, hTopTexture, NULL, &hTopRect);

		// draw the ammo pickups gui
		SDL_RenderCopy(renderer, aBackTexture, NULL, &aBackRect);

		switch (arrowPU)
		{
		case 1:
			SDL_RenderCopy(renderer, aTexture, NULL, &aOneRect);
			break;
		case 2:
			SDL_RenderCopy(renderer, aTexture, NULL, &aOneRect);
			SDL_RenderCopy(renderer, aTexture, NULL, &aTwoRect);
			break;
		case 3:
			SDL_RenderCopy(renderer, aTexture, NULL, &aOneRect);
			SDL_RenderCopy(renderer, aTexture, NULL, &aTwoRect);
			SDL_RenderCopy(renderer, aTexture, NULL, &aThreeRect);
			break;
		default:
			break;
		}

		// draw the ammo counter gui
		switch (ammoCount)
		{
		case 0:
			SDL_RenderCopy(renderer, Zero, NULL, &ammoRect);
			break;
		case 1:
			SDL_RenderCopy(renderer, One, NULL, &ammoRect);
			break;
		case 2:
			SDL_RenderCopy(renderer, Two, NULL, &ammoRect);
			break;
		case 3:
			SDL_RenderCopy(renderer, Three, NULL, &ammoRect);
			break;
		case 4:
			SDL_RenderCopy(renderer, Four, NULL, &ammoRect);
			break;
		case 5:
			SDL_RenderCopy(renderer, Five, NULL, &ammoRect);
			break;
		case 6:
			SDL_RenderCopy(renderer, Six, NULL, &ammoRect);
			break;
		case 7:
			SDL_RenderCopy(renderer, Seven, NULL, &ammoRect);
			break;
		case 8:
			SDL_RenderCopy(renderer, Eight, NULL, &ammoRect);
			break;
		case 9:
			SDL_RenderCopy(renderer, Nine, NULL, &ammoRect);
			break;
		case 10:
			SDL_RenderCopy(renderer, Ten, NULL, &ammoRect);
			break;
		default:
			SDL_RenderCopy(renderer, Zero, NULL, &ammoRect);
			break;
		}
	}
};
