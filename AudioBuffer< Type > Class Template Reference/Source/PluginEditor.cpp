/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioBufferTypeClassTemplateReferenceAudioProcessorEditor::AudioBufferTypeClassTemplateReferenceAudioProcessorEditor (AudioBufferTypeClassTemplateReferenceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //DelayGUI
    
   // Dry/Wet Slider GUI
    sDryWet.setRange(0.0f, 1.0f);
    sDryWet.setValue(0.5f);
    sDryWet.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 40);
    addAndMakeVisible(&sDryWet);
    
    //Slider dry/wet Listener
    sDryWet.addListener(this);
    
    //Dry/Wet Label GUI
    lDryWet.setText("Dry/Wet", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(&lDryWet);
    
    //===========================================================================================
    //Delay Slider GUI
    sDelay.setRange(0.0f, 5000.0f);
    sDelay.setValue(0.0f);
    sDelay.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 40); //remember to paint slider
    addAndMakeVisible(&sDelay);
    
    //Slider delay Listener
    sDelay.addListener(this);
    
    //Delay Label GUI
    lDelay.setText("Delay", juce::NotificationType::dontSendNotification); //remember to paint label
    addAndMakeVisible(&lDelay);
    
    
}

AudioBufferTypeClassTemplateReferenceAudioProcessorEditor::~AudioBufferTypeClassTemplateReferenceAudioProcessorEditor()
{
}

//==============================================================================
void AudioBufferTypeClassTemplateReferenceAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioBufferTypeClassTemplateReferenceAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    //Dry/Wet Slider & Label
    sDryWet.setBounds(0, 0, 200, 100);
    lDryWet.setBounds(0, 60, 200, 40);
    //==============================================
    
    //Delay Slider & Label
    sDelay.setBounds(0, 200, 200, 100);
    lDelay.setBounds(0, 260, 200, 40);
}


//================================================================================================

void AudioBufferTypeClassTemplateReferenceAudioProcessorEditor::sliderValueChanged(juce::Slider*  slider)
{
    if (slider == &sDryWet)
    {
        audioProcessor.dryWetTime = sDryWet.getValue();
    }
    
   //DBG("dryWetTime" << audioProcessor.dryWetTime);  //note debug mode has to be on. its off by default!
    
    if (slider == &sDelay)
    {
        audioProcessor.delayTime = sDelay.getValue();
    }
    
    DBG("delayTime" << audioProcessor.delayTime);
}
