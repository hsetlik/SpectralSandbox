#include "SpectralSandbox/Audio/Util.h"
#include "SpectralSandbox/Common.h"

namespace spec {
constexpr float c_getCenterFrequency(int bin, float sampleRate) {
  return ((float)bin * sampleRate) / (float)FFT_SIZE;
}
// generate our default bin values
constexpr std::array<float, NUM_FFT_BINS> c_defaultBinFrequencies() {
  std::array<float, NUM_FFT_BINS> arr;
  for (int b = 0; b < NUM_FFT_BINS; b++) {
    arr[(size_t)b] = c_getCenterFrequency(b, 44100.0f);
  }
  return arr;
}

// static container for the center frequencies
static std::array<float, NUM_FFT_BINS> binFreqs = c_defaultBinFrequencies();

// update the 'binFreqs' array with the new sample rate
void sampleRateUpdated() {
  const float sr = (float)SampleRate::get();
  for (int b = 0; b < NUM_FFT_BINS; b++) {
    binFreqs[(size_t)b] = ((float)b * sr) / (float)FFT_SIZE;
  }
}

int binForFrequency(float hz) {
  // the basic algebra of this conversion V
  // hz = (bin * sampleRate) / fftSize
  // bin * sampleRate = hz * fftSize
  // bin = (hz * fftSize) / sampleRate
  float fBin = (hz * (float)FFT_SIZE) / (float)SampleRate::get();
  return (int)fBin;
}

float frequencyForBin(int bin) {
  return binFreqs[(size_t)bin];
}
}  // namespace spec
