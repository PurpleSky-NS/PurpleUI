#pragma once
#include "MenuUI.h"
class TestMenu :public MenuUI
{
public:
	void OnCreate(const ViewRect &view)override
	{
		MenuUI::OnCreate(view);
		CreateMenu({ 
			{ "第一个菜单",[](void *) {},nullptr },
			{ "第二个菜单",[](void *) {},nullptr }
			}, 40, 5, 1, Center);
	}
};