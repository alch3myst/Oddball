/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomSlider.h"

//==============================================================================
/**
*/
class OddballtheAlchemystAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OddballtheAlchemystAudioProcessorEditor (OddballtheAlchemystAudioProcessor&);
    ~OddballtheAlchemystAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    CustomSlider wowoKnob;
    CustomSlider softnessKnob;
    CustomSlider brrrKnob;
    CustomSlider detuneKnob;
    CustomSlider tapKnob;
    CustomSlider dryWetKnob;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wowoKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> softnessKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> brrrKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> detuneKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tapKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetKnobAttachment;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OddballtheAlchemystAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OddballtheAlchemystAudioProcessorEditor)
};
