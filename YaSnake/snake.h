#pragma once

#include "SDL.h"
#include "food.h"

#include <queue>

class Snake {
private:
	enum Direction {
		LEFT,
		RIGHT,
		UP,
		DOWN,
		DEFAULT
	};
	struct Segment {
		int x;
		int y;
		Segment& operator=(const Segment& rhs);
		bool operator==(const Segment& rhs);
	};
	std::queue<int> directions;
	std::deque<Segment> tail;

	unsigned score;
	int posX, posY;
	int lastX{ posX }, lastY{ posY };
	int new_dir{ DEFAULT }, prev_dir{ DEFAULT };

	bool running{ true };
	bool dead{ false };
	bool add{ false };
	bool pause{ false };


public:
	Snake();
	Snake(const Snake&) = delete;
	Snake& operator=(const Snake&) = delete;

	int getPosX(int index) const;
	int getPosY(int index) const;
	int getSnakeSize() const;
	unsigned getScore() const;
	bool isAligned() const;
	bool isRunning() const;
	bool isDead() const;

	void updatePosition(unsigned map_width, unsigned map_height);
	void shiftSnake();
	void handleInput();
	bool checkFoodCollison(const Food& food);

};