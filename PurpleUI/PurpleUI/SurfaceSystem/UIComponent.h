#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "ViewRect.h"

class UISurface;
class UIComponent
{
public:

	inline void Enable();

	inline void Disable();

	virtual void OnEnable() {}

	virtual void OnDisable() {}
	
	//对于disable的组件，不会调用@OnDraw,@OnUpdate与@OnEvent[除非该组件为focusUI]
	bool IsEnable()const { return m_isEnable; }

	/*创建的时候调用,只会调用一次*/
	virtual void OnCreate(const ViewRect &srcView) {}

	/*返回值决定是否继续传递*/
	virtual bool OnEvent(char input) { return true; }

	virtual void OnDraw() {}

	//启动界面的时候用
	virtual void OnReset() {}

	//离开界面的时候用
	virtual void OnPause() {}

	//启动界面的时候用
	virtual void OnResume() {}

	virtual void OnUpdate() {}

	virtual void OnDestroy() {}

	virtual void OnGetFocus() {}

	virtual void OnLoseFocus() {}

	inline virtual bool IsFocus()const;

	inline UISurface *GetContext()const;

	/*绘制边框*/
	inline void DrawFrame(ViewRect &rect, char ch);

	/*获取字符串居中排布时，该字符串的起始坐标，str.size()应该<=w，w为一行的宽度*/
	inline static int GetTextCenterLayoutBeginPos(const std::string &str, int w);

	/*获取字符串靠右排布时，该字符串的起始坐标，str.size()应该<=w，w为一行的宽度*/
	inline static int GetTextRightLayoutBeginPos(const std::string &str, int w);


private:
	friend class UISurface;
	UISurface *m_context;
	bool m_isEnable = true;
	bool m_isFocus = false;
};
inline void UIComponent::Enable()
{
	if (m_isEnable)
		return;
	m_isEnable = true;
	OnEnable();
	OnDraw();
}
inline void UIComponent::Disable()
{
	if (!m_isEnable)
		return;
	m_isEnable = false;
	OnDisable();
}
int UIComponent::GetTextCenterLayoutBeginPos(const std::string &str, int w)
{
	return (w - str.size()) / 2;
}
int UIComponent::GetTextRightLayoutBeginPos(const std::string & str, int w)
{
	return w - str.size();
}
bool UIComponent::IsFocus() const
{
	return m_isFocus;
}
UISurface *UIComponent::GetContext() const
{
	return m_context;
}
void UIComponent::DrawFrame(ViewRect &view, char ch)
{
	Rect box = view.GetCutRect();
	std::string line(box.w, ch);
	view.SetText(0, 0, line);
	view.SetText(0, box.h - 1, line);
	for (int i = 1, limH = box.h - 1, limW = box.w - 1; i < limH; ++i)
		view(0, i) = view(limW, i) = ch;
}
#endif