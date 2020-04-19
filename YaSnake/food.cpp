
#include "food.h"

Food::Food() : posX{ 0 }, posY{ 0 } {};

Food::~Food() {
    SDL_DestroyTexture(texture);
}

int Food::getPosX() const {
    return posX;
}

int Food::getPosY() const {
    return posY;
}

SDL_Texture* Food::getFoodTexture() const {
    return texture;
}

void Food::setFoodPos(SDL_Renderer* renderer) {
    SDL_Surface* food_surface = IMG_Load("./food.png");
    if (!food_surface) {
        std::cerr << "Error in IMG_Load: " << SDL_GetError() << std::endl;
        return;
    }

    if ((food_surface->w % TILE_SIZE != 0) || (food_surface->h % TILE_SIZE != 0)) {
        std::cerr << "Image has to be multiple of tile size - " << TILE_SIZE << std::endl;
        return;
    }
    int i = RandGenerator::getIntInRange(0, 4);

    // take i-th sprite from .png file and make a new SDL_Surface from it
    const SDL_Rect src_rect{ i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
    SDL_Surface* sprite = SDL_CreateRGBSurface(0, TILE_SIZE, TILE_SIZE, 24, 0, 0, 0, 0);

    SDL_BlitSurface(food_surface, &src_rect, sprite, NULL);
    texture = SDL_CreateTextureFromSurface(renderer, sprite);

    SDL_FreeSurface(food_surface);

    int x = RandGenerator::getIntInRange(1, PLAY_WIDTH);
    int y = RandGenerator::getIntInRange(1, PLAY_HEIGHT);
    posX = x*TILE_SIZE;
    posY = y*TILE_SIZE;
}
