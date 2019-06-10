#include "UISurface.h"
UISurface::~UISurface()
{
	ClearUIComponents();
}
void UISurface::OnEnter()
{
	m_view.ResetSrcView();
	for (auto &msg : m_uiComs)
	{
		msg.ui->OnReset();
		if (msg.ui->IsEnable())
			msg.ui->OnDraw();
	}
}
void UISurface::OnEnter(void * data)
{
	OnEnter();
}
void UISurface::OnExit()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnPause();
}
void UISurface::OnBack()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnResume();
}
void UISurface::OnUpdate()
{
	system("cls");
	for (auto &msg : m_uiComs)
		if (msg.ui->IsEnable())
			msg.ui->OnUpdate();
	puts(m_view.GetViewData());
}
void UISurface::OnDestroy()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnDestroy();
	ClearUIComponents();
}
void UISurface::OnEvent(char input)
{
	if (m_focusUi != nullptr)
		if (!m_focusUi->OnEvent(input))
			return;
	for (auto &msg : m_uiComs)
		if (msg.ui != m_focusUi && msg.ui->IsEnable())
			msg.ui->OnEvent(input);
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
void UISurface::SetFocusUI(UIComponent *focusUi)
{
	if (m_focusUi != nullptr)
	{
		m_focusUi->m_isFocus = false;
		m_focusUi->OnLoseFocus();
	}
	m_focusUi = focusUi;
	m_focusUi->m_isFocus = true;
	focusUi->OnGetFocus();
}
UIComponent *UISurface::GetFocusUI() const
{
	return m_focusUi;
}
void UISurface::Draw()
{
	m_view.ResetSrcView();
	for (auto &msg : m_uiComs)
		if (msg.ui->IsEnable())
			msg.ui->OnDraw();
}
void UISurface::Draw(UIComponent *withoutUI)
{
	for (auto &msg : m_uiComs)
		if (msg.ui != withoutUI && msg.ui->IsEnable())
			msg.ui->OnDraw();
}
unsigned char UISurface::GetFps() const
{
	return m_fps;
}

void UISurface::OnCreatedUI()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnCreate(m_view);	
}
