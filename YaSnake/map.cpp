
#include "map.h"

Map::Map(unsigned w_width, unsigned w_height)
	: window_width(w_width), window_height(w_height) {
	assert((w_width <= MAX_WINDOW_WIDTH) && (w_height <= MAX_WINDOW_HEIGHT));
}

unsigned Map::getWindowWidth() const {
	return window_width;
}
unsigned Map::getWindowHeight() const{
	return window_height;
}

