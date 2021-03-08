#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <list>
#include "GameState.h"

#define SERVERIP "127.0.0.1"

#define UDPPORT 55000

class NetworkManager
{
public:
	NetworkManager(GameState* game);
	~NetworkManager();

	void Update(float deltaTime);

private:
	void ReceiveMessageUDP();
	void SyncPlayers(float deltaTime);
	void DisconnectPlayers();
	void Prediction(float deltaTime);

	GameState* gameState;

	struct message
	{
		float gameTime;
		sf::Vector2f position;
		float rotation;
		bool predicted;
	};
	struct client
	{
		float timeSinceLastRealMessage = 0.0f;
		sf::IpAddress address;
		unsigned short port;
		std::vector<message> receivedPackets;
	};

	std::vector<client> clientVector;


	sf::UdpSocket udpSocket;
	
	sf::Socket::Status udpStatus;
	sf::Packet packet;
	sf::IpAddress receiveAddress;
	unsigned short receivePort;

	float timer = 0.0f;
	float interval = 0.05f;
};

