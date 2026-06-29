#include "object_3d.h"
#include "player.h"
#include "deckblock.h"
#include "deck.h"
#include "cannon.h"
#include "gather.h"
#include "storage.h"
#include "enhance.h"
#include "flotsam.h"
#include "cannonball.h"
#include "explosion.h"
#include "net.h"
#include "controller.h"
#include "effect_3d.h"

#include "manager_field.h"
#include "manager_flotsam.h"
#include "manager_model.h"
#include "ui_enhance.h"
#include "ui_minimap.h"
#include "ui_resource.h"
#include "ship.h"


Ship::Ship(int teamid, XMFLOAT3 centerpositiion, Controller* controller) :m_TeamId(teamid), m_CenterPosition(centerpositiion)
{
	m_Players.reserve(2);
	m_DeckBlocks.reserve(MAP_MAX_NUM);
	m_Cannons.reserve(3);
	m_Gathers.reserve(3);
	m_Storages.reserve(2);


	m_pController = controller;

	FieldManager field(this);
	/*if (teamid == 0) {
		RegisterPlayers(new Player(this, XMFLOAT3(m_CenterPosition.x, m_CenterPosition.y + 0.5f, m_CenterPosition.z), uimanager, m_pController));
	}*/
	/*if (teamid == 1) {
		RegisterPlayers(new Player(this, XMFLOAT3(m_CenterPosition.x, m_CenterPosition.y + 1.0f, m_CenterPosition.z),uimanager, m_pController));
	}*/

	
	updateshipHP();

	m_pFlotsamManager = new FlotsamManager(this);
}

Ship::~Ship()
{
	for (int i = 0; i < m_Players.size(); i++) {
		if (m_Players[i]) {
			delete m_Players[i];
			m_Players[i] = nullptr;
		}
	}

	for (int i = 0; i < m_DeckBlocks.size(); i++) {
		if (m_DeckBlocks[i]) {
			delete m_DeckBlocks[i];
			m_DeckBlocks[i] = nullptr;
		}
	}

	for (int i = 0; i < m_Cannons.size(); i++) {
		if (m_Cannons[i]) {
			delete m_Cannons[i];
			m_Cannons[i] = nullptr;
		}
	}

	for (int i = 0; i < m_Gathers.size(); i++) {
		if (m_Gathers[i]) {
			delete m_Gathers[i];
			m_Gathers[i] = nullptr;
		}
	}

	for (int i = 0; i < m_Storages.size(); i++) {
		if (m_Storages[i]) {
			delete m_Storages[i];
			m_Storages[i] = nullptr;
		}
	}

	if (m_Enhance) {
		delete m_Enhance;
		m_Enhance = nullptr;
	}

	for (auto flotsams : m_Flotsams ) {
		if (flotsams) {
			delete flotsams;
		}
	}
	
	for (auto cannonballs : m_CannonBalls) {
		if (cannonballs) {
			delete cannonballs;
		}
	}

	for (auto explosions : m_Explosions) {
		if (explosions) {
			delete explosions;
		}
	}
	
	for (auto nets : m_Nets) {
		if (nets) {
			delete nets;
		}
	}
	delete m_pFlotsamManager;
	m_pFlotsamManager = nullptr;
	
}

void Ship::Update()
{
	update();

	m_pFlotsamManager->Update();
	updateshipHP();

	checkdestroy();
}

void Ship::Draw()
{
	ModelDraw(MODELNAME_SHIP, m_CenterPosition);
	draw();


}

void Ship::RegisterPlayers(Player* player)
{
	m_Players.push_back(player);
}

void Ship::RegisterDeckBlocks(DeckBlock* deckblock)
{
	m_DeckBlocks.push_back(deckblock);
}

void Ship::RegisterCannons(Cannon* cannon)
{
	m_Cannons.push_back(cannon);
}

void Ship::RegisterGathers(Gather* gather)
{
	m_Gathers.push_back(gather);
}

void Ship::RegisterStorages(Storage* storage)
{
	m_Storages.push_back(storage);
}

