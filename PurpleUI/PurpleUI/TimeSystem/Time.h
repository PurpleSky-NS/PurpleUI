#ifndef TIME_H
#define TIME_H

#include <ctime>

/*以秒为单位*/
class Time
{
public:
	/*获取自上一帧调用经过的时间*/
	inline static float GetDeltaTime();

	/*获取程序运行经过时间*/
	inline static float GetRunningTime();

private:
	friend class SurfaceManager;

	static float s_deltaTime;
	static float s_beginDeltaTime;

	/*设置这一帧的开始时间*/
	inline static void SetDeltaTime(float delta);

	/*设置这一帧的开始时间*/
	inline static void SetDeltaBeginTime();

	/*设置这一帧的结束时间*/
	inline static void SetDeltaEndTime();

};
inline void Time::SetDeltaTime(float delta)
{
	s_deltaTime = delta;
}
void Time::SetDeltaBeginTime()
{
	s_beginDeltaTime = clock() / (float)CLOCKS_PER_SEC;
}

void Time::SetDeltaEndTime()
{
	if (s_beginDeltaTime >= 0)
		s_deltaTime = clock() / (float)CLOCKS_PER_SEC - s_beginDeltaTime;
}

float Time::GetDeltaTime()
{
	return s_deltaTime;
}

float Time::GetRunningTime()
{
	return clock() / (float)CLOCKS_PER_SEC;
}

#endif