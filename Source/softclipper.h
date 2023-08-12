#pragma once
#include <JuceHeader.h>

using namespace juce;

class softclipper {
public:
    float process (float x, float alpha){
        float y = (2 / M_PI) * atan(x * alpha);
        float makeupGain = juce::Decibels::decibelsToGain(11.0f);
        float makedownGain = juce::Decibels::decibelsToGain(-alpha * 3);
        return y * makeupGain * makedownGain;
    }
};
