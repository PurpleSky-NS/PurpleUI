#include "TipsUI.h"
TipsUI *TipsUI::activeTips = nullptr;
const float TipsUI::LONG_LENGTH = 7.0f;
const float TipsUI::SHORT_LENGTH = 3.0f;
void TipsUI::CreateTips(const std::string &title, const std::vector<std::string> &content)
{
	//高度为frame:1+title:1+content
	//宽度为frame:1+space:3+content+space:2
	//	   ?frame:1+space:1+title+"："

	m_datas.clear();
	int maxWidth = 0;
	m_datas.push_back("*");
	m_datas.push_back("| " + title + "：");
	for (auto i : content)
		m_datas.push_back("|   " + i + "  ");

	for (auto i : m_datas)
		if (i.size() > (size_t)maxWidth)
			maxWidth = i.size();

	m_datas[0].append(maxWidth - 1, '-');
	for (size_t i = 1; i < m_datas.size(); ++i)
		m_datas[i].append(maxWidth - m_datas[i].size(), ' ');

	m_cutView.SetViewRect(m_srcView, { m_srcView.GetCutRect().w - maxWidth,m_srcView.GetCutRect().h - (int)m_datas.size(),maxWidth,(int)m_datas.size() });
}
void TipsUI::OnCreate(const ViewRect &srcView)
{
	m_srcView.SetViewRect(srcView);
	Begin = [&]()
	{
		Draw(m_passTime / m_fadeTime);
	};

	Stay = [&]()
	{
		Draw(1);
	};

	Fade = [&]()
	{
		Draw(1.0f - m_passTime / m_fadeTime);
	};

	Update = &Begin;

	m_beginTimer.SetUpadateTime(m_fadeTime);
	m_beginTimer.AddEvent([&]()
	{
		Draw(1);
		m_stayTimer.Run();
		Update = &Stay;
		m_updateTimer = &m_stayTimer;
		m_passTime = 0;
	});

	m_stayTimer.SetUpadateTime(m_stayTime);
	m_stayTimer.AddEvent([&]()
	{
		m_fadeTimer.Run();
		Update = &Fade;
		m_updateTimer = &m_fadeTimer;
		m_passTime = 0;
	});

	m_fadeTimer.SetUpadateTime(m_fadeTime);
	m_fadeTimer.AddEvent([&]()
	{
		Disable();
		activeTips = nullptr;
	});
	Disable();
}
void TipsUI::Reset()
{
	m_beginTimer.Run();
	m_updateTimer = &m_beginTimer;
	Update = &Begin;
	m_passTime = 0;
}
void TipsUI::Draw(float percent)
{
	percent = (percent > 1.0f ? 1.0f : percent);
	const Rect &box = m_cutView.GetCutRect();
	int count = int(percent * box.w);
	int spaceCount = box.w - int(percent * box.w);
	m_cutView.ResetCutView();
	for (int i = 0; i < box.h; ++i)
		m_cutView.SetText(spaceCount, i, m_datas[i].substr(0, count));
}