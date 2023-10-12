#pragma once

namespace GameMath
{
    enum class Direction
    {
        UP, DOWN, LEFT, RIGHT, NONE
    };

    int Random(const int& min, const int& max);
    float Random(const float& min, const float& max);

    Vector3 Cross(Vector3& vec1, Vector3& vec2);    // 두 벡터 외적
    float Dot(Vector3& vec1, Vector3& vec2);        // 두 벡터 내적 

    // 거리 계산
    float Distance(const Vector2& vec1, const Vector2& vec2);
    float Distance(const Vector3& vec1, const Vector3& vec2);
    
    bool NearlyEqual(const float& s1, const float& s2); // EPSILON값 비교

    float Clamp(const float& min, const float& max, float value);

    float Lerp(const float& start, const float& end, float t);
    Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
    Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
    Matrix Lerp(const Matrix& start, const Matrix& end, float t);

    Vector2 SLerp(const Vector2& start, const Vector2& end, float t);
    Vector3 SLerp(const Vector3& start, const Vector3& end, float t);
    // 노말값 외적 보간
    Vector3 GetNormalFromPolygon(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    // 라인 사이 보간
    Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);



};