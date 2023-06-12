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
stripInputGain(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/input.png")), 51),
stripBypass(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/bypass.png")), 2),
stripMix(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/mix.png")), 51),
stripOutputGain(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/output.png")), 51),
stripSidechain(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/screw.png")), 51),

// Low
stripLowThreshold(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/low_thresh.png")), 51),
stripLowSwitch(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/low_mode.png")), 3),
stripLowMakeupGain(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/low_gain.png")), 51),
stripLowSaturation(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/screw.png")), 51),

// Mid
stripMidThreshold(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/mid_thresh.png")), 51),
stripMidSwitch(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/mid_mode.png")), 3),
stripMidMakeupGain(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/mid_gain.png")), 51),
stripMidSaturation(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/screw.png")), 51),

// High
stripHighThreshold(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/high_thresh.png")), 51),
stripHighSwitch(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/high_mode.png")), 3),
stripHighMakeupGain(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/high_gain.png")), 51),
stripHighSaturation(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/screw.png")), 51),
// Crossovers
stripLowCrossover(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/crossover_low.png")), 51),
stripHighCrossover(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/crossover_high.png")), 51),


// VU assets

VUBackground(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/vu_background.png"))),

VUParent(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/vu_covers.png"))),

// Left VU
whiteL(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/whiteL.png")), false, 325, 155),
blueL(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/blueL.png")), false, 325, 155),
orangeL(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/orangeL.png")), false, 325, 155),
// Right VU
whiteR(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/whiteR.png")), true, 700, 155),
blueR(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/blueR.png")), true, 700, 155),
orangeR(ImageFileFormat::loadFrom(File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/orangeR.png")), true, 700, 155)




{

    setSize (1024, 576);
    startTimerHz(60);
    
    // Load single image assets
        background = juce::ImageFileFormat::loadFrom(juce::File("/Users/alfiebradic/juce_projects/MugiDynamics4216/Source/Images/mugibackground_sharp.png"));
    
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
    g.fillAll(juce::Colours::darkgrey);
    //247, 8
    
    backgroundComponent.setBounds(0, 0, 1024, 576);

}

void MugiDynamics4216AudioProcessorEditor::resized()
{

    // Master
    stripInputGain.setBounds(134, 93, 77, 77);
    stripBypass.setBounds(87, 41, 45, 45);
    stripMix.setBounds(943, 83, 41, 41);
    stripOutputGain.setBounds(810, 93, 77, 77);
    stripSidechain.setBounds(55, 100, 20, 20);
    
    // Low
    stripLowThreshold.setBounds(96, 271, 117, 117);
    stripLowSwitch.setBounds(211, 437, 48, 48);
    stripLowMakeupGain.setBounds(26, 408, 69, 69);
    stripLowSaturation.setBounds(142, 509, 20, 20);

    // Mid
    stripMidThreshold.setBounds(451, 321, 117, 117);
    stripMidSwitch.setBounds(625, 437, 48, 48);
    stripMidMakeupGain.setBounds(344, 406, 69, 69);
    stripMidSaturation.setBounds(501, 505, 20, 20);
    
    // High
    stripHighThreshold.setBounds(816, 276, 117, 117);
    stripHighSwitch.setBounds(928, 437, 48, 48);
    stripHighMakeupGain.setBounds(746, 409, 69, 69);
    stripHighSaturation.setBounds(861, 509, 20, 20);
    
    // Crossovers
    stripLowCrossover.setBounds(324, 323, 44, 44);
    stripHighCrossover.setBounds(653, 323, 44, 44);

    // VU stuff
    VUBackground.setBounds(0, 0, 1024, 756);
    whiteL.setBounds(332, 152, 120, 8);
    blueL.setBounds(327, 152, 117, 7);
    orangeL.setBounds(322, 152, 112, 7);
    whiteR.setBounds(586, 152, 120, 8);
    blueR.setBounds(591, 152, 116, 7);
    orangeR.setBounds(596, 152, 112, 7);
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
