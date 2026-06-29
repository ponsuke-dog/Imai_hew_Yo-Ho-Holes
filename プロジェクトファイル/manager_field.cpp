#include "deckblock.h"
#include "deck.h"
#include "gather.h"
#include "ship.h"
#include "storage.h"
#include "cannon.h"
#include "enhance.h"
#include "manager_field.h"

#include <vector>

FieldManager::FieldManager(Ship* ship)
{
	//b”Â‚Ì¶¬
	Deck deck(ship);

	auto deckblocks = ship->GetDeckBlocks();

	int cannon_count = 0;
	int gather_count = 0;

	for (int i = 0; i < deckblocks.size(); i++) {
		if (deckblocks[i]->GetDeckType() == DECKTYPE_GATHER) {
			ship->RegisterGathers(new Gather(deckblocks[i]->GetPosition(), ship, gather_count));
			gather_count++;
		}
		if (deckblocks[i]->GetDeckType() == DECKTYPE_REPAIR_STORAGE) {
			ship->RegisterStorages(new Storage(STORAGETYPE_REPAIR,deckblocks[i]->GetPosition(), ship));
		}
		if (deckblocks[i]->GetDeckType() == DECKTYPE_CANNON_STORAGE) {
			ship->RegisterStorages(new Storage(STORAGETYPE_CANNON, deckblocks[i]->GetPosition(), ship));
		}
		if (deckblocks[i]->GetDeckType() == DECKTYPE_CANNON) {

			ship->RegisterCannons(new Cannon(deckblocks[i]->GetPosition(), ship, cannon_count));

			cannon_count++;
		}
		if (deckblocks[i]->GetDeckType() == DECKTYPE_ENHANCE)
		{
			ship->RegisterEnhance(new Enhance(deckblocks[i]->GetPosition(), ship));
		}

	}

	
}

FieldManager::~FieldManager()
{
	
}