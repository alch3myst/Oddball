/*
  ==============================================================================

    CustomSlider.cpp
    Created: 2 Jun 2021 2:25:48pm
    Author:  QuaseUmPc

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomSlider.h"

//==============================================================================
CustomSlider::CustomSlider()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setLookAndFeel(&cLookAndFeel);
}

CustomSlider::~CustomSlider()
{
    setLookAndFeel(nullptr);
}

void CustomSlider::mouseDown(const juce::MouseEvent& e) {
    juce::Slider::mouseDown(e);
    setMouseCursor(juce::MouseCursor::NoCursor);
    mousePosition = juce::Desktop::getMousePosition();
}

void CustomSlider::mouseUp(const juce::MouseEvent& e) {
    juce::Slider::mouseUp(e);
    juce::Desktop::setMousePosition(mousePosition);
    setMouseCursor(juce::MouseCursor::NormalCursor);
}