#ifndef MAINSURFACE_H
#define MAINSURFACE_H

#include "TestMenu.h"
#include "CustomUI/TipsUI.h"

class MainSurface : public UISurface
{
public:
	class MainTips :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &view)override
		{
			TipsUI::OnCreate(view);
			CreateTips("这是个测试提示",
				{
					"众所周知，这仅仅只是个测试",
					"不骗你"
				});
		}
	};

	MainSurface() :
		UISurface(20, 120, 28)
	{}

	virtual void OnCreateUI()override
	{
		AddUIComponent(new MainTips());
		AddUIComponent(new TestMenu());
	}

	virtual void OnCreate()override
	{
		SetFocusUI(GetUIComponent<TestMenu>());
	}

	virtual void OnEnter()override
	{
		UISurface::OnEnter();
		GetUIComponent<MainTips>()->Show(TipsUI::SHORT_LENGTH);
	}
};
#endif