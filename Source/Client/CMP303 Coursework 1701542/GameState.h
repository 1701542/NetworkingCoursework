#pragma once
#include "State.h"
#include "Level.h"
#include "Character.h"
//#include "NetworkManager.h"

class GameState : protected State
{
public:
	GameState(sf::RenderWindow* hwnd, InputManager* input, _STATE* state);
	~GameState();

	struct message
	{
		float gameTime;
		sf::Vector2f position;
		float rotation;
	};

	void Update(float deltaTime);

	bool UpdatePlayer(unsigned short port, sf::Vector2f position, float rotation);

	void AddNewPlayer(unsigned short port);

	void DeletePlayer(unsigned short port);

	message GetGameDetails() { return { gameTime, playerVector[0].character->getPosition(),  playerVector[0].character->getRotation() }; };

	void SetGameTime(float newGameTime) { gameTime = newGameTime; };

	int GetPlayersSize() { return playerVector.size(); };

	std::vector<unsigned short> GetPlayers();

private:
	void UpdateInput();
	void Render(float deltaTime);

	struct playerStruct
	{
		unsigned short port;
		Character* character = nullptr;
	};

	std::vector<playerStruct> playerVector;

	float gameTime = 0.0f;


	sf::RenderWindow* window;
	InputManager* inputManager;
	_STATE* currentState;

	Level* level;
};

