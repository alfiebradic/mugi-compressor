/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MugiDynamics4216AudioProcessorEditor::MugiDynamics4216AudioProcessorEditor (MugiDynamics4216AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),

// LOAD FILMSTRIP IMAGES
// Master
stripInputGain(ImageCache::getFromMemory(BinaryData::input_png, BinaryData::input_pngSize), 51),
stripBypass(ImageCache::getFromMemory(BinaryData::bypass_png, BinaryData::bypass_pngSize), 2),
stripMix(ImageCache::getFromMemory(BinaryData::mix_png, BinaryData::mix_pngSize), 51),
stripOutputGain(ImageCache::getFromMemory(BinaryData::output_png, BinaryData::output_pngSize), 51),
stripSidechain(ImageCache::getFromMemory(BinaryData::screw_png, BinaryData::screw_pngSize), 51),
// Low
stripLowThreshold(ImageCache::getFromMemory(BinaryData::low_thresh_png, BinaryData::low_thresh_pngSize), 51),
stripLowSwitch(ImageCache::getFromMemory(BinaryData::low_mode_png, BinaryData::low_mode_pngSize), 3),
stripLowMakeupGain(ImageCache::getFromMemory(BinaryData::low_gain_png, BinaryData::low_gain_pngSize), 51),
stripLowSaturation(ImageCache::getFromMemory(BinaryData::screw_png, BinaryData::screw_pngSize), 51),
// Mid
stripMidThreshold(ImageCache::getFromMemory(BinaryData::mid_thresh_png, BinaryData::mid_thresh_pngSize), 51),
stripMidSwitch(ImageCache::getFromMemory(BinaryData::mid_mode_png, BinaryData::mid_mode_pngSize), 3),
stripMidMakeupGain(ImageCache::getFromMemory(BinaryData::mid_gain_png, BinaryData::mid_gain_pngSize), 51),
stripMidSaturation(ImageCache::getFromMemory(BinaryData::screw_png, BinaryData::screw_pngSize), 51),
// High
stripHighThreshold(ImageCache::getFromMemory(BinaryData::high_thresh_png, BinaryData::high_thresh_pngSize), 51),
stripHighSwitch(ImageCache::getFromMemory(BinaryData::high_mode_png, BinaryData::high_mode_pngSize), 3),
stripHighMakeupGain(ImageCache::getFromMemory(BinaryData::high_gain_png, BinaryData::high_gain_pngSize), 51),
stripHighSaturation(ImageCache::getFromMemory(BinaryData::screw_png, BinaryData::screw_pngSize), 51),
// Crossovers
stripLowCrossover(ImageCache::getFromMemory(BinaryData::crossover_low_png, BinaryData::crossover_low_pngSize), 51),
stripHighCrossover(ImageCache::getFromMemory(BinaryData::crossover_high_png, BinaryData::crossover_high_pngSize), 51),
// VU assets
VUBackground(ImageCache::getFromMemory(BinaryData::vu_background_png, BinaryData::vu_background_pngSize)),
VUParent(ImageCache::getFromMemory(BinaryData::vu_covers_png, BinaryData::vu_covers_pngSize)),
// Left VU
whiteL(ImageCache::getFromMemory(BinaryData::whiteL_png, BinaryData::whiteL_pngSize), false, 325, 155),
blueL(ImageCache::getFromMemory(BinaryData::blueL_png, BinaryData::blueL_pngSize), false, 325, 155),
orangeL(ImageCache::getFromMemory(BinaryData::orangeL_png, BinaryData::orangeL_pngSize), false, 325, 155),
// Right VU
whiteR(ImageCache::getFromMemory(BinaryData::whiteR_png, BinaryData::whiteR_pngSize), true, 700, 155),
blueR(ImageCache::getFromMemory(BinaryData::blueR_png, BinaryData::blueR_pngSize), true, 700, 155),
orangeR(ImageCache::getFromMemory(BinaryData::orangeR_png, BinaryData::orangeR_pngSize), true, 700, 155)

