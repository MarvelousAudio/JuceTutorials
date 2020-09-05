    #pragma once

    #include <JuceHeader.h>

    //==============================================================================
    /*
        This component lives inside our window, and this is where you should put all
        your controls and content.
    */
    class MainComponent  : public juce::AudioAppComponent,
                           public juce::ChangeListener
                           
                            
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
        
        //===============================================================================
        
    private:
        
        //function:openButtonClicked
        void openButtonClicked()
        {
            juce::FileChooser chooser ("Select a Aiff file to play...", {}, "*.aiff"); //type of file to be used in order to open
            
            if (chooser.browseForFileToOpen()) // opens finder/file explorer
            {
                
                auto file = chooser.getResult(); // get file
   
                if (file.existsAsFile()) // check if file exists
                {
                    
                    std::unique_ptr<juce::FileInputStream> fis(new juce::FileInputStream (file)); //must create fis in heap in order to create reader!
                    
                    auto* reader = readAiff.createReaderFor ( fis.release(),true); // creates a reader for audio file in .aiff format
                    
                    if (reader != nullptr) // if reader is not empty do this
                    {
                       
                        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource (reader, true));
                        
                        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate); //gets source of audio for transport to play
                        
                        playButton.setEnabled (true); //enables playbutton when file is loaded
                        
                        readerSource.reset(newSource.release()); // transfers pointer to readersoucre.
                      
                    }
                }
            }
        }//end:openButtonClick
        
        //===========================================================================================================================
        
        //function:changeListenerCallback
        void changeListenerCallback (juce::ChangeBroadcaster* source) override
        {
           if (source == &transportSource)
           {
               if (transportSource.isPlaying())
                   changeState (Playing);
               else
                   changeState (Stopped);
           }
        }//end:changeListenerCallback
        
        
        //============================================================================================================================
       
        // function:playButtonClicked()
        void playButtonClicked()
        {
            
            changeState(Starting); //when button is pressed it calls changeState to starting to start sample playback.
           
            
           
        }// end:playButtonClicked()
        
        //==============================================================================================================================
        
        // function stopButtonClicked()
        void stopButtonClicked()
        {
            changeState (Stopping);//when button is pressed it calls changeState to stopping, stopping the audio
            
            
        }// end:stopButtonClicked()
        
        //==============================================================================================================================
        
        //enum:TransportState
        enum TransportState
        {
            Stopped,
            Starting, //why enum? well each state is just really a number as we change state the function changeState is able to switch
            Playing,  //between them.
            Stopping
            
        };//end:TransportState
        
        //==============================================================================================================================
        
        
        //function changeState
        void changeState (TransportState newState)
        {
            if (state != newState)
            {
                state = newState;

                switch (state)                                          //switches states. each state has a task to do when called. each
                {                                                       //is from transportstate and are just interger values.
                    case Stopped:
                        stopButton.setEnabled (false);
                        playButton.setEnabled (true);
                        transportSource.setPosition (0.0);
                        break;

                    case Starting:
                        playButton.setEnabled (false);
                        transportSource.start();
                        break;

                    case Playing:
                        stopButton.setEnabled (true);

                        break;

                    case Stopping:
                        transportSource.stop();
                        break;
                }
            }
        }//end:changeState

        //==============================================================================
        // Your private member variables go here...
        juce::TextButton openButton;    //GUI text buttons
        juce::TextButton playButton;
        juce::TextButton stopButton;
        
        juce::AiffAudioFormat readAiff; //What the project is based on!
        
        
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        
        juce::AudioTransportSource transportSource;
        
        juce::AudioFormatManager formatManager;
        TransportState state;
        
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
