/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "myLookAndFeel.h"



struct CustomRotarySlider : juce::Slider {
    CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        
    }

    
};
//==============================================================================
/**
*/
class StereoDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    StereoDelayAudioProcessorEditor (StereoDelayAudioProcessor&);
    ~StereoDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StereoDelayAudioProcessor& audioProcessor;

    juce::Image cachedImage_BinaryData_aluminum_jpg_1;
    juce::Image cachedImage_BinaryData_aluminum_jpg_2;

    myLookAndFeelV1 myLookAndFeelV1;
    myLookAndFeelV2 myLookAndFeelV2;
    myLookAndFeelV3 myLookAndFeelV3;
    Slider myKnobSlider1;
    Slider myKnobSlider2;
    Slider myKnobSlider3;

    CustomRotarySlider lDelaySlider,
        rDelaySlider,
        feedbackSlider,
        fbToneSlider,
        fxMixSlider,
        outputSLider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment lDelaySliderAttachment,
        rDelaySliderAttachment,
        feedbackSliderAttachment,
        fbToneSliderAttachment,
        fxMixSliderAttachment,
        outputSLiderAttachment;

    std::vector<juce::Component*> getComps();
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> label2;

    juce::Label leftLabel;
    juce::Label rightLabel;
    juce::Label feedbackLabel;
    juce::Label outputLabel;
    juce::Label toneLabel;
    juce::Label mixLabel;
   
    
  

   
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoDelayAudioProcessorEditor)
};
