/*===============================================================


		UIëSëÃÇÃä«óù [manager_ui.h]

										Author : shusuke katori
										Date   : 2024/12/29

==================================================================*/

#ifndef MANAGER_UI_H

#define MANAGER_UI_H

class Ship;
class GaugeActionUI;
class EnhanceUI;
class GatherUI;
class MiniMap;
class Resource;
class StorageUI;

//åªç›ÇÃèÛë‘ä«óù
typedef enum
{

	UI_STATE_NONE = -1,
	UI_STATE_NORMAL,
	UI_STATE_GAUGE_ACTION,
	UI_STATE_GATHER_ACTION,
	UI_STATE_ENHANCE,
	UI_STATE_MAP,
	UI_STATE_MAX,

}UIState;


class ManagerUI
{

private:

	GaugeActionUI* m_GaugeActionUI = nullptr;
	EnhanceUI* m_EnhanceUI = nullptr;
	UIState m_UIState;
	MiniMap* m_MiniMap = nullptr;
	Resource* m_Resource = nullptr;
	GatherUI* m_GatherUI = nullptr;
	StorageUI* m_StorageUI = nullptr;
public:

	ManagerUI(Ship* p_ship);
	~ManagerUI();

	void Update(void);
	void Draw(void)const;


	void SetUIState(UIState state);

	GaugeActionUI* GetGaugeActionUI();
	EnhanceUI* GetEnhanceUI();
	MiniMap* GetMiniMap();
	Resource* GetResource();
	GatherUI* GetGatherUI();
	StorageUI* GetStorageUI();
};


#endif //MANAGER_UI_H
