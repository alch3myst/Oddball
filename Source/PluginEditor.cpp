/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OddballtheAlchemystAudioProcessorEditor::OddballtheAlchemystAudioProcessorEditor (OddballtheAlchemystAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    wowoKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WOWO", wowoKnob);
    softnessKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SOFTNESS", softnessKnob);
    brrrKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BRRR", brrrKnob);
    detuneKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DETUNE", detuneKnob);
    tapKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TAP", tapKnob);
    dryWetKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRYWET", dryWetKnob);     

    wowoKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    wowoKnob.setTextValueSuffix("\nWoWo");
    addAndMakeVisible(wowoKnob);

    softnessKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    softnessKnob.setTextValueSuffix("\nSoftness");

    brrrKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    brrrKnob.setTextValueSuffix("\nBRRR");

    detuneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    detuneKnob.setTextValueSuffix("\nGlitch");
     
    tapKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    tapKnob.setTextValueSuffix("\nTap");

    dryWetKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dryWetKnob.setTextValueSuffix("\nDry/Wet");


    wowoKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 31);
    softnessKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 31);
    brrrKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 31);
    detuneKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 31);
    tapKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 31);
    dryWetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 31);
    
    addAndMakeVisible(softnessKnob);
    addAndMakeVisible(brrrKnob);
    addAndMakeVisible(detuneKnob);
    addAndMakeVisible(tapKnob);
    addAndMakeVisible(dryWetKnob);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (370, 490);
    setResizable(false, false);
}

OddballtheAlchemystAudioProcessorEditor::~OddballtheAlchemystAudioProcessorEditor()
{
}

//==============================================================================
void OddballtheAlchemystAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.drawImage(juce::ImageCache::getFromMemory(BinaryData::bg_jpg, BinaryData::bg_jpgSize), 0,0, getWidth(), getHeight(),0,0,960,1280);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText(juce::String(audioProcessor.bpm), getLocalBounds(), juce::Justification::centred, 1);

}

void OddballtheAlchemystAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    wowoKnob.setBounds(getWidth() * 0.03, getHeight() * 0.07, 100, 120);
    softnessKnob.setBounds(getWidth() * 0.04, getHeight() * 0.32, 100, 120);
    brrrKnob.setBounds(getWidth() * 0.07, getHeight() * 0.57, 100, 120);

    detuneKnob.setBounds((getWidth() * 0.43) - 25, getHeight() * 0.22, 100, 120);
    tapKnob.setBounds((getWidth() * 0.5) - 25, getHeight() * 0.61, 100, 120);

    dryWetKnob.setBounds(getWidth() * 0.8 - 25, getHeight() * 0.5 -60, 100, 120);
}
