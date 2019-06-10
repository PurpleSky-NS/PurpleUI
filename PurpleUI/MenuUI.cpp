#include "MenuUI.h"
void MenuUI::OnCreate(const ViewRect & view)
{
	m_animationTimer.SetUpadateTime(0.3f);
	m_animationTimer.AddEvent([&]()
	{
		m_animationStep = ++m_animationStep % 3;
		DrawChooseItem(m_choose, m_animationStep);
	});
	m_srcView.SetViewRect(view);
}

void MenuUI::CreateMenu(const std::vector<MenuItem>& menuItems, int x, int y, int lineOffset, int pageCount, GravityMode mode, bool hasBottomTips)
{
	m_menuItems = menuItems;
	m_lineOffset = lineOffset;
	m_pageCount = pageCount;
	m_pageNum = menuItems.size() / pageCount + 1;
	m_pageLine = (pageCount - 1)*(lineOffset + 1) + 1;
	m_mode = mode;
	m_hasBottomTips = hasBottomTips;
	m_choose = 0;
	int maxWidth = 0;
	for (auto i : m_menuItems)
		if (i.name.size() > (size_t)maxWidth)
			maxWidth = i.name.size();
	maxWidth += (mode == GravityMode::Left ? 4 : 10);//加上箭头宽度
	if (hasBottomTips)
	{
		//提示栏宽度
		int tipWidth = std::string("第/页 第/个").size() + 2 * std::to_string(m_menuItems.size()).size() + 2 * std::to_string(m_menuItems.size() / pageCount).size();
		if (tipWidth > maxWidth)
			maxWidth = tipWidth;
	}
	m_menuView.SetViewRect(m_srcView, { x,y, maxWidth,m_pageLine + hasBottomTips });
}

void MenuUI::CreateMenu(const std::vector<MenuItem>& menuItems, int x, int y, int lineOffset, GravityMode mode)
{
	m_menuItems = menuItems;
	m_lineOffset = lineOffset;
	m_pageCount = menuItems.size();
	m_pageNum = 1;
	m_pageLine = (menuItems.size() - 1)*(lineOffset + 1) + 1;
	m_mode = mode;
	m_hasBottomTips = false;
	m_choose = 0;
	int maxWidth = 0;
	for (auto i : m_menuItems)
		if (i.name.size() > (size_t)maxWidth)
			maxWidth = i.name.size();
	maxWidth += (mode == GravityMode::Left ? 4 : 10);//加上箭头宽度
	m_menuView.SetViewRect(m_srcView, { x,y, maxWidth,m_pageLine });
}

void MenuUI::UpChoose()
{
	if (m_menuItems.empty())
		return;
	m_choose -= 1;
	if (m_choose < 0)
	{
		m_choose = m_menuItems.size() - 1;
		DrawPage(m_choose);
	}
	else if ((m_choose + 1) % m_pageCount == 0)//翻到上一页了
		DrawPage(m_choose);
	else
	{
		DrawNormalItem(m_choose + 1);
		DrawChooseItem(m_choose, m_animationStep = 0);
	}
	if (m_hasBottomTips)
		DrawTips();
	m_animationTimer.Run();
}

void MenuUI::DownChoose()
{
	if (m_menuItems.empty())
		return;
	m_choose += 1;
	if (m_choose == m_menuItems.size())
	{
		m_choose = 0;
		DrawPage(m_choose);
	}
	else if (m_choose%m_pageCount == 0)
		DrawPage(m_choose);
	else
	{
		DrawNormalItem(m_choose - 1);
		DrawChooseItem(m_choose, m_animationStep = 0);
	}
	if (m_hasBottomTips)
		DrawTips();
	m_animationTimer.Run();
}

void MenuUI::UpPage()
{
	m_choose -= m_pageCount;
	m_choose = (m_choose < 0 ? m_choose + m_pageCount : m_choose);
	DrawPage(m_choose);
	if (m_hasBottomTips)
		DrawTips();
	m_animationTimer.Run();
}

void MenuUI::DownPage()
{
	m_choose += m_pageCount;
	m_choose = (m_choose >= (int)m_menuItems.size() ? m_choose % m_pageCount : m_choose);
	DrawPage(m_choose);
	if (m_hasBottomTips)
		DrawTips();
	m_animationTimer.Run();
}

bool MenuUI::OnEvent(char input)
{
	switch (input)
	{
	case 'q':
		SurfaceManager::GetInstance()->DestroyActiveSurface();
		break;
	case 'w':
		UpChoose();
		break;
	case 's':
		DownChoose();
		break;
	case ' ':
	case '\r':
		m_menuItems[m_choose].Event(m_menuItems[m_choose].data);
		break;
	case 'a':
		if (m_pageCount > 1)
			UpPage();
		break;
	case 'd':
		if (m_pageCount > 1)
			DownPage();
		break;
	}
	return false;
}
