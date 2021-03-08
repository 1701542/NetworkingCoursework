#pragma once
#include "State.h"
#include "Level.h"
#include "Character.h"

class GameState : protected State
{
public:
	GameState(sf::RenderWindow* hwnd);
	~GameState();

	float getGameTime() { return gameTime; };

	void AddNewPlayer(unsigned short port);

	void UpdatePlayer(unsigned short port, sf::Vector2f position, float rotation);

	void DeletePlayer(unsigned short port);

	void Update(float deltaTime);

private:

	struct playerStruct
	{
		unsigned short port;
		Character* character = nullptr;
	};

	std::vector<playerStruct> playerVector;

	float gameTime = 0.0f;

	void Render(float deltaTime);

	sf::RenderWindow* window;

	Level* level;
};

