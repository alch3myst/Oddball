/*
  ==============================================================================

    CLookAndFeel.h
    Created: 2 Jun 2021 2:24:32pm
    Author:  QuaseUmPc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    
    CLookAndFeel();
    ~CLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    void drawLabel(juce::Graphics& g, juce::Label& label);
private:
    juce::Image knob;
};