void Ship::RegisterEnhance(Enhance* enhance)
{
	m_Enhance = enhance;
}

void Ship::RegisterFlotsams(Flotsam* flotsam)
{
	m_Flotsams.push_back(flotsam);
}

void Ship::RegisterCannonBalls(CannonBall* cannonball)
{
	m_CannonBalls.push_back(cannonball);
}

void Ship::RegisterExplosions(Explosion* explosion)
{
	m_Explosions.push_back(explosion);
}

void Ship::RegisterNets(Net* net)
{
	m_Nets.push_back(net);
}




std::vector<Player*>& Ship::GetPlayers()
{
	return m_Players;
}

std::vector<DeckBlock*>& Ship::GetDeckBlocks()
{
	return m_DeckBlocks;
}

std::vector<Cannon*>& Ship::GetCannons()
{
	return m_Cannons;
}

std::vector<Gather*>& Ship::GetGathers()
{
	return m_Gathers;
}

std::vector<Storage*>& Ship::GetStorages()
{
	return m_Storages;
}

Enhance*Ship::GetEnhance()
{
	return m_Enhance;
}

std::list<Flotsam*>& Ship::GetFlotsams()
{
	return m_Flotsams;
}

std::list<CannonBall*>& Ship::GetCannonBall()
{
	return m_CannonBalls;
}

std::list<Explosion*>& Ship::GetExplosions()
{
	return m_Explosions;
}

std::list<Net*>& Ship::GetNets()
{
	return m_Nets;
}

int Ship::GetTeamId()
{
	return m_TeamId;
}

XMFLOAT3 Ship::GetCenterPosition()
{
	return m_CenterPosition;
}

int Ship::GetShipHP()
{
	return m_ShipHP;
}

void Ship::SetShipHp(float hp)
{
	m_ShipHP = hp;
}

XMFLOAT3 Ship::GetPlayerPos(int teamID)
{
	return m_Players[teamID]->GetPosition();
}

void Ship::SetEnemyShip(Ship* pShip)
{
	m_EnemyShip = pShip;
}


Ship* Ship::GetEnemyShip(void)
{
	return m_EnemyShip;
}

void Ship::FlotsamUpdate()
{
	for (auto itr = m_Flotsams.begin(); itr != m_Flotsams.end(); ++itr) {
		if (*itr) {
			(*itr)->Update();
		}
	}

	m_pFlotsamManager->Update();
}

void Ship::StandByDraw()
{
	ModelDraw(MODELNAME_SHIP, m_CenterPosition);

	for (int i = 0; i < m_Players.size(); i++) {
		if (m_Players[i]) {
			m_Players[i]->Draw();
		}
	}

	for (int i = 0; i < m_DeckBlocks.size(); i++) {
		if (m_DeckBlocks[i]) {
			m_DeckBlocks[i]->Draw();
		}
	}

	for (int i = 0; i < m_Cannons.size(); i++) {
		if (m_Cannons[i]) {
			m_Cannons[i]->Draw();
		}
	}

	for (int i = 0; i < m_Gathers.size(); i++) {
		if (m_Gathers[i]) {
			m_Gathers[i]->Draw();
		}
	}

	for (int i = 0; i < m_Storages.size(); i++) {
		if (m_Storages[i]) {
			m_Storages[i]->Draw();
		}
	}

	if (m_Enhance) {

		m_Enhance->Draw();
	}
}

void Ship::Reset()
{
	for (auto pCannon:m_Cannons)
	{
		pCannon->Reset();
	}
	for (auto pGather:m_Gathers)
	{
		pGather->Reset();
	}
	m_Enhance->Reset();
}

