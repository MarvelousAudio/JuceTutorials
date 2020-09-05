    #include "MainComponent.h"

    //==============================================================================
    //function: constructor
    MainComponent::MainComponent(): state (Stopped)
    {
        // Make sure you set the size of the component after
        // you add any child components.
        setSize (800, 600);

        setAudioChannels(0, 2);
        //openButtonGUI
        openButton.setButtonText("Open...");
        openButton.onClick = [this] { openButtonClicked(); };
        addAndMakeVisible(&openButton);
        
        
        //playButtonGUI
        playButton.setButtonText("Play");
        playButton.onClick = [this] { playButtonClicked(); };
        addAndMakeVisible(&playButton);
        playButton.setEnabled(false);
        
        //stopButtonGUI
        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.onClick = [this] { stopButtonClicked(); };
        stopButton.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled (false);

        
        transportSource.addChangeListener(this); // call listener to change state
    }//end:constructor

    //==============================================================================
    
    //function:decstructor
    MainComponent::~MainComponent()
    {
        // This shuts down the audio device and clears the audio source.
        shutdownAudio();
    }//end:destructor

    //==============================================================================

    //function:prepareToPlay
    void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        
        
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate); //gets sample info
        transportSource.setPosition(0.0); //set transport position
    }//end:prepareToPlay

    //========================================================================================

    //function:getNextAudioBlock
    void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
    {
        if (readerSource.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        
       
        transportSource.getNextAudioBlock(bufferToFill); // how the audio is being played using the buffer
        
    }//end:getNextAudioBlock

    //====================================================================================================
    

    //function:releaseResources
    void MainComponent::releaseResources()
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.
        transportSource.releaseResources();
        // For more details, see the help for AudioProcessor::releaseResources()
        
    }//end:releaseResources

    //==============================================================================

    //function:paint
    void MainComponent::paint (juce::Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }//end:paint
    

    //=============================================================================

    //function:resized()
    void MainComponent::resized()
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        openButton          .setBounds (10, 10,  getWidth() - 20, 20); //paint buttons to GUI
        playButton          .setBounds (10, 40,  getWidth() - 20, 20);
        stopButton          .setBounds (10, 70,  getWidth() - 20, 20);
    }//end:resized()
    
