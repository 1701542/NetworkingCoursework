#include "NetworkManager.h"



NetworkManager::NetworkManager(GameState* game)
{
	gameState = game;
	udpSocket.setBlocking(false);

	udpStatus = udpSocket.bind(UDPPORT, sf::IpAddress::getLocalAddress());

	if (udpStatus != sf::Socket::Done)
		std::cout << "UDP socket failed to bind. Error: " << udpStatus << std::endl;
	else
		std::cout << "UDP socket successfully binded. " << sf::IpAddress::getLocalAddress() << "\t" << udpSocket.getLocalPort() << std::endl;
	

}


NetworkManager::~NetworkManager()
{
}

void NetworkManager::Update(float deltaTime)
{
	ReceiveMessageUDP();

	Prediction(deltaTime);

	SyncPlayers(deltaTime);

	DisconnectPlayers();
}

void NetworkManager::ReceiveMessageUDP()
{
	udpStatus = udpSocket.receive(packet, receiveAddress, receivePort);

	if (udpStatus == sf::Socket::Done)
	{
		message newMessage;
		newMessage.gameTime = gameState->getGameTime();
		newMessage.predicted = false;

		// Unpack the packet into the message structure
		packet >> newMessage.position.x >> newMessage.position.y >> newMessage.rotation;
		packet.clear();

		if (clientVector.empty())
		{
			// Initialise client details
			client newClient;
			newClient.timeSinceLastRealMessage = 0.0f;
			newClient.address = receiveAddress;
			newClient.port = receivePort;		
			newClient.receivedPackets.push_back(newMessage);

			// Add client to the list of clients
			clientVector.push_back(newClient);

			// Create an instance of player for the client and update with current details.
			gameState->AddNewPlayer(clientVector.back().port);
			gameState->UpdatePlayer(clientVector.back().port,
				clientVector.back().receivedPackets.back().position,
				clientVector.back().receivedPackets.back().rotation);

			std::cout << "Added a new player.\n";
		}
		else
		{
			// Iterate to find out if we know this player
			for (int i = 0; i < clientVector.size(); i++)
			{
				if (clientVector[i].port == receivePort)
				{
					clientVector[i].timeSinceLastRealMessage = 0.0f;

					// Keep the vector size at a maximum of 3 elements
					if (clientVector[i].receivedPackets.size() > 2)
						clientVector[i].receivedPackets.erase(clientVector[i].receivedPackets.begin());
					
					// Add the message to the end of the vector
					clientVector[i].receivedPackets.push_back(newMessage);

					// Update the player associated with this client
					gameState->UpdatePlayer(clientVector[i].port,
						clientVector[i].receivedPackets.back().position,
						clientVector[i].receivedPackets.back().rotation);

					// Exit the function because we found the existing client and handled the packet.
					return;
				}
			}

			// Add the client to the server because we don't recognise him
			client newClient;
			newClient.timeSinceLastRealMessage = 0.0f;
			newClient.address = receiveAddress;
			newClient.port = receivePort;		// Initialise client details
			newClient.receivedPackets.push_back(newMessage);

			// Add client to the list of clients
			clientVector.push_back(newClient);

			// Create an instance of player for the client and update with current details.
			gameState->AddNewPlayer(clientVector.back().port);
			gameState->UpdatePlayer(clientVector.back().port,
				clientVector.back().receivedPackets.back().position,
				clientVector.back().receivedPackets.back().rotation);

			std::cout << "Added a new player.\n";
		}

	}
}

void NetworkManager::DisconnectPlayers()
{
	// Loop throught the list of clients and delete disocnnected ones.
	if (!clientVector.empty())
	{
		for (int i = 0; i < clientVector.size(); i++)
		{

			// If the difference between the local game time
			// and the client's last message timestamp is greater
			// than 5 seconds "disconnect" the client.
			if (clientVector[i].timeSinceLastRealMessage > 5.0f)
			{
				// Delete the player to prevent drawing.
				gameState->DeletePlayer(clientVector[i].port);

				// Erase the instance of client
				clientVector.erase(clientVector.begin() + i);

				std::cout << "Removed a player.\n";
			}
		}
	}
}

