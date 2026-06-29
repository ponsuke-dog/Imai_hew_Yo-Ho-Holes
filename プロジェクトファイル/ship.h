//	ship.h
//	
//
//	2024.12.17			大山凌太朗
#ifndef SHIP_H
#define SHIP_H

#include "directx.h"
#include <list>
#include <vector>

class Object3D;
class Player;
class DeckBlock;
class Cannon;
class Gather;
class Storage;
class Enhance;
class Flotsam;
class CannonBall;
class Explosion;
class Net;

class GameManager;

class FlotsamManager;

class Controller;

class ManagerUI;

class Ship
{
private:

	int m_TeamId;
	XMFLOAT3 m_CenterPosition;
	GameManager* m_pGameManager = nullptr;
	FlotsamManager* m_pFlotsamManager = nullptr;
	Controller* m_pController = nullptr;
	Enhance* m_Enhance = nullptr;
	
	std::vector<Player*>	m_Players;
	std::vector<DeckBlock*> m_DeckBlocks;
	std::vector<Cannon*>	m_Cannons;
	std::vector<Gather*>	m_Gathers;
	std::vector<Storage*>	m_Storages;
	std::list<Flotsam*>		m_Flotsams;
	std::list<CannonBall*>	m_CannonBalls;
	std::list<Explosion*>	m_Explosions;
	std::list<Net*>			m_Nets;
	

	Ship* m_EnemyShip = nullptr;
	
	// 船の総HP
	int m_ShipHP = 0;

public:
	Ship(int teamid,XMFLOAT3 centerpositiion, Controller* controller);
	~Ship();

	void Update();
	void Draw();

	// コンテナへの登録
	void RegisterPlayers(Player* player);
	void RegisterDeckBlocks(DeckBlock* deckblock);
	void RegisterCannons(Cannon* cannon);
	void RegisterGathers(Gather* gather);
	void RegisterStorages(Storage* storage);
	void RegisterEnhance(Enhance* enhance);
	void RegisterFlotsams(Flotsam* flotsam);
	void RegisterCannonBalls(CannonBall* cannonball);
	void RegisterExplosions(Explosion* explosion);
	void RegisterNets(Net* net);
	


	std::vector<Player*>& GetPlayers();
	std::vector<DeckBlock*>& GetDeckBlocks();
	std::vector<Cannon*>& GetCannons();
	std::vector<Gather*>& GetGathers();
	std::vector<Storage*>& GetStorages();
	std::list<Flotsam*>& GetFlotsams();
	std::list<CannonBall*>& GetCannonBall();
	std::list<Explosion*>& GetExplosions();
	std::list<Net*>& GetNets();


	Enhance* GetEnhance();

	int	GetTeamId();
	XMFLOAT3 GetCenterPosition();

	int GetShipHP();
	void SetShipHp(float hp);

	XMFLOAT3 GetPlayerPos(int teamID);

	void SetEnemyShip(Ship* pShip);
	Ship* GetEnemyShip(void);

	//2025/02/19 ・・・カメラのスタンドバイ中の漂流物の利用の仮追加
	void FlotsamUpdate();
	void StandByDraw();

	void Reset();
private:
	void update();
	void draw();
	void checkdestroy();
	void updateshipHP();

};



#endif // !SHIP_H
