/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Base64.h"
#include <ShlObj_core.h>
#include <filesystem>
#include <fstream>
#include <iostream>

bool skipA = false;
//==============================================================================
OddballtheAlchemystAudioProcessor::OddballtheAlchemystAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
    #endif
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    #endif
        ), apvts(*this, nullptr, "Parameters", createParameters())
    #endif
{
    if (activationStatus == false) { activation(); }
}

OddballtheAlchemystAudioProcessor::~OddballtheAlchemystAudioProcessor()
{
}

//==============================================================================
const juce::String OddballtheAlchemystAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OddballtheAlchemystAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OddballtheAlchemystAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OddballtheAlchemystAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OddballtheAlchemystAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OddballtheAlchemystAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OddballtheAlchemystAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OddballtheAlchemystAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OddballtheAlchemystAudioProcessor::getProgramName (int index)
{
    return {};
}

void OddballtheAlchemystAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void OddballtheAlchemystAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OddballtheAlchemystAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
void OddballtheAlchemystAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    delayL.reset();
    delayR.reset();

    delayL.prepare(spec);
    delayR.prepare(spec);

    delayL.setDelay(0.f);
    delayR.setDelay(0.f);

    tapR.reset();
    tapR.prepare(spec);

}

void OddballtheAlchemystAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    juce::AudioPlayHead* playHead = this->getPlayHead();
    if (playHead) {
        playHead->getCurrentPosition(positionInfo);
        if (positionInfo.bpm) bpm = positionInfo.bpm;
    }
    bpmIntv = (60. / 130) * lastSampleRate;

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Parameters
    float softness = apvts.getRawParameterValue("SOFTNESS")->load();
    float wowo = apvts.getRawParameterValue("WOWO")->load();
    float detune = apvts.getRawParameterValue("DETUNE")->load();
    float dryWet = apvts.getRawParameterValue("DRYWET")->load();
    float brr = apvts.getRawParameterValue("BRRR")->load();
    float tap = apvts.getRawParameterValue("TAP")->load();

    rate =  brr/lastSampleRate;
    //rate = brr / getSampleRate();
    
    //float test = apvts.getRawParameterValue("TEST")->load();


    // Get l and r
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);

    float fadeLength = 30.f;
    float fade;

    /*
    **
    Audio processing
    **
    */

    // Tap Delay ammount
    tapR.setDelay(100 * tap);
    float bufferLength = buffer.getNumSamples();

    // Main fx loop
    for (int sample = 0; sample < bufferLength; ++sample)
    {
        // Saw wave lfo
        lfo += rate;
        if (lfo > 2.0) lfo -= 2.0;
        saw = fmodf(lfo - 1, 1.f);

        // final lfo
        lfoWave = std::cosf(saw / softness);
        phaser = (wowo * lfoWave) + (saw * detune);

        // TODO: fix the popping :l
        delayL.pushSample(0, channelDataL[sample]);
        delayR.pushSample(1, channelDataR[sample]);

        outL = delayL.popSample(0, phaser);
        outR = delayR.popSample(1, phaser);
        
        if (sample < fadeLength) fade = sample / fadeLength;
        if (sample > bufferLength - fadeLength) fade = (bufferLength - sample) / fadeLength;

        channelDataL[sample] = (channelDataL[sample] * (1 - dryWet)) + ( ((outL * cosf(saw)) * fade) * dryWet);
        //channelDataL[sample] = (channelDataL[sample] * (1 - dryWet)) + outL * dryWet;

        if (tap != 0.f) {
            // tap delay = easy stereo effect
            tapR.pushSample(1, (channelDataL[sample] * (1 - dryWet)) + (((outR * cosf(saw)) * fade) * dryWet));
            //tapR.pushSample(1, (channelDataL[sample] * (1 - dryWet)) + outR * dryWet);
            tapOut = tapR.popSample(1);
            channelDataR[sample] = tapOut;
        }
        else {
            channelDataR[sample] = (channelDataR[sample] * (1 - dryWet)) + (((outR * cosf(saw)) * fade) * dryWet);
        }
    }
}

// Create parameters here
juce::AudioProcessorValueTreeState::ParameterLayout OddballtheAlchemystAudioProcessor::createParameters() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SOFTNESS", "sofftness", 1.f, 8.f, 2.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("WOWO", "WoWo", 0.f, 4096.f, 4.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BRRR", "Brrr", 0, 128, 10));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DETUNE", "Glitch", -1024.f, 1024.f, 0.f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("TAP", "Tap", 0.f, 16.f, 0.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "Dry/Wet", 0.f, 1.f, 0.5f));
    return { parameters.begin(), parameters.end() };
}

//==============================================================================
bool OddballtheAlchemystAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OddballtheAlchemystAudioProcessor::createEditor()
{
    return new OddballtheAlchemystAudioProcessorEditor (*this);
}

//==============================================================================
void OddballtheAlchemystAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void OddballtheAlchemystAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
        
        // Update stuff here
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OddballtheAlchemystAudioProcessor();
}


void OddballtheAlchemystAudioProcessor::activation() {
    if (skipA) {
        activationStatus = true;
        return;
    }

    // check or create id file
    wchar_t* appDataPath = 0;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, &appDataPath))) {
        // Make the fa folder path
        std::wstringstream faFolder;
        faFolder << appDataPath << "\\FarAway";

        std::wstringstream idFilePath;
        idFilePath << appDataPath << "\\FarAway\\Oddball.id";

        std::wstringstream licenseFilePath;
        licenseFilePath << appDataPath << "\\FarAway\\Oddball.auth";

        // Create folder if not exists
        std::filesystem::create_directory(faFolder.str().c_str());

        // id file
        std::fstream idFile;
        idFile.open(idFilePath.str().c_str(), std::ios::in);

        // Check if id file exists
        if (idFile.is_open()) {
            // Parse id data to the actvationId global variable
            idFile >> activationId;
            idFile.close();
        }
        else {
            // Create a file with the id
            idFile.close();

            char set[20] = "0123456789waveburst";
            std::string salt = "";
            srand(time(NULL));

            for (size_t i = 0; i < 13; i++)
            {
                salt += set[rand() % 20];
            }

            std::string idData = "ob-" + salt;
            activationId = idData;

            std::fstream createIdFile;
            createIdFile.open(idFilePath.str().c_str(), std::ios::out);
            createIdFile << idData;
            createIdFile.close();
        }

        // Load license file and check if is valid
        // id file
        std::fstream licenseFile;
        licenseFile.open(licenseFilePath.str().c_str(), std::ios::in);

        std::string licenceData = "";
        std::string idload = "";

        // Check if id file exists
        if (licenseFile.is_open()) {
            licenseFile >> licenceData;
            licenseFile.close();

            if (licenceData != "") {
                licenceData = base64_decode(licenceData);

                for (size_t i = 0; i < 16; i++)
                {
                    idload += char(int(licenceData[i * 4]) - 7);
                }
            }
        }


        if (activationId != "" && idload != "") {
            if (activationId == idload) {
                activationStatus = true;
                skipA = true;
            }
            else {
                activationStatus = false;
            }
        }
        else {
            activationStatus = false;
        }
    }
    CoTaskMemFree(static_cast<void*>(appDataPath));
}