#include "Framework.h"
#include "Timer.h"

Timer::Timer()
{
    //1초동안 CPU진동수를 반환하는 함수
    QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);

    //현재 CPU진동수를 반환하는 함수
    QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

    timeScale = 1.0f / (float)periodFrequency;
}

Timer::~Timer()
{
}

void Timer::Update()
{
    // QueryPerformanceCounter 함수는 현재 시스템에서 사용 가능한 고성능 카운터의 값을 반환합니다.
    // 일반적으로 timeGetTime 함수와 같은 다른 함수보다 더 정확한 결과를 제공합니다.
    QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
    elapsedTime = (float)(curTime - lastTime) * timeScale;

    //if (lockFPS != 0)
    //{
    //    while (elapsedTime < (1.0f / lockFPS))
    //    {
    //        QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
    //        elapsedTime = (float)(curTime - lastTime) * timeScale;
    //    }
    //}

    lastTime = curTime;

    frameCount++;
    oneSecCount += elapsedTime;

    if (oneSecCount >= 1.0f)
    {
        frameRate = frameCount;
        frameCount = 0;
        oneSecCount = 0.0f;
    }
}

float Timer::GetElapsedTime()
{
    return elapsedTime > EPSILON ? EPSILON : elapsedTime * deltaScale;
}
