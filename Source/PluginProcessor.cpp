#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoDelayAudioProcessor::StereoDelayAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

StereoDelayAudioProcessor::~StereoDelayAudioProcessor()
{
}

const juce::String StereoDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int StereoDelayAudioProcessor::getNumPrograms()
{
    return 1;
}

int StereoDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoDelayAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String StereoDelayAudioProcessor::getProgramName(int index)
{
    return {};
}

void StereoDelayAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void StereoDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    _delayMax = int(std::ceil(float(sampleRate) * _delayMaxMsec / 1000.0f));
    _delayBuffer.resize(_delayMax);

    resetState();
}

void StereoDelayAudioProcessor::releaseResources()
{
}

void StereoDelayAudioProcessor::reset()
{
    resetState();
}

bool StereoDelayAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void StereoDelayAudioProcessor::resetState()
{
    _pos = 0;
    _filt0 = 0.0f;

    memset(_delayBuffer.data(), 0, _delayMax * sizeof(float));
}

void StereoDelayAudioProcessor::update()
{
    const float samplesPerMsec = float(getSampleRate()) / 1000.0f;


    float ldelParam = apvts.getRawParameterValue("L Delay")->load();
    _ldel = int(ldelParam * samplesPerMsec);
    if (_ldel < 4) _ldel = 4;
    if (_ldel > _delayMax) _ldel = _delayMax;

    float rdelParam = apvts.getRawParameterValue("R Delay")->load();
    _rdel = int(ldelParam * samplesPerMsec * rightDelayRatio(rdelParam));
    if (_rdel > _delayMax) _rdel = _delayMax;
    if (_rdel < 4) _rdel = 4;

    float toneParam = apvts.getRawParameterValue("Fb Tone")->load();
    _filt = toneParam / 200.0f + 0.5f;

    if (_filt > 0.5f) {                 
        _filt = 0.5f * _filt - 0.25f;   
        _lmix = -2.0f * _filt;          
        _hmix = 1.0f;
    }
    else {                            
        _hmix = 2.0f * _filt;          
        _lmix = 1.0f - _hmix;          
    }

    float hz = std::pow(10.0f, 2.2f + 4.5f * _filt);

    _filt = std::exp(-6.2831853f * hz / float(getSampleRate()));

    float feedbackParam = apvts.getRawParameterValue("Feedback")->load() / 100.0f;
    _feedback = 0.495f * feedbackParam;

    float gain = apvts.getRawParameterValue("Output")->load();
    gain = juce::Decibels::decibelsToGain(gain);

    float mix = apvts.getRawParameterValue("FX Mix")->load() / 100.0f;
    _wet = gain * (1.0f - (1.0f - mix) * (1.0f - mix)) * 0.5f;
    _dry = gain * (1.0f - mix * mix);

}

void StereoDelayAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    update();

    const float* in1 = buffer.getReadPointer(0);
    const float* in2 = buffer.getReadPointer(1);
    float* out1 = buffer.getWritePointer(0);
    float* out2 = buffer.getWritePointer(1);

    const float wet = _wet;
    const float dry = _dry;
    const float fb = _feedback;
    const float f = _filt;
    const float lmix = _lmix;
    const float hmix = _hmix;
    const int s = _delayMax;

    float f0 = _filt0;

    int p = _pos;

    int l = (p + _ldel) % s;
    int r = (p + _rdel) % s;

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float a = in1[i];
        float b = in2[i];
        float dl = _delayBuffer[l];
        float dr = _delayBuffer[r];
        float tmp = wet * (a + b) + fb * (dl + dr);

        f0 = f * (f0 - tmp) + tmp;

        _delayBuffer[p] = lmix * f0 + hmix * tmp;

        p--; if (p < 0) p = s - 1;
        l--; if (l < 0) l = s - 1;
        r--; if (r < 0) r = s - 1;

        out1[i] = dry * a + dl;
        out2[i] = dry * b + dr;
    }

    _pos = p;

    
    if (std::abs(f0) < 1.0e-10f) _filt0 = 0.0f; else _filt0 = f0;
}

juce::AudioProcessorEditor* StereoDelayAudioProcessor::createEditor()
{
    // return new juce::GenericAudioProcessorEditor(*this);
    return new StereoDelayAudioProcessorEditor(*this);
}

void StereoDelayAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void StereoDelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

float StereoDelayAudioProcessor::rightDelayRatio(float param) {
    switch (int(param * 17.9f)) {
    case 17: return 0.5000f;      
    case 16: return 0.6667f;
    case 15: return 0.7500f;
    case 14: return 0.8333f;
    case 13: return 1.0000f;
    case 12: return 1.2000f;
    case 11: return 1.3333f;
    case 10: return 1.5000f;
    case  9: return 2.0000f;
    default: return 4.0f * param;  
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout StereoDelayAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("L Delay", 1),
        "L Delay",
        juce::NormalisableRange<float>(0.1f, _delayMaxMsec, 0.01f, 0.4f),
        250.0f,
        juce::AudioParameterFloatAttributes().withLabel("ms")));

  

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("R Delay", 1),
        "R Delay",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.27f,
        juce::AudioParameterFloatAttributes()
        .withLabel("%")
        .withStringFromValueFunction(
            [](float value, int) { return juce::String(rightDelayRatio(value) * 100.0f, 1); }
    )));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("Feedback", 1),
        "Feedback",
        juce::NormalisableRange<float>(0.0f, 99.0f, 0.01f),
        70.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("Fb Tone", 1),
        "Fb Tone",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.01f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("Lo <> Hi")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("FX Mix", 1),
        "FX Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.01f),
        33.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("Output", 1),
        "Output",
        juce::NormalisableRange<float>(-10.0f, 12.0f, 0.01f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoDelayAudioProcessor();
}