#pragma once
#include "objects.h"
#include <iostream>
#include <fstream>
#include <string>

typedef enum gameType {
	campaign = 0,
	customCampaign,
	test
}gameType;

class MapManager {
private:
	int row, col;
	gameType type;
public:
	MapManager();
	void changeType(gameType temp);
	void deloadmap(GameManager& temp);
	bool loadmap(GameManager& temp);
	void savemap(GameManager temp);
};