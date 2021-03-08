#include "GameState.h"
#include <string>


GameState::GameState(sf::RenderWindow* hwnd, InputManager* input, _STATE* state)
{
	window = hwnd;
	inputManager = input;
	currentState = state;

	level = new Level(window);
}


GameState::~GameState()
{
}

void GameState::AddNewPlayer(unsigned short port)
{
	// Create a player at the end of the vector using the default constructor
	playerVector.emplace_back();

	// Initialise the empty player
	playerVector.back().port = port;
	playerVector.back().character = new Character(window, inputManager);
}

// Update a specific player
bool GameState::UpdatePlayer(unsigned short port, sf::Vector2f position, float rotation)
{
	for (int i = 0; i < playerVector.size(); i++)
	{
		if (playerVector[i].port == port)
		{
			playerVector[i].character->setPosition(position);
			playerVector[i].character->setRotation(rotation);
			return true;
		}
	}
	return false;
}

void GameState::DeletePlayer(unsigned short port)
{
	// Loop through the list of players
	for (int i = 0; i < playerVector.size(); i++)
	{
		// If the element port matches then delete the character then erase the element
		if (playerVector[i].port == port)
		{
			delete playerVector[i].character;
			playerVector.erase(playerVector.begin() + i);
			
			//	Return as we only need to delete this one instance of player
			return;
		}
	}
}

// Return a vector containing the identifying factor of each player 
std::vector<unsigned short> GameState::GetPlayers()
{
	std::vector<unsigned short> portVector;

	for (int i = 0; i < playerVector.size(); i++)
		portVector.push_back(playerVector[i].port);

	return portVector;
}

void GameState::Update(float deltaTime)
{
	gameTime += deltaTime;
	playerVector[0].character->Update(deltaTime, level->getMap());

	UpdateInput();
	Render(deltaTime);
}

// Check for input
void GameState::UpdateInput()
{
	if (inputManager->getKey(sf::Keyboard::Space))
	{
		*currentState = _STATE::MENU;
		inputManager->setKey(sf::Keyboard::Space, false);
	}
}

// Render the scene
void GameState::Render(float deltaTime)
{
	window->clear();

	level->Render();

	for (auto player : playerVector)
		player.character->Render();

	window->display();
}