#ifndef UIMAIN_H
#define UIMAIN_H

#include <cstdio>
#include <thread>
#include <conio.h>
#include "TimeSystem/Time.h"
#include "TimeSystem/Timer.h"
#include "SurfaceSystem/SurfaceManager.h"
#include "EventSystem/EventPump.h"

class UIMain
{
public:
	
	static void SetUIMain(UIMain *main);

	static void Run();

protected:

	UIMain();

	void _Run();

	virtual void OnRegisterSurface() = 0;

	/*注册界面*/
	void RegisterSurface(const std::string &name, UISurface *surface);

	/*启动界面*/
	void LaunchSurface(const std::string &name);

private:

	static UIMain *m_main;

	SurfaceManager *m_surfaceManager;
	Timer m_updateTimer;

};
#endif