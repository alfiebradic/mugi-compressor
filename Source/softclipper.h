#pragma once
#include <JuceHeader.h>

using namespace juce;

class softclipper {
public:
    float process (float x, float alpha){
        float y = (2 / M_PI) * atan(x * alpha);
        float makeupGain = juce::Decibels::decibelsToGain(7.5f);
        float makedownGain = juce::Decibels::decibelsToGain(-alpha * 1.7);
        return y * makeupGain * makedownGain;
    }
};
