/*
  ==============================================================================

    compressor.h
    Created: 10 Jun 2023 1:38:19pm
    Author:  Alfie Bradic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "envelope.h"

using namespace juce;

class compressor {
public:
    float processDown (float input, float threshold, float envMixdB, float ratioInv){
            float YdB = envMixdB;
            if (envMixdB > threshold){
                YdB = threshold + ratioInv * (envMixdB - threshold);
            }
            float GdB = YdB - envMixdB;
            float g = juce::Decibels::decibelsToGain(GdB);
        return g;
    }
    float processUp (float input, float threshold, float envMixdB, float ratio){

            float YdB = envMixdB;
            if (envMixdB < threshold){
                YdB = threshold + ratio * (threshold - envMixdB);
            }
            float GdB = YdB - envMixdB;
            float g = juce::Decibels::decibelsToGain(GdB);
        return g;
    }
    
    float computeGain (int setting, float input, float threshold, float envDown, float envLimit, float makeupGainRaw, float Ts){

        // DOWNWARDS COMPRESSION
        if (setting == 1){
            float gDown = processDown(input, threshold, envDown, ratioDownInv);
            // Smooth GR value for VU meter
            float GRraw = mEnvGRForVU.processPeak(gDown, 0.05, 0.08, Ts); // 0.0005 0.07
            gainReductionForVU = juce::Decibels::gainToDecibels(GRraw);
            // Mix
            mOutput = input * gDown * makeupGainRaw;
            return mOutput;
            
        }
        // UPWARDS COMPRESSION
        else if (setting == -1){
            float gUp = processUp(input, -0.05 * threshold, envLimit, ratioUp);
            float outputOfUpwardsComp = input * gUp;
            // Overcompression
            float gDown = processDown(outputOfUpwardsComp, threshold + headroom, envDown, ratioDownInv);
            // Smooth GR value for VU meter
            float GRraw = mEnvGRForVU.processPeak(gUp, 0.1, 0.5, Ts);
            gainReductionForVU = juce::Decibels::gainToDecibels(GRraw);
            // Mix
            mOutput = outputOfUpwardsComp * gDown * upGainOffset * makeupGainRaw;
            return mOutput;
        }
        // BYPASS
        else {
            float gZero = mEnvGRForVU.processPeak(0.f, 0.1, 0.2, Ts);
            gainReductionForVU = gZero;
            return input;
        }
    }
    
    float gainReductionForVU = 0.0f;
    
private:

    envelope mEnvGRForVU;
    float mOutput;
    const float ratioDown = 4;
    const float ratioDownInv = 1 / ratioDown;
    const float ratioUp = 16;
    const float headroom = 32.0f; // overcompression threshold offset
    const float upGainOffset = juce::Decibels::decibelsToGain(-4.0f);
    
};
