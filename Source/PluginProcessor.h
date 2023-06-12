/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "filter.h"
#include "detector.h"
#include "envelope.h"
#include "compressor.h"
#include "softclipper.h"
#include "FilmStrip.h"

//==============================================================================
/**
*/
class MugiDynamics4216AudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MugiDynamics4216AudioProcessor();
    ~MugiDynamics4216AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float getVuValue(const int channel, int band) const;
    
    float mInputGain;
    
    float gSmoothedForVU;
    juce::AudioProcessorValueTreeState parameters { *this, nullptr, "Parameters", createParameterLayout() };
    
private:
    
    // Wet filters
    std::vector<LinkwitzRileyLPF> mLowLPF;
    std::vector<LinkwitzRileyHPF> mLowHPF;
    std::vector<LinkwitzRileyHPF> mLowHPFclean;
    std::vector<LinkwitzRileyLPF> mHighLPF;
    std::vector<LinkwitzRileyHPF> mHighHPF;
    std::vector<APF> mAPF_a;
    std::vector<APF> mAPF_b;
    // Dry filters
    std::vector<LinkwitzRileyLPF> mLowLPFDry;
    std::vector<LinkwitzRileyHPF> mLowHPFDry;
    std::vector<LinkwitzRileyLPF> mHighLPFDry;
    std::vector<LinkwitzRileyHPF> mHighHPFDry;
    std::vector<APF> mAPF_aDry;
    std::vector<APF> mAPF_bDry;
    // Sidechain
    std::vector<ButterworthHPF> mSC;
    // Envelopes
    std::vector<envelope> mEnvGR; // For VU
    std::vector<envelope> mEnvLimitLow;
    std::vector<envelope> mEnvLimitMid;
    std::vector<envelope> mEnvLimitHigh;
    // Detectors
    std::vector<detector> mDetectorLow;
    std::vector<detector> mDetectorMid;
    std::vector<detector> mDetectorHigh;
    // Gain computers
    std::vector<compressor> mCompLow;
    std::vector<compressor> mCompMid;
    std::vector<compressor> mCompHigh;
    // Saturators
    std::vector<softclipper> mSatLow;
    std::vector<softclipper> mSatMid;
    std::vector<softclipper> mSatHigh;
    // Values for VU meters
    float mGRforVULowLeft;
    float mGRforVUMidLeft;
    float mGRforVUHighLeft;
    float mGRforVULowRight;
    float mGRforVUMidRight;
    float mGRforVUHighRight;;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MugiDynamics4216AudioProcessor)
};
