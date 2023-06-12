/*
  ==============================================================================

    detector.h
    Created: 11 Jun 2023 6:24:56pm
    Author:  Alfie Bradic

  ==============================================================================
*/

#pragma once
#include "envelope.h"

class detector {
public:
    
    float detect (float input, double Ts){
        
        float envStage1 = mEnv1.processPeak(input, mStage1_at, mStage1_rt, Ts);
        float envStage2 = mEnv2.processPeak(input, mStage2_at, mStage2_rt, Ts);
        float detection = envStage1 * mDetectorMix + (1 - mDetectorMix) * envStage2;
        float envMixdB = juce::Decibels::gainToDecibels(std::abs(detection));
        return envMixdB;
    }
    
private:
    
    envelope mEnv1;
    envelope mEnv2;
    
    const float mStage1_at = 0.0012;
    const float mStage1_rt = 0.078;
    const float mStage2_at = 0.1;
    const float mStage2_rt = 3.0;
    const float mDetectorMix = 0.72;
};
