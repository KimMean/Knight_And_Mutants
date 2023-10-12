#include "Framework.h"
#include "Timer.h"

Timer::Timer()
{
    //1�ʵ��� CPU�������� ��ȯ�ϴ� �Լ�
    QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);

    //���� CPU�������� ��ȯ�ϴ� �Լ�
    QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

    timeScale = 1.0f / (float)periodFrequency;
}

Timer::~Timer()
{
}

void Timer::Update()
{
    // QueryPerformanceCounter �Լ��� ���� �ý��ۿ��� ��� ������ ������ ī������ ���� ��ȯ�մϴ�.
    // �Ϲ������� timeGetTime �Լ��� ���� �ٸ� �Լ����� �� ��Ȯ�� ����� �����մϴ�.
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