void Ship::update()
{
	for (int i = 0; i < m_Players.size(); i++) {
		if (m_Players[i]) {
			m_Players[i]->Update();
		}
	}

	for (int i = 0; i < m_DeckBlocks.size(); i++) {
		if (m_DeckBlocks[i]) {
			m_DeckBlocks[i]->Update();
		}
	}

	for (int i = 0; i < m_Cannons.size(); i++) {
		if (m_Cannons[i]) {
			m_Cannons[i]->Update();
		}
	}

	for (int i = 0; i < m_Gathers.size(); i++) {
		if (m_Gathers[i]) {
			m_Gathers[i]->Update();
		}
	}

	for (int i = 0; i < m_Storages.size(); i++) {
		if (m_Storages[i]) {
			m_Storages[i]->Update();
		}
	}

	if (m_Enhance) {

		m_Enhance->Update();
	}

	for (auto itr = m_Flotsams.begin(); itr != m_Flotsams.end(); ++itr) {
		if (*itr) {
			(*itr)->Update();
		}
	}

	for (auto itr = m_CannonBalls.begin(); itr != m_CannonBalls.end(); ++itr) {
		if (*itr) {
			(*itr)->Update();
		}
	}

	for (auto itr = m_Explosions.begin(); itr != m_Explosions.end(); ++itr) {
		if (*itr) {
			(*itr)->Update();
		}
	}

	for (auto itr = m_Nets.begin(); itr != m_Nets.end(); ++itr) {
		if (*itr) {
			(*itr)->Update();
		}
	}
}

void Ship::draw()
{
	for (int i = 0; i < m_Players.size(); i++) {
		if (m_Players[i]) {
			m_Players[i]->Draw();
		}
	}

	for (int i = 0; i < m_DeckBlocks.size(); i++) {
		if (m_DeckBlocks[i]) {
			m_DeckBlocks[i]->Draw();
		}
	}

	for (int i = 0; i < m_Cannons.size(); i++) {
		if (m_Cannons[i]) {
			m_Cannons[i]->Draw();
		}
	}

	for (int i = 0; i < m_Gathers.size(); i++) {
		if (m_Gathers[i]) {
			m_Gathers[i]->Draw();
		}
	}

	for (int i = 0; i < m_Storages.size(); i++) {
		if (m_Storages[i]) {
			m_Storages[i]->Draw();
		}
	}

	if (m_Enhance) {

		m_Enhance->Draw();
	}

	for (auto itr = m_Flotsams.begin(); itr != m_Flotsams.end(); ++itr) {
		if (*itr) {
			(*itr)->Draw();
		}
	}

	for (auto itr = m_CannonBalls.begin(); itr != m_CannonBalls.end(); ++itr) {
		if (*itr) {
			(*itr)->Draw();
		}
	}

	for (auto itr = m_Explosions.begin(); itr != m_Explosions.end(); ++itr) {
		if (*itr) {
			(*itr)->Draw();
		}
	}

	for (auto itr = m_Nets.begin(); itr != m_Nets.end(); ++itr) {
		if (*itr) {
			(*itr)->Draw();
		}
	}
}

void Ship::checkdestroy()
{

	

	for (auto itr = m_Flotsams.begin(); itr != m_Flotsams.end(); ) {
		if ((*itr)->GetDestroy()) {
			delete* itr;
			itr = m_Flotsams.erase(itr);
		}
		else {
			++itr;
		}
	}

	for (auto itr = m_CannonBalls.begin(); itr != m_CannonBalls.end();) {
		if ((*itr)->GetDestroy()) {
			delete* itr;
			itr = m_CannonBalls.erase(itr);
		}
		else {
			++itr;
		}
	}

	for (auto itr = m_Explosions.begin(); itr != m_Explosions.end();) {
		if ((*itr)->GetDestroy()) {
			delete* itr;
			itr = m_Explosions.erase(itr);
		}
		else {
			++itr;
		}
	}

	for (auto itr = m_Nets.begin(); itr != m_Nets.end();) {
		if ((*itr)->GetDestroy()) {
			delete* itr;
			itr = m_Nets.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void Ship::updateshipHP()
{
	int hp = 0;
	for (auto deckblocks : m_DeckBlocks) {
		if (deckblocks->GetDeckType() == DECKTYPE_WOOD && deckblocks->GetUse()) {
			hp += (int)deckblocks->GetHitPoint();
		}
	}
	m_ShipHP = hp;
}

