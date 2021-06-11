/*
  ==============================================================================

    CustomSlider.h
    Created: 2 Jun 2021 2:25:48pm
    Author:  QuaseUmPc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CLookAndFeel.h"

//==============================================================================
/*
*/
class CustomSlider  : public juce::Slider
{
public:
    CustomSlider();
    ~CustomSlider() override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private:
    CLookAndFeel cLookAndFeel;
    juce::Point<int> mousePosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomSlider)
};
