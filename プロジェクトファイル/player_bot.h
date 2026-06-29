//	player_bot.h
//	
//
//	2025.02.14			‘åŽR—½‘¾˜N

#ifndef PLAYER_BOT_H
#define PLAYER_BOT_H

#include "player.h"

class Client;
class Ship;

class BotPlayer :public Player
{
private:
	Client* m_Client = nullptr;
public:
	BotPlayer(Ship* ship,Client* client, int id);
	~BotPlayer();

	void Update()override;
	void Draw()const override;

	
};


#endif // PLAYER_BOT_H
