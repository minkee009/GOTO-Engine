#include "BBox.h"
#include <algorithm>

using namespace GOTOEngine;

bool BBox::Intersect(BBox other)
{
	return (maxX >= other.minX && minX <= other.maxX && maxY >= other.minY && minY <= other.maxY);
}

bool BBox::Intersect(float pointX, float pointY)
{
	return (maxX >= pointX && minX <= pointX && maxY >= pointY && minY <= pointY);
}

int BBox::ApplyPenetration(float* posX, float* posY, BBox other)
{
    // First check if there's any intersection at all
    if (!Intersect(other)) {
        return 0; // No intersection
    }

    float penetrationX = std::min(maxX - other.minX, other.maxX - minX);
    float penetrationY = std::min(maxY - other.minY, other.maxY - minY);

    if (penetrationX < penetrationY) {
        if (minX < other.minX) {
            *posX -= penetrationX; // Push left
            return BBOX_FLAG_PUSHLEFT;
        }
        else {
            *posX += penetrationX; // Push right
            return BBOX_FLAG_PUSHRIGHT;
        }
    }
    else {
        if (minY < other.minY) {
            *posY -= penetrationY; // Push up (consistent with horizontal cases)
            return BBOX_FLAG_PUSHUP;
        }
        else {
            *posY += penetrationY; // Push down
            return BBOX_FLAG_PUSHDOWN;
        }
    }
}

int BBox::CalcPenetration(BBox other)
{
    // First check if there's any intersection at all
    if (!Intersect(other)) {
        return 0; // No intersection
    }

    float penetrationX = std::min(maxX - other.minX, other.maxX - minX);
    float penetrationY = std::min(maxY - other.minY, other.maxY - minY);

    if (penetrationX < penetrationY) {
        if (minX < other.minX)
            return BBOX_FLAG_PUSHLEFT;
        else
            return BBOX_FLAG_PUSHRIGHT;
    }
    else {
        if (minY < other.minY)
            return BBOX_FLAG_PUSHUP;
        else
            return BBOX_FLAG_PUSHDOWN;
    }
}
