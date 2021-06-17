/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class OddballtheAlchemystAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    OddballtheAlchemystAudioProcessor();
    ~OddballtheAlchemystAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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

    juce::AudioProcessorValueTreeState apvts;
    float bpm, rateTime, bpmIntv = 0;

private:

    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    juce::dsp::DelayLine<float> delayL{ 50800 };
    juce::dsp::DelayLine<float> delayR{ 50800 };

    juce::dsp::DelayLine<float> tapR{ 50800 };
    float tapOut = 0;

    float rate = 0;
    float lfo = 0;
    float saw = 0;
    float phaser;

    float lfoWave = 0;
    float lastSampleRate;

    float outL;
    float outR;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OddballtheAlchemystAudioProcessor)
};
