/*==============================================================================

   [.h]
														 Author : Imai Hayato
														 Date   : //
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef STORAGE_UI_H
#define STORAGE_UI_H

class Ship;

class StorageUI
{
private:
	Ship* m_pShip = nullptr;
	int m_NumTex = -1;
	int m_NumMultiplication = -1;
	int m_StorageTex[2] = {};
	XMFLOAT2 m_Postion{ 0.0f,0.0f };
	XMFLOAT2 m_Size = { 0.0f,0.0f };
	XMINT2 m_TextureSize = { 0,0 };
	int m_StorageNum[2] = {};
	int m_EnemyStorageNum[2] = {};
	int m_SizeRaito = 0;
public:
	StorageUI(Ship* pShip);
	~StorageUI();

	void Update();
	void Draw();
};


#endif // !STORAGE_UI_H