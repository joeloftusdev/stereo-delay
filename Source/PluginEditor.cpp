/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoDelayAudioProcessorEditor::StereoDelayAudioProcessorEditor(StereoDelayAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
   lDelaySliderAttachment(audioProcessor.apvts, "L Delay", lDelaySlider),
    rDelaySliderAttachment(audioProcessor.apvts, "R Delay", rDelaySlider),
    feedbackSliderAttachment(audioProcessor.apvts, "Feedback", feedbackSlider),
    fbToneSliderAttachment(audioProcessor.apvts, "Fb Tone", fbToneSlider),
    fxMixSliderAttachment(audioProcessor.apvts, "FX Mix", fxMixSlider),
    outputSLiderAttachment(audioProcessor.apvts, "Output", outputSLider)

{

   
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    for (auto* comp : getComps())
    {
        addAndMakeVisible(comp);
       
    }

    label.reset(new Label("new label",
        TRANS("STEREO DELAY\n")));
    addAndMakeVisible(label.get());
    label->setFont(Font("Bahnschrift", 23.3f, Font::plain).withTypefaceStyle("Bold").withExtraKerningFactor(0.1f));
    label->setJustificationType(Justification::centred);
    label->setEditable(false, false, false);
    label->setColour(Label::textColourId, Colours::black);
    label->setColour(TextEditor::textColourId, Colours::black);
    label->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    label2.reset(new Label("new label",
        TRANS("joeloftus.net")));
    addAndMakeVisible(label2.get());
    label2->setFont(Font("Bahnschrift", 12.0f, Font::plain).withTypefaceStyle("Bold"));
    label2->setJustificationType(Justification::centred);
    label2->setEditable(false, false, false);
    label2->setColour(Label::textColourId, Colours::black);
    label2->setColour(TextEditor::textColourId, Colours::black);
    label2->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    addAndMakeVisible(leftLabel);
    leftLabel.setText("Left Delay", juce::dontSendNotification);
    leftLabel.attachToComponent(&lDelaySlider, false);
    leftLabel.setJustificationType(juce::Justification::centred);
    leftLabel.setFont(juce::Font("Bahnschrift", 15.0f, juce::Font::plain));


    addAndMakeVisible(rightLabel);
    rightLabel.setText("Right Delay", juce::dontSendNotification);
    rightLabel.attachToComponent(&rDelaySlider, false);
    rightLabel.setJustificationType(juce::Justification::centred);
    rightLabel.setFont(juce::Font("Bahnschrift", 15.0f, juce::Font::plain));

    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setFont(juce::Font("Bahnschrift", 13.0f, juce::Font::plain));

    addAndMakeVisible(outputLabel);
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.attachToComponent(&outputSLider, false);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setFont(juce::Font("Bahnschrift", 13.0f, juce::Font::plain));

    addAndMakeVisible(toneLabel);
    toneLabel.setText("Tone", juce::dontSendNotification);
    toneLabel.attachToComponent(&fbToneSlider, false);
    toneLabel.setJustificationType(juce::Justification::centred);
    toneLabel.setFont(juce::Font("Bahnschrift", 13.0f, juce::Font::plain));

    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&fxMixSlider, false);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setFont(juce::Font("Bahnschrift", 13.0f, juce::Font::plain));




    lDelaySlider.setLookAndFeel(&myLookAndFeelV1);
    rDelaySlider.setLookAndFeel(&myLookAndFeelV1);
    feedbackSlider.setLookAndFeel(&myLookAndFeelV1);
    fbToneSlider.setLookAndFeel(&myLookAndFeelV1);
    fxMixSlider.setLookAndFeel(&myLookAndFeelV1);
    outputSLider.setLookAndFeel(&myLookAndFeelV1);

    

    
  
    cachedImage_BinaryData_aluminum_jpg_1 = juce::ImageCache::getFromMemory(BinaryData::aluminum_jpg, BinaryData::aluminum_jpgSize);
    cachedImage_BinaryData_aluminum_jpg_2 = juce::ImageCache::getFromMemory(BinaryData::aluminum_jpg, BinaryData::aluminum_jpgSize);

   
   


    setSize (800, 356);
}

StereoDelayAudioProcessorEditor::~StereoDelayAudioProcessorEditor()
{
}

