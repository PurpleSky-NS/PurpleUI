#ifndef EVENTPUMB_H
#define EVENTPUMB_H

#include <queue>
#include <conio.h>

class EventPump
{
public:
	/*更新事件，用该函数新开一个线程*/
	static void UpdateEventThread();

	/*关闭事件线程*/
	static void StopPumbThread();

	/*若无输入返回0*/
	static char GetInput();

	/*是否有输入*/
	static bool HasInput();

private:
	static std::queue<char> m_eventQueue;
	static bool m_isRunning;
};
#endif