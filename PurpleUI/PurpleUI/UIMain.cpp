#include "UIMain.h"
UIMain *UIMain::s_main = nullptr; 
SurfaceManager *UIMain::m_surfaceManager = nullptr;
void UIMain::SetUIMain(UIMain *main)
{
	s_main = main;
}
void UIMain::Run()
{
	s_main->OnRegisterSurface();
	m_surfaceManager->_Begin();
	std::thread(EventPump::UpdateEventThread).detach();//开个线程处理事件
	while (s_main->m_surfaceManager->_OnUpdata())
	{
		if (EventPump::HasInput())
			s_main->m_surfaceManager->GetActiveSurface()->OnEvent(EventPump::GetInput());
	}
	EventPump::StopPumbThread();
	m_surfaceManager->_End();
}

UIMain::UIMain()
{
	m_surfaceManager = SurfaceManager::GetInstance();
}

void UIMain::RegisterSurface(const std::string & name, UISurface * surface)
{
	m_surfaceManager->RegisterSurface(name, surface);
}

void UIMain::LaunchSurface(const std::string & name)
{
	m_surfaceManager->StartSurface(name);
}
