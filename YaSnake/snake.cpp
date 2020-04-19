
#include <iostream>
#include <algorithm>
#include "snake.h"
#include "map.h"


Snake::Snake() 
    : posX{ 2 * TILE_SIZE }, posY{ 2 * TILE_SIZE }, score(0) {
    tail.emplace_back(Segment{ posX, posY });
};

Snake::Segment& Snake::Segment::operator=(const Segment& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
}

bool Snake::Segment::operator==(const Segment& rhs) {
    return ((this->x == rhs.x) && (this->y == rhs.y));
}

int Snake::getPosX(int i) const {
	return tail[i].x;
}

int Snake::getPosY(int i) const {
	return tail[i].y;
}

int Snake::getSnakeSize() const {
    return (tail.size());
}

unsigned Snake::getScore() const {
    return score;
}

bool Snake::isAligned() const {
    return ((tail.back().x % TILE_SIZE == 0) && (tail.back().y % TILE_SIZE == 0));
}

bool Snake::isRunning() const {
    return running;
}

bool Snake::isDead() const {
    return dead;
}

void Snake::shiftSnake() {
    for (unsigned i = 0; i < tail.size() - 1; ++i) {
        tail[i] = tail[i + 1];
    }
    tail.back() = { posX, posY };
}

void Snake::updatePosition(unsigned map_width, unsigned map_height) {

    if (pause) {
        return;
    }

    if (add) {
        for (auto i = 0; i < TILE_SIZE; i = i+2) {
            tail.emplace_front(Snake::Segment{ lastX, lastY });
        }
        add = false;
    }
    
    /* check if head had collision with tail */
    if (std::find(std::begin(tail), std::end(tail) - 1, tail.back()) != tail.end() - 1) {
        dead = true;
    }
   
    if (!dead) {
        shiftSnake();
    }

    if (isAligned()) {
        lastX = tail.front().x;
        lastY = tail.front().y;

        /* get new direction from queue */
        if (directions.empty()) {
            new_dir = prev_dir;
        }
        else {
            new_dir = directions.front();
            directions.pop();
        }
    }


    // TODO: do this in more simple and readable way
    /* check if snake bumped into map border */
    if ((tail.back().x >= map_width-1.90*TILE_SIZE) ||
        (tail.back().x<= 0.95*TILE_SIZE) ||
        (tail.back().y >= map_height-1.90*TILE_SIZE)||
        (tail.back().y <= 0.95*TILE_SIZE)){
        new_dir = DEFAULT;
        dead = true;
        return;
    }
    
    // TODO: do this in more simple and readable way
    /* new direction should not be reverse of current direction */
    if ((new_dir == LEFT && prev_dir == RIGHT) ||
        (new_dir == RIGHT && prev_dir == LEFT) ||
        (new_dir == UP && prev_dir == DOWN) ||
        (new_dir == DOWN && prev_dir == UP)) {
        new_dir = prev_dir;
    }


    prev_dir = new_dir;
    switch (new_dir){
    case LEFT:
        posX -= 2;
        break;
    case RIGHT:
        posX += 2;
        break;
    case UP:
        posY -= 2;
        break;
    case DOWN:
        posY += 2;
        break;
    case DEFAULT:
        posX = posX;
        posY = posY;
        break;
    default:
        break;
    }
}

void Snake::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (SDL_QUIT == event.type ||
            (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym)) {
            running = false;
        }
        /* push input to the queue */
        if (SDL_KEYDOWN == event.type) {
            if ('a' == event.key.keysym.sym || SDLK_LEFT == event.key.keysym.sym) directions.push(LEFT);
            if ('d' == event.key.keysym.sym || SDLK_RIGHT == event.key.keysym.sym) directions.push(RIGHT);
            if ('w' == event.key.keysym.sym || SDLK_UP == event.key.keysym.sym) directions.push(UP);
            if ('s' == event.key.keysym.sym || SDLK_DOWN == event.key.keysym.sym) directions.push(DOWN);
            if ('p' == event.key.keysym.sym) pause ^= 1;
        }
    }
}

bool Snake::checkFoodCollison(const Food& food) {
    int foodX = food.getPosX();
    int foodY = food.getPosY();
    if (((foodX == posX) && (foodY == (posY - (TILE_SIZE - 4)))) ||
        ((foodX == posX) && (foodY == (posY + (TILE_SIZE - 4)))) ||
        ((foodY == posY) && (foodX == (posX + (TILE_SIZE - 4)))) ||
        ((foodY == posY) && (foodX == (posX - (TILE_SIZE - 4))))
        ) {
        add = true;
        score += 10;
        return true;
    }
    return false;
}
