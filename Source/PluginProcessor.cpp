/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MugiDynamics4216AudioProcessor::MugiDynamics4216AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Filters for processing
    mLowLPF.resize(2);
    mLowHPF.resize(2);
    mLowHPFclean.resize(2);
    mHighLPF.resize(2);
    mHighHPF.resize(2);
    mAPF_a.resize(2);
    mAPF_b.resize(2);
    mSC.resize(2);
    // Filters for dry signal only
    mLowLPFDry.resize(2);
    mLowHPFDry.resize(2);
    mHighLPFDry.resize(2);
    mHighHPFDry.resize(2);
    mAPF_aDry.resize(2);
    mAPF_bDry.resize(2);
    // Compressors
    mCompLow.resize(2);
    mCompMid.resize(2);
    mCompHigh.resize(2);
    // Envelopes
    mEnvGR.resize(2);
    mEnvLimitLow.resize(2);
    mEnvLimitMid.resize(2);
    mEnvLimitHigh.resize(2);
    mDetectorLow.resize(2);
    mDetectorMid.resize(2);
    mDetectorHigh.resize(2);
    // Saturators
    mSatLow.resize(2);
    mSatMid.resize(2);
    mSatHigh.resize(2);
}

MugiDynamics4216AudioProcessor::~MugiDynamics4216AudioProcessor()
{
}

//==============================================================================
const juce::String MugiDynamics4216AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MugiDynamics4216AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MugiDynamics4216AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MugiDynamics4216AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MugiDynamics4216AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MugiDynamics4216AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MugiDynamics4216AudioProcessor::getCurrentProgram()
{
    return 0;
}

void MugiDynamics4216AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MugiDynamics4216AudioProcessor::getProgramName (int index)
{
    return {};
}

void MugiDynamics4216AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MugiDynamics4216AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mGRforVULowLeft = 0.0f;
    mGRforVUMidLeft = 0.0f;
    mGRforVUHighLeft = 0.0f;
    mGRforVULowRight = 0.0f;
    mGRforVUMidRight = 0.0f;
    mGRforVUHighRight = 0.0f;
}

