#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setAudioChannels(0, 2);
    
   
    targetLevel = 0.125f; //initialize target level

    levelSlider.setRange (0.0, 0.25); //set range for levelSlider
    levelSlider.setValue (targetLevel, juce::dontSendNotification); //initializing value for levelSlider
    levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    levelSlider.onValueChange = [this] //levelSlider Listener that talks to GUI
    {
        targetLevel = (float) levelSlider.getValue();
        samplesToTarget = rampLengthSamples;
    };
    
    levelLabel.setText ("Noise Level", juce::dontSendNotification); //sets the text for levelLabel

    addAndMakeVisible (&levelSlider); //makes levelSlider visible to user
    addAndMakeVisible (&levelLabel);  //makes levelSlider visible to user
    //============================================================
    
    //envelope gate button
    noteOn.setButtonText("On");

    addAndMakeVisible(&noteOn);
    noteOn.onClick = [this] { NoteOnClicked(); };
    
    
    noteOff.setButtonText("Off");
    addAndMakeVisible(&noteOff);
    noteOff.onClick = [this] { NoteOffClicked(); };
    
    //============================================================
    
    //attack
    attackTime =  0.1f;  //initializes attackTime
    
    attackSlider.setRange(0.1f, 5.0f);  //set range for attackSlider in seconds
    attackSlider.setValue(attackTime, juce::dontSendNotification);  //initializes value for attackSlider
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
    
    
    attackSlider.addListener(this); // listener for attackSlider GUI talks to backend
    
    
    attackLabel.setText("Attack", juce::dontSendNotification);
    
    
    //makes attackSlider and attackLabel visible to the user
    addAndMakeVisible(&attackSlider);
    addAndMakeVisible(&attackLabel);
    
   
    //========================================================
    
    //decay
    decayTime = 0.1f; //initializes decayTime
    
    decaySlider.setRange(0.1f, 0.5f); //set range for decaySlider in seconds
    decaySlider.setValue(decayTime, juce::dontSendNotification); // initializes decaySlider
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);

    decaySlider.addListener(this); // listener for decaySlider GUI talks to backend
    
    decayLabel.setText("Decay", juce::dontSendNotification); //set text for decayLabel
    
    //makes decaySlider and decayLabel visible to the user
    addAndMakeVisible(&decaySlider);
    addAndMakeVisible(&decayLabel);

    //=======================================================
    
    //sustain
    sustainTime = 0.8f; //initializes sustainTime
    
    
    sustainSlider.setRange(0.1f, 0.8f); // sets range for sustainSlider in seconds
    sustainSlider.setValue(sustainTime, juce::dontSendNotification); // initializes sustainSlider
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
    
    sustainLabel.setText("Sustain", juce::dontSendNotification); //set text for decayLabel
    
    sustainSlider.addListener(this); // listener for decaySlider GUI talks to backend
    
    //makes sustainSlider and sustainLabel visible to user
    addAndMakeVisible(&sustainSlider);
    addAndMakeVisible(&sustainLabel);
    
    //=======================================================
    
    //release
    releaseTime = 0.1f; //initializes releaseTime
    
    
    releaseSlider.setRange(0.1f, 5.0f); //set range for releaseSlider in seconds
    releaseSlider.setValue(releaseTime, juce::dontSendNotification); //initializes value for releaseSlider
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20.0, 10.0);
    
    releaseLabel.setText("Release", juce::dontSendNotification); //sets text to releaseLabel
    
    releaseSlider.addListener(this); // Listener for releaseSlider GUI talks to backend
    
    //makes releaseSlider and releaseLabel visible to user
    addAndMakeVisible(&releaseSlider);
    addAndMakeVisible(&releaseLabel);
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    
    mADSRParams.setSampleRate(44100.0f); //sets samples rate for envelope
    resetParameters(); //call to resetParameters wich initializes samplesToTarget
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto numSamplesRemaining = bufferToFill.numSamples;
    auto offset = 0;
    
    mADSRParams.setParameters(params);
    if (samplesToTarget > 0)
    {
        
        auto levelIncrement = (targetLevel - currentLevel) / (float) samplesToTarget;
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
    
    //paints object to GUI
    levelLabel .setBounds (10, 10, 90, 20);
    levelSlider.setBounds (100, 10, getWidth() - 110, 20);
    
    attackLabel.setBounds(0, 100, 40, 20);
    attackSlider.setBounds(0, 120, 40, 40);
    
    decayLabel.setBounds(60, 100, 40, 20);
    decaySlider.setBounds(60, 120, 40, 40);
    
    sustainLabel.setBounds(120, 100, 100, 20);
    sustainSlider.setBounds(120, 120, 40, 40);
    
    releaseLabel.setBounds(180, 100, 100, 20);
    releaseSlider.setBounds(180, 120, 40, 40);
    
    noteOn.setBounds(0, 190, getWidth() - 20, 20);
    noteOff.setBounds(0, 210, getWidth() - 20, 20);
    
}
