#pragma once

#include <iostream>

#include "SDL.h"
#include "SDL_image.h"
#include "map.h"
#include "randGenerator.h"

class Food {
private:
	SDL_Texture* texture{ nullptr };
	int posX, posY;

public:
	Food();
	~Food();

	int getPosX() const;
	int getPosY() const;
	SDL_Texture* getFoodTexture() const;
	void setFoodPos(SDL_Renderer* renderer);
};