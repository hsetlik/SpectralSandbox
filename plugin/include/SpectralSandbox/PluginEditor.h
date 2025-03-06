#pragma once

#include "PluginProcessor.h"

namespace audio_plugin {

class SpectralSandboxProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit SpectralSandboxProcessorEditor(SpectralSandboxAudioProcessor&);
  ~SpectralSandboxProcessorEditor() override;
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  SpectralSandboxAudioProcessor& processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralSandboxProcessorEditor)
};

}  // namespace audio_plugin
