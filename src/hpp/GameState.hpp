#pragma once

#include "Game.hpp"

class GameState {
public:
	virtual void draw() = 0;
	virtual void update(const float dt) = 0;
	virtual void handleInput() = 0;
};