void MugiDynamics4216AudioProcessor::releaseResources()
{ 
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MugiDynamics4216AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MugiDynamics4216AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // COMP SETTINGS
    double Fs = getSampleRate();
    double Ts = 1 / Fs;

    const float limiter_at = 0.000001;
    const float limiter_rt = 0.1;
    
    // GET PARAM VALUES
    // Master
    float inputGaindB = *parameters.getRawParameterValue("inputgain");
    float inputGainRaw = juce::Decibels::decibelsToGain(inputGaindB);
    float outputGaindB = *parameters.getRawParameterValue("outputgain");
    float outputGainRaw = juce::Decibels::decibelsToGain(outputGaindB);
    int bypass = *parameters.getRawParameterValue("bypass");
    float dryWetMix = *parameters.getRawParameterValue("mix");
    float sidechainFreq = *parameters.getRawParameterValue("sidechain");
    // Low
    float lowThreshold = *parameters.getRawParameterValue("lowthreshold");
    int lowSwitch = *parameters.getRawParameterValue("lowswitch");
    float lowMakeupGaindB = *parameters.getRawParameterValue("lowmakeupgain");
    float lowMakeupGainRaw =juce::Decibels::decibelsToGain(lowMakeupGaindB);
    float lowSaturation = *parameters.getRawParameterValue("lowsaturation");
    // Mid
    float midThreshold = *parameters.getRawParameterValue("midthreshold");
    int midSwitch = *parameters.getRawParameterValue("midswitch");
    float midMakeupGaindB = *parameters.getRawParameterValue("midmakeupgain");
    float midMakeupGainRaw =juce::Decibels::decibelsToGain(midMakeupGaindB);
    float midSaturation = *parameters.getRawParameterValue("midsaturation");
    // High
    float highThreshold = *parameters.getRawParameterValue("highthreshold");
    int highSwitch = *parameters.getRawParameterValue("highswitch");
    float highMakeupGaindB = *parameters.getRawParameterValue("highmakeupgain");
    float highMakeupGainRaw =juce::Decibels::decibelsToGain(highMakeupGaindB);
    float highSaturation = *parameters.getRawParameterValue("highsaturation");
    // Crossovers
    float lowCrossover = *parameters.getRawParameterValue("lowcrossover");
    float highCrossover = *parameters.getRawParameterValue("highcrossover");
    
    //=================================================================================
    //===PROCESSING====================================================================
    //=================================================================================
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        mLowLPF[channel].updateCoefs(lowCrossover, Fs);
        mLowHPF[channel].updateCoefs(lowCrossover, Fs);
        mLowHPFclean[channel].updateCoefs(lowCrossover, Fs);
        mHighLPF[channel].updateCoefs(highCrossover, Fs);
        mHighHPF[channel].updateCoefs(highCrossover, Fs);
        mAPF_a[channel].updateCoefs(highCrossover, Fs);
        mAPF_b[channel].updateCoefs(highCrossover, Fs);
        
        mLowLPFDry[channel].updateCoefs(lowCrossover, Fs);
        mLowHPFDry[channel].updateCoefs(lowCrossover, Fs);
        mHighLPFDry[channel].updateCoefs(highCrossover, Fs);
        mHighHPFDry[channel].updateCoefs(highCrossover, Fs);
        mAPF_aDry[channel].updateCoefs(highCrossover, Fs);
        mAPF_bDry[channel].updateCoefs(highCrossover, Fs);
        
        mSC[channel].updateCoefs(sidechainFreq, Fs);
       
        auto* input = buffer.getReadPointer (channel);
        auto* output = buffer.getWritePointer (channel);
        
        for (auto sample = 0; sample < buffer.getNumSamples(); sample++){
  
            // Get input
            float in = input[sample] * inputGainRaw;
            float inDry = input[sample];

            // Split signal for saturated signals (high band sat comes later)
            float inLow = mSatLow[channel].process(in, lowSaturation);
            float inMid = mSatMid[channel].process(in, midSaturation);
            float inMidClean = in;
            
            // Crossover filtering for saturated signals
            float lowTemp1 = mLowLPF[channel].process(inLow);
            float lowTemp2 = mAPF_a[channel].process(lowTemp1);
            float lowBand = mAPF_b[channel].process(lowTemp2);
            float lowBandSC = mSC[channel].process(lowBand);
            
            float midTemp = mLowHPF[channel].process(inMid);
            float midTempClean = mLowHPFclean[channel].process(inMidClean);
            float midBand = mHighLPF[channel].process(midTemp);
            float highBandClean = mHighHPF[channel].process(midTempClean);
            float highBand = mSatHigh[channel].process(highBandClean, highSaturation);
            
            // Crossover filtering for dry signals (preserves phase for wet/dry)
            float lowTemp1Dry = mLowLPFDry[channel].process(inDry);
            float lowTemp2Dry = mAPF_aDry[channel].process(lowTemp1Dry);
            float lowBandDry = mAPF_bDry[channel].process(lowTemp2Dry);
            float midTempDry = mLowHPFDry[channel].process(inDry);
            float midBandDry = mHighLPFDry[channel].process(midTempDry);
            float highBandDry = mHighHPFDry[channel].process(midTempDry);
            
            // Get downwards envelopes
            float envLow = mDetectorLow[channel].detect(lowBandSC, Ts);
            float envMid = mDetectorLow[channel].detect(midBand, Ts);
            float envHigh = mDetectorLow[channel].detect(highBand, Ts);
            
            // Get upwards envelopes
            float envLimitLow = mEnvLimitLow[channel].processPeak(lowBandSC, limiter_at, limiter_rt, Ts);
            float envLimitMid = mEnvLimitMid[channel].processPeak(midBand, limiter_at, limiter_rt, Ts);
            float envLimitHigh = mEnvLimitHigh[channel].processPeak(highBand, limiter_at, limiter_rt, Ts);

            // Compress bands
            float outLow = mCompLow[channel].computeGain(lowSwitch, lowBand, lowThreshold, envLow, envLimitLow, lowMakeupGainRaw, Ts);
            float outMid = mCompMid[channel].computeGain(midSwitch, midBand, midThreshold, envMid, envLimitMid, midMakeupGainRaw, Ts);
            float outHigh = mCompHigh[channel].computeGain(highSwitch, highBand, highThreshold, envHigh, envLimitHigh, highMakeupGainRaw, Ts);
            
            // Mixing
            if (bypass == 1){
                float wetOutput = (outLow - (outMid - outHigh)) * outputGainRaw;
                float dryOutput = lowBandDry - (midBandDry - highBandDry);
                output[sample] = dryWetMix * wetOutput + (1 - dryWetMix) * dryOutput;
            }
            else {
                output[sample] = input[sample];
            }

            // Get GR data for VU meters
            mGRforVULowLeft = mCompLow[0].gainReductionForVU;
            mGRforVULowRight = mCompLow[1].gainReductionForVU;
            mGRforVUMidLeft = mCompMid[0].gainReductionForVU;
            mGRforVUMidRight = mCompMid[1].gainReductionForVU;
            mGRforVUHighLeft = mCompHigh[0].gainReductionForVU;
            mGRforVUHighRight = mCompHigh[1].gainReductionForVU;
        }
    }
}

