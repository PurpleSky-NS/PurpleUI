#ifndef UISURFACE_H
#define UISURFACE_H

#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include "UIComponent.h"

/*一个界面，应被子类继承并设计*/
class UISurface
{	
public:

	virtual ~UISurface();

	/*在这里创建并添加UI组件@AddUIComponent*/
	virtual void OnCreateUI() {}

	/*在界面第一次创建的时候调用,只会调用一次,最好设置FocusUI接受事件*/
	virtual void OnCreate() {}

	/*子类请调用这个*/
	/*在进入界面的时候调用*/
	inline virtual void OnEnter();

	/*子类请调用这个*/
	/*传递一个参数的版本*/
	inline virtual void OnEnter(void *data);

	/*在界面之上切换到另一个界面的时候调用*/
	inline virtual void OnExit();

	/*在从另一个界面返回界面时调用*/
	inline virtual void OnBack();

	/*在界面退出时调用*/
	virtual void OnQuit() {}

	/*子类请调用这个*/
	/*在界面绘制的时候调用*/
	inline virtual void OnUpdate();

	/*子类请调用这个*/
	/*在界面销毁的时候调用*/
	inline virtual void OnDestroy();

	/*处理事件输入*/
	inline virtual void OnEvent(char input);

	/*获取视图*/
	inline ViewRect *GetView();

	/*获取UI组件*/
	template<class T>
	inline T *GetUIComponent();

	/*获取UI组件*/
	template<class T>
	inline std::vector<T*> GetUIComponents();

	/*设置焦点UI，该UI会接受所有事件信息，并且有权决定是否将事件传递*/
	inline void SetFocusUI(UIComponent *focusUi);

	/*获取焦点UI*/
	inline UIComponent * GetFocusUI()const;

	/*强制界面重绘*/
	inline void Draw();

	/*强制界面重绘，不重绘withoutUI*/
	inline void Draw(UIComponent *withoutUI);

	/*获取帧率*/
	inline float GetFps()const;

	/*设置帧率*/
	inline void SetFps(float fps);

protected:

	UISurface(unsigned char fps,int w,int h)
		: m_fps(fps)
	{
		m_view.SetViewRect(w, h);
	}

	/*清空UI组件，释放托管的资源*/
	void ClearUIComponents();

	/*添加UI组件，是否进行内存托管[界面退出时自动释放资源]*/
	void AddUIComponent(UIComponent *newUI, bool isMemoryDeposit = true);

private:
	friend class SurfaceManager;

	struct UIComponentMsg
	{
		UIComponentMsg() = default;
		UIComponentMsg(UIComponent *ui, bool isMemoryDeposited = true)
		{
			this->ui = ui;
			this->isMemoryDeposit = isMemoryDeposited;
		}

		UIComponent *ui;
		bool isMemoryDeposit;
	};

	float m_fps;//帧率
	ViewRect m_view;
	std::vector<UIComponentMsg> m_uiComs;
	UIComponent *m_focusUi = nullptr;

	/*当创建完UI时调用*/
	void OnCreatedUI();
};
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
ViewRect *UISurface::GetView()
{
	return &m_view;
}
template<class T>
T *UISurface::GetUIComponent()
{
	return (T*)std::find_if(m_uiComs.begin(), m_uiComs.end(),[&](const UIComponentMsg &uiMsg)
	{
		return typeid(*(uiMsg.ui)) == typeid(T);
	})->ui;
}
template<class T>
std::vector<T*> UISurface::GetUIComponents()
{
	std::vector<T*> uis;
	for (auto &uiMsg : m_uiComs)
	{
		if (typeid(*(uiMsg.ui)) == typeid(T))
			uis.push_back(uiMsg.ui);
	}
	return uis;
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
float UISurface::GetFps() const
{
	return m_fps;
}
void UISurface::SetFps(float fps)
{
	m_fps = fps;
}
#endif

