/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class AudioBufferTypeClassTemplateReferenceAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                                   public juce::Slider::Listener
{
public:
    AudioBufferTypeClassTemplateReferenceAudioProcessorEditor (AudioBufferTypeClassTemplateReferenceAudioProcessor&);
    ~AudioBufferTypeClassTemplateReferenceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider*  slider) override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioBufferTypeClassTemplateReferenceAudioProcessor& audioProcessor;
    
    //void sliderValueChanged(juce::Slider*  slider) override;
    
    //DelayGUI
    juce::Slider sDryWet;
    juce::Slider sFeedBack; //sliders
    juce::Slider sDelay;
    
    juce::Label lDryWet;
    juce::Label lFeedBack; //Label
    juce::Label lDelay;
    
//    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetTree; //Tree
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioBufferTypeClassTemplateReferenceAudioProcessorEditor)
};
