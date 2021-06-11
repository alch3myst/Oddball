/*
  ==============================================================================

    CLookAndFeel.cpp
    Created: 2 Jun 2021 2:24:32pm
    Author:  QuaseUmPc

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CLookAndFeel.h"

//==============================================================================
CLookAndFeel::CLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    knob = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
}

CLookAndFeel::~CLookAndFeel()
{
}

void CLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    if (knob.isValid()) {
        const double rotation = (slider.getValue() - slider.getMinimum())
            / (slider.getMaximum() - slider.getMinimum());

        const int frames = 30;
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = juce::jmin(width / 1.0f, height / 1.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;

        int imgWidth = knob.getWidth();
        int imgHeight = knob.getHeight() / frames;
        g.drawImage(knob, 0, 0, imgWidth, imgHeight, 0, imgHeight * frameId, imgWidth, imgHeight);
    }
}

void CLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label) {
    g.setColour(juce::Colour(juce::uint8(255), 255, 255, 0.5f));
    g.fillRoundedRectangle(label.getLocalBounds().toFloat(), 3.0f);


    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font(getLabelFont(label));

        g.setColour(juce::Colour(juce::uint8(0), 0, 0, 1.0f));
        g.setFont(font);

        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
            juce::jmax(1, (int)(textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        g.setColour(juce::Colour(juce::uint8(255), 255, 255, 0.1f));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }
}