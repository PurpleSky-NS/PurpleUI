#include "ViewRect.h"
ViewRect::~ViewRect()
{
	Clear();
}

void ViewRect::SetViewRect(int w, int h, char ch)
{
	m_viewData = new char[(w + 1)*h];
	//ÉèÖÃViewÇøÓò
	m_srcRect.x = m_srcRect.y = 0;
	m_srcRect.h = h;
	m_srcRect.w = w;
	m_cutRect = m_srcRect;	
	m_isSrcData = true;
	ResetSrcView(ch);
}

void ViewRect::SetViewRect(const ViewRect & src)
{
	m_viewData = src.m_viewData;
	m_srcRect = src.m_srcRect;
	m_cutRect = src.m_cutRect;
	m_isSrcData = false;
}

void ViewRect::SetViewRect(const ViewRect &src, const Rect & cut)
{
	SetViewRect(src);
	m_cutRect.x = src.m_cutRect.x + cut.x >= 0 ? cut.x : 0;
	m_cutRect.y = src.m_cutRect.y + cut.y >= 0 ? cut.y : 0;
	m_cutRect.w = cut.w > src.m_cutRect.w ? src.m_cutRect.w : cut.w;
	m_cutRect.h = cut.h > src.m_cutRect.h ? src.m_cutRect.h : cut.h;
}

void ViewRect::SetViewRect(ViewRect && src)
{
	m_viewData = src.m_viewData;
	m_isSrcData = src.m_isSrcData ? true : false;
	m_srcRect = src.m_srcRect;
	m_cutRect = src.m_cutRect;
	src.m_isSrcData = false;
}
