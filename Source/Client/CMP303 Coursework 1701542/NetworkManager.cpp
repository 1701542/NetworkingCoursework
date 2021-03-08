#include "NetworkManager.h"



NetworkManager::NetworkManager(GameState* game)
{
	gameState = game;

	udpSocket.setBlocking(false);

	udpStatus = udpSocket.bind(sf::Socket::AnyPort, sf::IpAddress::getLocalAddress());

	if (udpStatus != sf::Socket::Done)
		std::cout << "UDP socket failed to bind. Error: " << udpStatus << std::endl;
	else
		std::cout << "UDP socket successfully binded. " << sf::IpAddress::getLocalAddress() << "\t" << udpSocket.getLocalPort() << std::endl;

	gameState->AddNewPlayer(udpSocket.getLocalPort());
}


NetworkManager::~NetworkManager()
{
}

void NetworkManager::Update(float deltaTime)
{
	SendMessage(deltaTime);

	ReceiveMessageUDP();
}

void NetworkManager::SendMessage(float deltaTime)
{
	timer += deltaTime;

	if (timer >= interval)
	{
		packet << gameState->GetGameDetails().position.x
			<< gameState->GetGameDetails().position.y
			<< gameState->GetGameDetails().rotation;

		//if(rand() % 100 > 35)
			udpStatus = udpSocket.send(packet,
				sf::IpAddress::getLocalAddress(),
				SERVERUDPPORT);

		packet.clear();

		if (udpStatus != sf::Socket::Done)
			std::cout << "Failed to send packet. Error: "
			<< udpStatus
			<< std::endl;

		timer = 0.0f;
	}
}

void NetworkManager::ReceiveMessageUDP()
{
	udpStatus = udpSocket.receive(packet, receiveAddress, receivePort);

	if (udpStatus == sf::Socket::Done)
	{
		size_t packetSize;
		float newGameTime;
		packet >> packetSize >> newGameTime;

		gameState->SetGameTime(newGameTime);

		std::vector<unsigned short> playerPorts;
		std::vector<sf::Vector2f> playerPositions;
		std::vector<float> playerRotations;

		for (int i = 0; i < packetSize; i++)
		{
			playerPorts.emplace_back();
			playerPositions.emplace_back();
			playerRotations.emplace_back();

			packet >> playerPorts.back()
				>> playerPositions.back().x
				>> playerPositions.back().y
				>> playerRotations.back();
		}

		// If true, a player needs to be removed
		if (packetSize < gameState->GetPlayersSize())
		{
			for (int i = 0; i < gameState->GetPlayers().size(); i++)
			{
				for (int j = 0; j < packetSize; j++)
				{
					if (gameState->GetPlayers()[i] == playerPorts[j])
					{
						goto nextPlayer;
					}
				}
				gameState->DeletePlayer(gameState->GetPlayers()[i]);

				nextPlayer:;
			}
		}
		
		// Update the players and add any new players
		for (int i = 0; i < packetSize; i++)
		{
			if (!gameState->UpdatePlayer(playerPorts.back(), playerPositions.back(), playerRotations.back()))
			{
				gameState->AddNewPlayer(playerPorts.back());
				gameState->UpdatePlayer(playerPorts.back(), playerPositions.back(), playerRotations.back());
			}
			playerPorts.pop_back();
			playerPositions.pop_back();
			playerRotations.pop_back();
		}

		packet.clear();
	}
}