//==============================================================================
void StereoDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff323e44));

    g.drawText("Search Path: " + File::getSpecialLocation(
        File::SpecialLocationType::userDesktopDirectory)
        .getChildFile("knob2.png").getFullPathName(),
        getLocalBounds().removeFromBottom(100), Justification::centred, true);

    {
        int x = 0, y = 0, width = proportionOfWidth(1.0f), height = proportionOfHeight(0.15f);
      
        g.setTiledImageFill(cachedImage_BinaryData_aluminum_jpg_1,
            0 - 0 + x,
            0 - 0 + y,
            1.0f);
        g.fillRect(x, y, width, height);
        g.fillRect(x, y, width, height);
    }

    {
        int x = 0, y = proportionOfHeight(0.5f) - (proportionOfHeight(0.6996f) / 2), width = proportionOfWidth(1.0f), height = proportionOfHeight(0.6996f);
        juce::Colour fillColour = juce::Colour(0xff181818);
      
        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
    }

    {
        int x = 0, y = proportionOfHeight(0.85f), width = proportionOfWidth(1.0f), height = proportionOfHeight(0.15f);
        g.setTiledImageFill(cachedImage_BinaryData_aluminum_jpg_2,
            0 - 0 + x,
            0 - proportionOfHeight(0.85f) + y,
            1.0f);
     
        g.fillRect(x, y, width, height);
    }

    {
        int x = 0, y = proportionOfHeight(0.1145f), width = proportionOfWidth(1.0f), height = proportionOfHeight(0.02f);
        juce::Colour fillColour = juce::Colour(0xff000f11);
    
        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.25f)), y = static_cast<float> (proportionOfHeight(0.5f) - (proportionOfHeight(0.66f) / 2)), width = static_cast<float> (proportionOfWidth(0.3f)), height = static_cast<float> (proportionOfHeight(0.66f));
        juce::Colour fillColour = juce::Colour(0xff3e4b32);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.0f);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.01f)), y = static_cast<float> (proportionOfHeight(0.5f) - (proportionOfHeight(0.66f) / 2)), width = static_cast<float> (proportionOfWidth(0.3f)), height = static_cast<float> (proportionOfHeight(0.66f));
        juce::Colour fillColour = juce::Colour(0xff3e4b32);

        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.0f);
    }

    {
        float x = static_cast<float> (proportionOfWidth(0.57f)), y = static_cast<float> (proportionOfHeight(0.5f) - (proportionOfHeight(0.66f) / 2)), width = static_cast<float> (proportionOfWidth(0.42f)), height = static_cast<float> (proportionOfHeight(0.66f));
        juce::Colour fillColour = juce::Colour(0xff3e4b32);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.0f);
    }


   

   
}

void StereoDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

   // auto bounds = getLocalBounds();
    //auto topArea = bounds.removeFromTop(bounds.getHeight() * 0.33);
    //auto leftArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    //auto rightArea = bounds.removeFromRight(bounds.getWidth() * 0.5);

    //lDelaySlider.setBounds(proportionOfWidth(0.25f), proportionOfHeight(0.5207f) - (proportionOfHeight(0.598f) / 2), proportionOfWidth(0.2297f), proportionOfHeight(0.548f));
    feedbackSlider.setBounds(proportionOfWidth(0.5689f), proportionOfHeight(0.4f) - (proportionOfHeight(0.3319f) / 2), proportionOfWidth(0.2095f), proportionOfHeight(0.26f));
    fxMixSlider.setBounds(proportionOfWidth(0.7811f), proportionOfHeight(0.4f) - (proportionOfHeight(0.3319f) / 2), proportionOfWidth(0.2095f), proportionOfHeight(0.26f));
    fbToneSlider.setBounds(proportionOfWidth(0.5689f), proportionOfHeight(0.72f) - (proportionOfHeight(0.3319f) / 2), proportionOfWidth(0.2095f), proportionOfHeight(0.26f));
    outputSLider.setBounds(proportionOfWidth(0.7811f), proportionOfHeight(0.72f) - (proportionOfHeight(0.3319f) / 2), proportionOfWidth(0.2095f), proportionOfHeight(0.26f));
    lDelaySlider.setBounds(proportionOfWidth(0.1257f) - (proportionOfWidth(0.2297f) / 2), proportionOfHeight(0.4986f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2297f), proportionOfHeight(0.402f));
    rDelaySlider.setBounds(proportionOfWidth(0.40f) - (proportionOfWidth(0.2297f) / 2), proportionOfHeight(0.4986f) - (proportionOfHeight(0.402f) / 2), proportionOfWidth(0.2297f), proportionOfHeight(0.402f));
    // lDelaySlider.setBounds(leftArea);
    //rDelaySlider.setBounds(rightArea);outputSLider
    //outputSLider.setBounds(leftArea.removeFromTop(bounds.getHeight() * 0.33));

  //  feedbackSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.33));
   // fbToneSlider.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    //fxMixSlider.setBounds(bounds);

    myKnobSlider1.setBoundsRelative(0.15, 0.2, 0.3, 0.3);
    myKnobSlider2.setBoundsRelative(0.35, 0.2, 0.3, 0.3);
    myKnobSlider3.setBoundsRelative(0.55, 0.2, 0.3, 0.3);

    label->setBounds(proportionOfWidth(0.0108f), proportionOfHeight(0.0443f), 250, 24);
    label2->setBounds(proportionOfWidth(0.877f), proportionOfHeight(0.0615f), 60, 24);

}

std::vector<juce::Component*> StereoDelayAudioProcessorEditor::getComps()
{
    return{
        &lDelaySlider,
        &rDelaySlider,
        &feedbackSlider,
        &fbToneSlider,
        &fxMixSlider,
        &outputSLider,
       
    };
}
