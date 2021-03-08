#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include "GameState.h"

#define SERVERUDPPORT 55000

class NetworkManager
{
public:
	NetworkManager(GameState* game);
	~NetworkManager();

	void Update(float deltaTime);

private:
	void SendMessage(float deltaTime);
	void ReceiveMessageUDP();

	GameState* gameState;

	sf::UdpSocket udpSocket;
	sf::Socket::Status udpStatus;
	sf::IpAddress receiveAddress;
	unsigned short receivePort;

	sf::Packet packet;
	float timer = 0.0f;
	float interval = 0.01f;
};

