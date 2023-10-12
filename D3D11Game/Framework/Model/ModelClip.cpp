#include "Framework.h"

ModelClip::~ModelClip()
{
    for (auto keyFrame : keyFrames)
    {
        delete keyFrame.second;
    }
}

// 클립 초기화
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

// 실행
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
    // 대기 (Map으로 정렬됨)
    if (eventIter->first > ratio) return;
    // 지정한 이벤트 실행
    eventIter->second();
    eventIter++;
}
// 뼈대 이름으로 프레임 찾기
KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (keyFrames.count(boneName) == 0)
        return nullptr;

    return keyFrames[boneName];
}
// 프레임 이벤트 설정
void ModelClip::SetEvent(Event event, float timeRatio)
{
    if (events.count(timeRatio) > 0) return;

    events[timeRatio] = event;
}
