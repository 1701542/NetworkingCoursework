#pragma once
#include "TileMap.h"

class Level
{
public:
	Level(sf::RenderWindow* hwnd);
	~Level();

	void Render();
	std::vector<int> getMap() { return obstacleMapCopy; };

private:
	void Init();

	sf::RenderWindow* window;
	TileMap groundTileMap, obstacleTileMap;

	int width = 32, height = 18;
	std::vector<int> obstacleMapCopy;

};

