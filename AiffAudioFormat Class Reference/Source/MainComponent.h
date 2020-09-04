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
        
        void openButtonClicked()
        {
            juce::FileChooser chooser ("Select a Aiff file to play...", {}, "*.aiff");
            
            if (chooser.browseForFileToOpen())
            {
                auto file = chooser.getResult();
    //            juce::FileInputStream fis(file); //creating inputstream
    //            auto* reader = readAiff.createReaderFor (&fis,true);
    //
                DBG("file opened"); //debug
                
                
                if (file.existsAsFile())
                {
                    DBG("file exists");
                    
                    //juce::FileInputStream fis(file); //creating inputstream
                    
                    //juce::FileInputStream fis(file);
                    //juce::AudioFormatReader mAudioFormatReader;
                    //fis.getStatus();//creating inputstream
                    std::unique_ptr<juce::FileInputStream> fis(new juce::FileInputStream (file));
                    //auto* reader = readAiff.createReaderFor (mAudioFormatReader.getFile(&file),true);
                    auto* reader = readAiff.createReaderFor ( fis.release(),true);                    //auto* reader = readAiff.createMemoryMappedReader(&fis);
                    //auto* reader = readAiff.createMemoryMappedReader(file);
                    
//                    fis.~FileInputStream();
                    if (reader != nullptr)
                    {
//                     if (! fis.failedToOpen())
//                     {
//                        DBG("file OpenedOK");
//                        DBG("can handlefile " << (int)readAiff.canHandleFile(file));
//                        
                        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource (reader, true));
                        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                        playButton.setEnabled (true);
                        
                        readerSource.reset(newSource.release());
                       // readerSource = reader;
    //                    readerSource = readAiff.createReaderFor(&fis, true);
                        //playButton.setEnabled (true);
                       // readerSource.reset (reader.release());
                    }
                    
                    //readerSource.reset (newSource.release());
                }
                
            }
        }
        
         void changeListenerCallback (juce::ChangeBroadcaster* source) override
        {
           if (source == &transportSource)
           {
               if (transportSource.isPlaying())
                   changeState (Playing);
               else
                   changeState (Stopped);
           }
        }
        
        void playButtonClicked()
        {
            //DBG("play works");
            changeState(Starting);
            //getNextAudioBlock(juce::AudioSourceChannelInfo());
            
            DBG("play works");
        }
        
        void stopButtonClicked()
        {
            changeState (Stopping);
        }
        
        enum TransportState
        {
            Stopped,
            Starting,
            Playing,
            Stopping
        };
        
        
        void changeState (TransportState newState)
        {
            if (state != newState)
            {
                state = newState;

                switch (state)
                {
                    case Stopped:
                        //stopButton.setEnabled (false);
                        playButton.setEnabled (true);
                        transportSource.setPosition (0.0);
                        break;

                    case Starting:
                        playButton.setEnabled (false);
                        transportSource.start();
                        break;

                    case Playing:
                        //stopButton.setEnabled (true);

                        break;

                    case Stopping:
                        transportSource.stop();
                        break;
                }
            }
        }

        //==============================================================================
        // Your private member variables go here...
        juce::TextButton openButton;
        juce::TextButton playButton;
        juce::TextButton stopButton;
        
        juce::AiffAudioFormat readAiff;
        //juce::AiffAudioFormatReader formatReaderAiff;
        
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        
        juce::AudioTransportSource transportSource;
        
        juce::AudioFormatManager formatManager;
        TransportState state;
        
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
