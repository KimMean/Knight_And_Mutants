#pragma once

struct Ray
{
    Vector3 pos;
    Vector3 dir;

    Ray()
    {
    }

    Ray(Vector3 pos, Vector3 dir) : pos(pos), dir(dir)
    {
    }
};

struct Contact
{
    string tag;

    Vector3 hitPoint;
    float distance;
};