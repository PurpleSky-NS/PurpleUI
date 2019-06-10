#include "EventPump.h"
std::queue<char> EventPump::m_eventQueue;
bool EventPump::m_isRunning = false;
void EventPump::UpdateEventThread()
{
	m_isRunning = true;
	while (m_isRunning)
		m_eventQueue.push(_getch());
}
void EventPump::StopPumbThread()
{
	m_isRunning = false;
}

char EventPump::GetInput()
{
	if (m_eventQueue.empty())
		return 0;
	char ch = m_eventQueue.front();
	m_eventQueue.pop();
	return ch;
}

bool EventPump::HasInput()
{
	return !m_eventQueue.empty();
}