void NetworkManager::SyncPlayers(float deltaTime)
{
	timer += deltaTime;

	if (timer >= interval)	// Syncing at set intervals
	{
		
		// Do not sync if there are no clients connected
		if (!clientVector.empty())
		{
			// Packets firstly appends the number of clients and the master game time.
			packet << clientVector.size() << gameState->getGameTime();

			// Loop through the clients and pack the packet with their info
			for (int i = 0; i < clientVector.size(); i++)
			{
				packet << clientVector[i].port
					<< clientVector[i].receivedPackets.back().position.x
					<< clientVector[i].receivedPackets.back().position.y
					<< clientVector[i].receivedPackets.back().rotation;
			}

			// Send the same packet to all clients
			for (int i = 0; i < clientVector.size(); i++)
			{
				udpStatus = udpSocket.send(packet, clientVector[i].address, clientVector[i].port);

				if (udpStatus != sf::Socket::Done)
					std::cout << "Failed to sync with client " << clientVector[i].port << "with Error: " << udpStatus;
			}
		}

		// Reset the timer and clear the packet
		timer = 0.0f;
		packet.clear();
	}
}

// Predict position and push it to the player's message list if they have not reacted in 25ms or more
void NetworkManager::Prediction(float deltaTime)
{
	if (!clientVector.empty())
	{
		for (int i = 0; i < clientVector.size(); i++)
		{
			// If there has not been a message within the last 25ms and there have been at least 2 packets received then predict the position 
			if (clientVector[i].timeSinceLastRealMessage > 0.025f &&
				clientVector[i].receivedPackets.size() > 1)
			{
				message newMessage;
				sf::Vector2f predictedPosition;

				sf::Vector2f distance;
				float time;
				sf::Vector2f speed;

				// Calculate the last distance travelled by the player.
				distance.x = clientVector[i].receivedPackets.back().position.x - clientVector[i].receivedPackets[clientVector[i].receivedPackets.size() - 2].position.x;
				distance.y = clientVector[i].receivedPackets.back().position.y - clientVector[i].receivedPackets[clientVector[i].receivedPackets.size() - 2].position.y;

				// Caclulate the time between the previous to positions
				time = clientVector[i].receivedPackets.back().gameTime - clientVector[i].receivedPackets[clientVector[i].receivedPackets.size() - 2].gameTime;

				// Calculate speed
				speed.x = distance.x / time;
				speed.y = distance.y / time;

				// Calculate the time since the previous message
				time = gameState->getGameTime() - clientVector[i].receivedPackets.back().gameTime;

				// Calculate the predicted position by adding the distance to the previous position
				predictedPosition.x = clientVector[i].receivedPackets.back().position.x + (speed.x * time);
				predictedPosition.y = clientVector[i].receivedPackets.back().position.y + (speed.y * time);
				
				// Add new position and timestamp but don't change the previous rotation
				newMessage.gameTime = gameState->getGameTime();
				newMessage.position = predictedPosition;
				newMessage.predicted = true;

				if (clientVector[i].receivedPackets.size() > 2)
					clientVector[i].receivedPackets.erase(clientVector[i].receivedPackets.begin());

				// Push the message
				clientVector[i].receivedPackets.push_back(newMessage);

				// Update the player
				gameState->UpdatePlayer(clientVector[i].port,
					clientVector[i].receivedPackets.back().position,
					clientVector[i].receivedPackets.back().rotation);

				std::cout << clientVector[i].timeSinceLastRealMessage << "\tPredicted position of " << clientVector[i].port << std::endl;
			}
			// Keep track of how long it has been since a real message has been received
			clientVector[i].timeSinceLastRealMessage += deltaTime;
		}
	}
}