#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setAudioChannels(0, 2);
    
    //mADSRParams.setSampleRate(44100.0f);
    targetLevel = 0.125f;

    levelSlider.setRange (0.0, 0.25);
    levelSlider.setValue (targetLevel, juce::dontSendNotification);
    levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    levelSlider.onValueChange = [this]
    {
        targetLevel = (float) levelSlider.getValue();
        samplesToTarget = rampLengthSamples;
    };
    
    levelLabel.setText ("Noise Level", juce::dontSendNotification);

    addAndMakeVisible (&levelSlider);
    addAndMakeVisible (&levelLabel);
    //============================================================
    
    //envelope gate button
    noteOn.setButtonText("On");
//    noteOnOff.setEnabled(true);
    addAndMakeVisible(&noteOn);
    noteOn.onClick = [this] { NoteOnClicked(); };
    
    
    noteOff.setButtonText("Off");
    addAndMakeVisible(&noteOff);
    noteOff.onClick = [this] { NoteOffClicked(); };
    
    //============================================================
    attackTime =  5.0f;
    
    attackSlider.setRange(0.1f, 5.0f);
    attackSlider.setValue(attackTime, juce::dontSendNotification);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
    
    
    attackSlider.addListener(this);
    
    
    attackLabel.setText("Attack", juce::dontSendNotification);
    
    addAndMakeVisible(&attackSlider);
    addAndMakeVisible(&attackLabel);
    
   
    //========================================================
//    decayTargetLevel = 0.1f;
//    decaySlider.setRange(0.1f, 500.0f);
//    decaySlider.setValue(decayTargetLevel, juce::dontSendNotification);
//    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
//
//    decaySlider.onValueChange = [this]
//    {
//        decayTargetLevel = (float) decaySlider.getValue();
//        //samplesToTarget = rampLengthSamples;
//    };
//
//    addAndMakeVisible(&decaySlider);
//
//    //=======================================================
//    sustainTargetLevel = 0.8f;
//    sustainSlider.setRange(0.1f, 0.8f);
//    sustainSlider.setValue(sustainTargetLevel, juce::dontSendNotification);
//    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
//
//    sustainSlider.onValueChange = [this]
//    {
//        sustainTargetLevel = (float) decaySlider.getValue();
//    };
//
//    addAndMakeVisible(&sustainSlider);
//
//    //=======================================================
//    releaseTargetLevel = 0.1f;
//    releaseSlider.setRange(0.1f, 5000.0f);
//    releaseSlider.setValue(releaseTargetLevel);
//    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
//    releaseSlider.onValueChange = [this]
//    {
//        releaseTargetLevel = (float) decaySlider.getValue();
//    };
//
//    addAndMakeVisible(&releaseSlider);
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    mADSRParams.setSampleRate(44100.0f);
    resetParameters();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto numSamplesRemaining = bufferToFill.numSamples;
    auto offset = 0;
    
    mADSRParams.setParameters(params);
    if (samplesToTarget > 0)
    {
        auto levelIncrement = (targetLevel - currentLevel) / (float) samplesToTarget;
//        auto attackIncrement = (attackTargetLevel - attackCurrentLevel) / (float) samplesToTarget;
        //auto decayIncrement = (decay)
        
        auto numSamplesThisTime = juce::jmin (numSamplesRemaining, samplesToTarget);
        
        for (auto sample = 0; sample < numSamplesThisTime; ++sample)
        {
            for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
                bufferToFill.buffer->setSample (channel, sample, random.nextFloat() * mADSRParams.getNextSample());
                
                                                
            
            currentLevel += levelIncrement;
            
            --samplesToTarget;
        }
        
        offset = numSamplesThisTime;
        numSamplesRemaining -= numSamplesThisTime;
        
        if (samplesToTarget == 0)
        {
            currentLevel = targetLevel;
        }
        
        
    }
    
    if (numSamplesRemaining > 0)
    {
        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample + offset);
            for (auto sample = 0; sample < numSamplesRemaining; ++sample)
                *buffer++ = (random.nextFloat()  *  mADSRParams.getNextSample());
        }
    }
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    levelLabel .setBounds (10, 10, 90, 20);
    levelSlider.setBounds (100, 10, getWidth() - 110, 20);
    
    attackLabel.setBounds(0, 100, 40, 20);
    attackSlider.setBounds(0, 120, 40, 40);
    noteOn.setBounds(0, 190, getWidth() - 20, 20);
    noteOff.setBounds(0, 210, getWidth() - 20, 20);
    
}