{
    setSize (1024, 576);
    startTimerHz(60);

    background = juce::ImageCache::getFromMemory(BinaryData::mugibackground_sharp_png, BinaryData::mugibackground_sharp_pngSize);
    
        if (background.isValid()){
            backgroundComponent.setImage(background);
            addAndMakeVisible(&backgroundComponent);
        }

    // ADD IMAGES
    addAndMakeVisible(VUBackground);
    // VU needles
    addAndMakeVisible(whiteL);
    addAndMakeVisible(blueL);
    addAndMakeVisible(orangeL);
    addAndMakeVisible(whiteR);
    addAndMakeVisible(blueR);
    addAndMakeVisible(orangeR);
    addAndMakeVisible(VUParent); // Cover
    
    // ADD SLIDERS
    // Master
    addSlider(stripInputGain);
    addSlider(stripBypass);
    addSlider(stripOutputGain);
    addSlider(stripMix);
    addSlider(stripSidechain);
    // Low
    addSlider(stripLowThreshold);
    addSlider(stripLowSwitch);
    addSlider(stripLowMakeupGain);
    addSlider(stripLowSaturation);
    // Mid
    addSlider(stripMidThreshold);
    addSlider(stripMidSwitch);
    addSlider(stripMidMakeupGain);
    addSlider(stripMidSaturation);
    // High
    addSlider(stripHighThreshold);
    addSlider(stripHighSwitch);
    addSlider(stripHighMakeupGain);
    addSlider(stripHighSaturation);
    // Crossovers
    addSlider(stripLowCrossover);
    addSlider(stripHighCrossover);
    
    // ADD LISTENERS
    // Master
    stripInputGain.addListener(this);
    stripBypass.addListener(this);
    stripMix.addListener(this);
    stripOutputGain.addListener(this);
    stripSidechain.addListener(this);
    // Low
    stripLowThreshold.addListener(this);
    stripLowSwitch.addListener(this);
    stripLowMakeupGain.addListener(this);
    stripLowSaturation.addListener(this);
    // Mid
    stripMidThreshold.addListener(this);
    stripMidSwitch.addListener(this);
    stripMidMakeupGain.addListener(this);
    stripMidSaturation.addListener(this);
    // High
    stripHighThreshold.addListener(this);
    stripHighSwitch.addListener(this);
    stripHighMakeupGain.addListener(this);
    stripHighSaturation.addListener(this);
    // Crossovers
    stripLowCrossover.addListener(this);
    stripHighCrossover.addListener(this);
}

MugiDynamics4216AudioProcessorEditor::~MugiDynamics4216AudioProcessorEditor()
{
}

//==============================================================================
void MugiDynamics4216AudioProcessorEditor::timerCallback()
{
    whiteL.setLevel(audioProcessor.getVuValue(0,1));
    whiteL.repaint();
    blueL.setLevel(audioProcessor.getVuValue(0,2));
    blueL.repaint();
    orangeL.setLevel(audioProcessor.getVuValue(0,3));
    orangeL.repaint();
    
    whiteR.setLevel(audioProcessor.getVuValue(1,1));
    whiteR.repaint();
    blueR.setLevel(audioProcessor.getVuValue(1,2));
    blueR.repaint();
    orangeR.setLevel(audioProcessor.getVuValue(1,3));
    orangeR.repaint();
}

//==============================================================================
void MugiDynamics4216AudioProcessorEditor::paint (juce::Graphics& g)
{
    backgroundComponent.setBounds(0, 0, 1024, 576);
}

void MugiDynamics4216AudioProcessorEditor::resized()
{

    // Master
    stripInputGain.setBounds(129, 91, 83, 83);
    stripBypass.setBounds(87, 41, 45, 45);
    stripMix.setBounds(943, 83, 45, 45);
    stripOutputGain.setBounds(810, 90, 83, 83);
    stripSidechain.setBounds(51, 98, 25, 25);
    // Low
    stripLowThreshold.setBounds(96, 271, 117, 117);
    stripLowSwitch.setBounds(211, 439, 48, 48);
    stripLowMakeupGain.setBounds(21, 408, 74, 74);
    stripLowSaturation.setBounds(139, 505, 27, 27);
    // Mid
    stripMidThreshold.setBounds(454, 320, 117, 117);
    stripMidSwitch.setBounds(623, 436, 48, 48);
    stripMidMakeupGain.setBounds(341, 410, 74, 74);
    stripMidSaturation.setBounds(498, 505, 27, 27);
    // High
    stripHighThreshold.setBounds(816, 273, 117, 117);
    stripHighSwitch.setBounds(928, 437, 48, 48);
    stripHighMakeupGain.setBounds(746, 409, 74, 74);
    stripHighSaturation.setBounds(860, 506, 27, 27);
    // Crossovers
    stripLowCrossover.setBounds(320, 319, 49, 49);
    stripHighCrossover.setBounds(651, 319, 49, 49);
    // VU stuff
    VUBackground.setBounds(0, 0, 1024, 756);
    whiteL.setBounds(329, 152, 120, 8);
    blueL.setBounds(325, 152, 117, 7);
    orangeL.setBounds(322, 152, 112, 7);
    whiteR.setBounds(576, 152, 120, 8);
    blueR.setBounds(584, 152, 116, 7);
    orangeR.setBounds(592, 152, 112, 7);
    VUParent.setBounds(0, 0, 1024, 756);
    
}

void MugiDynamics4216AudioProcessorEditor::addSlider(juce::Slider& slider){
    slider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    slider.setMouseDragSensitivity(200);
    addAndMakeVisible (&slider);
};

void MugiDynamics4216AudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    if(slider == &stripInputGain){
        audioProcessor.mInputGain = slider->getValue();
    }
}
