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
        //how the frontend talks to the backend using the Listener class and using a pure virtual class sliderValueChange
        //note this is for envelope sliders
        
        if (slider == &attackSlider){
            
        
            params.attack = attackSlider.getValue();
            
        }
        
        if (slider == &decaySlider)
        {
            params.decay = decaySlider.getValue();
            
        }
        
        if (slider == &sustainSlider)
        {
            params.sustain = sustainSlider.getValue();
            
        }
        
        if (slider == &releaseSlider)
        {
            params.release = releaseSlider.getValue();
            
        }
    }
    
    void resetParameters()
    {
        
        samplesToTarget = 0;
    }
    
    void NoteOnClicked()
    {
        
        
        if (noteOn.isOver())
        {
            mADSRParams.noteOn(); //this is like a gate on
            
        }
        
       
       
    }
    
    void NoteOffClicked()
    {
        
        if (noteOff.isOver())
       {
           mADSRParams.noteOff(); //this is like a gate off
           
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
    
    
    float currentLevel;
    float targetLevel;
    
    float attackTime;
    float decayTime;
    float sustainTime;
    float releaseTime;
    
    
    
    juce::ADSR::Parameters params; // to get access to private struct parameters in ADSR class
    
    int samplesToTarget;
    
    static constexpr auto rampLengthSamples = 128;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
