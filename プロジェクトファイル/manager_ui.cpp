/*===============================================================


		UI‘S‘Ì‚ÌŠÇ— [manager_ui.cpp]

										Author : shusuke katori
										Date   : 2024/12/29

==================================================================*/

#include "ship.h"
#include "ui_gauge_action.h"
#include "ui_enhance.h"
#include "ui_minimap.h"
#include "ui_resource.h"
#include "ui_gather.h"
#include "ui_storage.h"

#include "manager_ui.h"


ManagerUI::ManagerUI(Ship* p_ship)
{

	m_GaugeActionUI = new GaugeActionUI;
	m_EnhanceUI = new EnhanceUI;
	m_MiniMap = new MiniMap;
	m_Resource = new Resource;
	m_GatherUI = new GatherUI;
	m_StorageUI = new StorageUI(p_ship);
	m_UIState = UI_STATE_NORMAL;

	if (p_ship)
	{
		GetEnhanceUI()->SetEnhance(p_ship->GetEnhance());
		/*GetMiniMap()->SetMiniMap(p_ship);*/
	}
}

ManagerUI::~ManagerUI()
{
	delete m_GaugeActionUI;
	m_GaugeActionUI = nullptr;

	delete m_EnhanceUI;
	m_EnhanceUI = nullptr;

	delete m_MiniMap;
	m_MiniMap = nullptr;

	delete m_Resource;
	m_Resource = nullptr;

	delete m_GatherUI;
	m_GatherUI = nullptr;

	delete m_StorageUI;
	m_StorageUI = nullptr;
}

void ManagerUI::Update(void)
{

	switch (m_UIState)
	{

	case UI_STATE_NORMAL:
		m_MiniMap->Update();
		m_Resource->Update();
		m_StorageUI->Update();
		break;

	case UI_STATE_GAUGE_ACTION:

		m_GaugeActionUI->Update();
		break;
	case UI_STATE_GATHER_ACTION:
		m_GatherUI->Update();
		break;
	case UI_STATE_ENHANCE:
		m_EnhanceUI->Update();
		break;

	case UI_STATE_MAP:
		break;
	default:
		break;
	}
	
}

void ManagerUI::Draw(void)const
{
	SetDepthEnable(false);
	switch (m_UIState)
	{

	case UI_STATE_NORMAL:
		m_MiniMap->Draw();
		m_Resource->Draw();
		m_StorageUI->Draw();
		break;

	case UI_STATE_GAUGE_ACTION:

		m_GaugeActionUI->Draw();
		break;

	case UI_STATE_GATHER_ACTION:
		m_GatherUI->Draw();
		m_StorageUI->Draw();
		break;

	case UI_STATE_ENHANCE:
		m_EnhanceUI->Draw();
		break;

	case UI_STATE_MAP:
		break;
	default:
		break;
	}
	SetDepthEnable(true);

}

void ManagerUI::SetUIState(UIState state)
{
	m_UIState = state;
}

GaugeActionUI* ManagerUI::GetGaugeActionUI()
{
	return m_GaugeActionUI;
}

EnhanceUI* ManagerUI::GetEnhanceUI()
{
	return m_EnhanceUI;
}

MiniMap* ManagerUI::GetMiniMap()
{
	return m_MiniMap;
}

Resource* ManagerUI::GetResource()
{
	return m_Resource;
}

GatherUI* ManagerUI::GetGatherUI()
{
	return m_GatherUI;
}

StorageUI* ManagerUI::GetStorageUI()
{
	return m_StorageUI;
}
