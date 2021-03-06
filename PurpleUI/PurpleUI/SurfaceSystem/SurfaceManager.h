#ifndef SURFACEMANAGER
#define SURFACEMANAGER

#include <unordered_map>
#include <string>
#include "UISurface.h"
#include "TimeSystem/Timer.h"
#include "TimeSystem/Time.h"

/*使用单例类模式管理界面*/
class SurfaceManager
{
public:
	/*获取实例*/
	static SurfaceManager *GetInstance();

	/*注册界面*/
	void RegisterSurface(const std::string &name, UISurface *surface);

	/*获取界面*/
	UISurface *GetSurface(const std::string &name);

	inline UISurface *GetActiveSurface()const;

	/*启动界面*/
	void StartSurface(const std::string &name);

	/*启动界面*/
	void StartSurface(const std::string &name,void *data);

	/*销毁当前活动界面*/
	void DestroyActiveSurface();

	/*销毁当前活动界面，并且重启一个新的活动界面(即使当前活动界面在栈底)*/
	void RestartActiveSurface(const std::string& name);

	/*获取当前活动界面的FPS*/
	inline float GetActiveFPS()const;

	/*活动界面是否为空*/
	inline bool Empty()const;

private:
	friend class UIMain;
	static SurfaceManager *s_instance;

	SurfaceManager() = default;
	SurfaceManager(const SurfaceManager&) = delete;
	SurfaceManager(SurfaceManager&&) = delete;

	std::unordered_map<std::string, UISurface *> m_surfaces;//hash map就不实现了，太麻烦了
	std::vector<UISurface *> m_activeSurfaces;//如果用栈有些功能不好实现

	/*更新处于栈顶的界面，若返回false则没有活动界面，退出程序*/
	inline void OnUpdata();

	/*有事件*/
	inline void OnEvent(char input);

	/*注册完成，调用界面的OnCreate*/
	inline void Begin();

	/*程序结束，调用界面的OnDestroy*/
	inline void End();
};

UISurface *SurfaceManager::GetActiveSurface() const
{
	if (!m_activeSurfaces.empty())
		return m_activeSurfaces.back();
	return nullptr;
}
float SurfaceManager::GetActiveFPS()const
{
	if (m_activeSurfaces.empty())
		return INFINITY;
	return m_activeSurfaces.back()->GetFps();
}
bool SurfaceManager::Empty() const
{
	return m_activeSurfaces.empty();
}
void SurfaceManager::OnUpdata()
{
	if (!m_activeSurfaces.empty())
		GetActiveSurface()->OnUpdate();
}
void SurfaceManager::OnEvent(char input)
{
	if (!m_activeSurfaces.empty())
		m_activeSurfaces.back()->OnEvent(input);
}
void SurfaceManager::Begin()
{
	for (auto &i : m_surfaces)
		i.second->OnCreate();
}
void SurfaceManager::End()
{
	for (auto &i : m_surfaces)
	{
		i.second->OnDestroy();
		delete i.second;
	}
}
#endif
