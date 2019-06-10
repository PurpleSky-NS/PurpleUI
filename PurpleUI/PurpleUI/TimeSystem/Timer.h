#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <ctime>
#include <vector>

class Timer
{
public:
	typedef std::function<void()> EventHandler;

	Timer() = default;

	// 请在每一帧调用更新，在一帧中事件最多只会触发一次，若要触发多次请调用StayUpdate
	void Update()
	{
		if (!m_isRunning)
			return;
		float present = (float)clock() / (float)CLOCKS_PER_SEC;
		m_passTime += present - m_begin;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
		if (m_passTime >= m_updateTime)
		{
			m_passTime -= m_updateTime;
			Invoke();
		}
	}

	// 请在每一帧调用更新，在一帧中事件会触发多次（可能会卡帧），若要限制每帧只能触发一次请调用Update
	void StayUpdate()
	{
		if (!m_isRunning)
			return;
		float present = (float)clock() / CLOCKS_PER_SEC;
		m_passTime += present-m_begin;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
		while (m_passTime >= m_updateTime)
		{
			m_passTime -= m_updateTime;
			Invoke();
		}
	}

	//设置定时的时间
	void SetUpadateTime(float updateTime)
	{
		m_updateTime = updateTime;
	}

	//添加事件
	void AddEvent(const EventHandler &Event)
	{
		EventHandle.push_back(Event);
	}

	// 开始运行,重置计时器
	void Run()
	{
		m_isRunning = true;
		m_passTime = 0;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
	}

	//暂停计时器
	void Pause()
	{
		if (!m_isRunning)
			return;
		m_isRunning = false;
		m_passTime += (float)clock() / CLOCKS_PER_SEC - m_begin;
	}

	void Resume()
	{
		if (m_isRunning)
			return;
		m_isRunning = true;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
	}

	// 停止计时器并重置状态
	void Stop()
	{
		m_isRunning = false;
		m_passTime = 0;
		m_begin = 0;
	}

	// 清空定时器为初始状态
	void Clear()
	{
		m_isRunning = false;
		m_updateTime = INFINITY;
		m_passTime = 0;
		m_begin = 0;
		EventHandle.clear();
	}

private:
	std::vector<EventHandler> EventHandle;

	bool m_isRunning = false;
	float m_updateTime;
	float m_passTime;
	float m_begin;

	void Invoke()
	{
		for(auto &i:EventHandle)
			i();
	}
};
#endif