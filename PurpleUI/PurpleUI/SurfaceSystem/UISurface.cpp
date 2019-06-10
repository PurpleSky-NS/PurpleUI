#include "UISurface.h"
UISurface::~UISurface()
{
	ClearUIComponents();
}
void UISurface::AddUIComponent(UIComponent *newUI, bool isMemoryDeposit)
{
	UIComponentMsg msg = UIComponentMsg(newUI, isMemoryDeposit);
	newUI->m_context = this;
	m_uiComs.push_back(msg);
}
void UISurface::ClearUIComponents()
{
	for (auto &uiCom : m_uiComs)
		if (uiCom.isMemoryDeposit)
			delete uiCom.ui;
	m_uiComs.clear();
}

void UISurface::OnCreatedUI()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnCreate(m_view);	
}
