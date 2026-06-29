//	deck.h
//	
//
//	2024.11.19			ポンスケ
#ifndef DECK_H
#define DECK_H

#include "deckblock.h"

#define TEAM_NUM	(2)
#define MAP_WIDTH_NUM (15)
#define MAP_HEIGHT_NUM (27)
#define MAP_MAX_NUM (MAP_WIDTH_NUM * MAP_HEIGHT_NUM)
#define MAP_BLOCK_SIZE (1.0f)

class GameManager;
class Ship;

class Deck
{
private:	

public:
	Deck() = delete;
	Deck(Ship* ship);
	~Deck();

	void SetMap(Ship* ship);		// マップのセット	
};


#endif // !DECK_H