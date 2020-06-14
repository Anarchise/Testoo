#pragma once

#include "stdafx.h"

class lerpFrame_t
{
public:
    float yawAngle;                    // 0x00
    int yawing;                        // 0x04
    float pitchAngle;                  // 0x08
    int pitching;                      // 0x0C
    int animationNumber;               // 0x10
    void* animation;                   // 0x14 (animation_s*)
    int animationTime;                 // 0x1C
    float oldFramePos[3];              // 0x20
    float prevMovingPlatformOrigin[3]; // 0x2C
    float prevMovingPlatformAngles[3]; // 0x38
    float animSpeedScale;              // 0x44
    int oldFrameSnapshotTime;          // 0x48
    bool movingPlatformOriginValid;    // 0x4C
}; // 0x58
