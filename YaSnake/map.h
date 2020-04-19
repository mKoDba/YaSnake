#pragma once

#include <vector>
#include <cassert>

#include "SDL.h"

constexpr unsigned DEFAULT_WINDOW_WIDTH = 640;
constexpr unsigned DEFAULT_WINDOW_HEIGHT = 480;
constexpr unsigned MAX_WINDOW_WIDTH = 1920;
constexpr unsigned MAX_WINDOW_HEIGHT = 1080;

constexpr unsigned MAP_WIDTH = 20;
constexpr unsigned MAP_HEIGHT = 15;
constexpr unsigned PLAY_WIDTH = MAP_WIDTH - 2;
constexpr unsigned PLAY_HEIGHT = MAP_HEIGHT - 2;
constexpr int TILE_SIZE = 32;

const char map[] =	"00000000000000000000"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"0                  0"\
					"00000000000000000000";

class Map {
private:
	unsigned window_width;
	unsigned window_height;

public:
	Map(unsigned w_width = DEFAULT_WINDOW_WIDTH, unsigned w_height = DEFAULT_WINDOW_HEIGHT);
	unsigned getWindowWidth() const;
	unsigned getWindowHeight() const;
};