/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FilmStrip.h"

//==============================================================================
/**
*/
class MugiDynamics4216AudioProcessorEditor  : public juce::AudioProcessorEditor, FilmStrip::Listener, public Timer
{
public:
    MugiDynamics4216AudioProcessorEditor (MugiDynamics4216AudioProcessor&);
    ~MugiDynamics4216AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void addSlider(juce::Slider& slider);
    void timerCallback() override;

private:
    
    void sliderValueChanged (juce::Slider* slider) override;
    
    MugiDynamics4216AudioProcessor& audioProcessor;
    
    //===CONTROLS=========================
    
    // Master
    FilmStrip stripInputGain;
    FilmStrip stripBypass;
    FilmStrip stripMix;
    FilmStrip stripOutputGain;
    FilmStrip stripSidechain;
    
    // Low band
    FilmStrip stripLowThreshold;
    FilmStrip stripLowSwitch;
    FilmStrip stripLowMakeupGain;
    FilmStrip stripLowSaturation;
    
    // Mid band
    FilmStrip stripMidThreshold;
    FilmStrip stripMidSwitch;
    FilmStrip stripMidMakeupGain;
    FilmStrip stripMidSaturation;
    
    // High band
    FilmStrip stripHighThreshold;
    FilmStrip stripHighSwitch;
    FilmStrip stripHighMakeupGain;
    FilmStrip stripHighSaturation;
    
    // Crossovers
    FilmStrip stripLowCrossover;
    FilmStrip stripHighCrossover;
    
    
    //===SLIDER ATTACHMENTS======================
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    // Master
    
    SliderAttachment mInputGainAttachment{audioProcessor.parameters, "inputgain", stripInputGain};
    SliderAttachment mBypassAttachment{audioProcessor.parameters, "bypass", stripBypass};
    SliderAttachment mMixAttachment{audioProcessor.parameters, "mix", stripMix};
    SliderAttachment mOutputGainAttachment{audioProcessor.parameters, "outputgain", stripOutputGain};
    SliderAttachment mSidechainAttachment{audioProcessor.parameters, "sidechain", stripSidechain};
    
    // Low band
    SliderAttachment mLowThresholdAttachment{audioProcessor.parameters, "lowthreshold", stripLowThreshold};
    SliderAttachment mLowSwitchAttachment{audioProcessor.parameters, "lowswitch", stripLowSwitch};
    SliderAttachment mLowMakeupGainAttachment{audioProcessor.parameters, "lowmakeupgain", stripLowMakeupGain};
    SliderAttachment mLowSaturationAttachment{audioProcessor.parameters, "lowsaturation", stripLowSaturation};
    
    // Mid band
    SliderAttachment mMidThresholdAttachment{audioProcessor.parameters, "midthreshold", stripMidThreshold};
    SliderAttachment mMidSwitchAttachment{audioProcessor.parameters, "midswitch", stripMidSwitch};
    SliderAttachment mMidMakeupGainAttachment{audioProcessor.parameters, "midmakeupgain", stripMidMakeupGain};
    SliderAttachment mMidSaturationAttachment{audioProcessor.parameters, "midsaturation", stripMidSaturation};
    
    // High band
    SliderAttachment mHighThresholdAttachment{audioProcessor.parameters, "highthreshold", stripHighThreshold};
    SliderAttachment mHighSwitchAttachment{audioProcessor.parameters, "highswitch", stripHighSwitch};
    SliderAttachment mHighMakeupGainAttachment{audioProcessor.parameters, "highmakeupgain", stripHighMakeupGain};
    SliderAttachment mHighSaturationAttachment{audioProcessor.parameters, "highsaturation", stripHighSaturation};
    
    // Crossovers
    SliderAttachment mLowCrossoverAttachment{audioProcessor.parameters, "lowcrossover", stripLowCrossover};
    SliderAttachment mHighCrossoverAttachment{audioProcessor.parameters, "highcrossover", stripHighCrossover};

    
    //===IMAGE COMPONENTS=========================
    
    
    NeedleComponent whiteL;
    NeedleComponent blueL;
    NeedleComponent orangeL;
    
    NeedleComponent whiteR;
    NeedleComponent blueR;
    NeedleComponent orangeR;

 

    //===IMAGES===================================
    
    juce::Image background;
    juce::ImageComponent backgroundComponent;
    
    VUComponent VUBackground;
    juce::Image VUBackgroundImage;
    
    VUComponent VUParent;
    juce::Image VUParentImage;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MugiDynamics4216AudioProcessorEditor)
};
