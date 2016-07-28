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
	SDL_Texture *hTopTexture, *hMiddleTexture, *hBottomTexture;

	// health rectangles
	SDL_Rect hTopRect, hMiddleRect, hBottomRect;

	GUI(){}

	GUI(SDL_Renderer *renderer, string filePath, string audioPath){
		string path = filePath + "healthTop.png";

		hTopTexture = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "healthMiddle.png";

		hMiddleTexture = IMG_LoadTexture(renderer, path.c_str());

		path = filePath + "healthBottom.png";

		hBottomTexture = IMG_LoadTexture(renderer, path.c_str());

		// set up the rectangles
		hTopRect.x = 200;
		hTopRect.y = 20;

		int w, h;
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
	}

	void Draw(SDL_Renderer *renderer){
		SDL_RenderCopy(renderer, hBottomTexture, NULL, &hBottomRect);
		SDL_RenderCopy(renderer, hMiddleTexture, NULL, &hMiddleRect);
		SDL_RenderCopy(renderer, hTopTexture, NULL, &hTopRect);
	}
};
