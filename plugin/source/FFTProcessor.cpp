#include "SpectralSandbox/Audio/FFTProcessor.h"
#include "SpectralSandbox/Common.h"

FFTProcessor::FFTProcessor()
    : core(std::make_unique<juce::dsp::FFT>(FFT_ORDER)),
      window(FFT_SIZE + 1,
             juce::dsp::WindowingFunction<float>::WindowingMethod::hann,
             false) {}

float FFTProcessor::processSample(float input) {
  inPtr[samplePos] = input;
  float output = outPtr[samplePos];
  outPtr[samplePos] = 0.0f;
  samplePos = (samplePos + 1) % FFT_SIZE;
  ++hopCount;
  if (hopCount == hopSize) {
    hopReady();
    hopCount = 0;
  }
  return output;
}

void FFTProcessor::hopReady() {
  // 1. copy the relevant chunk of the input buffer into the real part of our
  // FFT buffer
  std::memcpy(fftBuf, inPtr + samplePos,
              (size_t)(FFT_SIZE - samplePos) * sizeof(float));
  if (samplePos > 0) {
    std::memcpy(fftBuf + FFT_SIZE - samplePos, inPtr,
                (size_t)samplePos * sizeof(float));
  }
  // 2. Do the forward transform to the frequency domain
  window.multiplyWithWindowingTable(fftBuf, FFT_SIZE);
  core->performRealOnlyForwardTransform(fftBuf, true);

  // 3.  Do the spectral processing and inverse
  // transform back to time domain
  std::complex<float>* spec = reinterpret_cast<std::complex<float>*>(fftBuf);
  processFreqDomain(spec);
  core->performRealOnlyInverseTransform(fftBuf);

  // 4. Apply the windowing function again
  window.multiplyWithWindowingTable(fftBuf, FFT_SIZE);

  // 5. copy to output buffer
  for (int i = 0; i < samplePos; i++) {
    outPtr[i] += fftBuf[i + FFT_SIZE - samplePos] * windowCorrection;
  }
  for (int i = 0; i < FFT_SIZE - samplePos; i++) {
    outPtr[i + samplePos] += fftBuf[i] * windowCorrection;
  }
}
