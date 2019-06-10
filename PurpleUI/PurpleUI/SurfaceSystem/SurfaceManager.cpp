#include "SurfaceManager.h"
SurfaceManager *SurfaceManager::s_instance = nullptr;
SurfaceManager * SurfaceManager::GetInstance()
{
	if (s_instance == nullptr)
		s_instance = new SurfaceManager();
	return s_instance;
}

void SurfaceManager::RegisterSurface(const std::string &name, UISurface * surface)
{
	m_surfaces.insert(std::pair<std::string, UISurface *>(name, surface));
	surface->OnCreateUI();
	surface->OnCreate();
	surface->OnCreatedUI();
}

UISurface * SurfaceManager::GetSurface(const std::string & name)
{
	return m_surfaces[name];
}

void SurfaceManager::StartSurface(const std::string &name)
{
	if(!m_activeSurfaces.empty())
		m_activeSurfaces.back()->OnExit();
	UISurface *surface = m_surfaces[name];
	m_activeSurfaces.push_back(surface);
	surface->OnEnter();
}

void SurfaceManager::StartSurface(const std::string & name, void *data)
{
	if (!m_activeSurfaces.empty())
		m_activeSurfaces.back()->OnExit();
	UISurface *surface = m_surfaces[name];
	m_activeSurfaces.push_back(surface);
	surface->OnEnter(data);
}

void SurfaceManager::DestroyActiveSurface()
{
	if (m_activeSurfaces.empty())
		return;
	m_activeSurfaces.back()->OnQuit();
	m_activeSurfaces.pop_back();
	if (!m_activeSurfaces.empty())
		m_activeSurfaces.back()->OnBack();
}

void SurfaceManager::RestartActiveSurface(const std::string &name)
{
	DestroyActiveSurface();
	StartSurface(name);
}
