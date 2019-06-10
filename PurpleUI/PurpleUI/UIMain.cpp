#include "UIMain.h"
UIMain *UIMain::m_main = nullptr; 

void UIMain::SetUIMain(UIMain *main)
{
	m_main = main;
}

void UIMain::Run()
{
	m_main->_Run();
}

void UIMain::_Run()
{
	m_main->OnRegisterSurface();
	m_surfaceManager->Begin();
	std::thread(EventPump::UpdateEventThread).detach();//开个线程处理事件
	Time::SetDeltaTime(1.0f / m_surfaceManager->GetActiveFPS());
	m_updateTimer.SetUpadateTime(1.0f / m_surfaceManager->GetActiveFPS());
	m_updateTimer.Run();
	while (!m_surfaceManager->Empty())
	{
		m_updateTimer.Update();
		if (EventPump::HasInput())
			m_main->m_surfaceManager->GetActiveSurface()->OnEvent(EventPump::GetInput());
		m_updateTimer.SetUpadateTime(1.0f / m_surfaceManager->GetActiveFPS());
	}
	EventPump::StopPumbThread();
	m_surfaceManager->End();
}

UIMain::UIMain()
{
	m_surfaceManager = SurfaceManager::GetInstance();
	m_updateTimer.AddEvent([&]()
	{
		Time::SetDeltaEndTime();
		Time::SetDeltaBeginTime();
		m_surfaceManager->OnUpdata();
	});
}

void UIMain::RegisterSurface(const std::string & name, UISurface * surface)
{
	m_surfaceManager->RegisterSurface(name, surface);
}

void UIMain::LaunchSurface(const std::string & name)
{
	m_surfaceManager->StartSurface(name);
}