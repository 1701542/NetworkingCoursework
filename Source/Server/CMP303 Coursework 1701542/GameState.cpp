#include "GameState.h"


GameState::GameState(sf::RenderWindow* hwnd)
{
	window = hwnd;

	level = new Level(window);
}


GameState::~GameState()
{
}

void GameState::AddNewPlayer(unsigned short port)
{
	playerVector.emplace_back();

	playerVector.back().port = port;
	playerVector.back().character = new Character(window);
}

void GameState::UpdatePlayer(unsigned short port, sf::Vector2f position, float rotation)
{
	for (int i = 0; i < playerVector.size();i++)
	{
		if (playerVector[i].port == port)
		{
			playerVector[i].character->setPosition(position);
			playerVector[i].character->setRotation(rotation);
			return;
		}
	}
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

void GameState::Update(float deltaTime)
{
	gameTime += deltaTime;

	if (!playerVector.empty())
		for (auto player : playerVector)
			player.character->Update(deltaTime, level->getMap());

	Render(deltaTime);
}

void GameState::Render(float deltaTime)
{
	window->clear();

	level->Render();

	if(!playerVector.empty())
		for (auto player : playerVector)
			player.character->Render();

	window->display();
}