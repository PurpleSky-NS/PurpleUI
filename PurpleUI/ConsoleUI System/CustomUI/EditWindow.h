#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <vector>
#include <string>
#include <cctype>
#include <functional>
#include "SurfaceManager.h"
#include "UIComponent.h"

/*编辑窗口UI*/
class EditWindow :public UIComponent
{
public:

	inline void Reset();

	inline void Show();

protected:

	struct EditDataMsg
	{
		std::string name;
		std::string tips;
		bool hasTips;
		bool isVisitable;
		std::function<bool(char)> TestInput;
		std::string resualt;
	};

	/*编辑完成，返回是否退出，应重写*/
	inline virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs);

	void OnCreate(const ViewRect &srcView)override;

	/*Oncreate调用，创建这个窗口*/
	void CreateEditor(int x, int y, const std::vector<EditDataMsg> &msgs);

	inline virtual void OnDraw();

	virtual void OnEnable()override;

	/*默认会回到原来的focusUI，如果有特殊需求，应重写该函数*/
	virtual void OnDisable()override;

	inline virtual void OnUpdate();

	virtual bool OnEvent(char input);
	
private:
	ViewRect m_srcView, m_view;
	std::vector<EditDataMsg> m_msgs;
	int m_edit = 0;
	char m_mark = '_';
	const float m_markChangeTime = 0.8f;
	Timer m_markChangeTimer;
	UIComponent *m_backFocus = nullptr;

	inline void DrawEditData(int index);

	inline void DrawNormalData(int index);
};
bool EditWindow::OnEditFinish(const std::vector<EditDataMsg>& msgs)
{ 
	return true; 
}
void EditWindow::OnCreate(const ViewRect & srcView)
{
	m_markChangeTimer.SetUpadateTime(m_markChangeTime);
	m_markChangeTimer.AddEvent([&]()
	{
		m_mark = (m_mark == '_' ? ' ' : '_');
	});
	m_markChangeTimer.Run();
	m_srcView.SetViewRect(srcView);
}

void EditWindow::Reset()
{
	for (auto &i : m_msgs)
		i.resualt.clear();
}

void EditWindow::Show()
{
	Enable();
	m_backFocus = GetContext()->GetFocusUI();
	GetContext()->SetFocusUI(this);
}

void EditWindow::OnDraw()
{
	if (m_msgs.empty())
		return;
	m_view.ResetCutView();
	for (size_t i = 0; i < m_msgs.size(); ++i)
		if (i == m_edit)
			DrawEditData(i);
		else
			DrawNormalData(i);
	DrawFrame(m_view, '*');
}

void EditWindow::OnUpdate()
{
	m_markChangeTimer.Update();
	DrawEditData(m_edit);
}
#endif