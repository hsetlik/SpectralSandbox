#pragma once

#include "../Common.h"
#include <juce_dsp/juce_dsp.h>
#include "juce_core/system/juce_PlatformDefs.h"

typedef std::unique_ptr<juce::dsp::FFT> fft_core_t;
typedef juce::dsp::WindowingFunction<float> window_func_t;

// main core FFT processor

class FFTProcessor {
private:
  static constexpr int overlap = 4;
  static constexpr int hopSize = FFT_SIZE / overlap;  // 256 samples
  // Gain correction for using Hann window with 75% overlap.
  static constexpr float windowCorrection = 2.0f / 3.0f;
  fft_core_t core;
  window_func_t window;
  // double buffering and FIFO stuff
  int samplePos = 0;
  float fftBuf[FFT_SIZE * 2];
  float inPtr[FFT_SIZE];
  float outPtr[FFT_SIZE];
  // other state stuff
  int hopCount = 0;
  // helpers
  void hopReady();

public:
  FFTProcessor();
  virtual ~FFTProcessor() = default;
  // do the processing one sample at a time
  float processSample(float input);
  // child classes override this to do the actual spectral manipulation
  virtual void processFreqDomain(std::complex<float>* data) = 0;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFTProcessor)
};
