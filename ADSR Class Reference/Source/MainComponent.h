#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
public juce::Slider::Listener

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    
    
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &attackSlider){
            
        
            params.attack = attackSlider.getValue();
            DBG("attackTime: " << params.attack);
        }
    }
    
    void resetParameters()
    {
        
        samplesToTarget = 0;
    }
    
    void NoteOnClicked()
    {
        DBG("toggle value: " << (int)noteOn.isOver());
        
        if (noteOn.isOver())
        {
            mADSRParams.noteOn();
            //DBG("toggle value" << noteOnOff.getToggleState());
        }
        
       
       
    }
    
    void NoteOffClicked()
    {
        DBG("toggle value: " << (int)noteOff.isOver());
        if (noteOff.isOver())
       {
           mADSRParams.noteOff();
           //DBG("toggle value" << noteOnOff.getToggleState());
       }
    }
   

    
private:
    //==============================================================================
    // Your private member variables go here...
    juce::Random random;
    juce::ADSR mADSRParams;
    
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    juce::Slider levelSlider;
    
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    juce::Label levelLabel;
    
    juce::TextButton noteOn;
    juce::TextButton noteOff;
    int buttonState;
    
    float currentLevel;
    float targetLevel;
    
    float attackTime;
    juce::ScopedPointer <juce::AudioProcessorValueTreeState::SliderAttachment> attackTree;
    
    juce::ADSR::Parameters params;
    
    int samplesToTarget;
    
    static constexpr auto rampLengthSamples = 128;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