//==============================================================================
bool MugiDynamics4216AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MugiDynamics4216AudioProcessor::createEditor()
{
    return new MugiDynamics4216AudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void MugiDynamics4216AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MugiDynamics4216AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
float MugiDynamics4216AudioProcessor::getVuValue(const int channel, int band) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0){
        if (band == 1) return mGRforVULowLeft;
        else if (band == 2) return mGRforVUMidLeft;
        else return mGRforVUHighLeft;
    }
    if (channel == 1){
        if (band == 1) return mGRforVULowRight;
        else if (band == 2) return mGRforVUMidRight;
        else return mGRforVUHighRight;
    }
    return 0.f;
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout MugiDynamics4216AudioProcessor::createParameterLayout(){

    // SKEW FACTORS FOR LOG PARAMS
    
    // Low crossover
    const float fMinLow = 100.0f;
    const float fMaxLow = 1000.f;
    const float fAtMidPointLow = 500.0f; // Effectively disabled here
    const float skewFactorLow = log2(0.5) / log2((fAtMidPointLow / fMinLow) / (fMaxLow / fMinLow));
    
    // High crossover
    const float fMinHigh = 2000.0f;
    const float fMaxHigh = 12000.f;
    const float fAtMidPointHigh = 7000.0f; // Effectively disabled here
    const float skewFactorHigh = log2(0.5) / log2((fAtMidPointHigh / fMinHigh) / (fMaxHigh / fMinHigh));

    
    return {
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("inputgain", 1),
                                                     "Input Gain",   // parameter name
                                                     -8.0f,           // minimum val
                                                     8.0f,           // maximum val
                                                     +0.0f),        // default value
        std::make_unique<juce::AudioParameterInt> (
                                                     juce::ParameterID("bypass", 1),
                                                     "Bypass",
                                                     0,
                                                     1,
                                                     1),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("mix", 1),
                                                     "Mix",
                                                     0.0f,
                                                     1.0f,
                                                     1.0f),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("outputgain", 1),
                                                     "Output Gain",
                                                     -8.0f,
                                                     8.0f,
                                                     +0.0f),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("sidechain", 1),
                                                     "Sidechain HPF",
                                                     5.0f,
                                                     100.0f,
                                                     5.0f),
        // Low
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("lowthreshold", 1),
                                                     "Low Threshold",
                                                     -24.0f,
                                                     0.0f,
                                                     0.0f),

        std::make_unique<juce::AudioParameterInt> (
                                                     juce::ParameterID("lowswitch", 1),
                                                     "Low Switch",
                                                     -1,
                                                     1,
                                                     1),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("lowmakeupgain", 1),
                                                     "Low Makeup Gain",
                                                     -8.0f,
                                                     8.0f,
                                                     +0.0f),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("lowsaturation", 1),
                                                     "Low Saturation",
                                                     0.8f,
                                                     5.0f,
                                                     0.8f),
        // Mid
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("midthreshold", 1),
                                                     "Mid Threshold",
                                                     -24.0f,
                                                     0.0f,
                                                     0.0f),

        std::make_unique<juce::AudioParameterInt> (
                                                     juce::ParameterID("midswitch", 1),
                                                     "Mid Switch",
                                                     -1,
                                                     1,
                                                     1),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("midmakeupgain", 1),
                                                     "Mid Makeup Gain",
                                                     -8.0f,
                                                     8.0f,
                                                     +0.0f),
        std::make_unique<juce::AudioParameterFloat> ( 
                                                     juce::ParameterID("midsaturation", 1),
                                                     "Mid Saturation",
                                                     0.8f,
                                                     5.0f,
                                                     0.8f),
        // High
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("highthreshold", 1),
                                                     "High Threshold",
                                                     -24.0f,
                                                     0.0f,
                                                     0.0f),

        std::make_unique<juce::AudioParameterInt> (
                                                     juce::ParameterID("highswitch", 1),
                                                     "High Switch",
                                                     -1,
                                                     1,
                                                     1),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("highmakeupgain", 1),
                                                     "High Makeup Gain",
                                                     -8.0f,
                                                     8.0f,
                                                     +0.0f),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("highsaturation", 1),
                                                     "High Saturation",
                                                     0.8f,
                                                     5.0f,
                                                     0.8f),
        // Crossovers
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("lowcrossover", 1),
                                                     "Low Crossover",
                                                     juce::NormalisableRange{fMinLow,
                                                                             fMaxLow,
                                                                             0.0f,
                                                                             skewFactorLow,
                                                                             false},
                                                     500.0f),
        std::make_unique<juce::AudioParameterFloat> (
                                                     juce::ParameterID("highcrossover", 1),
                                                     "High Crossover",
                                                     juce::NormalisableRange{fMinHigh,
                                                                             fMaxHigh,
                                                                             0.0f,
                                                                             skewFactorHigh,
                                                                             false},
                                                     7000.0f)
    };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MugiDynamics4216AudioProcessor();
}
