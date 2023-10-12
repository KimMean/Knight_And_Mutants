#include "Framework.h"

ModelClip::~ModelClip()
{
    for (auto keyFrame : keyFrames)
    {
        delete keyFrame.second;
    }
}

// Ŭ�� �ʱ�ȭ
void ModelClip::Init()
{
    eventIter = events.begin();
    playTime = 0.0f;
    duration = frameCount / tickPerSecond;
}

void ModelClip::SetLoop()
{
    if (playTime > duration)
    {
        playTime -= duration;
        eventIter = events.begin();
    }
}

// ����
void ModelClip::Excute()
{
    if (events.empty()) return;
    if (eventIter == events.end()) return;
    //int a = playTime / duration;
    //float b = a * duration;
    //float c = playTime - b;
    //float ratio = c / duration;
    //float ratio = playTime - ((playTime / duration) * duration);
    float ratio = playTime / duration;
    // ��� (Map���� ���ĵ�)
    if (eventIter->first > ratio) return;
    // ������ �̺�Ʈ ����
    eventIter->second();
    eventIter++;
}
// ���� �̸����� ������ ã��
KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (keyFrames.count(boneName) == 0)
        return nullptr;

    return keyFrames[boneName];
}
// ������ �̺�Ʈ ����
void ModelClip::SetEvent(Event event, float timeRatio)
{
    if (events.count(timeRatio) > 0) return;

    events[timeRatio] = event;
}
